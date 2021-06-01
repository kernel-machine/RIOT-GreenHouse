# Greenhouse powered by RIOT OS
- [First assignment](https://github.com/kernel-machine/RIOT-GreenHouse/tree/first_assignment)
- [Second assignment](https://github.com/kernel-machine/RIOT-GreenHouse/tree/second_assignment)

This project is an IoT Greenhouse based on nucleo-f401re and RIOT OS, the greenhouse is fully
autonomous, it opens a windows when the indoor environment humidity is too high, and there is
a pump that water the soil when it is dry, the pump get the water from a tank provided with a level sensor, 
in such a way that if the tank is dry, the pump isn't turn on to avoid to damage it.

The greenhouse can be monitored by the [web interface](https://kernel-machine.github.io/RIOT-GreenHouse/)

The advantages of having an IoT greenhouse are:
- The plants are always in a ideal environment, with the right soil moisture and air humidity
- The plants are watered even if the user are not present and the user can controls remotely that the plants are fine
- If well tuned it can increase the productivity.
- If well tuned it can save water, watering only if needed with the optimal quantity
### First assignment
- [Technical details](./Tech.md)

### Second Assignment
- [Technical details with new network diagram](./Tech.md)
- [Setup 802.15.4 mesh network](./docs/SecondAssignment/Setup6LoWPAN.md)
- [Evaluation](./docs/SecondAssignment/Evaluation.md)

### Third Assignment
- [How to run](./docs/ThirdAssignment/HowToRun.md)
- [LoRaWAN analysis](./docs/ThirdAssignment)

### Additional materials
- [Blog post](https://luca-giovannesi.medium.com/diy-iot-greenhouse-ef70b42969fa)
- [YouTube video](https://youtu.be/ebZW7UfPQpk)
