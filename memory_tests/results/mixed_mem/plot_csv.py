#!/usr/bin/env python

import sys
import matplotlib
matplotlib.use('PDF')
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import mlab
from util import *

infile = sys.argv[1]
f1 = open(infile, 'r')
csvfile = infile + '.csv'
f2 = open(csvfile, 'w')

maxthreads = 0
linearray = []
ops = []
for line in f1.readlines():
    if 'nthreads:' in line:
        linearray.append(line.strip())
        maxthreads=max(maxthreads,int(get_val(line,'nthreads:')))
        cur_op = get_val(line,'op:')
        if cur_op not in ops:
            ops.append(cur_op)

#print header
f2.write('nthreads ' + ' '.join(ops) + " \n")

#print column values
for thread in range(1, maxthreads+1):
    f2.write(str(thread) + " ")
    for op in ops:
        for line in linearray:
            if ('nthreads:' + str(thread) + ' ') in line and ('op:' + op + ' ') in line:
                f2.write(get_val(line,'cycles:') + ' ')
    f2.write('\n')     

f2.close()
#########################################################################          

a = mlab.csv2rec(csvfile, delimiter=' ')

## SHORT version
selected_cols = (
'rmw_only_shrd_1var',
#'delay1',
#'delay1_rmw_shrd_1var',
'delay3',
'delay3_rmw_shrd_1var',
'delay10',
'delay10_rmw_shrd_1var',
#'wr_only_shrd_1var',
#'delay1_wr_shrd_1var',
#'delay3_wr_shrd_1var',
#'delay10_wr_shrd_1var',
)

# Create markers according to column name
def create_marker(column):
    if column.find('wr_') >= 0:
        return '^-'
    elif column.find('rmw_') >= 0:
        return 's-'
    else:
        return 'o-'

# Create color according to column name
def create_color(column):
    if column.startswith('delay10'):
        return 'green'
    elif column.startswith('delay3'):
        return 'orange'
    elif column.startswith('delay1'):
        return 'blue'
    else:
        return 'black'



curves = []
for col in selected_cols:
    c = plt.plot(a.nthreads, a[col], create_marker(col),
            linewidth=2, color=create_color(col) )
    curves.append(c)

plt.legend( curves, map(str.upper, selected_cols), loc='best' )

leg = plt.gca().get_legend()
ltext = leg.get_texts()
llines = leg.get_lines()
plt.setp(ltext, fontsize='small')
plt.setp(llines, linewidth=2)

r = plt.axis()
plt.axis([0.5, maxthreads+0.5, 0, r[3]])
plt.grid(True)

plt.ylabel('Avg. cycles per operation')
plt.xlabel('#Threads')
plt.title('Delay = PAUSE instruction')

plt.show()
plt.savefig('pause.pdf')
