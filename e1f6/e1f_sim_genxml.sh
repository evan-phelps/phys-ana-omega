#!/bin/bash

cat << EOF
<Request>
  <Email email="ephelps@jlab.org" request="false" job="true" />
  <Project name="e1f" />
  <Track name="simulation" />
  <Name name="ep_e1f_sim" />
  <Command><![CDATA[
    ./e1f_sim_skim_small.sh
  ]]></Command>

  <TimeLimit unit="minutes" time="30" />
  <DiskSpace space="5" unit="GB" />
  <Memory space="3584" unit="MB" />

EOF
for run in "$@"
do
   echo '  <Job>'
   echo '    <Input src="/volatile/clas/clase1/ephelps/phys-ana-omega/e1f6/e1f_sim_skim_small.sh" dest="e1f_sim_skim_small.sh" />'
   echo '    <Input src="/volatile/clas/clase1/ephelps/phys-ana-omega/e1f6/skim_small_sim.py" dest="skim_small_sim.py" />'

   for fn in $(ls /mss/home/ephelps/omega/simulations/e1f/${run}/recon.*.root)
   do
      echo '    <Input src="mss:'${fn}'" dest="'`basename ${fn}`'" />'
   done

   echo '    <Output src="skim.root" dest="/volatile/clas/clase1/ephelps/batch/e1f_sim/'${run}'_skim_small.root" />'
   echo '  </Job>'
done

echo '</Request>'

