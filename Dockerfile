FROM debian:bullseye-slim

RUN apt-get update && \
    apt-get install -y gcc libc6-dev libcurl4-openssl-dev && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY dolar.c .

RUN gcc -Wall dolar.c -lcurl -o dolar

ENTRYPOINT ["./dolar"]
