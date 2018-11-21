FROM phusion/baseimage:0.11

MAINTAINER Mohit Agnihotri "mohitkumar39@gmail.com"

WORKDIR /home/circleci
ENV HOME /home/circleci

RUN apt-get -qq update &&  apt-get -qqy upgrade --fix-missing

RUN apt-get install -y --no-install-recommends git cmake ninja-build gperf \
         ccache doxygen dfu-util device-tree-compiler \
         python3-ply python3-pip python3-setuptools python3-wheel xz-utils file \
         make gcc-multilib autoconf automake libtool librsvg2-bin git python3-dev \
         libgtest-dev g++ make cmake wget build-essential clang clang-tidy ninja-build libyaml-dev python3-dev python-dev \
         protobuf-compiler

RUN cd /usr/src/gtest && \
     mkdir -p build && cd build && \
     cmake .. &&  make -j8 && \
     cp *.a /usr/local/lib/ && \
     rm -rf ../build

RUN  pip3 install --no-cache-dir --upgrade pip

ARG SOLC_URL=https://github.com/ethereum/solidity/releases/download/v0.4.23/solc-static-linux
ARG SOLC_VERSION=v0.4.23
RUN  curl -L ${SOLC_URL} > /tmp/solc-${SOLC_VERSION} && \
     chmod 775 /tmp/solc-${SOLC_VERSION} && \
     mv /tmp/solc-${SOLC_VERSION} /usr/local/bin/ && \
     ln -s /usr/local/bin/solc-${SOLC_VERSION} /usr/local/bin/solc

#RUN wget http://pyyaml.org/download/pyyaml/PyYAML-3.13.tar.gz && tar -xvf PyYAML-3.13.tar.gz && cd PyYAML-3.13 && python setup.py install

RUN  pip3 install --no-cache-dir web3 eth_abi eth_utils click pyaml protobuf
RUN ssh-keyscan -H github.com >> /root/.ssh/known_hosts

RUN ls -laF
RUN pwd
COPY install_zephyr_env.sh /home/circleci
RUN chmod +x install_zephyr_env.sh
RUN /home/circleci/install_zephyr_env.sh

RUN apt-get autoremove && apt-get autoclean && rm -rf /var/lib/apt/lists/*
