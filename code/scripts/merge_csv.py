#!/usr/bin/env python
## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: t; tab-width: 4; coding: utf-8; -*-
#title           : merge_csv.py
#description     : This script takes two cvs files and merges them.
#author          : Marco Romanelli
#date            : 09/05/2017
#version         : 2
#usage           : python merge_csv.py <file1.csv> <file2.csv>
#python_version  : 2.7
#==============================================================================

import getopt, sys
from os.path import isfile
import csv
from numpy import mean
import pandas as pd


def main(argv):
	csv1, csv2 = argv
	print "Parsing first file \"%s\"..." % csv1
	reader1 = pd.read_csv( csv1 )

	print "Parsing second file \"%s\"..." % csv2
	reader2 = pd.read_csv( csv2 )

	print "Merging..."
	merged = reader1.merge( reader2, on="Instance" )

	print "Saving..."
	merged.to_csv( "merged.csv", index=False, quoting=csv.QUOTE_ALL)

	# Exit
	print "Done. Exit."
	print


if __name__ == '__main__':
	# Check parameters
	if len(sys.argv) != 3:
		assert False, "[!] Missing argument."

	main(sys.argv[1:])
