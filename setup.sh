#!/bin/bash

export WSOMEGA=$(dirname `which "${BASH_SOURCE[0]}"`)
export PYTHONPATH=$PYTHONPATH:$WSOMEGA/python
