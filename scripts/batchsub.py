#!/apps/python/PRO/bin/python
#/usr/bin/python2
#/apps/python/PRO/bin/python
#/apps/python/PRO/bin/python

import sys
import os
import argparse
import collections
import re
from string import lstrip
from glob import glob
from xml.etree.cElementTree import Element, ElementTree, tostring
from xml.dom.minidom import parseString
from xml.sax.saxutils import unescape
from os.path import basename

class Job:
    def __init__(self):
        #batch job attributes
        self.jobname = 'epskim'
        self.project = 'E1F'
        self.os = 'centos62'
        self.track = 'analysis'
        self.space = 15360             #in megabytes (MB)
        self.memory = 1024            #in megabytes (MB)
        self.time = 30              #in minutes
        self.command = None
        self.infiles = {}
        self.outfiles = {}
        self.joblist = []
        self.isgrouped = False
        self.rundict = {}
        #directories
        self.dirscripts = os.getcwd()
        self.dirout = '/home/ephelps/w/data/main'
        self.dirin = os.getcwd()

    def addscript(self, fn, batchfn = None):
        if batchfn is None: batchfn = fn
        self.infiles[self.dirscripts+'/'+fn] = batchfn

    def addinfile(self, fn, batchfn = None):
        if batchfn is None: batchfn = fn
        self.infiles[self.dirin+'/'+fn] = batchfn

    def addoutfile(self, fn, batchfn = None):
        if batchfn is None: batchfn = fn
        self.outfiles[batchfn] = self.dirout+'/'+fn
        
    def make_xml(self):
        ereq = Element('Request')
        eproject = Element('Project')
        eproject.attrib['name'] = self.project
        etrack = Element('Track')
        etrack.attrib['name'] = self.track
        ejobname = Element('Name')
        ejobname.attrib['name'] = self.jobname
        eos = Element('OS')
        eos.attrib['name'] = self.os
        espace = Element('DiskSpace')
        espace.attrib['unit'] = 'MB'
        espace.attrib['space'] = str(self.space)
        ememory = Element('Memory')
        ememory.attrib['unit'] = 'MB'
        ememory.attrib['space'] = str(self.memory)
        etime = Element('TimeLimit')
        etime.attrib['unit'] = 'minutes'
        etime.attrib['time'] = str(self.time)
        ecommand = Element('Command')
        ecommand.text = "<![CDATA[\n"+self.command+"\n]]>"

        eheader = [ eproject, etrack, ejobname, eos,
                    espace, ememory, etime, ecommand ]
        for e in eheader:
            ereq.append(e)

        #        for j in self.joblist:
        ejob = Element('Job')
        
        for src, dest in self.infiles.iteritems():
            einfile = Element('Input')
            einfile.attrib['src'] = src
            einfile.attrib['dest'] = dest
            ejob.append(einfile)

        for src, dest in self.outfiles.iteritems():
            eoutfile = Element('Output')
            eoutfile.attrib['src'] = src
            eoutfile.attrib['dest'] = dest
            ejob.append(eoutfile)

        if self.joblist:
            if self.isgrouped:
                self.groupbyrun()
                for rn, fns in self.rundict.items():
                    #print rn, fns
                    rnjob = Element('Job')
                    eoutfile = Element('Output')
                    eoutfile.attrib['src'] = 'out.root'
                    eoutfile.attrib['dest'] = "%s/%s.root" % (self.dirout,rn)

                    scriptfile = Element('Input')
                    scriptfile.attrib['src'] = '%s/%s' % (self.dirscripts,'run.omega')
                    scriptfile.attrib['dest'] = 'run.omega'

                    exefile = Element('Input')
                    exefile.attrib['src'] = '%s/%s' % (self.dirscripts,'omega')
                    exefile.attrib['dest'] = 'omega'

                    rnjob.append(scriptfile)
                    rnjob.append(exefile)
                    for fn in fns:
                        einfile = Element('Input')
                        einfile.attrib['src'] = "%s/%s" % (self.dirin,fn)
                        einfile.attrib['dest'] = fn
                        rnjob.append(einfile)
                    rnjob.append(eoutfile)
                    ereq.append(rnjob)
            else:
                #print 'isNOTgrouped!'
                elist = Element('List')
                elist.attrib['name'] = 'basename'
                elist.text = ' '.join(self.joblist)
                eforeach = Element('ForEach')
                eforeach.attrib['list'] = 'basename'
                eforeach.append(ejob)
                ereq.append(elist)
                ereq.append(eforeach)
        else:
            ereq.append(ejob)
        
        return ereq

    def groupbyrun(self):
        for s in self.joblist:
            rn = '00000'
            #get substrings that contain 5 and only 5 consecutive digits
            tmpstr = re.findall('\D*\d{5}\D',s)
            #get the five digits
            if tmpstr: rn = re.findall('\d{5}',tmpstr[0])[0]
            if rn not in self.rundict: self.rundict[rn] = []
            if s not in self.rundict[rn]: self.rundict[rn].append(s)        
    def test(self):
        if self.command is None:
            self.command = './test.sh > outfile'
            self.addscript('batchruntest.sh','test.sh')
        if not self.joblist:
            self.joblist = [ 'test1', 'test2', 'test3' ]
            self.addoutfile('$\{basename.root\}','outfile')
        e = self.make_xml()
        prettyPrint(e)

def prettyPrint(element):
    txt = tostring(element)
    print unescape(parseString(txt).toprettyxml())

def main(argv=None):
    if argv is None:
        argv = sys.argv
    
    parser = argparse.ArgumentParser(description="Generates XML for the JLab batch farm, e.g., 'jsub -xml <xmlfile>'.\n\nExample:\n\tbatchsub.py --datadir=mss:/home/ephelps/testdata -e /full/path/to/script --data_local=GSIM.OUT.A00 -l root_*.root")
    parser.add_argument('--test', action='store_true', dest='istest',
                        help='Generates test xml.')
    parser.add_argument('--datadir=', action='store', dest='data_dir',
                        help='Sets the directory where the input files are located. Default is present working directory.')
    parser.add_argument('-l', action='store_true', dest='isls',
                        help='Generates input file list from directory listing that corresponds to DATA_DIR/DATA_FILE, where DATA_FILE can be a regular expression.')
    parser.add_argument('--group-by-run', action='store_true', dest='isgrouped',
                        help='Groups data files into jobs based on run number, which is assumed to be a five-digit sequence within the filename')
    parser.add_argument('--data_local=', action='store', dest='data_local',
                        help='Specifies the name of the data file in the batch environment. This is helpful when a configuration file has a static input file name, such as with gsim or user_ana.')
    parser.add_argument('data_file', nargs='+',
                        help='Sets the list of the input data files to be processed.')
    parser.add_argument('-e', action='store', nargs=1, dest='command',
                        help='Command to be executed.')
    results = parser.parse_args()

    job = Job()
    
    if results.data_dir:
        job.dirin = results.data_dir
    if results.command:
        job.command = results.command[0]
    if results.data_file:
        if results.isgrouped:
            job.group = True
        if results.isls:
            job.joblist = [ os.path.basename(fullpath) for fullpath in glob(lstrip(job.dirin,'mss:')+'/'+results.data_file[0]) ]
            if not job.joblist:
                raise parser.error('No files matching \"'+lstrip(job.dirin,'mss:')+'/'+results.data_file[0]+'\"!')
        else:
            job.joblist = results.data_file
        job.addoutfile('${basename}.root','outfile')
        if not results.data_local: results.data_local = '${basename}'
        job.addinfile('${basename}',results.data_local)
    if results.isgrouped:
        #print 'add "isgrouped" functionality'
        job.isgrouped = True
    if results.istest:
        job.test()
    else: prettyPrint(job.make_xml())
   
if __name__ == '__main__':
    sys.exit(main())
