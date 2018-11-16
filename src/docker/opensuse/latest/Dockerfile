FROM opensuse:latest

RUN zypper install -y autoconf automake make git
RUN zypper install -y gcc-c++ flex man

RUN useradd developer

USER developer

WORKDIR /home/developer

CMD sleep 10000000
