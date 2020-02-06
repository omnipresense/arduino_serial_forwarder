/*
   OmniPreSense Sensor reader for arduino.
   The readings from the OmniPreSense radar are read by this sketch
   Some light-duty processing is performed ont he input
   The output is sent out the Arduino back to a host connected by USB
   The listening program can be the arduino Serial Monitor OR any program that can read USB ports, such as
   TeraTerm or Putty (or on Linux, minicom or screen)

  The circuit for an Arduino Uno or equivalent:
   RX is digital pin 10 (connect to TX of the OmniPreSense sensor)
   TX is digital pin 11 (connect to RX of the OmniPreSense sensor)

  This is based on the "Software serial multple serial test" found at the Arduino web site.

  created back in the mists of time
  modified 25 May 2012
  by Tom Igoe
  based on Mikal Hart's example

  This example code is in the public domain.

*/
#include <SoftwareSerial.h>

// this line lets us call the Serial object by a more descriptive name, usb_serial
HardwareSerial & usb_serial = Serial;
// this declares that we have an OmniPreSense board hooked up to Arduino pin 10 & 11
SoftwareSerial ops_serial(10, 11); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(19200);
  while (!usb_serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  usb_serial.println("Setting up connection to sensor at 19200 baud!");

  // set the data rate for the SoftwareSerial port
  ops_serial.begin(19200);
  ops_serial.write("\r\nOU\r\n"); // insist on getting the unit before the value
}

float reading = 0.0;
String inString = "";    // string to hold speed input from serial line
void loop() { // run over and over
  // pass along anything from teh USB port to the OPS board connected via pin 10&11
  if (usb_serial.available()) {
    char c = usb_serial.read();
    //debug: let me see what I typed
    //usb_serial.write(c);
    ops_serial.write(c);
  }

  if (ops_serial.available() > 0)
  {
    int inChar = ops_serial.read();

    // if it's the end of a line....
    if (inChar != '\n' && inChar != '\r')
    {
      // As long as the incoming byte is not a newline,
      // convert the incoming byte to a char and add it to the string
      inString += (char)inChar;
    }
    else {
      // if you get a line with a reading, process it
      if (inString.length() > 1 && inString.charAt(0) != '{') {  // but ignore empty lines and JSON
        char *comma_pos = inString.indexOf(',');
        String units = "";
        if (comma_pos < 1) { // no comma found, so lets assume it's not a OPS243C and that it's a velocity
          reading = inString.toFloat();
          units = "";
        } else {
          // split the line into unit and reading.  Unit string is always surrounded by ""
          units = inString.substring(0, comma_pos);
          // now lets remove the quotes around the units
          if (units.charAt(0) == '"') {
            units = units.substring(1, units.length() - 1);
            inString.remove(0, comma_pos + 1); // now delete the units value and the comma
            reading = inString.toFloat();
          }
        }

        // Aggressively round close to zero
        if (reading < 0.1 && reading > -0.1)  
          reading = 0.0;

        if (reading != 0) {
          if (units.equals("m") || units.equals("cm") || units.equals("in") || units.equals("ft") || units.equals("yd")) {
            usb_serial.print("Distance: ");
            usb_serial.print(reading);
            usb_serial.print(" ");
            usb_serial.print(units);
            usb_serial.println();
          } else { // again, if no unit, assume it's a velocity
            usb_serial.print("Velocity: ");
            usb_serial.print(reading);
            usb_serial.print(" ");
            usb_serial.print(units);
            usb_serial.println();
          }
        }
      }

      // clear the string for new input:
      inString = "";
    }
  }
}
