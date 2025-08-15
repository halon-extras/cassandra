FROM --platform=linux/amd64 ubuntu:22.04
LABEL org.opencontainers.image.authors="support@halon.io"

ARG HALON_REPO_USER
ARG HALON_REPO_PASS

RUN apt-get update

RUN apt-get install -y wget gnupg
RUN apt-get install -y apt-transport-https

RUN wget -qO - https://raw.githubusercontent.com/halon/changelog/master/pgp-keys/7F0A73B5.asc | apt-key add -
RUN echo "deb https://repo.halon.io/ jammy stable" >> /etc/apt/sources.list.d/halon.list
RUN echo "machine repo.halon.io login ${HALON_REPO_USER} password ${HALON_REPO_PASS}" >> /etc/apt/auth.conf
RUN apt-get update && apt-get install -y halon=5.10.0

RUN /usr/bin/install -d /var/run/halon
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH

RUN apt-get install -y build-essential cmake git libssl-dev curl libkrb5-dev libuv1-dev zlib1g-dev

RUN mkdir /tmp/halon
RUN cd /tmp/halon \
    && curl -O -L https://github.com/datastax/cpp-driver/archive/refs/tags/2.17.1.tar.gz \
    && echo "53b4123aad59b39f2da0eb0ce7fe0e92559f7bba0770b2e958254f17bffcd7cf 2.17.1.tar.gz" | sha256sum -c \
    && tar -zxvf 2.17.1.tar.gz && cd cpp-driver-2.17.1 \
    && mkdir -p /cpp-driver && cp LICENSE.txt /cpp-driver/LICENSE \
    && mkdir build && cd build \
    && cmake .. \
    && make && make install
RUN rm -rf /tmp/halon

RUN echo -n "UBU2204" > /OSRELEASE.txt

RUN apt-get install -y supervisor
COPY ./supervisord.conf /etc/supervisor/conf.d/supervisord.conf
CMD ["/usr/bin/supervisord"]