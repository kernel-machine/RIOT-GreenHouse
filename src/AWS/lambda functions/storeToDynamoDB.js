/*
This lambda parse messages received by IoT core and store it on dynamoDB
*/

const AWS = require('aws-sdk');
AWS.config.update({
    region: "us-east-1",
    endpoint: "http://dynamodb.us-east-1.amazonaws.com:80"
});

var docClient = new AWS.DynamoDB.DocumentClient();

exports.handler = async(event, context, callback) => {

    var params = {
        TableName: "MQTTMessages",
        Item: {
            "message_id":  1,
            "timestamp": Date.now(),
            "humidity": event.hum,
            "temperature":event.temp,
            "pump":event.pump,
            "water_level":event.water_level,
            "soil":event.soil,
            "servo":event.servo
        }
    };
    return docClient.put(params, function(err, data) {
        if (err) {
            console.error("Unable to add movie");
        } else {
            console.log("PutItem succeeded:");
        }
    }).promise();

}