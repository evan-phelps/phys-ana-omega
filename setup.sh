#!/bin/bash

export WSOMEGA=$(dirname `which "${BASH_SOURCE[0]}"`)
WSPYTHON=$WSOMEGA/python
test `echo $PYTHONPATH | grep -c $WSPYTHON` -gt 0 || export PYTHONPATH=$PYTHONPATH:$WSPYTHON
