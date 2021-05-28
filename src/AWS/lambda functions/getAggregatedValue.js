const AWS = require('aws-sdk');
AWS.config.update({
    region: "us-east-1",
    endpoint: "http://dynamodb.us-east-1.amazonaws.com:80"
});

var docClient = new AWS.DynamoDB.DocumentClient();

exports.handler = async(event, context, callback) => {


    var params = {
        TableName : "aggregated_value",
        KeyConditionExpression: "msg_id = :start",
        ExpressionAttributeValues: {
            ":start":1,
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