/*
    This lambda publish a the data received by the HTTPS request from the web interface
    to the MQTT broker of IoT Core
*/

var AWS = require('aws-sdk');
var iotdata = new AWS.IotData({ endpoint: 'xxxxxxxxxxx-ats.iot.us-east-1.amazonaws.com' });

exports.handler = async(event) => {
    console.log("Event => " + JSON.stringify(event));
    var params = {
        topic: "cmd",
        payload: event.body,
        qos: 0
    };

    return iotdata.publish(params, function(err, data) {
        if (err) {
            console.log("ERROR => " + JSON.stringify(err));
        }
        else {
            console.log("Success");
        }
    }).promise();
};
