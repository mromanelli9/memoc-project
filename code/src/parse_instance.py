#!/usr/bin/python
## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

import getopt, sys
import numpy as np
import matplotlib.pyplot as plt
import math
import os.path

def diplay_pcb(N, points):
	x = [item[0] for item in points]
	y = [item[1] for item in points]

	plt.scatter(x, y)
	plt.show()

def distance(p1, p2):
	x1, y1 = p1
	x2, y2 = p2
	return math.sqrt( (x2 - x1)**2 + (y2 - y1)**2 )

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
	fp = open(filename,'r')
	lines = fp.readlines()
	fp.close()

	n = int(lines[5].split(" ")[2])
	t_points = lines[8:-1]
	points = [map(int, e.split(" ")[1:]) for e in t_points]
	del lines
	del t_points

	if N:
		points = points[:N]
		n = N

	print "I got %d points [dim: %d]." % (len(points), n)

	# diplay_pcb(n, points)

	print "Computing distances..."

	data = str(N) + '\n'
	for i in range(0, N):
		for j in range(0, N):
			if i == j:
				d = 0
			else:
				d = round(distance(points[i], points[j]), 2)
			data += str(d) + ' '
		data += '\n'
	data += '\n'

	outputFile = "instances/" + \
				os.path.splitext(os.path.basename(filename))[0] + \
				"_n" + str(N) + ".tsp"
	print "Saving (%s)..." % (outputFile)
	fp = open(outputFile, 'w')
	fp.write(data)
	fp.close()

	print "Done. Exit."
	print

if __name__ == '__main__':
	main(sys.argv[1:])
