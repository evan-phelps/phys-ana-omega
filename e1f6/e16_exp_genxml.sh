#!/bin/bash

cat << EOF
<Request>
  <Email email="ephelps@jlab.org" request="false" job="true" />
  <Project name="e1-6a" />
  <Track name="analysis" />
  <Name name="ep_e16" />
  <Command><![CDATA[
    ./e16_exp_skim_small.sh
  ]]></Command>

  <TimeLimit unit="minutes" time="60" />
  <DiskSpace space="50" unit="GB" />
  <Memory space="3584" unit="MB" />

EOF
for run in "$@"
do
   echo '  <Job>'
   echo '    <Input src="/volatile/clas/clase1/ephelps/phys-ana-omega/e1f6/e16_exp_skim_small.sh" dest="e16_exp_skim_small.sh" />'
   echo '    <Input src="/volatile/clas/clase1/ephelps/phys-ana-omega/e1f6/skim_small.py" dest="skim_small.py" />'

   for fn in $(ls /mss/clas/e1-6a/production/pass2/v1/data/run_${run}_pass2.a??.1)
   do
      echo '    <Input src="mss:'${fn}'" dest="'`basename ${fn}`'.bos" />'
   done

   echo '    <Output src="skim.root" dest="/volatile/clas/clase1/ephelps/batch/e16/'${run}'_skim_small.root" />'
   echo '  </Job>'
done

echo '</Request>'

