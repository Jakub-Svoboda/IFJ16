#!/bin/bash
make

for i in `ls tests/other/*ok*`; do
printf "test file is $i\n\n"
./project $i 2> /dev/null
retval=$?
if [ "$retval" -ne "0" ]
then 
echo "tests/other" $i
echo Neco je spatne
echo Navratova hodnota: $retval
printf "\n\n"
fi
done

for i in `ls tests/builtin/*`; do
printf "test file is $i\n\n"
./project $i 2> /dev/null
retval=$?
if [ "$retval" -ne "0" ]
then 
echo "tests/builtin" $i
echo Neco je spatne
echo Navratova hodnota: $retval
printf "\n\n"
fi
done

