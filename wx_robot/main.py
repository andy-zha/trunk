#!/usr/bin/python3
#code=utf-8

from session import *


if __name__ == '__main__':
    fri = SearchFriend('老小')
    while True:
        SendText(fri, '你真好看')
    embed()
