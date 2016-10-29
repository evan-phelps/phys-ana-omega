#!/bin/bash

source /group/clas/builds/environment.sh
export SHELL="/bin/bash"

export ROOTSYS=/apps/root/5.34.05/root
source $ROOTSYS/bin/thisroot.sh
export PATH=.:$PATH

python skim_small.py -o skim.root -b -i "run_?????_pass2.a??.1.bos.root" -w /volatile/clas/clase1/ephelps/phys-ana-omega/e1f6 -c /volatile/clas/clase1/ephelps/phys-ana-omega/e1f6/input.e16.exp.parms

