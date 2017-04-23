var rp = require('request-promise');

var options = {
    uri: 'http://signalk:3000/signalk/v1/api/delta',
    method: 'POST',
    body: {
        "context": "vessels.self",
        "updates": [
            {
                "values": [
                    {
                        "path": "environment.inside.temperature",
                        "value": 25.00
                    }
                ],
                "source": {
                    "label": "esp8266fakesensor"
                }
            }
        ]


    },
    json: true // Automatically parses the JSON string in the response
};

//console.log("BLA")

rp(options)
    .then(function (response) {
        console.log(response.body);
    })
    .catch(function (err) {
        console.log(err.message)
    });
