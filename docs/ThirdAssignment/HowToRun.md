## How to run

- Create an account on [The Things Network](https://www.thethingsnetwork.org/)

- Create a device on your The Things Network

- Configure the device with the TTN

  - Edit the file `src/riot-application/Makefile` with your TTN credentials

    - Put here your credential keys

      ```makefile
      DEVEUI ?= <DEVICE EUI>
      APPEUI ?= <APPLICATION EUI>
      APPKEY ?= <APPLICATION KEY>
      ```

    - If you use more than one device, put a unique integer for each device in:

      ```
      NODE_ID ?= 1
      ```

  - Build the firmware of the device

    ```
    cd src/riot-application
    make
    ```
    By default, the output data returned by the sensor are replaced with dummy data, if you want to use real sensors
    you have to pass the parameter `FAKE=0` to the `make` command
    

- Setup AWS software

  - You have to setup your AWS IoT core in such a way that when there is an incoming message on the topic `gh_aggr` the lambda function [storeAggregatedValue.js](../../src/AWS/lambda%20functions/storeAggregatedValue.js) will be executed. You have also create the table on dynamoDB used inside the lambda function.
  - You have to create another lambda function to get the aggregated values; using the file [getAggregatedValue.js](../../src/AWS/lambda%20functions/getAggregatedValue.js) create a new lambda function and connect it to a Gateway API to get the results with an HTTPS request.

- Setup and run bridge.py 

  - Edit the file [bridge.py](../../src/EdgeSoftware/bridge.py)

    - Configure The Things Network parameters
  
      ```python
      TTN_HOST = "eu.thethings.network"
      TTN_PORT = 8883
      TTN_USERNAME = "greenhouse-km"  # Put here your application-id
      TTN_PASSWORD = "ttn-account-v2.GioWfbe3v7tW6WEA1M_SXwCMO1pKeRMRc3dmPQc0pco"  # Put here your application process key
      TTN_CA_CERT = "mqtt-ca.pem"  # Path to your The Things Network CA
      ```

    - Configure the credentials and certificates of your AWS account
  
      ```python
      AWS_HOST = "a3paxdkh0ekdqh-ats.iot.us-east-1.amazonaws.com"  # AWS endpoint
      AWS_PORT = 8883
      AWS_CA_CERTS = "/etc/mosquitto/certs/root-CA.crt"
      AWS_CERT_FILE = "/etc/mosquitto/certs/greenhouse.cert.pem"
      AWS_KEY_FILE = "/etc/mosquitto/certs/greenhouse.private.key"
      ```
  
  - Run the bridge
    `python bridge.py`

- Setup the IoT-LAB experiment

  Setup an IoT-LAB experiment with the board **st-lrwan1**, if you wish to use multiple nodes you have to create multiple devices on TTN and change the parameters on the Makefile for each device.

- Get the aggregated result.

  After about 1 minutes you should get some aggregated data calling the lambda function [getAggregatedValue.js](../../src/AWS/lambda%20functions/getAggregatedValue.js) previously created.

