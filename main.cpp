/*
On Taranis you have to configure switch 'SD or another' for activate trainer mod
In my case: cable to Taranis, female connector have to be white color and male connector to Arduino have to be red color  
*/

#include <hiduniversal.h>
#include <usbhub.h>
#include "hid_joystick_rptparser.hpp"
#include "ppm_encoder.hpp"

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

#define OUT_PPM_PIN 13
#define BAUD_RATE 9600
#define ID 0
#define CHANNEL_ZERO 0
#define CHANNEL_ONE 1
#define CHANNEL_TWO 2
#define CHANNEL_THREE 3

//Global variables
uint16_t joyX{0};       
uint16_t joyY{0};       
uint16_t throttle{0};

//Objects
USB Usb;
USBHub Hub(&Usb);
HIDUniversal Hid(&Usb);
JoystickEvents JoyEvents;
JoystickReportParser Joy(&JoyEvents);


void setup() {
  
  Serial.begin(BAUD_RATE);
   //*********Joystick section *************
  
  if (Usb.Init() == -1)
      Serial.println("OSC did not start.");

  delay(200);

  if (!Hid.SetReportParser(ID, &Joy))
      ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1  );
  //*****************************************
  
  ppmEncoder.init(OUT_PPM_PIN);
}


void loop() {
  //*********Joystick section***********
  //Print all the values into the serial monitor:
    Usb.Task();
    /*       
    Serial.print(" X: ");
    Serial.print(joyX);
    Serial.print(" Y: ");
    Serial.print(joyY);
    Serial.print(" Throttle: ");
    Serial.print(throttle);
    Serial.println();
    */
    //*************************************
    
    ppmEncoder.setTXChannel(CHANNEL_ZERO, joyX);
    ppmEncoder.setTXChannel(CHANNEL_ONE, joyY);
    ppmEncoder.setTXChannel(CHANNEL_TWO, throttle);
  
}

// Joystick method
void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt)
{
  //Put the values from the joystick in another variable:
  joyX = JoystickEvents::MapValue(evt->X);
  joyY = JoystickEvents::MapValue(evt->Y);
  throttle = JoystickEvents::MapValue(evt->Rz);
}




