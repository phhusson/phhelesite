#!/bin/bash

plop=$(wget -q -O - "http://www.agoravox.fr/article.php3?id_article=37420" 2>/dev/null  |grep '<table border="0" width="93%" align="center" class="article_texte">' -A 400 )

line=$(echo "$plop" |grep -n '</table>' |head -n 1 |awk -F ':' '{print $1}')

plop2=$(echo "$plop" |head -n $line | html2text -nobs -width 60 |grep -Ev '(\[|\])' |tail -n +3)

#line="$(echo "$plop2" |grep -n 'Toutes les infos' |head -n 1 |awk -F ':' '{print $1}')"
#line=$(($line - 1))

echo "$plop2" # |head -n $line |grep -v "Plus d'infos"
