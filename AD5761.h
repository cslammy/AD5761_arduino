//References: https://wiki.analog.com/resources/tools-software/uc-drivers/ad5761r
/* Input Shift Register Commands */
#define CMD_NOP              0x0
#define CMD_WR_TO_INPUT_REG       0x1
#define CMD_UPDATE_DAC_REG        0x2
#define CMD_WR_UPDATE_DAC_REG     0x3
#define CMD_WR_CTRL_REG         0x4
#define CMD_NOP_ALT_1         0x5
#define CMD_NOP_ALT_2         0x6
#define CMD_SW_DATA_RESET       0x7
#define CMD_RESERVED          0x8
#define CMD_DIS_DAISY_CHAIN       0x9
#define CMD_RD_INPUT_REG        0xA
#define CMD_RD_DAC_REG          0xB
#define CMD_RD_CTRL_REG         0xC
#define CMD_NOP_ALT_3         0xD
#define CMD_NOP_ALT_4         0xE
#define CMD_SW_FULL_RESET       0xF

void AD5761_initialize_SPI();
void AD5761_full_reset(); 
void AD5761_software_reset();
void AD5761_write(uint8_t reg_addr_cmd,uint16_t reg_data);
void AD5761_out(uint16_t reg_data);
void AD5761_read(uint8_t reg_addr_cmd);
void AD5761_reg_to_print();
void AD5761_printRegisterData();

static byte SPI_Buff[3] = {0,0,0};  // array used to read data 


const int ssPin = 17; //promicro D17 or PB0

void AD5761_initialize_SPI() {
  pinMode(ssPin, OUTPUT);  // Set the SS pin as an output
  digitalWrite(ssPin, HIGH);  // Set the SS pin HIGH
  SPI.begin();  // Begin SPI hardware
  SPI.setClockDivider(SPI_CLOCK_DIV64);  // Slow down SPI clock
  SPI.setDataMode(SPI_MODE1); // chatgpt says SPI MODE 1 not 2.  
  //doesn't seem to matter.
  //SPI.setDataMode(SPI_MODE2); 
}

void AD5761_full_reset() {

    AD5761_write(CMD_SW_FULL_RESET, 0); 
}

void AD5761_software_reset() {

    AD5761_write(CMD_SW_DATA_RESET, 0); 
}

void AD5761_write(uint8_t reg_addr_cmd, uint16_t reg_data)
{
  uint8_t data[3];
  delay(1);
  digitalWrite(ssPin, LOW);
 delay(1);

  data[0] = reg_addr_cmd;
  data[1] = (reg_data & 0xFF00) >> 8;
  data[2] = (reg_data & 0x00FF) >> 0;
  for (int i=0; i<3; i++)
  {
    SPI.transfer(data[i]);
  }
  delay(1);
  digitalWrite(ssPin, HIGH);
  delay(1);
}

void AD5761_out(uint16_t reg_data)
{
  uint8_t data[3];
  delay(1);
  digitalWrite(ssPin, LOW);
 delay(1);

  data[0] = CMD_WR_UPDATE_DAC_REG;
  data[1] = (reg_data & 0xFF00) >> 8;
  data[2] = (reg_data & 0x00FF) >> 0;
  for (int i=0; i<3; i++)
  {
    SPI.transfer(data[i]);
  }
  delay(1);
  digitalWrite(ssPin, HIGH);
  delay(1);
}


void AD5761_read(uint8_t reg_addr_cmd)
{
  delay(1);
  digitalWrite(ssPin, LOW);
  delay(1);
  SPI_Buff[0] = SPI.transfer(reg_addr_cmd);
  SPI_Buff[1] = SPI.transfer(0xCA); // dummy
  SPI_Buff[2] = SPI.transfer(0xAC); // dummy
  delay(1);
  digitalWrite(ssPin, HIGH);
}

void AD5761_reg_to_print(){
  AD5761_read(CMD_RD_CTRL_REG);
  AD5761_read(CMD_RD_CTRL_REG);
}

void AD5761_printRegisterData() {
  Serial.print("CMD:");
  Serial.print(SPI_Buff[0], HEX);
  
  Serial.print(" DATA:");
  Serial.print(SPI_Buff[1], HEX);
  Serial.print(" ");

  Serial.println(SPI_Buff[2], HEX)
  ;
}


 