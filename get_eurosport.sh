#!/bin/bash

plop=$(wget -q -O - http://www.eurosport.fr/$1 2>/dev/null |grep -A 100 storyfulltitleboxcontainer|iconv -f UTF-8 -t ISO-8859-15 )

line=$(echo $plop |grep -n generated |head -n 1 |awk -F ':' '{print $1}')

plop2=$(echo $plop |head -n $line | html2text -nobs -width 60 |grep -Ev '(\[|\])' |tail -n +3)

line="$(echo "$plop2" |grep -n 'Toutes les infos' |head -n 1 |awk -F ':' '{print $1}')"
line=$(($line - 1))

echo "$plop2" |head -n $line |grep -v "Plus d'infos"
