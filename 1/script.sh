#!/bin/bash
 
/usr/bin/time -f "%Uu %Ss %E" bin/q1exception_2 100000000 500 1003
/usr/bin/time -f "%Uu %Ss %E" bin/q1exception_2 100000000 50 1003
/usr/bin/time -f "%Uu %Ss %E" bin/q1exception_2 100000000 25 1003
/usr/bin/time -f "%Uu %Ss %E" bin/q1exception_2 100000000 25 1003

 
/usr/bin/time -f "%Uu %Ss %E" bin/q1returnglobal_2 100000000 500 1003
/usr/bin/time -f "%Uu %Ss %E" bin/q1returnglobal_2 100000000 50 1003
/usr/bin/time -f "%Uu %Ss %E" bin/q1returnglobal_2 100000000 25 1003
/usr/bin/time -f "%Uu %Ss %E" bin/q1returnglobal_2 100000000 12 1003

/usr/bin/time -f "%Uu %Ss %E" bin/q1returntype_2 100000000 500 1003
/usr/bin/time -f "%Uu %Ss %E" bin/q1returntype_2 100000000 50 1003
/usr/bin/time -f "%Uu %Ss %E" bin/q1returntype_2 100000000 25 1003
/usr/bin/time -f "%Uu %Ss %E" bin/q1returntype_2 100000000 12 1003

/usr/bin/time -f "%Uu %Ss %E" bin/q1returntypec_2 100000000 500 1003
/usr/bin/time -f "%Uu %Ss %E" bin/q1returntypec_2 100000000 50 1003
/usr/bin/time -f "%Uu %Ss %E" bin/q1returntypec_2 100000000 25 1003
/usr/bin/time -f "%Uu %Ss %E" bin/q1returntypec_2 100000000 12 1003