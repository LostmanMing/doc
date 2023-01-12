import cv2
import logging

def save_image(num, image):
    """保存图片函数
    num : 当前图片序号
    image_path : 图片保存路径
    """
    image_path = r'F:\EOC\coal\dataset\mcar_sleep\{}_{}.jpg'\
        .format(image_name, str(num))      # 需手动设置
    cv2.imwrite(image_path,image)

# 初始化日志配置
logging.basicConfig(level=logging.INFO,
                        format='%(asctime)s - %(levelname)s: %(message)s')


if __name__ == '__main__':

    video_path = r'F:\EOC\coal\data\mcar_sleep\mcar_sleep8.mp4'   # 需手动设置
    vid_cap = cv2.VideoCapture(video_path)
    image_name = video_path.split('.')[0].split('\\')[-1]

    fps = vid_cap.get(cv2.CAP_PROP_FPS)
    logging.info('video fps ' + str(fps))

    print("video fps {}".format(fps))

    if vid_cap.isOpened():
        ret = vid_cap.read()
    else:
        ret = False
    logging.error('Frame is null!')

    count = 0
    frame_interval = 1         # 每多少帧保存一次
    frame_interval_count = 0

    logging.info('Video split start : imwrite one every {} frame .......'.format(str(frame_interval)))


    while ret:
        ret, frame = vid_cap.read()
        if frame is None:
            logging.info("Frame is None! Spliting finished!")
            break
        if frame_interval_count % frame_interval == 0:
            save_image(count, frame)
            logging.info("num：" + str(count) + ", frame: " +
                        str(frame_interval_count))
            count += 1
        frame_interval_count += 1
        cv2.waitKey(1)

    vid_cap.release()