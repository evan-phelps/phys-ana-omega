#!/bin/bash

source /group/clas/builds/environment.sh
export SHELL="/bin/bash"

export ROOTSYS=/apps/root/5.34.05/root
source $ROOTSYS/bin/thisroot.sh
export PATH=.:$PATH

cp -r /home/ephelps/omega/src/MomCorr ./MomCorr
python skim_small.py -o skim.root -i "root_?????_pass1.a??.root" -w /home/ephelps/omega/src -c /home/ephelps/omega/src/input.e1f.exp.parms
