#!/usr/bin/env python
## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: t; tab-width: 4; coding: utf-8; -*-
#title           : gen_instances.py
#description     : This script will create instances from a real TSP file probem
#					provided in the VLSI data sets by Andre Rohe.
#					The instances will have this format: bcl380_n[$i].tsp,
#					with $i within the range 5-<limit>.
#author          : Marco Romanelli
#date            : 30/05/2017
#version         : 1
#usage           : python gen_instances.py -f <instanceName> [-n <limit>]
#python_version  : 2.7
#==============================================================================


import getopt, sys
import numpy as np
# import matplotlib.pyplot as plt	# enable this to show a PCB
import math
import os.path
import random


def diplay_pcb(N, points):
	""" Create a window and show graphically where
	the points are placed.
    """
	x = [item[0] for item in points]
	y = [item[1] for item in points]

	plt.scatter(x, y)
	plt.show()

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

def main(argv):
	""" Main function. """

	# Handle parameters
	try:
		(opts, args) = getopt.getopt(argv, "f:n:")
	except getopt.GetoptError as err:
		# print help information and exit:
		print "parse_istance.py -f <instanceName> [-n <limit>]"
		print str(err)  # will print something like "option -a not recognized"
		sys.exit(2)

	filename = None
	N = None
	step = 5

	for o, a in opts:
		if o == "-f":
			filename = a
		elif o == "-n":
			N = int(a)
		else:
			assert False, "unhandled option"

	# Parse a file
	print "Parsing file '%s'..." % filename
	points = readPoints(filename)
	print "I got %d points." % len(points)

	# Display the PCB
	# diplay_pcb(N, points)

	print "Starting instances generation (%d different instances with step %d)." % (N, step)

	# Generate instances iteratively
	i = 1
	instances = []
	p = None
	for itr in xrange(step, N+step, step):
		print "Generating instance #%d with %d points." % (i, itr)
		p = heuristic(points, itr)
		instances.append(p)
		i += 1

	# Compute distance for each pair of points
	# in each instance
	print "Computing distances..."
	for i, current in enumerate(instances):
		print "Working on instance #%d." % (i+1)

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
					"_n" + str(n) + ".tsp"
		print "\tSaving (%s)..." % (outputFile)
		fp = open(outputFile, 'w')
		fp.write(data)
		fp.close()

	# Exit
	print "Done. Exit."
	print

if __name__ == '__main__':
	main(sys.argv[1:])
