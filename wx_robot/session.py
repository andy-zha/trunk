#!/usr/bin/python3
#会话类

from wxpy import *

bot = Bot()
tul = Tuling(api_key='')


# 查找好友
def SearchFriend(name):
    my_friend = bot.friends().search(name)[0]
    return my_friend

# 查找群
def SearchGroup(group):
    my_group = bot.groups.search(group)[0]
    return my_group

# 发送文本
def SendText(fri, txt):
    try:
        fri.send(txt)
    except  ResponseError:
        return -1
    return 0

# 自动回复群聊消息(除去自己）
@bot.register(Group, TEXT)
def AutoReplyGroup(msg):
    if TEXT == msg.type:
        tul.do_reply(msg)
    if RECORDING == msg.type:
        tul.do_reply("不听不听，王八念经")