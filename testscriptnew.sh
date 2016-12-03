#!/bin/bash
make

echo Test output file > testoutput.txt

for i in `ls tests/*/*`; do
printf "testing file: $i\n"
echo ---------------------------------------------$i--------------------------------------------- >> testoutput.txt
./project $i >> testoutput.txt 2>&1
#./project $i
retval=$?
if [ "$retval" -ne "0" ]
then 
echo Navratova hodnota: $retval >> testoutput.txt
fi
done

./project tests/1.java < tests/inputfiles/1.txt >> testoutput.txt
./project tests/2.java < tests/inputfiles/2.txt	>> testoutput.txt
./project tests/3.java < tests/inputfiles/3.txt	>> testoutput.txt
./project tests/4.java < tests/inputfiles/4.txt	>> testoutput.txt
./project tests/5.java < tests/inputfiles/5.txt	>> testoutput.txt
./project tests/13.java < tests/inputfiles/13.txt >> testoutput.txt
./project tests/14.java < tests/inputfiles/14.txt >> testoutput.txt
./project tests/15.java < tests/inputfiles/15.txt >> testoutput.txt
./project tests/17.java < tests/inputfiles/17.txt >> testoutput.txt
./project tests/18.java < tests/inputfiles/18.txt >> testoutput.txt
./project tests/ok-complex-1.java < tests/inputfiles/ok-complex-1.txt >> testoutput.txt
./project tests/string_test_4.code < tests/inputfiles/string_test_4.txt >> testoutput.txt
./project tests/classMainComplex.java < tests/inputfiles/classMainComplex.txt >> testoutput.txt

echo
echo --------------------------------------------
#comm -2 -3 <(sort testoutput.txt) <(sort testoutputcorrect.txt) > testdiff.txt
if diff testoutput.txt testoutputcorrect.txt ; then
echo ALL TESTS OK
fi
echo --------------------------------------------