# Dockerfile
FROM --platform=$BUILDPLATFORM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    git \
    cmake \
    libssl-dev \
    libcurl4-openssl-dev \
    libarchive-dev

WORKDIR /src
COPY . .

RUN make clean && make

# Copy only the compiled binary to a clean location
RUN mkdir -p /out && cp lem /out/lem

# Final stage to extract only binary
FROM scratch AS export-stage
COPY --from=0 /out/lem /lem