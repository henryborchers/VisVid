FROM sonarsource/sonar-scanner-cli:latest
RUN apk --no-cache add cmake clang clang-dev make gcc g++ libc-dev linux-headers sdl2-dev ffmpeg-dev

RUN wget https://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip && \
    mkdir -p /build-wrapper && \
    unzip build-wrapper-linux-x86.zip -d /build-wrapper
ENV PATH="/build-wrapper/build-wrapper-linux-x86:${PATH}"
USER scanner-cli
CMD /bin/sh
