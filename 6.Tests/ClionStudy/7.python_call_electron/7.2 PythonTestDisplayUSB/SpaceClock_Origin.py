import numpy as np
import cv2 as cv
from PIL import Image, ImageDraw, ImageFont
import time


class spaceclock:
    def __init__(self):
        self.WIN_SIZE = 500
        self.WIN_HALF = (int)(self.WIN_SIZE / 2)
        self.COLOR_BLACK = (0, 0, 0)
        self.human_imgs_dir = 'images/spaceman/human '
        self.other_imgs_dir = 'images/spaceman/other'
        self.human_imgs_list = []
        self.other_imgs_list = []
        self.week = ("一", "二", "三", "四", "五", "六", "日")

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

        for i in range(6):
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

        # 左上空气湿度90 %
        img = self.putChineseText(img, text='90%', pos=(self.WIN_HALF - 155, 75), textColor=self.COLOR_BLACK,
                                  textSize=40)
        # 火箭
        img = self.drawImg(img, img2_file=self.other_imgs_list[5], pos=(self.WIN_HALF - 90, 35), resize=(60, 60))

        # 右上
        img = self.putChineseText(img, text='空气良好', pos=(self.WIN_HALF - 25, 35), textColor=self.COLOR_BLACK,
                                  textSize=23)
        img = self.putChineseText(img, text='晴天', pos=(self.WIN_HALF - 25, 65), textColor=self.COLOR_BLACK,
                                  textSize=23)
        img = self.putChineseText(img, text='25℃', pos=(self.WIN_HALF - 25, 95), textColor=self.COLOR_BLACK,
                                  textSize=23)
        img = self.putChineseText(img, text='26°', pos=(self.WIN_HALF + 38, 65), textColor=self.COLOR_BLACK,
                                  textSize=23)
        img = self.putChineseText(img, text='17°', pos=(self.WIN_HALF + 38, 95), textColor=self.COLOR_BLACK,
                                  textSize=23)
        img = self.drawImg(img, img2_file=self.other_imgs_list[4], pos=(self.WIN_HALF + 73, 60),
                           resize=(30, 30))  # 上面的箭头
        img = self.drawImg(img, img2_file=self.other_imgs_list[3], pos=(self.WIN_HALF + 73, 90),
                           resize=(30, 30))  # 下面的箭头
        img = self.drawImg(img, img2_file=self.other_imgs_list[1], pos=(self.WIN_HALF + 105, 70), resize=(60, 60))  # 太阳

        # 下部分
        img = self.putChineseText(img, text='睡眠', pos=(100, self.WIN_HALF + 130), textColor=self.COLOR_BLACK,
                                  textSize=30)
        img = self.putChineseText(img, text='距离', pos=(self.WIN_HALF + 90, self.WIN_HALF + 130),
                                  textColor=self.COLOR_BLACK, textSize=30)
        img = self.putChineseText(img, text='7h30m', pos=(185, self.WIN_HALF + 125), textColor=self.COLOR_BLACK,
                                  textSize=30)
        img = self.putChineseText(img, text='9.88km', pos=(215, self.WIN_HALF + 180), textColor=self.COLOR_BLACK,
                                  textSize=30)

        # 中间 心率
        img = self.putChineseText(img, text='80~128', pos=(60, self.WIN_HALF + 30), textColor=self.COLOR_BLACK,
                                  textSize=25)
        img = self.drawImg(img, img2_file=self.other_imgs_list[0], pos=(65, self.WIN_HALF + 50), resize=(70, 70))  # 心率图
        img = self.putChineseText(img, text='92', pos=(135, self.WIN_HALF + 60), textColor=self.COLOR_BLACK,
                                  textSize=30)

        # 步数
        img = self.drawImg(img, img2_file=self.other_imgs_list[2], pos=(self.WIN_HALF + 65, self.WIN_HALF + 65),
                           resize=(55, 55))
        img = self.putChineseText(img, text='9527', pos=(self.WIN_HALF + 125, self.WIN_HALF + 75),
                                  textColor=self.COLOR_BLACK, textSize=30)

        # 时间、日期相关
        localtime = time.localtime(time.time())
        img = self.putChineseText(img, text='三月三十一', pos=(self.WIN_HALF + 110, self.WIN_HALF - 20),
                                  textColor=self.COLOR_BLACK,
                                  textSize=23)
        str = '周%s %d-%d' % (self.week[localtime.tm_wday], localtime.tm_mon, localtime.tm_mday)
        img = self.putChineseText(img, text=str, pos=(self.WIN_HALF + 110, self.WIN_HALF + 10),
                                  textColor=self.COLOR_BLACK, textSize=23)
        str = '%02d:%02d' % (localtime.tm_hour, localtime.tm_min)
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
