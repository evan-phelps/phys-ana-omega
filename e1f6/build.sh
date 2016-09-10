#!/bin/bash

g++ `root-config --cflags --glibs` -c H10.C
g++ `root-config --cflags --glibs` -c DH_RunQuality.cpp
g++ `root-config --cflags --glibs` -c Config.cpp
g++ `root-config --cflags --glibs` -c HandlerChain.cpp
g++ `root-config --cflags --glibs` *.o processH10s.cpp -o processH10s

# g++ -c `root-config --cflags --ldflags --glibs` Config.cpp
# g++ -c `root-config --cflags --ldflags --glibs` DataHandler.h
# g++ -c `root-config --cflags --ldflags --glibs` HandlerChain.cpp
# g++ -c `root-config --cflags --ldflags --glibs` H10.C
# g++ -c `root-config --cflags --ldflags --glibs` DH_EC_Hists_PreEid.h
# g++ -c `root-config --cflags --ldflags --glibs` DH_RunQuality.cpp
# g++ -c `root-config --cflags --ldflags --glibs` DH_EC_Hists.h
# g++ -c `root-config --cflags --ldflags --glibs` DH_CloneH10.h
# g++ -c `root-config --cflags --ldflags --glibs` DH_Hists_Monitor.h
# g++ -c `root-config --cflags --ldflags --glibs` DH_Eid.h
# g++ -c `root-config --cflags --ldflags --glibs` DH_SC_Hists_PrePid.h
# g++ -c `root-config --cflags --ldflags --glibs` DH_CC_Hists.h
# g++ `root-config --cflags --ldflags --glibs` Config.o HandlerChain.o H10.o DH_RunQuality.o -o processH10s processH10s.cpp
