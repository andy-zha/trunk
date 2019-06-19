#!/usr/bin/python3
# code=utf-8

from wxpy import *
from pro import *

bot = Bot()
pro = Pro()


class Assistant:
    def __init__(self):
        pass

    def ReplyGroup(self, msg):
        length = len(msg.member)
        member = str(msg.member)
        print(member)
        if len(msg.text) == 8 and msg.text == "@xiao.an":
            print(length)
            print(member[8:11])
            answer = "@" + member[9:11] + " 你好，可爱的小安为您服务！"
            msg.reply(answer)
        else:
            ans = pro.find(str(msg.text))
            print(ans)
            answer = "@" + member[9:11] + ans
            msg.reply(answer)

    def Reply(self, msg):
        if len(msg.text) == 0:
            msg.reply("你好，小安很高兴为你服务！")
        answer = pro.find(str(msg.text))
        if len(answer) == 0:
            msg.reply("你好，小安很高兴为你服务！")
            return
        msg.reply(answer)
        return

