#!/usr/bin/env python3

import sys
import re

def ip2digit(ip):
    #切割ip串
	l_value = ip.split('.')
	l_value.reverse()

    #计算十进制ip
	i_value = 0
	power = 1
	count = 0
	for value in l_value:
	    #异常检测
		if 3 < len(value) or False == value.isdigit():
			return 0
		i_value += int(value) * power
		power *= 2 ** 8
		count += 1

    #检测异常并返回
	if 4 != count:
		return 0
	else:	
		return i_value	


if __name__ == '__main__':
	while 1:
		print("please input ip addr:", end = "")
		ipaddr = input()
		if 'q' == ipaddr:
			sys.exit()

		result = ip2digit(ipaddr)
		if 0 == result:
			print("ip invalid!")
		else:
			print("result of the converter:", result)
