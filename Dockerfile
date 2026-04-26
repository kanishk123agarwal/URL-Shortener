FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    g++ cmake libsqlite3-dev libasio-dev git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# The workspace already includes 'include/crow' and 'include/asio'.
# We can also clone Crow to ensure headers are available as per the guide.
RUN git clone https://github.com/CrowCpp/Crow.git /tmp/crow && \
    mkdir -p include && cp -r /tmp/crow/include/crow include/crow

COPY . .

RUN cmake -B build -DCMAKE_BUILD_TYPE=Release . \
    && cmake --build build --parallel $(nproc)

EXPOSE 8080
ENV DB_PATH=/data/urls.db

CMD ["./build/url_shortener"]
