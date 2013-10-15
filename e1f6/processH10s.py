#!/usr/bin/python

import sys
import ROOT as r
import getopt


def usage():
    print(
        'processH10s [-f] [-w <workingdir>] [-c <parmsfilepath>] [-i <inputstring>] [-N <numtoproc>] [-t treepath] [-o outfile]')
    print('     -f  activate fast count at expense of accurate progress.')


def main(argv):

    parmfile, rootfilepattern, outfile = 'input.parms', '*.root', 'out.root'
    numproc, fastcount = -1, False
    treepath = 'h10'
    wdir = '.'

    try:
        opts, args = getopt.getopt(
            argv, "hc:i:N:ft:o:w:", ['help', 'config=', 'input='])
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

    for dep in ['Config.cpp', 'DataHandler.h', 'HandlerChain.cpp', 'H10.C',
                'DH_EC_Hists_PreEid.h', 'DH_RunQuality.cpp', 'DH_EC_Hists.h',
                'DH_CloneH10.h', 'DH_Hists_Monitor.h', 'DH_Eid.h',
                'DH_SC_Hists_PrePid.h', 'DH_CC_Hists.h']:
        r.gROOT.ProcessLine('.L %s/%s+' % (wdir, dep))

    hnames = [
        "mon_raw", "echists_raw", "cchists_raw", "runquality", "cchists_qskim", "echists_qskim",
        "mon_qskim", "eid", "echists_eskim", "cchists_eskim", "mon_eskim", "h10_eskim", "scpid_eskim"]
    hclasses = [
        r.DH_Hists_Monitor, r.DH_EC_Hists_PreEid, r.DH_CC_Hists, r.DH_RunQuality, r.DH_CC_Hists, r.DH_EC_Hists,
        r.DH_Hists_Monitor, r.DH_Eid, r.DH_EC_Hists, r.DH_CC_Hists, r.DH_Hists_Monitor, r.DH_CloneH10, r.DH_SC_Hists_PrePid]

    fout = r.TFile(outfile, 'RECREATE')
    chain = r.TChain(treepath)
    chain.Add(rootfilepattern)
    processor = r.H10(chain, parmfile)

    for name, handler in zip(hnames, hclasses):
        processor.Add(handler(name, fout))
    processor.Loop(numproc, fastcount)

if __name__ == "__main__":
    main(sys.argv[1:])
