#! /bin/bash

cont=$1

docker exec -u root -i $cont /bin/bash < debian-install-packages.sh || exit 1
docker exec -i $cont git reset --hard || exit 1
docker exec -i $cont git clean -x -f -d || exit 1
docker exec -i $cont git pull || exit 1
docker exec -i $cont /bin/bash < run-configure.sh || exit 1
docker exec -i $cont /bin/bash < compile-and-install.sh || exit 1
docker exec -i $cont /bin/bash < run-make-check.sh || exit 1
