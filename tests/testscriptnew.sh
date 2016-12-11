#!/bin/bash

echo Test output file > tests/testoutput.txt

for i in `ls tests/tests_without_input/*/*`; do
printf "testing file: $i\n"
echo ---------------------------------------------$i--------------------------------------------- >> tests/testoutput.txt
./project $i >> tests/testoutput.txt 2>&1
#./project $i
retval=$?
if [ "$retval" -ne "0" ]
then
echo  Navratova hodnota: $retval >> tests/testoutput.txt
fi
done

for i in `ls tests/tests_with_input/*.java`; do
printf "testing file: $i\n"
a=${i%.*}.txt
echo ---------------------------------------------$i--------------------------------------------- >> tests/testoutput.txt
./project $i < $a >> tests/testoutput.txt 2>&1
#./project $i
retval=$?
if [ "$retval" -ne "0" ]
then
echo Navratova hodnota: $retval >> tests/testoutput.txt
fi
done

echo
echo --------------------------------------------
#comm -2 -3 <(sort testoutput.txt) <(sort testoutputcorrect.txt) > testdiff.txt
if diff tests/testoutput.txt tests/testoutputcorrect.txt ; then
echo ALL TESTS OK
fi
echo --------------------------------------------
