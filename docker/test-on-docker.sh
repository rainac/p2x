#! /bin/bash

cont=$1

WD=/home/devel/
BRANCH=${2:-master}
REPO=$(readlink -f $PWD/..)

dopts=-i
if test -t 0; then
    dopts="$dopts -t"
fi

docker cp scripts $cont:$WD
docker exec -u root $dopts $cont mkdir -p /repos
docker cp $REPO $cont:repos

docker exec -u root $dopts $cont $WD/scripts/debian-install-packages.sh || exit 1
docker exec $dopts $cont $WD/scripts/get-source.sh /repos/p2x $BRANCH $WD/src/p2x || exit 1
docker exec $dopts $cont $WD/scripts/run-configure.sh || exit 1
docker exec $dopts $cont $WD/scripts/compile-and-install.sh || exit 1
docker exec $dopts $cont $WD/scripts/run-make-check.sh || exit 1
