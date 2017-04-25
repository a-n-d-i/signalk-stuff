FROM alpine:latest

RUN apk -U add curl bash && rm -f /var/cache/apk/*

RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app

COPY wait-for-it.sh /usr/src/app/
RUN chmod a+x ./wait-for-it.sh
