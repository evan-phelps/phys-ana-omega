# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <codecell>

import pandas as pd
pd.set_option('max_columns', 10)
pd.set_option('max_rows', 40)
pd.set_option('line_width', 150)

# <codecell>

df = pd.concat([pd.read_table('../e1f6/e1f_bos_fns.list', header=None),pd.read_table('../e1f6/e16_bos_fns.list', header=None)], ignore_index=True)
df['run'] = df[0].str.split('_').apply(lambda x: int(x[1]))
df['filenum'] = df[0].str.split('.').apply(lambda x: int(x[1].strip('[a-Z]')))
df['exp'] = df.run.apply(lambda x: 'e1f' if x > 37000 else 'e1-6a')
df['msspath'] = df.exp.apply(lambda x: '/mss/clas/%s/production/%s/data'%(x, "pass2/v1" if x=="e1f" else "pass1/v1"))
df.columns = ['filename', 'run', 'filenum', 'exp', 'msspath']

# <codecell>

print(df.head())
print(df.tail())

# <codecell>

filesPerRun = df.groupby(df.run).apply(lambda x: pd.DataFrame(data={'exp':unique(x.exp.values), 'path':unique(x.msspath.values), 'files':', '.join(x.filename.values)}))

#from xml.etree.ElementTree import Element, SubElement, Comment, tostring
from lxml.etree import Element, SubElement, CDATA, tostring
import re

def getxml(exp, pretty=True):
    rootout = 'out.root'
    wdir = '~/omega/src'
    exp2 = exp.lower().replace('-','').replace('a','')
    fpr = filesPerRun[filesPerRun.exp==exp]
    req = Element('Request')
    SubElement(req, 'Project').attrib['name'] = exp
    SubElement(req, 'Track').attrib['name'] = 'analysis'
    SubElement(req, 'Name').attrib['name'] = 'epskim'
    disk = SubElement(req, "DiskSpace")
    disk.attrib['space'] = '80'
    disk.attrib['unit'] = 'GB'
    memory = SubElement(req, "Memory")
    memory.attrib['space'] = '4'
    memory.attrib['unit'] = 'GB'
    reqcom = SubElement(req, 'Command')
    reqcom.text = CDATA('bash -l -c "python %s/processH10s.py -b -o %s -w %s -c %s/%s"'%(wdir,rootout,wdir,wdir,'input.%s.exp.parms'%exp2))
    
    for run, path, files in zip(fpr.index, fpr.path.values, fpr.files.values):
        reqjob = SubElement(req, 'Job')
        for fn in files.split(','):
            fn = fn.strip()
            inpfile = SubElement(reqjob, 'Input')
            inpfile.attrib['src'] = '%s/%s'%('mss:'+path,fn)
            inpfile.attrib['dest'] = '%s.bos'%fn
        outfile = SubElement(reqjob, 'Output')
        outfile.attrib['src'] = rootout
        outfile.attrib['dest'] = '~/w/%d.root'%int(run[0])
    
    return tostring(req, pretty_print=pretty)

# <codecell>

f = open('skim.e16.xml', 'w')
f.write(getxml('e1-6a'))
f.close()

f = open('skim.e1f.xml', 'w')
f.write(getxml('e1f'))
f.close()

# <codecell>


