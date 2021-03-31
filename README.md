# Green house powered by RIOT OS
### Work in progress
This project is an IoT Green house based on nucleo-f401re and RIOT OS, the greenhouse is fully
autonomous, it opens a windows when the environment humidity is too high, and there is
a pump that water the soil when it is dry, the pump get the water from a tank provided with level sensor, 
in such a way that if the tank is dry, the user will be allerted to refill it.

The advantages of having an IoT greenhouse are:
- Plats are always in a ideal environment, with the right soil moisture and air humidity
- Plats are watered even if the user are not present and the user can controls remotely that the plants are fine
- If well tuned it can increase the productivity.

### Sensors and actuators used
The sensor used are:
- DHT11, a digital sensor use to measure air temperature and humidty.
- SOIL MOISTURE sensor, an analog sensor used to measure the SOIL MOISTURE
- WATER LEVEL sensor, an analog sensor use to measure the water level insiede the tank

Tha actuators are:
- RELAY, used to toggle a 230V pump
- SERVO MOTOR, used to open or close a windows on the greenhouse

All sensor sensor are periodically fetched, the DHT11 and soil moisture sensor are fetched to 
detect when perform action, while the water level sensor is fetched to publish its value on user's interface.

#### Actions
The SERVO MOTOR is activated by this rule:

`IF DHT11.Humidiy > MAX_HUMIDITY THEN setServo(180) ELSE setServo(0)`

So when the air humidity gather that a fixed threshold the servo rotate of 180° opening the window, 
otherwise the window is closed

While for the RELAY:

`IF soil moisture < MIN_MOISTURE && water level > 20% THEN turn on ELSE turn off`

The relay is turned on only if the soil is dry and there is enough water in the tank.

The relay is turned on for a specific number of seconds is such a way to pump the right
quantity of water for the jar size.
Action is checked every 30 minutes in order to allow the water to penetrate in to the 
terrain and have a correct soil moisture read.

### Collected data

Each sensor report these data:
#### DHT11
- **Sensor type** digital
- **Temperature**
    - **Unit** Celsius degree °C
    - **Range** 0-50 °C
    - **Accuracy** at most ±2°C
- **Humidity**
    - **Unit** Relative Humidity %RH
    - **Range** 
        - 30 - 90 %RH at 0°C
        - 20 - 90 %RH at 25°C
    - **Accuracy** ±4 %RH at 25° C
    
#### Water level sensor
- **Sensor type** analog
- **Range**
    - 0V fully dry
    - VCC fully immersed 
    
#### Soil moisture
- **Sensor type** analog
- **Range**
    - 0V fully dry
    - VCC sensor in water
    
For analog sensors the fetched data is the tension read by the ADCs, it's converted in to
percentage by software.
       
Water level, Temperature, Humidity and Soil moisture are published on MQTT-SN broker. 

### Setup MQTT Broker
