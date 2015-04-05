#!/usr/bin/env python

import sys
import matplotlib
matplotlib.use('PDF')
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from matplotlib import mlab
from util import *
from colorpalettes import *

infile = sys.argv[1]
machine = sys.argv[2]
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

# Data, info and preferences about a specific plot
class PlotData:
    def __init__(self, columns, title, outfile,
                 legendpos='best', legendcols=1, 
                 legendbbox=None, legendfontsize='small'):
        self.columns = columns 
        self.title = title
        self.outfile = outfile
        self.legendpos = legendpos
        self.legendcols = legendcols
        self.legendbbox = legendbbox
        self.legendfontsize = legendfontsize

# plots list
plots = [
        PlotData([
            'rd_shrd_1var',
            'wr_shrd_1var',
            'rmw_shrd_1var'
            ], 
            'Memory operations on a shared variable',
            'shared_1var_ops.pdf', 
            'best', 1),
    
        PlotData([
            'rd_priv_1var',
            'wr_priv_1var',
            'rmw_priv_1var'
            ], 
            'Memory operations on a private variable',
            'priv_1var_ops.pdf',
            'best', 1),

        PlotData([
            'rd_priv_1var',
            'wr_priv_1var',
            'rmw_priv_1var',
            'rd_priv_fit_l1',
            'wr_priv_fit_l1',
            'rmw_priv_fit_l1',
            'rd_priv_nofit_llc',
            'wr_priv_nofit_llc',
            'rmw_priv_nofit_llc',
            ],
            'Operations on thread-private data',
            'all_priv.pdf', 
            'best', 2),


        PlotData([
            'rd_shrd_1var',
            #'wr_shrd_1var',
            'rmw_shrd_1var',
            'rd_shrd_1var_misal',
            #'wr_shrd_1var_misal',
            'rmw_shrd_1var_misal',
            #'atomic_rmw_shrd_1var',
            #'atomic_rmw_shrd_1var_misal',
            ],
            'Operations on aligned and misaligned shared variables',
            'al_vs_misal.pdf'),


        PlotData([
            'rd_shrd_1var',
            'wr_shrd_1var',
            'rmw_shrd_1var',
            'rd_shrd_fit_l1',
            'wr_shrd_fit_l1',
            'rmw_shrd_fit_l1',
            'rd_shrd_nofit_llc',
            'wr_shrd_nofit_llc',
            'rmw_shrd_nofit_llc',
            ], 
            'Operations on shared working sets',
            'all_shared.pdf',
            'upper left', 2, (-0.01, 1), 'x-small'),

        PlotData([
            'rd_priv_1var',
            'wr_priv_1var',
            'rmw_priv_1var',
            'atomic_rmw_priv_1var',
            ],
            'Private RMW operations',
            'priv_1var_ops_plus_atomic.pdf',
            'best', 1),

        PlotData([
            'rd_shrd_1var',
            'wr_shrd_1var',
            'rmw_shrd_1var',
            'atomic_rmw_shrd_1var',
            ],
            'Shared RMW operations',
            'shared_1var_ops_plus_atomic.pdf',
            'best', 1),


        PlotData([
            'rmw_shrd_1var',
            'rmw_shrd_fit_l1',
            'rmw_shrd_nofit_llc',
            'atomic_rmw_shrd_1var',
            'atomic_rmw_shrd_fit_l1',
            'atomic_rmw_shrd_nofit_llc'
            ],
            'RMWs on shared working sets',
            'all_rmw_shared.pdf', 
            'upper left', 1, (-0.01, 0.81), 'x-small'),

        PlotData([
            'rmw_priv_fit_l1',
            'rmw_shrd_fit_l1',
            'atomic_rmw_priv_fit_l1',
            'atomic_rmw_shrd_fit_l1'
            ],
            'Shared and private RMWs scalability fitting in L1',
            'priv_vs_shrd_fit_l1.pdf'),

        PlotData([
            'wr_priv_1var',
            'mfence_wrmem',
            'mfence_lockadd_wrmem'
            ],
            'Memory fence performance on single variable writes',
            'fences_1var.pdf'),
  
        PlotData([
            'mfence_reg',
            'mfence_lockadd_reg'
            ],
            '(almost) Raw memory fence performance',
            'fences_reg.pdf'),
   
        PlotData([
            'rmw_priv_1var',
            'mfence_rmwmem_1var',
            'mfence_lockadd_rmwmem_1var'
            ],
            'Memory fence performance on single variable RMWs',
            'fences_rmw_1var.pdf'),

        PlotData([
            'rmw_priv_nofit_llc',
            'mfence_rmwmem_nofit_llc',
            'mfence_lockadd_rmwmem_nofit_llc'
            ],
            'Memory fence performance on LLC missing RMWs',
            'fences_rmw_llc_miss.pdf'),

]

# Create markers according to column name
def create_marker(column):
    if column.startswith('rd_'):
        return 'v-'
    elif column.startswith('wr_'):
        return '^-'
    elif column.startswith('rmw_'):
        return 's-'
    elif column.startswith('atomic_rmw_'):
        return 'o-'
    elif column.startswith('mfence_lockadd'):
        return 'p-'
    elif column.startswith('mfence_'):
        return 'D-'
    else:
        return 'x-'

def get_markerfacecolor(column):
    if column.endswith('nofit_llc'):
        return 'white'
    else:
        return color_map[col]

def get_fillstyle(column):
    if column.endswith('fit_l1'):
        return 'left'
    elif column.endswith('misal'):
        return 'bottom'
    else:
        return 'full'

# Create colors according to column name 
color_map  =  {
                 'rd_shrd_1var':pal[27], 
                 'rd_shrd_1var_misal':pal[27], 
                 'rd_shrd_fit_l1':pal[27], 
                 'rd_shrd_nofit_llc':pal[27], 

                 'rd_priv_1var':pal[38],
                 'rd_priv_fit_l1':pal[38],
                 'rd_priv_nofit_llc':pal[38],

                 'wr_priv_1var':pal[37],
                 'wr_priv_fit_l1':pal[37],
                 'wr_priv_nofit_llc':pal[37],

                 'wr_shrd_1var':pal[25],
                 'wr_shrd_1var_misal':pal[25],
                 'wr_shrd_fit_l1':pal[25],
                 'wr_shrd_nofit_llc':pal[25],
                 
                 'rmw_shrd_1var':pal[73],
                 'rmw_shrd_1var_misal':pal[73],
                 'rmw_shrd_fit_l1':pal[73],
                 'rmw_shrd_nofit_llc':pal[73],

                 'rmw_priv_1var':pal[35],
                 'rmw_priv_fit_l1':pal[35],
                 'rmw_priv_nofit_llc':pal[35],

                 'atomic_rmw_shrd_1var':pal[45],
                 'atomic_rmw_shrd_fit_l1':pal[45],
                 'atomic_rmw_shrd_nofit_llc':pal[45],

                 'atomic_rmw_priv_1var':pal[25],
                 'atomic_rmw_priv_fit_l1':pal[25],
                 'atomic_rmw_priv_nofit_llc':pal[25],

                 'mfence_wrmem':pal4[0],
                 'mfence_lockadd_wrmem':pal4[1],
                 'mfence_reg':pal4[2] ,
                 'mfence_lockadd_reg':pal4[3],
                 'mfence_rmwmem_1var':pal15[0],
                 'mfence_lockadd_rmwmem_1var':pal15[1],
                 'mfence_rmwmem_nofit_llc':pal15[2],
                 'mfence_lockadd_rmwmem_nofit_llc':pal15[3],

               }

 
# Create colors according to column name 
marker_map  =  {  
                 'rd_shrd_1var':'v-',
                 'rd_shrd_1var_misal':'v-',
                 'rd_shrd_fit_l1':'v-',
                 'rd_shrd_nofit_llc':'v-',

                 'rd_priv_1var':'<-',
                 'rd_priv_fit_l1':'<-',
                 'rd_priv_nofit_llc':'<-',

                 'wr_priv_1var':'>-',
                 'wr_priv_fit_l1':'>-',
                 'wr_priv_nofit_llc':'>-',

                 'wr_shrd_1var':'s-',
                 'wr_shrd_1var_misal':'s-',
                 'wr_shrd_fit_l1':'s-',
                 'wr_shrd_nofit_llc':'s-',
                 
                 'rmw_shrd_1var':'o-',
                 'rmw_shrd_1var_misal':'o-',
                 'rmw_shrd_fit_l1':'o-',
                 'rmw_shrd_nofit_llc':'o-',

                 'rmw_priv_1var':'D-',
                 'rmw_priv_fit_l1':'D-',
                 'rmw_priv_nofit_llc':'D-',

                 'atomic_rmw_shrd_1var':'>-',
                 'atomic_rmw_shrd_fit_l1':'>-',
                 'atomic_rmw_shrd_nofit_llc':'>-',

                 'atomic_rmw_priv_1var':'v-',
                 'atomic_rmw_priv_fit_l1':'v-',
                 'atomic_rmw_priv_fit_l1':'v-',

                 'mfence_wrmem':'H-',
                 'mfence_lockadd_wrmem':'+-',
                 'mfence_reg':'x-' ,
                 'mfence_lockadd_reg':'h-',
                 'mfence_rmwmem_1var':'H-',
                 'mfence_lockadd_rmwmem_1var':'+-',
                 'mfence_rmwmem_nofit_llc':'*-',
                 'mfence_lockadd_rmwmem_nofit_llc':'^-',
               }

                
i = 1
for p in plots:
    #New figure for each plot
    plt.figure(i)
    i = i+1

    #list with all curves in the plot
    curves = []
    for col in p.columns:
        c = plt.plot( a.nthreads, 
                      a[col],
                      marker_map[col],
                      color = color_map[col],
                      markeredgecolor = color_map[col],
                      fillstyle = get_fillstyle(col),
                      markerfacecolor = get_markerfacecolor(col),
                      markerfacecoloralt = 'white',
                      linewidth=2, 
                      markersize=7)
        curves.append(c)
     
    #plot legend
    plt.legend( curves, map(str.upper, p.columns),
                loc=p.legendpos, 
                ncol=p.legendcols, 
                columnspacing=0.4,
                bbox_to_anchor=p.legendbbox )
    leg = plt.gca().get_legend()
    ltext = leg.get_texts()
    llines = leg.get_lines()
    plt.setp(ltext, fontsize=p.legendfontsize)
    plt.setp(llines, linewidth=1.5)
   
    #plot axes
    r = plt.axis()
    plt.axis([0.5, maxthreads+0.5, 0, r[3]])
    plt.axes().yaxis.grid(True)
    plt.ylabel('Avg. cycles per operation')
    plt.xlabel('#Threads')
    plt.title(p.title + ' (' + machine + ')', fontsize='medium')
    
    #plt.show()
    plt.savefig(machine + '_' + p.outfile)

