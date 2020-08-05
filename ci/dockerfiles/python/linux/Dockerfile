FROM python:3.8 as wheel_builder
RUN python -m pip install --upgrade pip && \
    pip install wheel && \
    pip install --upgrade setuptools

COPY ci/dockerfiles/python/shared/requirements-ci.txt /requirements/
RUN pip wheel -w /wheels/ -r /requirements/requirements-ci.txt --no-cache

FROM python:3.8
ARG USER_ID=1000
ARG GROUP_ID=1000
RUN groupadd -g $GROUP_ID user && \
    useradd -u $USER_ID -s /bin/sh -m -d /home/user -g user user
COPY --from=wheel_builder /wheels/ /wheels/

COPY ci/dockerfiles/python/shared/requirements-ci.txt /requirements/
RUN pip install --no-index --find-links=/wheels -r /requirements/requirements-ci.txt
ENV PIP_FIND_LINKS=/wheels

