FROM node:alpine

RUN apk update; \
  apk add curl bash


# Create app directory
RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app

COPY wait-for-it.sh /usr/src/app/
RUN chmod a+x ./wait-for-it.sh

# Install app dependencies
COPY package.json /usr/src/app/
RUN npm install
