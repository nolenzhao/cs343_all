#!/bin/bash

/usr/bin/time -f "%Uu %Ss %E" bin/q2throwcatch 12 12 103 28
/usr/bin/time -f "%Uu %Ss %E" bin/q2longjmp 12 12 103 28

/usr/bin/time -f "%Uu %Ss %E" bin/q2throwcatch-O2 12 12 103 28
/usr/bin/time -f "%Uu %Ss %E" bin/q2longjmp-O2 12 12 103 28