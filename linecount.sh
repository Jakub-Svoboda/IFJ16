#!/bin/bash
linecount=0
filesize=0
for i in `ls -d *.[hc]`;do
linecount=$((linecount+`wc -l <$i`))
filesize=$((filesize + `stat -c%s "$i"`))
done
echo Number of Lines: $linecount
echo Size in bytes: $filesize
