
FROM ubuntu:latest

RUN adduser developer

RUN apt-get update

RUN apt-get upgrade -y

RUN apt-get install -y aptitude

RUN aptitude install -y autoconf automake xmlstarlet flex gcc g++ git sudo

RUN aptitude install -y make

RUN adduser developer sudo

USER developer

WORKDIR /home/developer
