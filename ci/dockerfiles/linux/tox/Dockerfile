FROM ubuntu:20.04 as base_wheel_builder
RUN apt update && \
    apt-get install -y software-properties-common && \
    add-apt-repository ppa:deadsnakes/ppa && \
    apt update
COPY ci/dockerfiles/linux/tox/apt-packages.txt /tmp/apt-packages.txt
COPY ci/dockerfiles/python/shared/requirements-ci.txt   /tmp/
RUN cat /tmp/apt-packages.txt | xargs apt install -y
RUN cat /tmp/apt-packages.txt | xargs apt install -y

ARG USER_ID=1000
ARG GROUP_ID=1000

RUN groupadd -g $GROUP_ID user && \
    useradd -u $USER_ID -s /bin/sh -m -d /home/user -g user user


# ==============================================================================
FROM base_wheel_builder as wheel_builder_py36
RUN apt install -y python3.6-dev python3.6-venv python3-pip
ARG PIP_EXTRA_INDEX_URL
ARG PIP_INDEX_URL

RUN PYTHON=python3.6 && \
    $PYTHON -m pip install pip --upgrade pip && \
    $PYTHON -m pip install wheel && \
    $PYTHON -m pip wheel -w /wheels --no-cache-dir -r /tmp/requirements-ci.txt

# ==============================================================================
FROM base_wheel_builder as wheel_builder_py37
RUN apt install -y python3.7-dev python3.7-venv python3-pip

ARG PIP_EXTRA_INDEX_URL
ARG PIP_INDEX_URL

RUN PYTHON=python3.7 && \
    $PYTHON -m pip install pip --upgrade pip && \
    $PYTHON -m pip install wheel && \
    $PYTHON -m pip wheel -w /wheels --no-cache-dir -r /tmp/requirements-ci.txt

# ==============================================================================
FROM base_wheel_builder as wheel_builder_py38
RUN apt install -y python3.8-dev python3.8-venv python3-pip

ARG PIP_EXTRA_INDEX_URL
ARG PIP_INDEX_URL

COPY ci/dockerfiles/python/shared/requirements-ci.txt /tmp/
RUN PYTHON=python3.8 && \
    $PYTHON -m pip install pip --upgrade pip && \
    $PYTHON -m pip install wheel && \
    $PYTHON -m pip wheel -w /wheels --no-cache-dir -r /tmp/requirements-ci.txt

# ==============================================================================
FROM base_wheel_builder as wheel_builder_py39
RUN apt install -y python3.9-dev python3.9-venv python3-pip

ARG PIP_EXTRA_INDEX_URL
ARG PIP_INDEX_URL
RUN PYTHON=python3.9 && \
    $PYTHON -m pip install pip --upgrade pip && \
    $PYTHON -m pip install wheel && \
    $PYTHON -m pip wheel -w /wheels --no-cache-dir -r /tmp/requirements-ci.txt

# ==============================================================================
FROM base_wheel_builder as conan_builder

RUN pip3 install conan cmake
COPY ci/dockerfiles/conan/remotes.json  /root/.conan/remotes.json
ADD conanfile.py /tmp/conanfile.py
RUN conan config home && \
    conan install /tmp --build libx264 --build libx265 --build openjpeg --build  missing -o ffmpeg:openjpeg=False --no-import && \
    conan remove "*" -b --src -f && \
    conan remove --locks

USER user
WORKDIR /home/user
COPY --chown=user:user ci/dockerfiles/conan/remotes.json  /home/user/.conan/remotes.json
ADD conanfile.py /tmp/conanfile.py
ENV CONAN_USER_HOME=/home/user/
RUN conan config home && \
    conan config home && \
    conan install /tmp --build  missing --no-import && \
    conan remove "*" -b --src -f && \
    conan remove --locks
# ==============================================================================
FROM base_wheel_builder
RUN cat /tmp/apt-packages.txt | xargs apt install -y

ARG PIP_EXTRA_INDEX_URL
ARG PIP_INDEX_URL

COPY --from=conan_builder --chown=user:user /home/user/.conan /home/user/.conan
COPY --from=wheel_builder_py36 /wheels/* /wheels/
COPY --from=wheel_builder_py37 /wheels/* /wheels/
COPY --from=wheel_builder_py38 /wheels/* /wheels/
COPY --from=wheel_builder_py39 /wheels/* /wheels/
ENV PIP_FIND_LINKS=/wheels
RUN pip3 install tox conan

USER user
WORKDIR /home/user
COPY --chown=user:user ci/dockerfiles/conan/remotes.json  /home/user/.conan/remotes.json
ADD conanfile.py /tmp/conanfile.py
ENV CONAN_USER_HOME=/home/user/
RUN conan config home && \
    conan install /tmp --build  missing -o ffmpeg:openjpeg=False --no-import && \
    conan remove "*" -b --src -f && \
    conan remove --locks
WORKDIR /src
CMD tox