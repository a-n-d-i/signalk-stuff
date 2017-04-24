var rp = require('request-promise');

var options = {
    uri: 'http://signalk:3000/signalk/v1/api/_test/delta',
    method: 'POST',
    body: {
        "context": "vessels.self",
        "updates": [
            {
                "values": [
                    {
                        "path": "environment.inside.temperature",
                        "value": 206.15
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
var sendData = function () {
    options.body.updates[0].values[0].value = Math.floor((Math.random() * 20) + 273);
    rp(options)
        .then(function (response) {
            console.log(response);
            console.log("SUCCESS");
        })
        .catch(function (err) {
            console.log(err.message)
        });
}


setInterval(sendData, 2000)