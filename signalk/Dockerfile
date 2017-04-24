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

USER root

RUN mv /var/signalk-server-node-master/settings /usr/local/share/signalk; \
  ln -s /etc/signalk/settings /var/signalk-server-node-master/settings

COPY signalk.conf.example /usr/local/share/signalk

USER signalk

VOLUME ["/etc/signalk"]

EXPOSE 3000
ENTRYPOINT ["bin/signalk-server", "-s",  "settings/volare-file-settings.json"]
