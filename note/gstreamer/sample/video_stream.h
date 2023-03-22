//
// Created by Administrator on 2022/11/17.
//

#ifndef COAL_AI_VIDEO_STREAM_H
#define COAL_AI_VIDEO_STREAM_H
#include "../main.h"


struct video_stream_struct
{
    std::mutex video_stream_mutex;
    cv::Mat video_stream_frame; /*视频帧*/

    class VideoOptions* colorOptions;
    class GSVideoWriterForNV* witer_livestream;
};

extern struct video_stream_struct  video_stream[16];

void video_stream_del(struct video_stream_struct*  video_stream);
void video_stream_Init(struct VideoCaptureMT*  videocapture);
extern void video_stream_funtion( struct VideoCaptureMT*  videocapture ,struct video_stream_struct*  video_stream);







#endif //COAL_AI_VIDEO_STREAM_H
