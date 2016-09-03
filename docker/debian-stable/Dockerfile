FROM debian

RUN adduser devel
RUN adduser devel staff

RUN apt-get update
RUN apt-get -y upgrade
RUN apt-get -y install git g++ gcc make automake

USER devel
WORKDIR /home/devel

RUN mkdir src

WORKDIR src

RUN git clone https://github.com/rainac/p2x.git

WORKDIR p2x

CMD git pull
