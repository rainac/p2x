FROM debian:unstable

RUN adduser devel
RUN adduser devel staff

RUN apt-get update
RUN apt-get -y upgrade
RUN apt-get -y install git

USER devel
WORKDIR /home/devel

RUN mkdir src

WORKDIR src
