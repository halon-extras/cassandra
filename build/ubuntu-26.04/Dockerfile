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
RUN apt-get update && apt-get install -y halon-smtpd=6.11.0

RUN apt-get install -y build-essential cmake git file libssl-dev curl libkrb5-dev libuv1-dev zlib1g-dev libarchive-tools

RUN mkdir /tmp/halon
RUN cd /tmp/halon \
  && curl -O -L https://github.com/apache/cassandra-cpp-driver/archive/refs/tags/2.17.1.tar.gz \
  && echo "e6ab5f5c60a916dd6c0dd9a19a883a4a1ab3d6b4e95cab925a186fecff08344e 2.17.1.tar.gz" | sha256sum -c \
  && bsdtar -zxvf 2.17.1.tar.gz && cd cassandra-cpp-driver-2.17.1 \
  && mkdir build && cd build \
  && cmake -DCASS_BUILD_STATIC=ON -DCASS_BUILD_SHARED=OFF -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_POLICY_VERSION_MINIMUM=3.5 .. \
  && make && make install

RUN echo -n "UBU2604" > /OSRELEASE.txt

COPY build.sh /build.sh
CMD ["/build.sh"]
