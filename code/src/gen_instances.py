#!/usr/bin/python
## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

import getopt, sys
import numpy as np
import matplotlib.pyplot as plt
import math
import os.path
import random

def diplay_pcb(N, points):
	x = [item[0] for item in points]
	y = [item[1] for item in points]

	plt.scatter(x, y)
	plt.show()

def distance(p1, p2):
	x1, y1 = p1
	x2, y2 = p2
	return math.sqrt( (x2 - x1)**2 + (y2 - y1)**2 )

def readPoints(filename, N=None):
	fp = open(filename,'r')
	lines = fp.readlines()
	fp.close()

	n = int(lines[5].split(" ")[2])
	t_points = lines[8:-1]
	points = [map(int, e.split(" ")[1:]) for e in t_points]
	del lines
	del t_points

	if N:
		return points[:N], N
	else:
		return points, n

def crop(p, times=1):
	points = p
	itr = 1
	while (itr <= times):
		print "Crop"
		x_list = [el[0] for el in points]
		y_list = [el[1] for el in points]

		x_max = max(x_list)
		y_max = max(y_list)
		x_half = math.floor(x_max / 2)
		y_half = math.floor(y_max / 2)

		quadrant = random.randrange(4)+1
		x_min, y_min = None, None
		if quadrant == 1:
			x_min = x_half
			y_min = y_half
		elif quadrant == 2:
			x_min = 0
			y_min = y_half
			x_max = x_half
		elif quadrant == 3:
			x_min = 0
			x_max = x_half
			y_min = 0
			y_max = y_half
		elif quadrant == 4:
			x_min = x_half
			y_min = 0
			y_max = y_half

		new_points_x = [(el - x_min) for el in x_list if (el <= x_max and el >= x_min)]
		new_points_y = [(el - y_min) for el in y_list if (el <= y_max and el >= y_min)]
		points = zip(new_points_x, new_points_y)
		itr += 1

	return points

def reduce(points, n=None):
	if n == None:
		return points

	p = points
	ub = n + (n/100*10)

	while (len(p) >= n):
		print "Riduco..."
		p = crop(p)

	if len(p) > n:
		p = p[:n]

	return p

def remove_duplicate(l):
	return list(set(l))


def main(argv):
	try:
		(opts, args) = getopt.getopt(argv, "f:n:")
	except getopt.GetoptError as err:
		# print help information and exit:
		print "parse_istance.py -f <instanceName> [-n <limit>]"
		print str(err)  # will print something like "option -a not recognized"
		sys.exit(2)

	filename = None
	N = None

	for o, a in opts:
		if o == "-f":
			filename = a
		elif o == "-n":
			N = int(a)
		else:
			assert False, "unhandled option"


	print "Parsing file '%s'..." % filename
	points, N = readPoints(filename, N)

	print "I got %d points." % N

	# diplay_pcb(N, points)

	points = crop(points, 5)

	print len(points)
	point = remove_duplicate(points)
	print points, len(points)

	diplay_pcb(len(points), points)


	# print "Computing distances..."
	#
	# data = str(N) + '\n'
	# for i in range(0, N):
	# 	for j in range(0, N):
	# 		if i == j:
	# 			d = 0
	# 		else:
	# 			d = round(distance(points[i], points[j]), 2)
	# 		data += str(d) + ' '
	# 	data += '\n'
	# data += '\n'
	#
	# outputFile = "instances/" + \
	# 			os.path.splitext(os.path.basename(filename))[0] + \
	# 			"_n" + str(N) + ".tsp"
	# print "Saving (%s)..." % (outputFile)
	# fp = open(outputFile, 'w')
	# fp.write(data)
	# fp.close()

	print "Done. Exit."
	print

if __name__ == '__main__':
	main(sys.argv[1:])
