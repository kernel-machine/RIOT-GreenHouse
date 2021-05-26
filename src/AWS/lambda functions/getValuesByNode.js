const AWS = require('aws-sdk');
AWS.config.update({
    region: "us-east-1",
    endpoint: "http://dynamodb.us-east-1.amazonaws.com:80"
});

var docClient = new AWS.DynamoDB.DocumentClient();

exports.handler = async(event, context, callback) => {

    let deviceId = Number(event.queryStringParameters.device)

    var params = {
        TableName : "MQTTMessages2",
        KeyConditionExpression: "message_id = :start",
        FilterExpression : "device_id = :dev_id",
        ExpressionAttributeValues: {
            ":start":1,
            ":dev_id":deviceId
        },
        ScanIndexForward : false
    };


    let retrivedData = new Promise((resolve,reject) => {
        docClient.query(params, (err, data) => {
            if(err)
                reject(err)
            else
                resolve(data)
        })
    })

    return retrivedData

}