FROM ubuntu:21.04

RUN apt-get -y update
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Madrid
RUN apt-get install -y tzdata
RUN apt-get -y install wget build-essential perl python git \
    g++ make qt5-qmake qtbase5-dev qttools5-dev libqt5charts5-dev libqt5sql5-sqlite libhamlib-dev libqt5serialport5-dev

ENV QT_SELECT qt5

ENV USER_NAME devel

ARG host_uid=1001
ARG host_gid=1001
RUN groupadd -g $host_gid $USER_NAME && useradd -g $host_gid -m -s /bin/bash -u $host_uid $USER_NAME

USER $USER_NAME
WORKDIR /Work


