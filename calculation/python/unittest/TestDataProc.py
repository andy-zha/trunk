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
		Metrix = [[10, 2, 8], [5, 35, 5], [5, 2, 15]]
		self.assertEqual(1.0010569427929712, data.GetKappa(Metrix))

if __name__ == "__main__":
	unittest.main()
