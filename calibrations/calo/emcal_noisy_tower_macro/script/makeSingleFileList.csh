#!/bin/csh

set v=`cat $1`
#set nLines=`wc -l $fileList`
set i=1

if( -f fileLists/singleFileRunnumberList.list )then
    rm fileLists/singleFileRunnumberList.list
endif
touch fileLists/singleFileRunnumberList.list

while ( $i <= $#v )
    head -n 1 ${v[$i]} >> fileLists/singleFileRunnumberList.list
   @ i++
 end
