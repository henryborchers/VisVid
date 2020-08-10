FROM sonarsource/sonar-scanner-cli:latest
RUN apk --no-cache add cmake clang clang-dev make gcc g++ libc-dev linux-headers sdl2-dev ffmpeg-dev curl

#RUN wget https://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip && \
#    mkdir -p /build-wrapper && \
#    unzip build-wrapper-linux-x86.zip -d /build-wrapper
#ENV PATH="/build-wrapper/build-wrapper-linux-x86:${PATH}"
ENV SONAR_SCANNER_VERSION=4.4.0.2170
ENV SONAR_SCANNER_HOME=$HOME/.sonar/sonar-scanner-$SONAR_SCANNER_VERSION-linux
RUN curl --create-dirs -sSLo $HOME/.sonar/sonar-scanner.zip https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-$SONAR_SCANNER_VERSION-linux.zip && \
    unzip -o $HOME/.sonar/sonar-scanner.zip -d $HOME/.sonar/
ENV PATH=$SONAR_SCANNER_HOME/bin:$PATH
ENV SONAR_SCANNER_OPTS="-server"

RUN curl --create-dirs -sSLo $HOME/.soexportnar/build-wrapper-linux-x86.zip https://sonarcloud.io/static/cpp/build-wrapper-linux-x86.zip && \
    unzip -o $HOME/.sonar/build-wrapper-linux-x86.zip -d $HOME/.sonar/
ENV PATH=$HOME/.sonar/build-wrapper-linux-x86:$PATH
USER scanner-cli
CMD /bin/sh
