#!
tr  -s ' ' "\n" |
sed 's/[^a-zA-Z]//g' |
egrep -v "^\s*$" |
tr 'A-Z' 'a-z' |
sort |
uniq -c |
awk '{print $2 " => " $1}' |
cat

