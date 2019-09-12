#! /usr/bin/python2.7
import os # get file list
from zplot import *
import glob # file matching
import re # regular expression
import math

# Path e.g., "prefix_hash_1_1_104857600_64.perf"

_benchpath = os.path.dirname(os.path.abspath(__file__))

# High level Variable
_Memtable = ('QSIM', 'MQ-SIM','ZYNQ')
#_Memtable = ('skip_list','cuckoo-tp','cuckoo',)
#_Memtable = ('skip_list','cuckoo-tp',)#'cuckoo',)
#_Threads = ('1','2','4','8','16',)
#_Threads = ('16','8','4','2','1',)
_Threads = ('16',)
_TotalEntires = 104857600
#_Buffsize = ('67108864', '17179869184')
#_Buffsize = ('8589934592',)
#_Buffsize = ('16384')
_Buffsize = ('17179869184',)

# Datatype in .perf file
_Benchmarks = ('SEQ_RD', 'SEQ_WR', 'RAND_RD', 'RAND_WR', 'SKEWED_RD', 'SKEWED_WR')
#_Benchmarks = ('Load_A', 'A', 'B', 'C', 'D', 'F','Load_E','E',)
#_Benchmarks = ('fillseq', )

# For plotting
_header = '# line workload memtable iops\n'
_linedivide = '0 0 0\n'
_Legendtext = {}
for mem in _Memtable:
	_Legendtext[mem] = mem
#	if mem == 'skip_list':
#		_Legendtext[mem]='QSIM'
#	elif mem == 'hash_linkedlist':
#		_Legendtext[mem] = 'HashLinkedList'
#	elif mem == 'prefix_hash':
#		_Legendtext[mem] = 'HashSkipList'
#	elif mem == 'cuckoo':
#		_Legendtext[mem] = 'MQ-SIM'
#	elif mem == 'toss_sync':
#		_Legendtext[mem] = 'TOSS-sync'
#	elif mem == 'toss_async':
#		_Legendtext[mem] = 'TOSS-async'
#	elif mem == 'cuckoo-tp':
#		_Legendtext[mem] = 'ZYNQ'
#
_Normbase = _Memtable[0]

# For Output configuration
_title = '4_blktrace_validation'
_graphextension = 'eps'
_benchdatadir = '4_blktrace_validation'


#######################################################################

def get_ymax(coll_list, t):
	ymax = 0

	for c in coll_list:
		ymax = ymax if t.getmax(c) < ymax else t.getmax(c)

	#if type(ymax)!=type(1): ymax=100000

	ymax = ymax + ymax * 0.1

	round_ymax = 0
	round_scale = 10 if ymax < 100 else 100

	while round_ymax < ymax:
		round_ymax += round_scale

	return round_ymax

# REQUIRE : table_ must have contents about zplot-format data.
# @@@Param
# targetname_ : column name which is included data want to normalize.
# basetarget_ : data value in targetcolumn.
# groupname_ :
def get_norm(basetarget_='ZYNQ',
			 groupname_='workload',
			 normdataname_='iops',
			 table_=table()):

	# Check is there 'norm' ?
	# And Get a index number about basetarget_.
	normbaseindex_ = -1

	for col in table_.cnames:
		if col == 'norm':
			abort('norm column already exists.')
		elif col == normdataname_:
			normbaseindex_ = table_.cnames.index(normdataname_)

	if normbaseindex_ == -1:
		abort('cannot get a baseindex. maybe there is no column !!! %s ' % normdataname_)

	# Make a Grouplist.
	grouplist_ = []
	for gl in table_.query(select='%s' % groupname_):
		if gl not in grouplist_:
			grouplist_.append(gl)

	if grouplist_.__len__() == 0:
		abort('cannot make a grouplist. maybe there is no data in %s column !!! %s ' % groupname_)

	# We must get column name which is included data want to normalize.
	isCol = False
	targetindex_ = 0
	for x in table_.query():
		targetindex_ = 0
		for y in x:
			if y == basetarget_:
				# Gotcha!!
				isCol = True
				break
			targetindex_ +=1
		if isCol == True :
			break

	if isCol == False:
		abort('cannot get a basetarget column. maybe there is no data %s !!! %s ' % basetarget_)

	targetcolumn_ = table_.cnames[targetindex_]


	table_.addcolumn(column='norm')

	for gl in grouplist_:
		#wr = targetname_ + '="%s" AND ' + groupname_ +'="%s"' % (basetarget_, gl)
		wr = "%s='%s' AND %s='%s'" % (targetcolumn_, basetarget_, groupname_, gl[0])
		_basedata = table_.query(where=wr)[0][normbaseindex_]
		wr = "%s='%s'" % (groupname_,gl[0])
		for x in table_.query(where=wr):
			targetdata = x[normbaseindex_]
			normdata = str(round( float(float(targetdata) / float(_basedata)), 1))
			table_.update(set='norm=%s' % normdata,where='rownumber=%s' % x[0])

	return table_


def plot_all_iops_norm_two():
	_div = 5
	ymax = 50000
	for Threads in _Threads:
		for Buffersize in _Buffsize:
			if Threads == '32' and Buffersize == '67108864':
				continue;
			#_extractpath = os.path.join(_benchpath, 'ycsb.' + Threads + '.' + Buffersize + '.dat')
			_extractpath = os.path.join(_benchpath, 'zplot_sample.dat')

			t = table(file=_extractpath, separator=' ')
			t = get_norm('ZYNQ','workload', 'iops', t)

			options = [(_Memtable[0], _Legendtext[_Memtable[0]], 'black', 'dline2', 'black', 0.55, 2.5),
					   (_Memtable[1], _Legendtext[_Memtable[1]], 'red', 'utriangle', 'black', 2, 2),
					   (_Memtable[2], _Legendtext[_Memtable[2]], 'orange', 'hline', 'black', 0.55, 2.4),
					   #(_Memtable[3], _Legendtext[_Memtable[3]], 'darkcyan', 'dline1', 'black', 0.55, 3),
					   #(_Memtable[4], _Legendtext[_Memtable[4]], 'powderblue', 'circle', 'black', 0.55, 3),
					   #(_Memtable[5], _Legendtext[_Memtable[5]], 'orchid', 'square', 'black', 2, 3),
					   ]

			c = canvas(_graphextension, 'zplot_sample', dimensions=['6.2in', '3in'])
			#c = canvas(_graphextension, 'ycsb_' + Threads + '_' + Buffersize + '_all' , dimensions=['9.6in', '2.2in'])

			if Threads == '1':
			 	ymax = 50000
			 	_div = 5
			elif Threads == '16':
				ymax = 200000
				_div = 5

			d = drawable(c,
						 xrange=[0, t.getmax('line') + 1],
						 yrange=[0.1, ymax],
						 coord=[50, 22],
						 dimensions=['5in', '2.5in'],
						 #yscale='log10',
						 )
			w = 'memtable="%s"' % "ZYNQ"

			xym =[]
			for x, y in t.query(select='workload,line', where=w):
				y = str(float(y) + 1)
				xym.append((str(x) , y))

			ym = []
			for y in range(0, _div + 1, 1):
				ym.append((str(y * ((ymax/_div)/1000)), str(y* (ymax/_div))))
				#ym.append((str(y), str(y* (ymax/_div))))

			axis(d,
				 style='xy',
				 linewidth=0.65,
				 xmanual=xym,
				 ymanual=ym,
				 #yauto=[0,ymax,ymax / _div],
				 ticstyle='in',
				 # yauto=[0,ymax,ymax/_div],
				 doxmajortics=False,
				 ytitle='IOPS(x1000)',
				 # ytitleshift=[10,0],
				 #title= 'YCSB_' + Threads + '_' + Buffersize + 'MB',
				 xtitle='Workloads',
				 xlabelfontsize=9.0,
				 ylabelfontsize=9.0,
				 ytitlesize=10.0,
				 xtitlesize=10.0,
				 doxlabels=True,
				 xlabelshift=[0, 0],
				 #xlabelshift=[0, 2],
				 )
			p = plotter()
			L = legend()

			for mem, legtext, bgcol, fillpattern, fillcol, fillsize, fillskip in options:
				w = 'memtable="%s"' % mem
				st = table(table=t, where=w)
				# print st.query()
				# m_iosize=0
				# if st.query(where='iosize="%s"' % '1M').__sizeof__() >=1:
				# 	m_iosize= st.query(where='iosize="%s"' % '1M')[0][4]
				#
				# print st.query(where='iosize="%s"' % '1M')
				barargs = {'drawable': d,
						   'table': st,
						   'xfield': 'line',
						   'yfield': 'iops',
						   'fill': True,
						   'barwidth': 1,
						   'legend': L,
						   #'linewidth': 0,

						   'fillskip': fillskip,
						   'fillsize': fillsize,
						   'fillstyle': fillpattern,
						   'fillcolor': fillcol,
						   'legendtext': legtext,
						   'bgcolor': bgcol,
						   }
				if mem == 'MQ-SIM':
					barargs['labelfield'] = 'norm'
					barargs['labelformat'] = '%sx'
					barargs['labelsize'] = 8.5
					barargs['labelrotate'] = 90
					barargs['labelshift'] = [0, 0]
					#barargs['labelshift'] = [0, -1.5]
					barargs['labelanchor'] = 'l,c'
				elif mem == 'ZYNQ':
					barargs['labelfield'] = 'norm'
					barargs['labelformat'] = '%sx'
					barargs['labelsize'] = 8.5
					barargs['labelrotate'] = 90
					barargs['labelshift'] = [0, 0]
					#barargs['labelshift'] = [0, -1.5]
					barargs['labelanchor'] = 'l,c'
				# elif mem == 'skip_list':
				# 	barargs['labelfield'] = 'iops'
				# 	barargs['labelformat'] = '%s'
				# 	barargs['labelsize'] = 7
				# 	barargs['labelrotate'] = 90
				# 	barargs['labelshift'] = [2, 8]
					#barargs['labelanchor'] = 'l,c'
				# if mem == _Normbase:
				# 	barargs['labelfield'] = 'iops'
				# 	barargs['labelformat'] = '%s'
				# 	barargs['labelsize'] = 7
				# 	barargs['labelrotate'] = 90
				# 	barargs['labelshift'] = [0, -1.5]
				# 	barargs['labelanchor'] = 'l,c'
				# elif sst == _base and m_iosize!=0:
				# 	barargs['labelfield']='iops'
				# 	barargs['labelformat'] = '%sMB'
				# 	barargs['labelsize'] = 5.5
				# 	barargs['labelrotate'] = 90
				# 	barargs['labelshift'] = [1.45,4]


				p.verticalbars(**barargs)
			L.draw(c,
				   coord=[d.left() + 15, d.top()-10],
				   style='right',
				   skipnext=3,
				   skipspace=80,
				   fontsize=8.5,
				   height=10,
				   hspace=1,
				   # boxsize=100,
				   # boxsizey=60,
				   )
			c.render()
			print '[Plot Complete!!] Result file is created at "' + c.output_file + "'"

if __name__ == '__main__':
	#extract_perf()
	#plot_all_iops()
	plot_all_iops_norm_two()
	#plot_all_iops_norm()

