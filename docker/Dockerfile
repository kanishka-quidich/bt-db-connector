FROM cassandra:latest

# Set environment variables
ENV DEBIAN_FRONTEND=noninteractive

# Install system dependencies
RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    cmake \
    python3 \
    python3-pip \
    python3-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*
#Cpp driver dependencies
RUN apt-get update && apt-get install -y \
    pkg-config \
    libuv1-dev \
    libssl-dev \
    libboost-all-dev \
    && rm -rf /var/lib/apt/lists/*
# Clone the C++ Cassandra driver repository into an absolute path
RUN git clone https://github.com/datastax/cpp-driver.git /root/cpp-driver
WORKDIR /root/cpp-driver
RUN mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    make && make install

WORKDIR /
# Install Cassandra dependencies and netcat (nc)
RUN apt-get update && apt-get install -y python3 ninja-build meson netcat-openbsd net-tools

#Install ZMQ
RUN apt-get update && apt-get install -y \
    libzmq3-dev \
    && rm -rf /var/lib/apt/lists/*

# Expose the Cassandra port
EXPOSE 9042

# Set the working directory
WORKDIR /data

COPY init_schema.cql /data/init_schema.cql

ENTRYPOINT ["docker-entrypoint.sh"]
CMD ["cassandra", "-f"]
