FROM ubuntu
MAINTAINER Henry Borchers <henryborchers@yahoo.com>


RUN apt-get update -y && \
    apt-get install \
    # apt-transport-https \
    # ca-certificates \
    # curl \
    # libncursesw5-dev \
    # libncurses5-dev \
    # gcc \
    # make \
    # wget \
    ffmpeg \
    # libsqlite3-dev \
    # libssl-dev \
    # python-dev \
    # tk-dev \
    # glibc-source \
    # libgdbm-dev \
    # libgtest-dev \
    # libbz2-dev \
    build-essential -y

# USER jenkins

# COPY plugins.txt /usr/share/jenkins/plugins.txt
# RUN /usr/local/bin/plugins.sh /usr/share/jenkins/plugins.txt
