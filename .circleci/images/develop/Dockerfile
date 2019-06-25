#
# dockerfile for "developer" image
#
FROM anyledger/base:latest

WORKDIR /home/circleci
ENV HOME /home/circleci

COPY build_zephyr_nrf52840_pca10056.sh /home/circleci
RUN chmod +x build_zephyr_nrf52840_pca10056.sh
RUN ./build_zephyr_nrf52840_pca10056.sh
