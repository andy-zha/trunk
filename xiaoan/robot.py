#!usr/bin/python3
# code=utf-8

from assistant import *

my_robot = Assistant()


@bot.register(Group)
def auto_reply_group(msg):
    if TEXT == msg.type:
        if msg.is_at:
            my_robot.ReplyGroup(msg)


@bot.register(Friend)
def auto_reply_friend(msg):
    if TEXT == msg.type:
        my_robot.Reply(msg)


bot.join()
