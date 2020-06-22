#! /bin/csh

set begin=0
set end=7412
set i=${begin}

rm list.popl
rm list.frg

while(${i} <= ${end})
  /home/rtake/poplcheck_20200526/bin/poplchk popl${i}.log
  awk 1  popl${i}.log.popl >> list.popl
  awk 1  popl${i}.log.frg >> list.frg
  @ i++
end
