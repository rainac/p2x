FROM debian:stable

RUN adduser developer

RUN apt-get update

RUN apt-get upgrade -y

RUN apt-get install -y aptitude

RUN aptitude install -y autoconf automake xmlstarlet flex gcc g++ git sudo make

RUN adduser developer sudo

USER developer

WORKDIR /home/developer

CMD sleep 1000000000
