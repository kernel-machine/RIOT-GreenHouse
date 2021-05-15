### AWS Lambda function used:
- in `getData.js` there is the code of the lambda function that generate a json with the data
for fill the web interface, data are fetch on DynamoDB.
  
- in `storeToDynamoDB.js` there is the lambda function called when a new message is received by
IoT core, this function process the message and store it on dynamoDB.
  
- in `generateMQTTMessage.js` there is the lambda that generate an MQTT message when it is called
by the web interface, this is used to control the actuators manually.
  
- in `getValuesByNode.js` there is the lambda function that returns all value of a specific device