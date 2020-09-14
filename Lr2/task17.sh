#!/bin/bash
inputFile=$1
encoding=$(file -b $inputFile)

if [ "$encoding" != "ASCII text" ]; then
    echo "Not ASCII text"
    exit 0
fi

wordCounter=0
#search
i=1  
while read line; do  
if [[ $line == "$2" ]] || [[ $line == "$2 "* ]] || [[ $line == *" $2 "* ]]; then
   wordCounter=$((wordCounter+1))
fi
i=$((i+1))  
done < $inputFile

echo "Substrings in file: $wordCounter"
