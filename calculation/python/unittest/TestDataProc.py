#!/usr/bin/env python3

import sys
import os
import unittest

corepath = os.path.join(os.path.abspath(os.path.dirname(__file__)), '../core')
sys.path.append(corepath)

from DataProc import DataProc

class Test_DataProc(unittest.TestCase):
	def setUp(self):
		pass

	def tearDown(self):
		pass

	def test_getkappa(self):
		data = DataProc()
		Metrix = [[10, 5, 5], [2, 35, 2], [8, 5, 15]]
		self.assertEqual(0.5170641447368421, data.GetKappa(Metrix))

	def test_getarithmeticmean(self):
		proc = DataProc()
		Tuples = [[3,5],[4,6],[5,7],[6,8],[7,9]]
		self.assertEqual(5.285714285714286,proc.GetAirthMeticMean(Tuples))
