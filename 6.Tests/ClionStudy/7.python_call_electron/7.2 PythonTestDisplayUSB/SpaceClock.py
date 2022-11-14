import numpy as np
import cv2 as cv
from PIL import Image, ImageDraw, ImageFont
import time
import weather as we


class spaceclock:
    def __init__(self):
        self.WIN_SIZE = 500
        self.WIN_HALF = (int)(self.WIN_SIZE / 2)
        self.COLOR_BLACK = (0, 0, 0)
        self.human_imgs_dir = 'images/spaceman/human '
        self.other_imgs_dir = 'images/spaceman/other'
        self.wea_imgs_dir = 'images/weather/'
        self.human_imgs_list = []
        self.other_imgs_list = []
        self.week = ("一", "二", "三", "四", "五", "六", "日")
        self.worktime = time.localtime(time.time())

        self.w = we.weather()

    # 向图像中插入中文字符
    def putChineseText(self, img, text, pos, textColor=(0, 255, 0), textSize=30):
        if isinstance(img, np.ndarray):  # 判断是否OpenCV图片类型
            img = Image.fromarray(cv.cvtColor(img, cv.COLOR_BGR2RGB))
        # 创建一个可以在给定图像上绘图的对象
        draw = ImageDraw.Draw(img)
        # 字体的格式
        fontStyle = ImageFont.truetype(
            "msyh.ttc", textSize, encoding="utf-8")
        # 绘制文本
        draw.text(pos, text, textColor, font=fontStyle)
        # 转换回OpenCV格式
        return cv.cvtColor(np.asarray(img), cv.COLOR_RGB2BGR)

    def loadImgs(self):
        for i in range(59):
            # str = '(%d).jpg'%(i+2)
            str = f'({i + 2}).jpg'  # python3.6特性
            self.human_imgs_list.append(self.human_imgs_dir + str)

        for i in range(8):
            str = f'{i + 1}.png'
            self.other_imgs_list.append(self.other_imgs_dir + str)

    # img1: 目标图像
    # img2: 插入图像(背景黑色,图标颜色值需要大于10)
    # img[height1:height1,weight:weight]
    # pos: (pos_x, pos_y): posx: 水平坐标, pos_y: 竖直坐标
    def drawImg(self, img1, img2_file, pos, resize):
        img2 = cv.imread(img2_file)
        # 缩放插入图像
        img2 = cv.resize(img2, resize)
        # 把logo放在指定位置，创建了ROI
        rows, cols, channels = img2.shape
        roi = img1[pos[1]:pos[1] + rows, pos[0]:pos[0] + cols]
        # 现在创建logo的相反掩码
        img2gray = cv.cvtColor(img2, cv.COLOR_BGR2GRAY)
        # 背景为黑色的图片,颜色翻转为白色255值
        # 图标处深色的位置,颜色翻转为黑色0值
        # 参数中10代表阈值,小于10判定为最大值(这里设定为了255,即纯白色),大于10判定为最小值(值为0,即纯黑色)
        ret, mask_inv = cv.threshold(img2gray, 10, 255, cv.THRESH_BINARY_INV)
        # 现在将ROI中logo的区域涂黑
        img1_bg = cv.bitwise_and(roi, roi, mask=mask_inv)
        # 将涂黑的logo区域放入原图
        img1[pos[1]:pos[1] + rows, pos[0]:pos[0] + cols] = img1_bg
        return img1

    def putImg(self, img1, img2_file, pos, resize):
        img2 = cv.imread(img2_file)
        # 缩放插入图像
        img2 = cv.resize(img2, resize)
        # 我想把logo放在左上角，所以我创建了ROI
        rows, cols, channels = img2.shape
        img1[pos[1]:pos[1] + rows, pos[0]:pos[0] + cols] = img2
        return img1

    def gameDraw(self, img):
        # 绘制表盘
        cv.circle(img, center=(self.WIN_HALF, self.WIN_HALF), radius=self.WIN_HALF, color=(240, 230, 223), thickness=-1)
        cv.circle(img, center=(self.WIN_HALF, self.WIN_HALF), radius=self.WIN_HALF - 10, color=(68, 68, 68),
                  thickness=20)

        # 绘制线条
        # 最上面竖线
        cv.line(img, pt1=(self.WIN_HALF - 30, 20), pt2=(self.WIN_HALF - 30, 130), color=self.COLOR_BLACK, thickness=4)
        # 横线x2
        cv.line(img, pt1=(self.WIN_HALF - 195, self.WIN_HALF - 120), pt2=(self.WIN_HALF + 195, self.WIN_HALF - 120),
                color=self.COLOR_BLACK,
                thickness=4)
        cv.line(img, pt1=(self.WIN_HALF - 195, self.WIN_HALF + 120), pt2=(self.WIN_HALF + 195, self.WIN_HALF + 120),
                color=self.COLOR_BLACK,
                thickness=4)
        # 下面线条x3
        cv.line(img, pt1=(self.WIN_HALF + 80, self.WIN_HALF + 120), pt2=(self.WIN_HALF + 80, self.WIN_HALF + 175),
                color=self.COLOR_BLACK,
                thickness=4)
        cv.line(img, pt1=(self.WIN_HALF + 80, self.WIN_HALF + 175), pt2=(self.WIN_HALF - 60, self.WIN_HALF + 175),
                color=self.COLOR_BLACK,
                thickness=4)
        cv.line(img, pt1=(self.WIN_HALF - 60, self.WIN_HALF + 175), pt2=(self.WIN_HALF - 60, self.WIN_HALF + 175 + 48),
                color=self.COLOR_BLACK,
                thickness=4)

        # 左上 湿度图标
        img = self.drawImg(img, img2_file=self.other_imgs_list[7], pos=(self.WIN_HALF - 90, 35), resize=(60, 60))
        # 空气湿度值
        img = self.putChineseText(img, text='%s' % self.w.humidity, pos=(self.WIN_HALF - 155, 75),
                                  textColor=self.COLOR_BLACK, textSize=40)

        # 右上
        img = self.putChineseText(img, text='风力%s' % self.w.win_speed, pos=(self.WIN_HALF - 25, 35),
                                  textColor=self.COLOR_BLACK,
                                  textSize=23)
        img = self.putChineseText(img, text='%s' % self.w.wea, pos=(self.WIN_HALF - 25, 65), textColor=self.COLOR_BLACK,
                                  textSize=23)
        img = self.putChineseText(img, text='%s℃' % self.w.tem, pos=(self.WIN_HALF - 25, 95),
                                  textColor=self.COLOR_BLACK,
                                  textSize=23)
        img = self.putChineseText(img, text='%s°' % self.w.tem_day, pos=(self.WIN_HALF + 38, 65),
                                  textColor=self.COLOR_BLACK,
                                  textSize=23)
        img = self.putChineseText(img, text='%s°' % self.w.tem_night, pos=(self.WIN_HALF + 38, 95),
                                  textColor=self.COLOR_BLACK,
                                  textSize=23)
        img = self.drawImg(img, img2_file=self.other_imgs_list[4], pos=(self.WIN_HALF + 73, 60),
                           resize=(30, 30))  # 上面的箭头
        img = self.drawImg(img, img2_file=self.other_imgs_list[3], pos=(self.WIN_HALF + 73, 90),
                           resize=(30, 30))  # 下面的箭头
        str_wea_dir = self.wea_imgs_dir + self.w.wea_img + '.png'
        img = self.drawImg(img, img2_file=str_wea_dir, pos=(self.WIN_HALF + 105, 70), resize=(60, 60))  # 天气图标

        # 下部分
        wktime = time.localtime(time.time())
        h = int(wktime.tm_hour) - int(self.worktime.tm_hour)
        m = int(wktime.tm_min) - int(self.worktime.tm_min)
        img = self.putChineseText(img, text='工作', pos=(100, self.WIN_HALF + 130), textColor=self.COLOR_BLACK, textSize=30)
        img = self.putChineseText(img, text='小憩', pos=(self.WIN_HALF + 90, self.WIN_HALF + 130),
                                  textColor=self.COLOR_BLACK, textSize=30)
        img = self.putChineseText(img, text='%dh%dm' % (h, m), pos=(185, self.WIN_HALF + 125), textColor=self.COLOR_BLACK,
                                  textSize=30)
        img = self.putChineseText(img, text='%dmin' % (60-int(wktime.tm_min)), pos=(215, self.WIN_HALF + 180), textColor=self.COLOR_BLACK,
                                  textSize=30)

        # 中间 环境质量
        str_air = self.w.air
        img = self.putChineseText(img, text='空气 %s' % str_air, pos=(60, self.WIN_HALF + 40),
                                  textColor=self.COLOR_BLACK,
                                  textSize=25)
        int_air = int(str_air)
        if 0 < int_air <= 50:
            str = '环境 优'
        elif int_air <= 100:
            str = '环境 良'
        elif int_air <= 150:
            str = '轻度污染'
        elif int_air <= 200:
            str = '中度污染'
        elif int_air <= 300:
            str = '重度污染'
        else:
            str = '严重污染'
        img = self.putChineseText(img, text=str, pos=(60, self.WIN_HALF + 75), textColor=self.COLOR_BLACK,
                                  textSize=25)

        # # 步数
        # img = self.drawImg(img, img2_file=self.other_imgs_list[2], pos=(self.WIN_HALF + 65, self.WIN_HALF + 65),
        #                    resize=(55, 55))
        # img = self.putChineseText(img, text='9527', pos=(self.WIN_HALF + 125, self.WIN_HALF + 75),
        #                           textColor=self.COLOR_BLACK, textSize=30)

        # 时间、日期相关
        localtime = time.localtime(time.time())
        # img = self.putChineseText(img, text='三月三十一', pos=(self.WIN_HALF + 110, self.WIN_HALF - 20),
        #                           textColor=self.COLOR_BLACK,textSize=23)
        str = '周%s %d-%d' % (self.week[localtime.tm_wday], localtime.tm_mon, localtime.tm_mday)
        img = self.putChineseText(img, text=str, pos=(self.WIN_HALF + 80, self.WIN_HALF + 75),
                                  textColor=self.COLOR_BLACK, textSize=23)
        str = '%d:%02d' % (localtime.tm_hour, localtime.tm_min)
        img = self.putChineseText(img, text=str, pos=(105, 120), textColor=self.COLOR_BLACK, textSize=80)
        str = '%02d' % localtime.tm_sec
        img = self.putChineseText(img, text=str, pos=(335, 160), textColor=self.COLOR_BLACK, textSize=47)
        return img

    def animation(self, img, index=None):
        img = self.putImg(img, img2_file=self.human_imgs_list[index], pos=(175, 210), resize=(140, 130))
        return img


if __name__ == "__main__":
    s = spaceclock()
    s.loadImgs()

    img = np.zeros((s.WIN_SIZE, s.WIN_SIZE, 3), np.uint8)

    index = 0
    while True:
        img = s.gameDraw(img)
        s.animation(img, index)
        index = (index + 1) % 59

        cv.imshow('image', img)
        if cv.waitKey(1) == ord('q'):
            break
    cv.destroyAllWindows()
