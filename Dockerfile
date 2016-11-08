FROM jenkins:2.19.1
MAINTAINER Henry Borchers <henryborchers@yahoo.com>


# Suppress apt installation warnings
ENV DEBIAN_FRONTEND=noninteractive

# change user to root
USER root


# Used to set the docker group ID
# Set to 497 by default.
ARG DOCKER_GID=497


# Create Docker Group with GID
# set default value of 497 if DOCKER_GID is set to blank string by Docker Compose
RUN groupadd -g ${DOCKER_GID:-497} docker

ARG DOCKER_ENGINE=1.10.2
#ARG DOCKER_ENGINE=1.12.2
ARG DOCKER_COMPOSE=1.6.2
#ARG DOCKER_COMPOSE=1.8.1

RUN apt-get update -y && \
    apt-get install \
    apt-transport-https \
    ca-certificates \
    curl \
    libncursesw5-dev \
    libncurses5-dev \
    gcc \
    make \
    wget \
    ffmpeg \
    libsqlite3-dev \
    libssl-dev \
    python-dev \
    tk-dev \
    build-essential -y

RUN wget https://cmake.org/files/v3.6/cmake-3.6.2.tar.gz \
    && tar xvzf cmake-3.6.2.tar.gz \
    && cd cmake-3.6.2 \
    && ./bootstrap \
    && make \
    && make install

RUN wget https://www.python.org/ftp/python/3.5.2/Python-3.5.2.tgz \
    && tar xvzf Python-3.5.2.tgz \
    && cd Python-3.5.2 \
    && ./configure \
    && make \
    && make install \
    && pip3 install --upgrade pip \
    && pip3 install tox

#RUN apt-key adv --keyserver hkp://p80.pool.sks-keyservers.net:80 --recv-keys 58118E89F3A912897C070ADBF76221572C52609D && \
#    echo "deb https://apt.dockerproject.org/repo ubuntu-jessie main" | tee /etc/apt/sources.list.d/docker.list && \
##    echo "deb https://apt.dockerproject.org/repo ubuntu-xenial main" | tee /etc/apt/sources.list.d/docker.list && \
#    apt-get update -y  && \
#    apt-get purge lxc-docker* -y && \
#    apt-get install docker-engine=${DOCKER_ENGINE:-1.12.2}-0~xenial -y && \
##    apt-get install docker-engine=${DOCKER_ENGINE:-1.10.2}-0~xenial -y && \
#    usermod -aG docker jenkins && \
#    usermod -aG users jenkins


RUN apt-key adv --keyserver hkp://pgp.mit.edu:80 --recv-keys 58118E89F3A912897C070ADBF76221572C52609D && \
    echo "deb https://apt.dockerproject.org/repo ubuntu-trusty main" | tee /etc/apt/sources.list.d/docker.list && \
    apt-get update -y && \
    apt-get purge lxc-docker* -y && \
    apt-get install docker-engine=${DOCKER_ENGINE:-1.10.2}-0~trusty -y && \
    usermod -aG docker jenkins && \
    usermod -aG users jenkins


#run pip3 install docker-compose==${DOCKER_COMPOSE:-1.8.1}
run pip3 install docker-compose==${DOCKER_COMPOSE:-1.6.2}

USER jenkins

COPY plugins.txt /usr/share/jenkins/plugins.txt
RUN /usr/local/bin/plugins.sh /usr/share/jenkins/plugins.txt
