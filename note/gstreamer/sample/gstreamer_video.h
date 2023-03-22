//
// Created by ky on 22-10-21.
//

#pragma once
#include "../main.h"
#include <atomic>
#include <memory>
#include "gst/gst.h"
#include "gst/gstcaps.h"
#include "string"
#include "iostream"

//#define  GS_DEBUG

enum class IMAGE_FORMAT{
    JPEG,BGRA,BGR,GRAY16,GRAY8,RGB
};
enum class DEVICE_TYPE{
    FILE,RTMP
};
class VideoOptions{
public:
    IMAGE_FORMAT imageFormat;
    DEVICE_TYPE deviceType;
    int frameRate;
    int width;
    int height;
    std::string uri;


};
class GSVideoWriterForNV {
public:

    static bool gstreamerInitialized;
    static bool gstreamerInit();
    bool open();
    void close();
    bool init();
    bool buildLaunchStr();
    bool buildCapsStr();
    void checkMsgBus();
    bool Render(void *image, uint32_t size, uint32_t width, uint32_t height);
    static void onNeedData( _GstElement* pipeline, uint32_t size, void* user_data );
    static void    onEnoughData( _GstElement* pipeline, void* user_data );
    //右值引用
    static GSVideoWriterForNV* create(VideoOptions &&videoOption);
    ~GSVideoWriterForNV();
//private:
    explicit GSVideoWriterForNV(VideoOptions videoOption);
private:
    VideoOptions videoOption;
    std::atomic<bool>  mStreaming;
    std::atomic<bool>  mNeedData;
    std::string mLaunchStr;
    std::string mCapsStr;
    _GstBus *mBus= nullptr;
    _GstCaps *mBufferCaps= nullptr;
    _GstElement* mAppSrc= nullptr;
    _GstElement* mPipeline= nullptr;
};







