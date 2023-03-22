//
// Created by Administrator on 2022/11/17.
//

#include "video_stream.h"


struct video_stream_struct  video_stream[16];

void video_stream_del(struct video_stream_struct*  video_stream)
{
    delete video_stream->colorOptions;
    delete video_stream->witer_livestream;
    video_stream->colorOptions =NULL;
    video_stream->witer_livestream =NULL;
}
/*摄像头初始化函数*/
void video_stream_Init(struct VideoCaptureMT*  videocapture,struct video_stream_struct*  video_stream)
{
    video_stream->colorOptions =  new VideoOptions;
    video_stream->colorOptions->width = videocapture->frame_width;
    video_stream->colorOptions->height = videocapture->frame_height;
    video_stream->colorOptions->imageFormat = IMAGE_FORMAT::BGR;
    video_stream->colorOptions->frameRate = videocapture->fps;
    video_stream->colorOptions->deviceType = DEVICE_TYPE::RTMP;

    video_stream->colorOptions->uri =  videocapture->livestream_url+"/myapp/"+ to_string(tcp_client.bcd_id[0])+ to_string(tcp_client.bcd_id[1]) \
                 + to_string(tcp_client.bcd_id[2])+ to_string(tcp_client.bcd_id[3])+ to_string(tcp_client.bcd_id[4])+ to_string(tcp_client.bcd_id[5]) \
                 +"_ch"+videocapture->channel_name;
    //video_stream->colorOptions->uri = "rtmp://58.218.237.198:1935/myapp/livestream";
    //video_stream->colorOptions->uri = " rtmp://192.168.1.107/live/livestream";
    spdlog::info( " video_stream.cpp 文件 推流通道地址 video_stream->colorOptions->uri = {}" ,video_stream->colorOptions->uri );
    video_stream->witer_livestream = GSVideoWriterForNV::create(std::move(*video_stream->colorOptions));
    video_stream->witer_livestream->open();

}


/*******************************************************************
** 函数名:    captureHandle
** 函数描述: 处理视频流函数
** 参数:
** 返回:      无
********************************************************************/
void video_stream_funtion( struct VideoCaptureMT*  videocapture ,struct video_stream_struct*  video_stream)
{

    while (1)
    {
        sleep(1);

        if( (videocapture->m_IsOpen == true)&&(!videocapture->livestream_url.empty())&&(videocapture->livestream_url != "false"))
        {
            uint8_t empty_num = 0;
            cv::Mat frame(videocapture->frame_height, videocapture->frame_width, CV_8UC3), gray(
                    videocapture->frame_height, videocapture->frame_width, CV_8UC3);
            video_stream_Init(videocapture,video_stream);

            while (videocapture->m_IsOpen)
            {
                if (video_stream->video_stream_frame.empty())
                {
                    if (++empty_num > 10)
                    {
                        empty_num = 0;
                        spdlog::info( " video_stream.cpp 文件-----推流通道-----  视频为空 ,退出循环" );
                    }

                }
                else
                {
                    video_stream->video_stream_mutex.lock();
                    video_stream->video_stream_frame.copyTo(frame);
                    video_stream->video_stream_mutex.unlock();
                    empty_num = 0;
                    gray = frame.clone();
                }
                //spdlog::info("{} {}",gray.rows,gray.cols);
                /*****************************************推流******************************************************/
                video_stream->witer_livestream->Render(gray.data, 3 * video_stream->colorOptions->width *
                                                                 video_stream->colorOptions->height,
                                                      video_stream->colorOptions->width,
                                                      video_stream->colorOptions->height);
                cv::waitKey(30);
            }
            video_stream->witer_livestream->close();
            video_stream_del(video_stream);
        }
        else
        {
            //spdlog::info( " video_stream.cpp 文件-----推流通道-----  失败" );
        }

    }


}