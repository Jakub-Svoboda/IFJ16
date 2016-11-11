#!/bin/bash
make

var=0
i=1
for i in `ls tests/1/`; do
echo "tests/1" $i
./project tests/1/$i 2> /dev/null
if [ "$?" -eq "1" ]
then 
echo Correct
fi
done

i=1
for i in `ls tests/2/`; do
echo "tests/2" $i
./project tests/2/$i 2> /dev/null
if [ "$?" -eq "2" ]
then 
echo Correct
fi
done

i=1
for i in `ls tests/3/`; do
echo "tests/2" $i
./project tests/3/$i 2> /dev/null
if [ "$?" -eq "3" ]
then 
echo Correct
fi
done