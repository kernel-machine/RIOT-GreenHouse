/*
This lambda function return data for the REST request in the web interface
 */

const AWS = require('aws-sdk');
AWS.config.update({
    region: "us-east-1",
    endpoint: "http://dynamodb.us-east-1.amazonaws.com:80"
});

var docClient = new AWS.DynamoDB.DocumentClient();

var params = {
    TableName : "MQTTMessages",
    KeyConditionExpression: "message_id = :start",
    ExpressionAttributeValues: {
        ":start":1,
    },
    Limit: 60,
    ScanIndexForward : false
};


exports.handler = async(event, context, callback) => {

    let items = docClient.query(params, function(err, data) {
        if (err) {
            return JSON.stringify(err, null, 2);
        } else {
        }
    }).promise();

    return items

}