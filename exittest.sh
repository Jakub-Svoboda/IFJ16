#!/bin/bash
make
for i in `ls tests/1/`; do
./project tests/1/$i 2> /dev/null
if [ "$?" -ne "1" ]
then 
echo "tests/1" $i
echo Neco je spatne
fi
done

for i in `ls tests/2/`; do
./project tests/2/$i 2> /dev/null
if [ "$?" -ne "2" ]
then 
echo "tests/2" $i
echo Neco je spatne
fi
done

for i in `ls tests/3/`; do
./project tests/3/$i 2> /dev/null
if [ "$?" -ne "3" ]
then 
echo "tests/3" $i
echo Neco je spatne
fi
done
