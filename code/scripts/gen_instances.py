#!/usr/bin/env python
## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: t; tab-width: 4; coding: utf-8; -*-
#title           : gen_instances.py
#description     : This script will create instances from a real TSP file probem
#					provided in the VLSI data sets by Andre Rohe.
#author          : Marco Romanelli
#date            : 03/05/2017
#version         : 2
#usage           : python gen_instances.py -f <instanceName> [-n <limit>]
#python_version  : 2.7
#==============================================================================


import getopt, sys
import numpy as np
import math
import os.path
import random
from os import walk


def distance(p1, p2):
	""" Compute the distance between two point. """
	x1, y1 = p1
	x2, y2 = p2
	return math.sqrt( (x2 - x1)**2 + (y2 - y1)**2 )


def readPoints(filename):
	""" Read a file with path "filename" and return
	a list of points, eg:
	[[pt1_x,pt1_y],[pt2_x, pt2_y],...]
	"""
	fp = open(filename,'r')
	lines = fp.readlines()
	fp.close()

	# Read the length (= number of points)
	n = int(lines[5].split(" ")[2])

	# Skip the first 8 lines and the last one
	t_points = lines[8:-1]
	points = [map(int, e.split(" ")[1:]) for e in t_points]
	del lines
	del t_points

	return points


def heuristic(points, n):
	""" This function takes a list of points
	and a number n and return a smallers list
	of n elements randomly taken.
	"""

	# If no limit is provided, return the entire list
	if n == None:
		return points

	loop = True
	p = None

	# Loop until a "valid" list of point is founded
	while (loop):
		loop = False

		# select randomly a segment of the list
		r = random.randrange(len(points) - n)
		p = points[r:(r+n)]

		# The new list of points must have at least two element
		# with different x (or y) coordinate
		if len(set([el[0] for el in p])) <= 2:
			loop = True
		if len(set([el[1] for el in p])) <= 2:
			loop = True

	points = p
	return points

def generate_from_vlsi(filename, step=5, limit=100):
	""" Generate a set of instances from one original vlsi file.
		Instances will have the form:
	 		<vlsi-filename>_n<N>.tsp
		where N:
			<step> <= N <= <limit>
			with step of <step>
	"""

	# Parse a file
	print "Parsing file '%s'..." % filename
	points = readPoints(filename)
	print " I got %d points." % len(points)

	print " Starting instances generation..."

	# Generate instances iteratively
	i = 1
	instances = []
	p = None
	for itr in range(step, limit + step, step):
		print "  Generating instance #%d with %d points." % (i, itr)
		p = heuristic(points, itr)
		instances.append(p)
		i += 1

	# Compute distance for each pair of points
	# in each instance
	print " Computing distances..."
	for i, current in enumerate(instances):
		print "  Working on instance #%d." % (i+1)

		n = len(current)
		data = str(n) + '\n'
		for i in range(0, n):
			for j in range(0, n):
				if i == j:
					d = 0
				else:
					d = round(distance(current[i], current[j]), 2)
				data += str(d) + ' '
			data += '\n'
		data += '\n'

		outputFile = "instances/" + \
					os.path.splitext(os.path.basename(filename))[0] + \
					"_n" + str(n).zfill(3) + ".tsp"
		print "  Saving (%s)..." % (outputFile)
		fp = open(outputFile, 'w')
		fp.write(data)
		fp.close()

	print


def main(argv):
	if (len(sys.argv)) != 2:
		print "[!] Missing parameters."
		sys.exit(-1)

	folder = str(sys.argv[1])

	# Parameters
	generation_step = 10
	generation_limit = 150

	# Run over all vlsi-dataset folder
	for el in os.listdir(folder):
		ext = os.path.splitext(el)[-1].lower()
		if (ext == ".tsp" and (not el.startswith('.'))):
			generate_from_vlsi( folder + el, generation_step, generation_limit)

	# Exit
	print "Done. Exit."
	print

if __name__ == '__main__':
	main(sys.argv[1:])
