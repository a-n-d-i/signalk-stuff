FROM grafana/grafana

RUN mkdir /var/dashboards

ENV GF_DASHBOARDS_JSON_ENABLED=true
ENV GF_DASHBOARDS_JSON_PATH=/var/dashboards

COPY ./dashboards/mydash.json /var/dashboards

ENTRYPOINT /run.sh ; touch /tmp/Bla