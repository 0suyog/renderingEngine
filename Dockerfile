FROM archlinux:latest

RUN echo -e '\n[multilib]\nInclude = /etc/pacman.d/mirrorlist' >> /etc/pacman.conf && \
    pacman -Syu --noconfirm base-devel cmake git make mesa libxkbcommon libxrandr lib32-mesa libxinerama libxcursor libxi && \
    pacman -Scc --noconfirm

WORKDIR /engine  

COPY . .

RUN ./Scripts/Setup-Linux.sh

WORKDIR /engine/build  


CMD ["make","run"]
