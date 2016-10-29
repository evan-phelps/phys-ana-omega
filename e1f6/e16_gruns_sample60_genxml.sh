#!/bin/bash

cat << EOF
<Request>
  <Email email="ephelps@jlab.org" request="false" job="true" />
  <Project name="e1-6a" />
  <Track name="analysis" />
  <Name name="ep_e16_sample60" />
  <Command><![CDATA[
    ./e16_exp_eskim.sh
  ]]></Command>

  <TimeLimit unit="minutes" time="60" />
  <DiskSpace space="50" unit="GB" />
  <Memory space="3" unit="GB" />

EOF
for run in $(cat e16_gruns_sample60.txt)
do
   echo '  <Job>'
   echo '    <Input src="/volatile/clas/clase1/ephelps/phys-ana-omega/e1f6/test/e16/e16_exp_eskim.sh" dest="e16_exp_eskim.sh" />'
   echo '    <Input src="/volatile/clas/clase1/ephelps/phys-ana-omega/e1f6/test/e16/efid_hists.py" dest="efid_hists.py" />'
   echo '    <Input src="/volatile/clas/clase1/ephelps/phys-ana-omega/e1f6/test/e16/input.e16.exp.parms" dest="input.e16.exp.parms" />'

   for fn in $(ls /mss/clas/e1-6a/production/pass2/v1/data/run_${run}_pass2.a??.1)
   do
      echo '    <Input src="mss:'${fn}'" dest="'`basename ${fn}`'.bos" />'
   done

   echo '    <Output src="eskim.root" dest="/volatile/clas/clase1/ephelps/phys-ana-omega/e1f6/test/e16/'${run}'_eskim.root" />'
   echo '  </Job>'
done

echo '</Request>'

