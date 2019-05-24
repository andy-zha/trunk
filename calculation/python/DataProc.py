#!/usr/bin/env python3

import sys

class DataProc:
	#构造函数
	def __init__():
		pass

	#获取kappa系数
	def GetKappa(Metrix):
		#异常判断，元组长度为0，返回0.0
		if 0 == len(Metrix):
			return 0.0

		#异常判断，矩阵维度长度不相等，返回0.0
		uLenOne = len(Metrix)
		uLenTwo = len(Metrix[0])
		if uLenOne != uLenTwo:
			return 0.0

		#计算所有项之和；a0,a1...;b0,b1...
		uSumOne = []
		uSumTwo = []
		for Indexone in range(0, uLenOne):
			for Indextwo in range(0, uLenTwo):
				uSum += Metrix[Indexone][Indextwo]
				uSumOne[Indexone] += Metrix[Indexone][Indextwo]
				uSumTwo[Indexone] += Metrix[Indexone][Indextwo]
		
		#计算对角线之和与所有项积之和
		for Indexone in range(0, uLenOne):
			uDiagonalSum = Metrix[Indexone][Indexone]
			uProductSum = uSumOne[Indexone] * uSumTwo[Indexone]

		#计算po,pe与kappa
		po = float(uDiagonalSum) / uSum
		pe = float(uProductSum) / uSum * uSum
		kappa = (po - pe) / (1 - pe)
		return kappa
