FROM ubuntu:24.04
LABEL org.opencontainers.image.authors="support@halon.io"

ARG TARGETARCH
ARG TARGETOS

ARG HALON_REPO_USER
ENV HALON_REPO_USER=$HALON_REPO_USER
ARG HALON_REPO_PASS
ENV HALON_REPO_PASS=$HALON_REPO_PASS

RUN apt-get update

RUN apt-get install -y wget gnupg
RUN apt-get install -y apt-transport-https

RUN wget -qO - https://raw.githubusercontent.com/halon/changelog/master/pgp-keys/7F0A73B5.asc | apt-key add -
RUN echo "deb https://repo.halon.io/ noble stable" >> /etc/apt/sources.list.d/halon.list
RUN echo "machine repo.halon.io login ${HALON_REPO_USER} password ${HALON_REPO_PASS}" >> /etc/apt/auth.conf
RUN apt-get update && apt-get install -y halon=6.3.0

RUN /usr/bin/install -d /var/run/halon
ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH

RUN apt-get install -y build-essential cmake git libssl-dev curl libkrb5-dev libuv1-dev zlib1g-dev

RUN mkdir /tmp/halon
RUN cd /tmp/halon \
    && curl -O -L https://github.com/apache/cassandra-cpp-driver/archive/refs/tags/2.17.1.tar.gz \
    && echo "e6ab5f5c60a916dd6c0dd9a19a883a4a1ab3d6b4e95cab925a186fecff08344e 2.17.1.tar.gz" | sha256sum -c \
    && tar -zxvf 2.17.1.tar.gz && cd cassandra-cpp-driver-2.17.1 \
    && mkdir -p /cpp-driver && cp LICENSE.txt /cpp-driver/LICENSE \
    && mkdir build && cd build \
    && cmake .. \
    && make && make install
RUN rm -rf /tmp/halon

RUN echo -n "UBU2404" > /OSRELEASE.txt

COPY ./entrypoint.sh /entrypoint.sh
RUN chmod 755 /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]

RUN apt-get install -y supervisor
COPY ./supervisord.conf /etc/supervisor/conf.d/supervisord.conf
CMD ["/usr/bin/supervisord"]