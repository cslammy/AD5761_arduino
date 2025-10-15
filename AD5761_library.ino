#include <SPI.h> // Include the Arduino SPI library
#include "AD5761.h"

//const int ssPin = 10;




void setup()
{
  Serial.begin(9600);
  //next while needed for serial and Micro/Leonardo.
  while (!Serial) {
                   ;
  }
  Serial.println("Arduino Micro Serial Enabled");

//initialize SPI settings.
AD5761_initialize_SPI();
  
  // software reset
AD5761_software_reset(); 
 
Serial.println("Readback from control register before setting:");

AD5761_reg_to_print();
  
AD5761_printRegisterData();

  // write control register
  // [23:21][20][19:16]   [15:11]  [10:9] 8   7   6   5 [4:3] [2:0]
  // |      |  |        |          |    |   |   |   |   |    |      |
  //   X X X  0  0 1 0 0  X X X X X  CV  OVR B2C ETS IRO  PV    RA
  //
  //                                 CV  : Clear voltage selection 00=zero, 01=midscale, 10,11=full scale
  //                                 OVR : 5% overrange 0=5% overrange disabled, 1=5% overrange enabled
  //                                 B2C : Bipolar range 0=DAC input for bipolar range is straight binary coded
  //                                                     1=DAC input for bipolar output range is twos complement code
  //                                 ETS : Thermal shutdown alert 0=does not power down when die temperature exceeds 150degC
  //                                                              1=powers down when die temperature exceeds 150degC
  //                                 IRO : Internal reference 0=off, 1=on
  //                                 PV  : Power up voltage 00=zero scale, 01=midscale, 10,11=full scale
  //                                 RA  : Output range
  //                                       000=-10 to +10
  //                                       001=0 to +10
  //                                       010=-5 to +5
  //                                       011=0 to +5
  //                                       100=-2.5 to +7.5
  //                                       101=-3 to +3
  //                                       110=0 to +16
  //                                       111=0 to +20
  
  AD5761_write(CMD_WR_CTRL_REG, 0b0000000000000);

  Serial.println("Readback from control register after setting:");
  // read control register
  AD5761_reg_to_print();

  AD5761_printRegisterData();

}


//test output
void loop()
{

  AD5761_out(0x0000);
  delay(200);  

 AD5761_out(0x2000);
  delay(200);

  AD5761_out(0x4000);
  delay(200);
  
  AD5761_out(0x6000);
  delay(200);

  AD5761_out(0x8000);
  delay(200);
  
  AD5761_out(0xb000);
  delay(200);
  
  AD5761_out(0xd000);
  delay(200);

  AD5761_out(0xffff);
  delay(200);

}
 