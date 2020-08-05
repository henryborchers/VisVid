ARG PYTHON_VERSION=3.8
FROM python:${PYTHON_VERSION} as wheel_builder

RUN python -m pip install --upgrade pip && \
    pip install wheel && \
    pip install --upgrade setuptools

COPY ci/dockerfiles/python/shared/requirements-ci.txt /requirements/
RUN pip wheel -w /wheels/ -r /requirements/requirements-ci.txt --no-cache

FROM python:${PYTHON_VERSION}
COPY ci/dockerfiles/python/shared/apt-packages.txt /packages/apt-packages.txt
RUN apt-get update && \
    xargs -a /packages/apt-packages.txt apt install -y  && \
    rm -rf /var/lib/apt/lists/*

ARG USER_ID=1000
ARG GROUP_ID=1000
RUN groupadd -g $GROUP_ID user && \
    useradd -u $USER_ID -s /bin/sh -m -d /home/user -g user user
COPY --from=wheel_builder /wheels/ /wheels/

COPY ci/dockerfiles/python/shared/requirements-ci.txt /packages/
RUN pip install --no-index --find-links=/wheels -r /packages/requirements-ci.txt
ENV PIP_FIND_LINKS=/wheels

