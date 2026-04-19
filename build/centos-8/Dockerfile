FROM rockylinux:8
LABEL org.opencontainers.image.authors="support@halon.io"

ARG TARGETARCH
ARG TARGETOS

ARG HALON_REPO_USER
ARG HALON_REPO_PASS

RUN yum update -y && yum install -y epel-release
RUN crb enable

RUN echo "[halon]" >> /etc/yum.repos.d/Halon.repo
RUN echo "name=Halon repository" >> /etc/yum.repos.d/Halon.repo
RUN echo "baseurl=https://${HALON_REPO_USER}:${HALON_REPO_PASS}@repo.halon.io/dists/rhel8/${TARGETARCH}/stable" >> /etc/yum.repos.d/Halon.repo
RUN echo "gpgcheck=1" >> /etc/yum.repos.d/Halon.repo
RUN echo "gpgkey=https://raw.githubusercontent.com/halon/changelog/master/pgp-keys/7F0A73B5.asc" >> /etc/yum.repos.d/Halon.repo
RUN echo "enabled=1" >> /etc/yum.repos.d/Halon.repo
RUN yum install -y halon-5.10.0

RUN yum install -y cmake3 make gcc-c++ rpm-build git openssl-devel krb5-libs libuv-devel zlib-devel

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

RUN echo -n "CENTOS8" > /OSRELEASE.txt

RUN yum install -y 'dnf-command(config-manager)'
RUN yum config-manager --set-enabled powertools

COPY build.sh /build.sh
CMD ["/build.sh"]
