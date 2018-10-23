FROM circleci/python:3.6.6-jessie-browsers

MAINTAINER WonJong Shin "shinonejong@gmail.com"

WORKDIR /home/circleci
ENV HOME /home/circleci

RUN sudo apt-get -qq update && sudo apt-get -qqy upgrade && \
    sudo apt-get install -y libgtest-dev g++ make cmake wget build-essential clang clang-tidy-3.8 ninja-build && \
    sudo apt-get autoremove && sudo apt-get autoclean && sudo rm -rf /var/lib/apt/lists/*
RUN cd /usr/src/gtest && \
    sudo mkdir -p build && cd build && \
    sudo cmake .. && sudo make -j8 && \
    sudo cp *.a /usr/local/lib/ && \
    sudo rm -rf ../build
RUN sudo pip3 install --no-cache-dir --upgrade pip

ARG SOLC_URL=https://github.com/ethereum/solidity/releases/download/v0.4.23/solc-static-linux
ARG SOLC_VERSION=v0.4.23
RUN sudo curl -L ${SOLC_URL} > /tmp/solc-${SOLC_VERSION} && \
    sudo chmod 775 /tmp/solc-${SOLC_VERSION} && \
    sudo mv /tmp/solc-${SOLC_VERSION} /usr/local/bin/ && \
    sudo ln -s /usr/local/bin/solc-${SOLC_VERSION} /usr/local/bin/solc

RUN sudo pip3 install --no-cache-dir web3 eth_abi eth_utils click
