FROM --platform=linux/amd64 rockylinux:9
LABEL org.opencontainers.image.authors="support@halon.io"

ARG HALON_REPO_USER
ARG HALON_REPO_PASS

RUN yum update -y && yum install -y epel-release
RUN crb enable

RUN echo "[halon]" >> /etc/yum.repos.d/Halon.repo
RUN echo "name=Halon repository" >> /etc/yum.repos.d/Halon.repo
RUN echo "baseurl=https://${HALON_REPO_USER}:${HALON_REPO_PASS}@repo.halon.io/dists/rhel9/amd64/stable" >> /etc/yum.repos.d/Halon.repo
RUN echo "gpgcheck=1" >> /etc/yum.repos.d/Halon.repo
RUN echo "gpgkey=https://raw.githubusercontent.com/halon/changelog/master/pgp-keys/7F0A73B5.asc" >> /etc/yum.repos.d/Halon.repo
RUN echo "enabled=1" >> /etc/yum.repos.d/Halon.repo
RUN yum install -y halon-5.12.1

RUN yum install -y cmake3 make gcc-c++ rpm-build git openssl-devel krb5-libs libuv-devel zlib-devel

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

RUN echo -n "ROCKY9" > /OSRELEASE.txt

COPY build.sh /build.sh
CMD ["/build.sh"]
