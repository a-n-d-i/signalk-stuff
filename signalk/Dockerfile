FROM alpine:latest

RUN apk update; \
  apk add ca-certificates git nodejs-lts wget sudo openssl bash curl

RUN adduser signalk -D -h /var/signalk-server-node-master

RUN mkdir -p /etc/signalk/settings
RUN mkdir -p /usr/local/share/signalk

USER signalk

RUN wget -P /tmp https://github.com/SignalK/signalk-server-node/archive/master.tar.gz; \
    tar zxf /tmp/master.tar.gz -C /var; \
    rm -f /tmp/master.tar.gz

WORKDIR "/var/signalk-server-node-master"

RUN npm install tkurki/signalk-to-influxdb --save
RUN npm install ; npm cache clean

COPY wait-for-it.sh .
COPY startup.sh .
COPY signalk-to-influxdb.json ./plugin-config-data/signalk-to-influxdb.json

USER root
RUN chmod a+x ./wait-for-it.sh
RUN chmod a+x ./startup.sh
RUN chown signalk.signalk ./plugin-config-data/signalk-to-influxdb.json


RUN mv /var/signalk-server-node-master/settings /usr/local/share/signalk; \
  ln -s /etc/signalk/settings /var/signalk-server-node-master/settings

USER signalk

#ENV DEBUG=signalk-server
ENV DEBUG=signalk-to-influxdb

VOLUME ["/etc/signalk"]

EXPOSE 3000
#CMD curl -X POST http://influx:8086/query?q=CREATE+DATABASE+boatdata
#ENTRYPOINT ["bin/signalk-server", "-s",  "settings/volare-file-settings.json"]

