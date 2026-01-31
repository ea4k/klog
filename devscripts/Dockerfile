FROM ubuntu:24.04@sha256:cd1dba651b3080c3686ecf4e3c4220f026b521fb76978881737d24f200828b2b

RUN apt-get -y update
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/*
ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Madrid
RUN apt-get install  --no-install-recommends -y tzdata
RUN apt-get  --no-install-recommends -y install wget build-essential perl python git \    
    debhelper cmake qt6-tools-dev-tools qt6-base-dev libqt6charts6-dev qt6-serialport-dev \
    libhamlib++-dev qmake6 libqt6sql6-sqlite libhamlib-dev qt6-location-dev \
    qt6-positioning-dev qt6-l10n-tools qt6-declarative-private-dev

ENV QT_SELECT qt6

ENV USER_NAME devel

ARG host_uid=1001
ARG host_gid=1001
RUN groupadd -g $host_gid $USER_NAME && useradd -g $host_gid -m -s /bin/bash -u $host_uid $USER_NAME

USER $USER_NAME
WORKDIR /Work
