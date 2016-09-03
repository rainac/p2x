#! /bin/bash

cont=$1

docker exec -u root -i $cont /bin/bash < debian-install-packages.sh
docker exec -i $cont git reset --hard
docker exec -i $cont git clean -x -f -d
docker exec -i $cont git pull
docker exec -i $cont /bin/bash < run-configure.sh
docker exec -i $cont /bin/bash < compile-and-install.sh
docker exec -i $cont /bin/bash < run-make-check.sh
