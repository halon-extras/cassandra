FROM ubuntu:26.04
LABEL org.opencontainers.image.authors="support@halon.io"

ARG TARGETARCH
ARG TARGETOS

ARG HALON_REPO_USER
ARG HALON_REPO_PASS

RUN apt-get update

RUN apt-get install -y wget gnupg
RUN apt-get install -y apt-transport-https

RUN wget -qO- https://raw.githubusercontent.com/halon/pgp-keys/master/pgp-keys/7F0A73B5.gpg > /usr/share/keyrings/halon-archive-keyring.gpg
RUN echo "deb [arch=${TARGETARCH} signed-by=/usr/share/keyrings/halon-archive-keyring.gpg] https://repo.halon.io/ resolute stable" > /etc/apt/sources.list.d/halon.list
RUN echo "machine repo.halon.io login ${HALON_REPO_USER} password ${HALON_REPO_PASS}" >> /etc/apt/auth.conf
RUN apt-get update && apt-get install -y halon=6.11.0

RUN apt-get install -y build-essential cmake git file libssl-dev curl libkrb5-dev libuv1-dev zlib1g-dev

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

RUN echo -n "UBU2604" > /OSRELEASE.txt

COPY build.sh /build.sh
CMD ["/build.sh"]
