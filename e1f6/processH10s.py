#!/usr/bin/python

import sys
import ROOT as r
import getopt
from subprocess import call


def usage():
    print(
        'processH10s [-f] [-w <workingdir>] [-c <parmsfilepath>] [-i <inputstring>] [-N <numtoproc>] [-t treepath] [-o outfile]')
    print('     -f  activate fast count at expense of accurate progress.')


def main(argv):

    parmfile, rootfilepattern, outfile = 'input.parms', '*.root', 'out.root'
    numproc, fastcount = -1, False
    treepath = 'h10'
    wdir = '.'
    bos = False

    try:
        opts, args = getopt.getopt(
            argv, "hc:i:N:ft:o:w:b", ['help', 'config=', 'input='])
    except getopt.GetoptError:
        usage()
        sys.exit(2)

    for opt, arg in opts:
        if opt in ('-h', '--help'):
            usage()
            sys.exit(2)
        elif opt in ('-c', '--config'):
            parmfile = arg
        elif opt in ('-i', '--input'):
            rootfilepattern = arg
        elif opt == '-N':
            numproc = int(arg)
        elif opt == '-t':
            treepath = arg
        elif opt == '-o':
            outfile = arg
        elif opt == '-w':
            wdir = arg
        elif opt == '-f':
            fastcount = True
        elif opt == '-b':
            bos = True

    if bos:
        bash_bos2root = ''' for bosfile in $(ls *.bos)
                            do
                                nt10maker -t1 $bosfile -o${bosfile}.hbook && \
                                h2root ${bosfile}.hbook;
                            done
                        '''
        try:
            retcode = call(bash_bos2root, shell=True)
            if retcode < 0:
                print >>sys.stderr, "Child was terminated by signal", retcode
            else:
                print >>sys.stderr, "Child returned", retcode
        except OSError as e:
            print >>sys.stderr, "Execution failed:", e

    for dep in ['Config.cpp', 'DataHandler.h', 'HandlerChain.cpp', 'H10.C',
                'DH_EC_Hists_PreEid.h', 'DH_RunQuality.cpp', 'DH_EC_Hists.h',
                'DH_CloneH10.h', 'DH_Hists_Monitor.h', 'DH_Eid.h',
                'DH_SC_Hists_PrePid.h', 'DH_CC_Hists.h']:
        r.gROOT.ProcessLine('.L %s/%s++' % (wdir, dep))

    handlers = [("mon_raw", r.DH_Hists_Monitor),
                ("echists_raw", r.DH_EC_Hists_PreEid),
                ("cchists_raw", r.DH_CC_Hists),
                ("runquality", r.DH_RunQuality),
                ("cchists_qskim", r.DH_CC_Hists),
                ("echists_qskim", r.DH_EC_Hists),
                ("mon_qskim", r.DH_Hists_Monitor),
                ("eid", r.DH_Eid),
                ("echists_eskim", r.DH_EC_Hists),
                ("cchists_eskim", r.DH_CC_Hists),
                ("mon_eskim", r.DH_Hists_Monitor),
                ("h10_eskim", r.DH_CloneH10),
                ("scpid_eskim", r.DH_SC_Hists_PrePid)]

    fout = r.TFile(outfile, 'RECREATE')
    chain = r.TChain(treepath)
    chain.Add(rootfilepattern)
    processor = r.H10(chain, parmfile)

    for name, handler in handlers:
        processor.Add(handler(name, fout))
    processor.Loop(numproc, fastcount)


if __name__ == "__main__":
    main(sys.argv[1:])
