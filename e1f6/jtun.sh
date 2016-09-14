#!/bin/bash

xterm -e "ssh -X -f -o ExitOnForwardFailure=yes -L 5922:ifarm1401:22 login.jlab.org sleep 10 && ssh -X -p 5922 localhost"

