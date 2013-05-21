#!/bin/bash

echo "checking for WSOMEGA env variable"
[ -z "$WSOMEGA" ] && echo "does not exist; attempting setup.sh" && source $(dirname `which "${BASH_SOURCE[0]}"`)/setup.sh
[ -z "$WSOMEGA" ] && echo "failed to invoke setup.sh" && exit 1

INFILE=$1
echo "moving $WSOMEGA/$INFILE"
mv $INFILE $INFILE.bak
echo "moved to $WSOMEGA/$INFILE.bak"
echo "invoking bcpp -fi $INFILE.bak -fnc $WSOMEGA/bcpp.cfg -f 2 -qb 5 -na -no > $INFILE"
bcpp -fi $INFILE.bak -fnc $WSOMEGA/bcpp.cfg -f 2 -qb 5 -na -no > $INFILE
