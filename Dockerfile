FROM python:3.6-alpine

# Copy in your requirements file
ADD test/test_requirements.txt /requirements.txt

# OR, if you’re using a directory for your requirements, copy everything (comment out the above and uncomment this if so):
# ADD requirements /requirements

# Install build deps, then run `pip install`, then remove unneeded build deps all in a single step. Correct the path to your production requirements file, if needed.
RUN mkdir /code/
WORKDIR /code
COPY . /code
ADD test/test_requirements.txt /code/requirements.txt

RUN set -ex \
    && apk add --no-cache --virtual .build-deps \
            gcc \
            make \
            libc-dev \
            musl-dev \
            linux-headers \
            pytest \
            pcre-dev \
            postgresql-dev \
    && make clean \
    && make \
    && chmod +x ifj18 \
    && pip install -U pip \
    && LIBRARY_PATH=/lib:/usr/lib /bin/sh -c "pip install --no-cache-dir -r /requirements.txt" \
    && runDeps="$( \
            scanelf --needed --nobanner --recursive /venv \
                    | awk '{ gsub(/,/, "\nso:", $2); print "so:" $2 }' \
                    | sort -u \
                    | xargs -r apk info --installed \
                    | sort -u \
    )" \
    && apk add --virtual .python-rundeps $runDeps \
    && apk del .build-deps

# Copy your application code to the container (make sure you create a .dockerignore file if any large files or directories should be excluded)

#COPY client /code/client



#RUN DATABASE_URL=none /venv/bin/python manage.py migrate --noinput
#RUN DATABASE_URL=none /venv/bin/python manage.py update_rates

#ENTRYPOINT ["/code/docker-entrypoint.sh"]

# Start uWSGI
CMD ["pytest", "-v", "test/unit_tests"]