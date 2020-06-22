#!
tr -cs "[:alpha:]" "\n" |
tr "[:upper:]" "[:lower:]" |
sed "/^[[:space:]]*$/d" |
sort |
uniq -c |
awk '{print $2 " => " $1}'




