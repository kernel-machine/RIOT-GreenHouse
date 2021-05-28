const AWS = require('aws-sdk');
AWS.config.update({
    region: "us-east-1",
    endpoint: "http://dynamodb.us-east-1.amazonaws.com:80"
});

var docClient = new AWS.DynamoDB.DocumentClient();

exports.handler = async(event, context, callback) => {



    var params = {
        TableName: 'aggregated_value',
        Key: {
            'msg_id' : 1,
        },
        UpdateExpression:
            'set temp_max = :t_max, temp_min = :t_min, temp_avg = :t_avg,' +
            'hum_max = :h_max, hum_min = :h_min, hum_avg = :h_avg,' +
            'soil_max = :s_max, soil_min = :s_min, soil_avg = :s_avg,'+
            'last_update = :last_update',
        ExpressionAttributeValues: {
            ':t_max' : event.temp_max,
            ':t_min' : event.temp_min,
            ':t_avg' : event.temp_avg,
            ':h_max' : event.hum_max,
            ':h_min' : event.hum_min,
            ':h_avg' : event.hum_avg,
            ':s_max' : event.soil_max,
            ':s_min' : event.soil_min,
            ':s_avg' : event.soil_avg,
            ':last_update' : Date.now()
        }
    };

    return docClient.update(params, function(err, data) {
        if (err) {
            console.error("Unable to add movie");
        } else {
            console.log("PutItem succeeded:");
        }
    }).promise();

}