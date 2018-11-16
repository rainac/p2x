#! /bin/zsh

#set -x

dockerOS=archlinux
dockerTag=latest

mydir=$(dirname $(readlink -f ${(%):-%x}))
curdir=$PWD
workdir=$curdir

opts="O:T:w:vh"
while getopts $opts OPTION; do
    case $OPTION in
	("O")
	    dockerOS=$OPTARG
	;;
	("T")
	    dockerTag=$OPTARG
	;;
	("w")
	    workdir=$OPTARG
	;;
	("")
	    break
	;;
    esac
done

dockerBPath=$dockerOS/$dockerTag
dockImage=build-$dockerOS-$dockerTag
dock=$dockImage

cmd=()
while [[ -n "$1" ]]; do
    cmd=($cmd ${argv[$OPTIND]})
    shift
done

cd $mydir
while ! [[ -f "src/p2x.cc" ]]; do
    cd ..
    if [[ "$PWD" = "/" ]]; then
	echo "Error: non in src tree"
	exit 1
    fi
done
top_srcdir=$PWD

if ! [[ -f "src/docker/$dockerBPath/Dockerfile" ]]; then
    echo "Error: Docker file src/docker/$dockerBPath/Dockerfile not found"
    exit 1
fi

if ! docker images | grep $dockImage > /dev/null; then
    echo "BUILD $dockImage"
    cd src/docker/$dockerBPath && docker build -t $dockImage .
fi

if ! docker ps -a | grep $dock > /dev/null; then
    echo "RUN ($dock) $dockImage"
    docker run --name=$dock -t -d -v /tmp:/tmp $dockImage
fi

echo "EXEC ($dock) $cmd (in $workdir)"
docker exec -w $workdir -t $dock $cmd
