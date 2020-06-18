#!
cat $1 |
    tr -cs "[:alpha:]" "\n" |
    tr "[:upper:]" "[:lower:]" |
    sort |
    uniq -c |
    awk '{print $2 " => " $1}'




