#!/usr/bin/env python
## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: t; tab-width: 4; coding: utf-8; -*-
#title           : enhance_csv.py
#description     : This script takes as input the results.csv file generated
#					by the program main and create a xls file
#					with original data and field with formula
#					in order to make future analysis less expensive.
#author          : Marco Romanelli
#date            : 06/05/2017
#version         : 1 (NB: alpha version!)
#usage           : python enhance_csv.py -f <file.csv>
#python_version  : 2.7
#==============================================================================

import getopt, sys
from os.path import isfile
import csv
from numpy import mean


def readCSV(filename):
	try:
		fp = open(filename)
		data = csv.DictReader(fp, delimiter=',', quoting=csv.QUOTE_ALL)
		return data
    except csv.Error as e:
		assert False, "[!] file %s, line %d: %s" % (filename, reader.line_num, e)

def writeCSV(data, filename):
	with open(filename, 'w') as fp:
		wr = csv.writer(fp, quoting=csv.QUOTE_ALL)

		# Write keys
		wr.writerow([k for k in data[0].keys()])

		# Write values
		for r in data:
			wr.writerow([v for v in r.values()])

def compute_mean_data(l, field, by):
	new_data = []
	times_data = []
	previous_value = l[0][by];	# init this value with the value in the first row

	for row in l:
		# if current row has an instance with the same "dimension" field
		# as the previous one, just append values,
		# otherwise compute mean value
		if row[by] == previous_value:
			times_data.append(float(row[field]))
		else:
			nrow = {by : previous_value,
					field : mean(times_data)}
			new_data.append(nrow)

			# update the list
			times_data = [float(row[field])]

			# Update the new value
			previous_value = row[by]

	# Update with the last value
	nrow = {by : previous_value,
			field : mean(times_data)}
	new_data.append(nrow)

	writeCSV(new_data, field.replace(' ', '_') + "_mean.csv")

def compute_gap(data1, data2):
	list1 = sorted(data1, key=lambda d: str(d["Instance"]))
	list2 = sorted(data2, key=lambda d: str(d["Instance"]))

	if len(list1) != len(list2):
		assert False, "[!] Different number of rows."

	new_data = []
	for i in range(0, len(list1)):
		if list1[i]["Instance"] != list2[i]["Instance"]:
			assert False, "[!] Different instance in row number %d." % (i + 1)

		# Compute GAP
		if list1[i]["CPLEX Sol."] != "-1":
			cplexSol = float(list1[i]["CPLEX Sol."])
			gaSol = float(list2[i]["GA Sol"])
			gap = (1 - (gaSol/ (cplexSol * 1.0))) * 100
		else:
			gap = ""

		c = {
			"Instance" : list1[i]["Instance"],
			"Dimension": list1[i]["Dimension"],
			"GAP":	gap
			}

		new_data.append(c)

	writeCSV(new_data, "gap.csv")

def main():
	try:
		opts, args = getopt.getopt(sys.argv[1:], "f:d:", ["cplex", "ga"])
	except getopt.GetoptError as err:
		assert False, "[!] %s." % str(err)

	# store the parameter
	csv_file = None
	csv_files = []
	field = None

	for o, a in opts:
		if o == "-f":
			# check if a is a string
			csv_file = a
		elif o == "-d":
			csv_files = a.split(',')
		elif o == "--cplex":
			field = "CPLEX Time"
		elif o == "--ga":
			field = "GA Time"
		else:
			assert False, "[!] Unhandled option."


	if csv_file:
		data = readCSV(csv_file)

		# sort by "Dimension" column
		sortedlist = sorted(data, key=lambda d: int(d["Dimension"]))

		compute_mean_data(sortedlist, field, "Dimension")
	else:
		data1 = readCSV(csv_files[0])
		data2 = readCSV(csv_files[1])

		compute_gap(data1, data2)


	# Exit
	print "Done. Exit."
	print


if __name__ == '__main__':
	main()
