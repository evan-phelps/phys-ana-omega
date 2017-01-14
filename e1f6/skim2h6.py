#!/usr/bin/python

import sys
import ROOT as r
import getopt
from subprocess import call


def usage():
    print(
        'efid_hists.py [-f] [-w <workingdir>] [-c <parmsfilepath>] [-i <inputstring>] [-N <numtoproc>] [-t treepath] [-o outfile]')
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
                'DH_SC_Hists_PrePid.h', 'DH_CC_Hists.h', 'DH_EC_Hists_PreEid.h',
                'DH_EC_Hists.h', 'DH_Efid_Hists.h', 'DH_Hists_Monitor.h',
                'DH_Eid.h', 'DH_CloneH10.h', 'DH_RunQuality.cpp', 'DH_Efid.h',
                'DH_CC_Nphe.h', 'DH_W_Skim.h', 'DH_MMp_Skim.h', 'DH_MMp_Exclusive.h',
                'DH_Hfid.h', 'mom_corr.cpp', 'DH_Pcor.h', 'DH_SC_BadPdls.h',
		'DH_Q2W_Cuts.h', 'DH_H6Maker.h']:
        r.gROOT.ProcessLine('.L %s/%s+' % (wdir, dep))

    handlers = [("mon_raw", r.DH_Hists_Monitor),
                # pcor should go before fiducial cuts (efid and hfid),
                # but only if fiducial parameters are recalculated
                # based on already-corrected momenta
                ("eid_efid_nphe_hfid_pcor", r.DH_Pcor),
		          ("efid2", r.DH_Efid),
                ("eid_efid_nphe_hfid_pcor_badsc", r.DH_SC_BadPdls),
                ("eid_efid_nphe_hfid_pcor_badsc_mmp", r.DH_MMp_Exclusive),
                ("mon_eid_efid_nphe_hfid_pcor_badsc_mmp", r.DH_Hists_Monitor),
                #("h10_eid_efid_nphe_hfid_pcor_badsc_mmp", r.DH_CloneH10)
                ("q2w_cuts", r.DH_Q2W_Cuts),
                ("mon_eid_efid_nphe_hfid_pcor_badsc_mmp_q2w", r.DH_Hists_Monitor),
                ("h6_eid_efid_nphe_hfid_pcor_badsc_mmp", r.DH_H6Maker_Exp)
               ]

    fout = r.TFile(outfile, 'RECREATE')
    chain = r.TChain(treepath)
    chain.Add(rootfilepattern)
    processor = r.H10(chain, parmfile)

    for name, handler in handlers:
        processor.Add(handler(name, fout))
    processor.Loop(numproc, fastcount)


if __name__ == "__main__":
    main(sys.argv[1:])
