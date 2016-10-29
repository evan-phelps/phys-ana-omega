#!/bin/bash

source /group/clas/builds/environment.sh
export SHELL="/bin/bash"

export ROOTSYS=/apps/root/5.34.05/root
source $ROOTSYS/bin/thisroot.sh
export PATH=.:$PATH

python skim_small.py -o skim.root -i "root_?????_pass1.a??.root" -w /volatile/clas/clase1/ephelps/phys-ana-omega/e1f6 -c /volatile/clas/clase1/ephelps/phys-ana-omega/e1f6/input.e1f.exp.parms

