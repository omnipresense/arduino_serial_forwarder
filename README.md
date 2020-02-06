# arduino_serial_forwarder
This arduino sketch will receive data from an OmniPreSense radar sensor and forward it to a connected USB serial port.

It takes the code from www.hackster.io/jimwhitfield/hack-of-the-future-a-flux-capacitor-with-omnipresense-radar-c6e954?f=1
and weaves it into an arduino sketch that reads from UART pins connected to the sensor and forwards information onto the connected USB serial connection.  (This is described at https://www.arduino.cc/en/Tutorial/TwoPortRXExample)

More specifically, It takes the comma-separated data received from the OmniPreSense sensor, pulls apart the unit string from the sensor reading (of velocity or distance), does a simple "unit string recognition" to determine if it is range or distance,  and outputs a more human-friendly output out to the USB serial port.

Also, at project initialization, it sets the baud rate of the connection to 19200 and issues the OU command to turn on output of units, so that this sketch should work if it is connected to an OPS243 Combo or any other version.
