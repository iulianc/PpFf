#!
#git log --author=iulian --pretty=format:%s V1..

if [[ $# == 0 ]]; then
    USER='iulian'
else
    USER="$1"
fi
git shortlog --author=${USER} V1..



