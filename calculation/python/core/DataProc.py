#!/usr/bin/env python3

import sys

class DataProc:
	#构造函数
	def __init__(self):
		pass

	#获取kappa系数
	def GetKappa(self, Metrix):
		#异常判断，元组长度为0，返回0.0
		if 0 == len(Metrix):
			return 0.0

		#异常判断，矩阵维度长度不相等，返回0.0
		uLenOne = len(Metrix)
		uLenTwo = len(Metrix[0])
		if uLenOne != uLenTwo:
			return 0.0

		#计算所有项之和；a0,a1...;b0,b1...
		uSum = 0
		uSumOne = [0 for col in range(uLenOne)]
		uSumTwo = [0 for col in range(uLenTwo)]
		for Indexone in range(0, uLenOne):
			for Indextwo in range(0, uLenTwo):
				uSum += Metrix[Indexone][Indextwo]
				uSumOne[Indexone] += Metrix[Indexone][Indextwo]
				uSumTwo[Indexone] += Metrix[Indexone][Indextwo]
		
		#计算对角线之和与所有项积之和
		uDiagonalSum = 0
		uProductSum = 0
		for Indexone in range(0, uLenOne):
			uDiagonalSum += Metrix[Indexone][Indexone]
			uProductSum += uSumOne[Indexone] * uSumTwo[Indexone]

		#计算po,pe与kappa
		po = float(uDiagonalSum) / uSum
		pe = float(uProductSum) / (uSum * uSum)
		kappa = (po - pe) / (1 - pe)
		return kappa

	#计算算数平均值
	def GetAirthMeticMean(self, Tuples):
		TuplesLen = len(Tuples)
		if 0 == TuplesLen:
			return 0.0
		
		#计算所有项总和与所有项
		Sum = 0.0
		Total = 0
		for Index in range(0, TuplesLen):
			Sum += Tuples[Index][0] * Tuples[Index][1]
			Total += Tuples[Index][1]

		#返回平均值
		mean = float(Sum) / Total
		return mean
