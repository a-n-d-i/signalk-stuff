curl -X POST http://influx:8086/query?q=CREATE+DATABASE+boatdata
./bin/signalk-server -s settings/volare-file-settings.json