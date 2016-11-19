#!/bin/bash
make

for i in `ls tests/other/*ok*`; do
./project tests/1/$i 2> /dev/null
retval=$?
if [ "$retval" -ne "0" ]
then 
echo "tests/1" $i
echo Neco je spatne
echo Navratova hodnota: $retval
fi
done

for i in `ls tests/builtin/*`; do
./project tests/2/$i 2> /dev/null
retval=$?
if [ "$retval" -ne "0" ]
then 
echo "tests/2" $i
echo Neco je spatne
echo Navratova hodnota: $retval
fi
done

