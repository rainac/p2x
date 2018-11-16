FROM base/archlinux:latest

RUN pacman -Syuu

RUN pacman -S --noconfirm autoconf automake make git flex gcc

RUN useradd developer

USER developer

WORKDIR /home/developer

CMD sleep 1000000000
