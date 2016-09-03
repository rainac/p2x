#! /bin/bash

cont=$1

WD=/home/devel/
BRANCH=${2:-master}

docker cp scripts $cont:$WD/scripts

docker exec -u root -it $cont $WD/scripts/debian-install-packages.sh || exit 1
docker exec -it $cont git reset --hard || exit 1
docker exec -it $cont git clean -x -f -d || exit 1
docker exec -it $cont git checkout $BRANCH || exit 1
docker exec -it $cont git pull || exit 1
docker exec -it $cont $WD/scripts/run-configure.sh || exit 1
docker exec -it $cont $WD/scripts/compile-and-install.sh || exit 1
docker exec -it $cont $WD/scripts/run-make-check.sh || exit 1
