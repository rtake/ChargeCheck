#! /bin/csh

set i=0
@ max=$argv[1]

while( $i <= $max) 
  /home/rtake/ChargeCheck/bin/poplchk popl${i}.log
  @ i++
end
