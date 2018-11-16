FROM centos:latest

RUN yum install -y autoconf automake make git
RUN yum install -y gcc-c++ flex man

RUN adduser developer

USER developer

WORKDIR /home/developer

CMD sleep 10000000
