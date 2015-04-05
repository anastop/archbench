#!/usr/bin/env python

import sys
import matplotlib
#matplotlib.use('PDF')
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
#'rd_priv_1var',
#'rd_priv_1var_misal',
#'rd_priv_fit_l1',
#'rd_priv_nofit_llc',
'rd_shrd_1var',
#'rd_shrd_1var_misal',
#'rd_shrd_fit_l1',
#'rd_shrd_nofit_llc',
#'wr_priv_1var',
#'wr_priv_1var_misal',
#'wr_priv_fit_l1',
#'wr_priv_nofit_llc',
'wr_shrd_1var',
#'wr_shrd_1var_misal',
#'wr_shrd_fit_l1',
#'wr_shrd_nofit_llc',
#'rmw_priv_1var',
#'rmw_priv_1var_misal',
#'rmw_priv_fit_l1',
#'rmw_priv_nofit_llc',
'rmw_shrd_1var',
#'rmw_shrd_1var_misal',
#'rmw_shrd_fit_l1',
#'rmw_shrd_nofit_llc',
#'atomic_rmw_priv_1var',
#'atomic_rmw_priv_1var_misal',
#'atomic_rmw_priv_fit_l1',
#'atomic_rmw_priv_nofit_llc',
#'atomic_rmw_shrd_1var',
#'atomic_rmw_shrd_1var_misal',
#'atomic_rmw_shrd_fit_l1',
#'atomic_rmw_shrd_nofit_llc',
#'mfence_reg', 
#'mfence_wrmem', 
#'mfence_rmwmem_1var', 
#'mfence_rmwmem_nofit_llc', 
#'mfence_lockadd_reg', 
#'mfence_lockadd_wrmem', 
#'mfence_lockadd_rmwmem_1var', 
#'mfence_lockadd_rmwmem_nofit_llc', 
)

curves = []
for col in selected_cols:
    c = plt.plot(a.nthreads, a[col], 'o-', linewidth=2)
    curves.append(c)

plt.legend( curves, map(str.upper, selected_cols), loc='best' )


## VERBOSE version
"""
l = plt.plot( 
        a.nthreads, a.wr_priv_1var,'o-',
        a.nthreads, a.wr_priv_1var_misal,'o-',
        a.nthreads, a.wr_priv_fit_l1,'o-',
        a.nthreads, a.wr_priv_nofit_llc,'o-',
        a.nthreads, a.wr_shrd_1var,'o-',
        a.nthreads, a.wr_shrd_1var_misal,'o-',
        a.nthreads, a.wr_shrd_fit_l1,'o-',
        a.nthreads, a.wr_shrd_nofit_llc,'o-',
        linewidth=2)

plt.legend( l, 
       (
         'WR-PRIV-1VAR',
         'WR-PRIV-1VAR-MISAL', 
         'WR-PRIV-FIT-L1',
         'WR-PRIV-NOFIT-LLC',
         'WR-SHRD-1VAR',
         'WR-SHRD-1VAR-MISAL', 
         'WR-SHRD-FIT-L1', 
         'WR-SHRD-NOFIT-LLC',
         ),
         loc='best'
         )
"""

   

leg = plt.gca().get_legend()
ltext = leg.get_texts()
llines = leg.get_lines()
plt.setp(ltext, fontsize='small')
plt.setp(llines, linewidth=2)

r = plt.axis()
plt.axis([0.5, maxthreads+0.5, 0, r[3]])
#plt.grid(True)

plt.ylabel('Avg. cycles per operation')
plt.xlabel('#Threads')
#plt.title('Performance')

plt.show()
#plt.savefig('myfig.pdf')
