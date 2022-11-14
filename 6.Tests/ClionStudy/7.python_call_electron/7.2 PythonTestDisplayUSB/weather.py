import requests
import json


class weather:
    nums = None
    cityid = None
    city = None
    date = None
    week = None
    updata_time = None
    wea = None
    wea_img = None
    tem = None
    tem_day = None
    tem_night = None
    win = None
    win_speed = None
    win_meter = None
    air = None
    pressure = None
    humidity = None

    def __init__(self):
        self.get_weather()

    def get_weather(self):
        # 打开天气api网站
        r = requests.get(
            # appid: 注册用户ID
            # appsecret: 用户密钥
            # cityid:
            # 1.江宁: CN101190104
            # 2.浦口: CN101190107
            # 3.玄武: CN101190108
            # 4.秦淮: CN101190109
            # unescape: 1-转义中文
            'https://v0.yiketianqi.com/free/day?appid=77634591&appsecret=2bzQD923&unescape=1&cityid=CN101190108'
        )
        r.encoding = 'utf-8'

        # 转换string为json格式
        data = json.loads(r.content.decode())

        # 解析json获取有效数据
        self.nums = data['nums']
        self.cityid = data['cityid']
        self.city = data['city']
        self.date = data['date']
        self.week = data['week']
        self.update_time = data['update_time']
        self.wea = data['wea']
        self.wea_img = data['wea_img']
        self.tem = data['tem']
        self.tem_day = data['tem_day']
        self.tem_night = data['tem_night']
        self.win = data['win']
        self.win_speed = data['win_speed']
        self.win_meter = data['win_meter']
        self.air = data['air']
        self.pressure = data['pressure']
        self.humidity = data['humidity']


if __name__ == "__main__":
    w = weather()

    print('今日请求次数: %s' % w.nums)
    print('城市ID: %s' % w.cityid)
    print('城市名: %s' % w.city)
    print('日期: %s' % w.date)
    print('星期: %s' % w.week)
    print('更新时间: %s' % w.updata_time)
    print('天气情况: %s' % w.wea)
    print('天气图标标识: %s' % w.wea_img)
    print('当前温度: %s' % w.tem)
    print('白天温度(高温): %s' % w.tem_day)
    print('夜晚温度(低温): %s' % w.tem_night)
    print('风向: %s' % w.win)
    print('风力: %s' % w.win_speed)
    print('风速: %s' % w.win_meter)
    print('空气质量: %s' % w.air)
    print('气压: %s' % w.pressure)
    print('湿度: %s' % w.humidity)
