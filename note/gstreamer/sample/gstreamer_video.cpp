//
// Created by ky on 22-10-21.
//

#include "gstreamer_video.h"
#include <gst/app/gstappsrc.h>

#include <utility>

#include "spdlog/spdlog.h"
#include "gst/gstbus.h"
#include "iostream"
#include "sstream"
#include "unistd.h"

//静态变量,定义与初始化分开,全局共享初始化标记
bool GSVideoWriterForNV::gstreamerInitialized = false;

//工具类,输出相关日志信息
inline const char *gst_debug_level_str(GstDebugLevel level) {
    switch (level) {
        case GST_LEVEL_NONE:
            return "GST_LEVEL_NONE   ";
        case GST_LEVEL_ERROR:
            return "GST_LEVEL_ERROR  ";
        case GST_LEVEL_WARNING:
            return "GST_LEVEL_WARNING";
        case GST_LEVEL_INFO:
            return "GST_LEVEL_INFO   ";
        case GST_LEVEL_DEBUG:
            return "GST_LEVEL_DEBUG  ";
        case GST_LEVEL_LOG:
            return "GST_LEVEL_LOG    ";
        case GST_LEVEL_FIXME:
            return "GST_LEVEL_FIXME  ";
#ifdef GST_LEVEL_TRACE
            case GST_LEVEL_TRACE:	return "GST_LEVEL_TRACE  ";
#endif
        case GST_LEVEL_MEMDUMP:
            return "GST_LEVEL_MEMDUMP";
        default:
            return "<unknown>        ";
    }
}

//工具类
static const char *gst_stream_status_string(GstStreamStatusType status) {
    switch (status) {
        case GST_STREAM_STATUS_TYPE_CREATE:
            return "CREATE";
        case GST_STREAM_STATUS_TYPE_ENTER:
            return "ENTER";
        case GST_STREAM_STATUS_TYPE_LEAVE:
            return "LEAVE";
        case GST_STREAM_STATUS_TYPE_DESTROY:
            return "DESTROY";
        case GST_STREAM_STATUS_TYPE_START:
            return "START";
        case GST_STREAM_STATUS_TYPE_PAUSE:
            return "PAUSE";
        case GST_STREAM_STATUS_TYPE_STOP:
            return "STOP";
        default:
            return "UNKNOWN";
    }
}

//日志回调,输出自定义日志(由GSTREAMER 管理)
void rilog_debug_function(GstDebugCategory *category, GstDebugLevel level,
                          const gchar *file, const char *function,
                          gint line, GObject *object, GstDebugMessage *message,
                          gpointer data) {
    if (level > GST_LEVEL_WARNING /*GST_LEVEL_INFO*/ )
        return;

    //gchar* name = NULL;
    //if( object != NULL )
    //	g_object_get(object, "name", &name, NULL);

    const char *typeName = " ";
    const char *className = " ";

    if (object != nullptr) {
        typeName = G_OBJECT_TYPE_NAME(object);
        className = G_OBJECT_CLASS_NAME(object);
    }
#ifdef GS_DEBUG
    spdlog::info("{} {} {} {}", gst_debug_level_str(level), className, typeName,
                 gst_debug_category_get_name(category));
    spdlog::info("{} {} {} {} {} {}", gst_debug_category_get_name(category), file, line, function,
                 gst_debug_message_get(message));
#endif


}

// gst_message_print,工具类
gboolean gst_message_print(GstBus *bus, GstMessage *message, gpointer user_data) {
    switch (GST_MESSAGE_TYPE (message)) {
        case GST_MESSAGE_ERROR: {
            GError *err = nullptr;
            gchar *dbg_info = nullptr;

            gst_message_parse_error(message, &err, &dbg_info);
            spdlog::error("gstreamer {} ERROR {}", GST_OBJECT_NAME (message->src), err->message);
            spdlog::debug("gstreamer Debugging info: {}", (dbg_info) ? dbg_info : "none");
            g_error_free(err);
            g_free(dbg_info);
            break;
        }
        case GST_MESSAGE_EOS: {
            spdlog::info("gstreamer {} recieved EOS signal...", GST_OBJECT_NAME(message->src));
            break;
        }
        case GST_MESSAGE_STATE_CHANGED: {
            GstState old_state, new_state;

            gst_message_parse_state_changed(message, &old_state, &new_state, nullptr);


            /*spdlog::info("gstreamer changed state from {} to {} ==> {}", gst_element_state_get_name(old_state),
                         gst_element_state_get_name(new_state),
                         GST_OBJECT_NAME(message->src));*/
            break;
        }
        case GST_MESSAGE_STREAM_STATUS: {
            GstStreamStatusType streamStatus;
            gst_message_parse_stream_status(message, &streamStatus, nullptr);
#ifdef GS_DEBUG
            spdlog::info("gstreamer stream status {} ==> {}",
                         gst_stream_status_string(streamStatus),
                         GST_OBJECT_NAME(message->src));
#endif
            break;
        }
        case GST_MESSAGE_TAG: {
            GstTagList *tags = nullptr;
            gst_message_parse_tag(message, &tags);
            gchar *txt = gst_tag_list_to_string(tags);

            if (txt != nullptr) {
#ifdef GS_DEBUG
                spdlog::info("gstreamer {} {}", GST_OBJECT_NAME(message->src), txt);
#endif
                g_free(txt);
            }

            //gst_tag_list_foreach(tags, gst_print_one_tag, NULL);

            if (tags != nullptr)
                gst_tag_list_free(tags);

            break;
        }
        default: {
#ifdef GS_DEBUG
            spdlog::info("gstreamer message {} ==> {}\n", gst_message_type_get_name(GST_MESSAGE_TYPE(message)),
                         GST_OBJECT_NAME(message->src));
#endif
            break;
        }
    }

    return TRUE;
}

//全局初始化gstreamer,并设置日志
bool GSVideoWriterForNV::gstreamerInit() {

    if (gstreamerInitialized) {
        return true;
    }


    int argc = 0;
    //char* argv[] = { "none" };

    if (!gst_init_check(&argc, nullptr, nullptr)) {
        spdlog::error("failed to initialize gstreamer library with gst_init()");
        return false;
    }

    gstreamerInitialized = true;

    uint32_t ver[] = {0, 0, 0, 0};
    gst_version(&ver[0], &ver[1], &ver[2], &ver[3]);
#ifdef GS_DEBUG
    spdlog::info("initialized gstreamer, version {}.{}.{}.{}", ver[0], ver[1], ver[2], ver[3]);
#endif
    // 移除默认日志回调
    gst_debug_remove_log_function(gst_debug_log_default);
    gst_debug_set_default_threshold(GstDebugLevel::GST_LEVEL_DEBUG);
    //设置指定日志回调
    gst_debug_add_log_function(rilog_debug_function, nullptr, nullptr);
    gst_debug_set_active(true);
    //是否彩色
    gst_debug_set_colored(false);
    return true;
}

bool GSVideoWriterForNV::open() {
    //判断流是否打开
    if (mStreaming) {
        return true;
    }
#ifdef GS_DEBUG
    // transition pipline to STATE_PLAYING
    spdlog::info("gstEncoder-- starting pipeline, transitioning to GST_STATE_PLAYING");
#endif
    //设置管道状态
    const GstStateChangeReturn result = gst_element_set_state(mPipeline, GST_STATE_PLAYING);;
    if (result == GST_STATE_CHANGE_ASYNC) {
        spdlog::debug("GST_STATE_CHANGE_ASYNC");
    } else if (result != GST_STATE_CHANGE_SUCCESS) {
        spdlog::error("gstEncoder -- failed to set pipeline state to PLAYING (error {})", result);
        return false;
    }


    //等待各组件准备完成
    checkMsgBus();
    usleep(100 * 1000);
    checkMsgBus();

    mStreaming = true;

    return true;

}

void GSVideoWriterForNV::checkMsgBus() {

    while (true) {
        GstMessage *msg = gst_bus_pop(mBus);
        if (!msg)
            break;
        gst_message_print(mBus, msg, this);
        gst_message_unref(msg);
    }
}

//初始化,针对当前对象的初始化
bool GSVideoWriterForNV::init() {
    if (!gstreamerInit()) {
        spdlog::error("failed init gstreamer");
        return false;
    }
    //构造主干字符串
    if (!buildLaunchStr()) {
        spdlog::error("gstEncoder -- failed to build pipeline string");
        return false;
    }

    GError *err = nullptr;
    //创建管道
    mPipeline = gst_parse_launch(mLaunchStr.c_str(), &err);
    //如果存在错误信息，则输出日志
    if (err != nullptr) {
        spdlog::error("gstEncoder -- failed to create pipeline {}", err->message);
        g_error_free(err);
        return false;
    }

    //类型强制转换
    GstPipeline *pipeline = GST_PIPELINE(mPipeline);

    if (!pipeline) {
        spdlog::error("gstEncoder -- failed to cast GstElement into GstPipeline");
        return false;
    }
    //生成总线
    mBus = gst_pipeline_get_bus(pipeline);
    if (!mBus) {
        spdlog::error("gstEncoder -- failed to retrieve GstBus from pipeline");
        return false;
    }

    // get the appsrc element
    GstElement *appsrcElement = gst_bin_get_by_name(GST_BIN(pipeline), "mysource");
    GstAppSrc *appsrc = GST_APP_SRC(appsrcElement);

    if (!appsrcElement || !appsrc) {
        spdlog::error("gstEncoder -- failed to retrieve appsrc element from pipeline");
        return false;
    }

    mAppSrc = appsrcElement;
    //注册回调事件,设置是否可以填充数据
    g_signal_connect(appsrcElement, "need-data", G_CALLBACK(onNeedData), this);
    g_signal_connect(appsrcElement, "enough-data", G_CALLBACK(onEnoughData), this);

    return true;
}



//构造gstreamer字符串
bool GSVideoWriterForNV::buildLaunchStr() {
    std::ostringstream ss;
    if (videoOption.imageFormat!=IMAGE_FORMAT::BGR){
        spdlog::error("only support BGR");
        return false;
    }

    ss << "appsrc name=mysource  is-live=true do-timestamp=true format=3 ! ";
    ss << " nvvideoconvert ! video/x-raw(memory:NVMM),format=(string)I420, width= " << videoOption.width << ", height= " << videoOption.height
       << " ,framerate=" << videoOption.frameRate << "/1 !"; //转换成 h.264 或 h.265 支持的 YUV 格式
    if (videoOption.deviceType == DEVICE_TYPE::FILE) {
        ss << "nvv4l2h264enc ! h264parse ! matroskamux ! ";   //nvidia h.264解码器
        ss << "filesink location=" << videoOption.uri;        //写文件的输出
    } else if (videoOption.deviceType == DEVICE_TYPE::RTMP) {
        //ss << "nvv4l2h264enc maxperf-enable=1 profile=4 preset-level=1 iframeinterval=500 control-rate=1 bitrate=2000000 ! h264parse ! flvmux ! rtmpsink location="
        //   << videoOption.uri;
        ss << "nvv4l2h264enc control-rate=1 bitrate=2000000 !  h264parse  ! flvmux  ! rtmpsink location=" //输出到rtmp服务器
           << videoOption.uri;

    } else {
        spdlog::error("not support  deviceType");
        return false;
    }


    mLaunchStr = ss.str();
    spdlog::info("use {}",mLaunchStr);
    return true;
}

void GSVideoWriterForNV::onNeedData(_GstElement *pipeline, uint32_t size, void *user_data) {
    spdlog::debug("gstEncoder -- appsrc requesting data ({} bytes)", size);

    if (!user_data) {
        return;
    }
    auto enc = (GSVideoWriterForNV *) user_data;
    enc->mNeedData = true;
}

void GSVideoWriterForNV::onEnoughData(_GstElement *pipeline, void *user_data) {
    spdlog::debug("gstEncoder -- appsrc signalling enough data");

    if (!user_data) {
        return;
    }
    auto *enc = (GSVideoWriterForNV *) user_data;
    enc->mNeedData = false;
}

//关闭流
void GSVideoWriterForNV::close() {

    //避免二次关闭
    if (!mStreaming) {
        return;
    }

    // send EOS
    mNeedData = false;

    //spdlog::info("gstEncoder -- shutting down pipeline, sending EOS");
    GstFlowReturn eos_result = gst_app_src_end_of_stream(GST_APP_SRC(mAppSrc));

    if (eos_result != 0) {
        spdlog::error("gstEncoder -- failed sending appsrc EOS (result {})", eos_result);
    }


    sleep(1);

    // stop pipeline
    //spdlog::info("gstEncoder -- transitioning pipeline to GST_STATE_NULL");
    //向管道发送EOS
    const GstStateChangeReturn result = gst_element_set_state(mPipeline, GST_STATE_NULL);

    if (result != GST_STATE_CHANGE_SUCCESS) {
        spdlog::error("gstEncoder -- failed to set pipeline state to NULL (error {})", result);
    }


    sleep(1);
    checkMsgBus();
    mStreaming = false;
    spdlog::info("gstEncoder -- pipeline stopped");
}

//发送JPEG格式的图片，并将其封装成视频流
bool GSVideoWriterForNV::Render(void *image, uint32_t size, uint32_t width, uint32_t height) {

    //
    if (!image || size == 0) {
        return false;
    }
    if(width!=videoOption.width||height!=videoOption.height){
        spdlog::error("width or height not correspond");
        return false;
    }
    // confirm the stream is open
    if (!mStreaming) {
        if (!open()) {
            return false;
        }
    }

    // check to see if data can be accepted
    //为了防止输入速度过快,暂停输入1useconds,为了保证所有帧都被处理，可开启，否则 应忽略该帧
    while (!mNeedData) {
        usleep(1);
    }
    //忽略该帧
    if (!mNeedData) {
#ifdef GS_DEBUG
        spdlog::info("gstEncoder -- pipeline full, skipping frame ({} bytes)", size);
#endif
        return true;
    }

    // construct the buffer caps for this size image
    //构造视频捕获器输入
    if (!mBufferCaps) {
        //构造输入字符串
        if (!buildCapsStr()) {
            spdlog::error("gstEncoder -- failed to build caps string");
            return false;
        }

        //根据输入字符串创建
        mBufferCaps = gst_caps_from_string(mCapsStr.c_str());
        mCapsStr.c_str()
        if (!mBufferCaps) {
            spdlog::error("gstEncoder -- failed to parse caps from string:");
            spdlog::error("   {}", mCapsStr.c_str());
            return false;
        }

        //设置输入组件
        gst_app_src_set_caps(GST_APP_SRC(mAppSrc), mBufferCaps);
    }


    //创建缓冲内存
    // allocate gstreamer buffer memory
    GstBuffer *gstBuffer = gst_buffer_new_allocate(nullptr, size, nullptr);

    // map the buffer for write access
    GstMapInfo map;

    if (gst_buffer_map(gstBuffer, &map, GST_MAP_WRITE)) {
        if (map.size != size) {
            spdlog::error("gstEncoder -- gst_buffer_map() size mismatch, got %zu bytes, expected %zu bytes", map.size,
                          size);
            gst_buffer_unref(gstBuffer);
            return false;
        }
        //拷贝内存
        memcpy(map.data, image, size);
        gst_buffer_unmap(gstBuffer, &map);
    } else {
        spdlog::error("gstEncoder -- failed to map gstreamer buffer memory (%zu bytes)", size);
        gst_buffer_unref(gstBuffer);
        return false;
    }

    // queue buffer to gstreamer
    GstFlowReturn ret;
    g_signal_emit_by_name(mAppSrc, "push-buffer", gstBuffer, &ret);
    gst_buffer_unref(gstBuffer);

    if (ret != 0) {
        spdlog::error("gstEncoder -- appsrc pushed buffer abnormally (result {})", ret);
    }
    checkMsgBus();
    return true;
}

bool GSVideoWriterForNV::buildCapsStr() {

    std::ostringstream ss;
    if (videoOption.imageFormat == IMAGE_FORMAT::JPEG) {
        ss << " image/jpeg";
    } else if (videoOption.imageFormat == IMAGE_FORMAT::BGR) {
        ss << " video/x-raw,format=BGR ";
    } else if (videoOption.imageFormat == IMAGE_FORMAT::GRAY16) {
        ss << " video/x-raw,format=GRAY16_BE ";
    } else {
        spdlog::error("not support capture");
        return false;
    }

    ss << ", width=" << videoOption.width;
    ss << ", height=" << videoOption.height;
    ss << ", framerate=" << (int) videoOption.frameRate << "/1";
    mCapsStr = ss.str();
    //spdlog::info("gstEncoder -- new caps: {}", mCapsStr.c_str());
    return true;
}

GSVideoWriterForNV::~GSVideoWriterForNV() {

    close();
    if (mAppSrc != nullptr) {
        gst_object_unref(mAppSrc);
        mAppSrc = nullptr;
    }

    if (mBus != nullptr) {
        gst_object_unref(mBus);
        mBus = nullptr;
    }

    if (mPipeline != nullptr) {
        gst_object_unref(mPipeline);
        mPipeline = nullptr;
    }
    spdlog::debug("do ~");
}

GSVideoWriterForNV *GSVideoWriterForNV::create(VideoOptions &&videoOption) {

    auto writer = new GSVideoWriterForNV(videoOption);

    if (!writer->init()) {
        spdlog::error("gstEncoder -- failed to create encoder engine");
        return nullptr;
    }

    return writer;
}

GSVideoWriterForNV::GSVideoWriterForNV(VideoOptions videoOption) : videoOption(std::move(videoOption)) {

    mNeedData = false;
    mStreaming = false;

}

