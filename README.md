# SBUS Transmitter and Receiver
## Transmitter Remote Controller Radio Sbus Signal Receiver 2.4g 12ch

2.4G 6CH S-FHSS FSK Transmitter Remote Controller Radio System And SBUS Signal Receiver
Basic Parameters Of Remote Control:
1. Suitable For Use In: Fixed-wing Aircraft Model,Multi-rotor Model,Electric Vehicle, Electric/Model/Tanker;
2. Transmit Power: Less Than Or Equal To 1mW;
3. Transmission Frequency: 2401MHZ--2525MHZ;
4. Ground Control Range: Greater Than 800 Meters;
5. Transmitter Power Supply Requirements: DC+9V(Batteries);
6. Receiver Power Supply Requirements: DC 5V (Arduino Nano VIN 7.5-12V);

# What is SBUS protocol?
SBUS is a bus protocol for receivers to send commands to servos. Unlike PWM, SBUS uses a bus architecture where a single serial line can be connected with up to 16 servos with each receiving a unique command.

## What is the range of SBUS protocol?
SBUS messages consist of 25 UART bytes. A message provides 16 proportional channels of 11 bits each, 2 digital channels of 1 bit each, and two flags (frame lost, failsafe). Applications may communicate values between 192 and 1792 on the wire, and map these to values 1000 to 2000 for their firmware purposes.
![image](https://github.com/rizacelik/SBUS-transmitter-and-receiver/assets/19993109/0eb56246-5a30-48aa-ac02-1726b23f2328)

# Construction Process.
I used one PS2 Gamepad for the transmitter. I removed all the parts except the joystick. I made room for the on off p2 button and the Potantiometer using a blade. You can use a knife as it is a soft material.

![image](https://github.com/rizacelik/SBUS-transmitter-and-receiver/assets/19993109/cfb93903-9ff7-4c1e-bff0-d0dfb2c9a441)
![image](https://github.com/rizacelik/SBUS-transmitter-and-receiver/assets/19993109/62a7e5e6-70df-48ab-90a4-9045b1f5b3c2)
![image](https://github.com/rizacelik/SBUS-transmitter-and-receiver/assets/19993109/7e43b134-5977-4e9d-ab32-b30a462bc662)
![image](https://github.com/rizacelik/SBUS-transmitter-and-receiver/assets/19993109/1706f378-15e7-4cc8-b85f-856cd72cdb8a)
![image](https://github.com/rizacelik/SBUS-transmitter-and-receiver/assets/19993109/290a4d3c-28d3-4b65-94bc-1b24e161d523)
![image](https://github.com/rizacelik/SBUS-transmitter-and-receiver/assets/19993109/2a4ed7e5-695d-4b16-8acd-ebba42d7e795)
![image](https://github.com/rizacelik/SBUS-transmitter-and-receiver/assets/19993109/ca68c25e-8212-4bf1-9b58-72810b6dfe16)
![image](https://github.com/rizacelik/SBUS-transmitter-and-receiver/assets/19993109/42523d89-ead9-4b09-9566-459e62e32319)

# Circuit diagram(Transmitter)
![image](https://github.com/rizacelik/SBUS-transmitter-and-receiver/assets/19993109/6c013dae-b590-4879-86a7-0240f14436d4)

# Circuit diagram(Receiver)

![image](https://github.com/rizacelik/SBUS-transmitter-and-receiver/assets/19993109/b3b49363-efca-4b80-9fa0-e937e864ce38)

# Isolate the transmitter from external interference using aluminum tape.
![image](https://github.com/rizacelik/SBUS-transmitter-and-receiver/assets/19993109/81c78719-67fd-489f-917a-1c8c0b87591b)

