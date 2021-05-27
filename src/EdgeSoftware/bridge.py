"""
This file is the bridge that receive data from The Things Network MQTT broker, it perform the aggregation
and send the aggregated data to the AWS MQTT broker.
"""
import time
import paho.mqtt.client as mqtt
import json
import base64
from Aggregator import Aggregator
import threading
from signal import signal, SIGINT

TTN_HOST = "eu.thethings.network"
TTN_PORT = 8883
TTN_USERNAME = "greenhouse-km"  # Put here your application-id
TTN_PASSWORD = "ttn-account-v2.xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"  # Put here your application process key
TTN_CA_CERT = "mqtt-ca.pem"  # Path to your The Things Network CA

AWS_HOST = "a3paxdkh0ekdqh-ats.iot.us-east-1.amazonaws.com"  # AWS endpoint
AWS_PORT = 8883
AWS_CA_CERTS = "/etc/mosquitto/certs/root-CA.crt"
AWS_CERT_FILE = "/etc/mosquitto/certs/greenhouse.cert.pem"
AWS_KEY_FILE = "/etc/mosquitto/certs/greenhouse.private.key"

AGGREGATED_VALUE_SEND_INTERVAL_MS = 60 * 1000
# Interval to send data to the AWS broker
# Data are send only if there are new data aggregated


class Bridge:
    isLocalConnected = False

    aggregator = Aggregator()
    run = True

    def __init__(self):
        self.clientAWS = mqtt.Client()
        self.clientAWS.on_connect = self.on_connect_aws
        self.clientAWS.on_message = self.on_message_aws
        self.clientAWS.tls_set(AWS_CA_CERTS, AWS_CERT_FILE, AWS_KEY_FILE)
        self.clientAWS.connect(AWS_HOST, AWS_PORT)

        self.clientTTN = mqtt.Client()
        self.clientTTN.on_connect = self.on_connect_ttn
        self.clientTTN.on_message = self.on_message_ttn
        self.clientTTN.tls_set(TTN_CA_CERT)
        self.clientTTN.username_pw_set(TTN_USERNAME, TTN_PASSWORD)
        self.clientTTN.connect(TTN_HOST, TTN_PORT)

        threading.Thread(target=lambda: self.send_aggregated_data()).start()

    def on_connect_aws(self, client, userdata, flags, rc):
        print("Connected to the AWS broker with result code " + str(rc))
        self.isLocalConnected = True

    def on_message_aws(self, client, userdata, msg):
        print("Received from AWS ", str(msg))

    def on_connect_ttn(self, client, userdata, flags, rc):
        print("Connected to the TTN broker with result code " + str(rc))

        # Subscribing in on_connect() means that if we lose the connection and
        # reconnect then subscriptions will be renewed.
        client.subscribe("+/devices/+/up")

    # The callback for when a PUBLISH message is received from the server.
    def on_message_ttn(self, client, userdata, msg):
        payload = json.loads(msg.payload)
        msg_str = payload['payload_raw']
        msg_str = base64.b64decode(msg_str).decode('utf-8')
        received_msg = json.loads(msg_str)
        soil = received_msg['soil']
        temp = received_msg['temp']
        hum = received_msg['hum']

        self.aggregator.add_temperature(temp)
        self.aggregator.add_hum(hum)
        self.aggregator.add_soil(soil)

    def send_aggregated_data(self):
        last_hash = hash(self.aggregator)
        last_time_ms = time.time_ns() / 1000
        while self.run:
            now_in_ms = time.time_ns() / 1000
            should_be_re_run = now_in_ms - last_time_ms > AGGREGATED_VALUE_SEND_INTERVAL_MS
            if should_be_re_run and self.isLocalConnected and self.aggregator.are_there_some_data() \
                    and last_hash != hash(self.aggregator):
                last_time_ms = time.time_ns() / 1000
                msg = {
                    "temp_max": self.aggregator.get_temp_max(),
                    "temp_min": self.aggregator.get_temp_min(),
                    "temp_avg": self.aggregator.get_temp_avg(),
                    "hum_max": self.aggregator.get_hum_max(),
                    "hum_min": self.aggregator.get_hum_min(),
                    "hum_avg": self.aggregator.get_hum_avg(),
                    "soil_max": self.aggregator.get_soil_max(),
                    "soil_min": self.aggregator.get_soil_min(),
                    "soil_avg": self.aggregator.get_soil_avg(),
                }
                self.clientAWS.publish("gh_aggr", json.dumps(msg))
                last_hash = hash(self.aggregator)

            time.sleep(1)

    def handler(self, signal_received, frame):
        # Handle any cleanup here
        print('CTRL-C detected. Exiting gracefully')
        self.run = False

    def run(self):
        signal(SIGINT, self.handler)
        while self.run:
            self.clientAWS.loop()
            self.clientTTN.loop()

        self.clientTTN.disconnect()
        self.clientAWS.disconnect()


b = Bridge()
b.run()
