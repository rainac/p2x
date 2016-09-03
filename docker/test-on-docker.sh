#! /bin/bash

cont=$1

WD=/home/devel/
BRANCH=${2:-master}

docker cp scripts $cont:$WD/scripts

dopts=-i
if test -t 0; then
    dopts="$dopts -t"
fi

docker exec -u root $dopts $cont $WD/scripts/debian-install-packages.sh || exit 1
docker exec $dopts $cont git reset --hard || exit 1
docker exec $dopts $cont git clean -x -f -d || exit 1
docker exec $dopts $cont git checkout $BRANCH || exit 1
docker exec $dopts $cont git pull || exit 1
docker exec $dopts $cont $WD/scripts/run-configure.sh || exit 1
docker exec $dopts $cont $WD/scripts/compile-and-install.sh || exit 1
docker exec $dopts $cont $WD/scripts/run-make-check.sh || exit 1
