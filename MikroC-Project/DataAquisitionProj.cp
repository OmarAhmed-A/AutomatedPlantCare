#line 1 "C:/Users/omara/Desktop/VSCODE/PIC/MikroC-Project/DataAquisitionProj.c"







sbit SOIL_SENSOR_PIN at RA0_bit;
sbit LDR_PIN at RA1_bit;
sbit PUMP_PIN at RC2_bit;


sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D7 at RB3_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D4 at RB0_bit;


sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D7_Direction at TRISB3_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D4_Direction at TRISB0_bit;








void initUART();
void initPWM();
void startWatering();
void stopWatering();
void readSensors();
void logData(char temp, char hum);
void printToLCD(char *message);
void initTimer();
unsigned long millis();
unsigned short int dht11_read_byte();
void dht11_read(unsigned short int *dht11_humi, unsigned short int *dht11_temp);



unsigned int soilMoistureValue = 0;
int lightLevel = 0;
unsigned long lastLogTime = 0;
unsigned long wateringStartTime = 0;
bit autoMode;
unsigned long millisCount = 0;
char humidity, temperature;
char temp[] = "Temp = 00.0 C";
char humi[] = "Humi = 00.0 %";

void Interrupt()
{

 if (TMR1IF_bit)
 {
 TMR1IF_bit = 0;
 TMR1H = 0xF8;
 TMR1L = 0x30;
 millisCount++;
 }
}
void InitTimer1()
{
 T1CON = 0x01;
 TMR1IF_bit = 0;
 TMR1H = 0xF8;
 TMR1L = 0x30;
 TMR1IE_bit = 1;
 INTCON = 0xC0;
}

void main()
{

 ADCON0 = 0x01;
 TRISC2_bit = 0;
 autoMode = 1;

 Lcd_Init();
 Lcd_Cmd(_LCD_CLEAR);

 initUART();
 initPWM();


 while (1)
 {



 readSensors();


 dht11_read(&humidity, &temperature);

 temp[7] = temperature / 10 + '0';
 temp[8] = temperature % 10 + '0';
 humi[7] = humidity / 10 + '0';
 humi[8] = humidity % 10 + '0';
 temp[11] = 223;


 printToLCD("Logging data...");
 delay_ms(1000);
 logData(humidity, temperature);


 if (soilMoistureValue <  500  && autoMode)
 {
 startWatering();
 printToLCD("Watering...");
 }


 if (wateringStartTime > 0 && (millis() - wateringStartTime >=  1000 ))
 {
 stopWatering();
 wateringStartTime = 0;
 printToLCD("Watering complete");
 }


 if (millis() - lastLogTime >=  10000 )
 {
 printToLCD("Logging data...");
 logData(humidity, temperature);
 lastLogTime = millis();
 }


 if (UART1_Data_Ready())
 {
 char command[20];
 char delimiter[3] = "\r\n";
 UART1_Read_Text(command, delimiter, 255);

 if (strcmp(command, "water") == 0)
 {
 startWatering();
 }
 else if (strcmp(command, "stop") == 0)
 {
 stopWatering();
 }
 else if (strcmp(command, "auto") == 0)
 {
 autoMode = 1;
 }
 else if (strcmp(command, "manual") == 0)
 {
 autoMode = 0;
 }
 }

 delay_ms(100);
 }
}

void initUART()
{

 UART1_Init( 9600 );
}

void initPWM()
{

 PWM1_Init(5000);
 PWM1_Start();
 PWM1_Set_Duty(0);
}

void startWatering()
{

 PWM1_Set_Duty(75);
 wateringStartTime = millis();
}

void stopWatering()
{

 PWM1_Set_Duty(0);
}

void readSensors()
{


 soilMoistureValue = ADC_Read(SOIL_SENSOR_PIN);






 lightLevel = ADC_Read(LDR_PIN);


}

void logData(char Temp, char hum)
{


 char Data[50];
 sprinti(Data, "Soil:%.2d,Temp:%.1f,Humid:%.1f,Light:%.2d", soilMoistureValue, temp, hum, lightLevel);
 printToLCD(Data);


 UART1_Write_Text(Data);
}

void printToLCD(char *message)
{

 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1, 1, message);
}

void initTimer()
{


 INTCON.GIE = 1;

 INTCON.PEIE = 1;

 PIE1.TMR1IE = 1;

 TMR1L = 0x3C;
 TMR1H = 0xB0;
 T1CON.T1CKPS0 = 0;
 T1CON.T1CKPS1 = 0;

 T1CON.TMR1ON = 1;
}

unsigned long millis()
{

 return millisCount;
}

unsigned short int dht11_read_byte()
{
 unsigned short int i = 8, dht11_byte = 0;

 while (i--)
 {
 while (! RB6_bit )
 ;

 delay_us(40);

 if ( RB6_bit )
 {
 dht11_byte |= (1 << i);
 while ( RB6_bit )
 ;
 }
 }
 return (dht11_byte);
}


void dht11_read(unsigned short int *dht11_humi, unsigned short int *dht11_temp)
{

  RB6_bit  = 0;
  TRISB6_bit  = 0;
 delay_ms(25);
  RB6_bit  = 1;
 delay_us(70);
  TRISB6_bit  = 1;


 while ( RB6_bit )
 ;
 while (! RB6_bit )
 ;
 while ( RB6_bit )
 ;


 *dht11_humi = dht11_read_byte();
 dht11_read_byte();
 *dht11_temp = dht11_read_byte();
 dht11_read_byte();
 dht11_read_byte();
}
