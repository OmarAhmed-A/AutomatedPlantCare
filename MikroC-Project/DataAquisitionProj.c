

// DHT11 pin connection (here data pin is connected to pin RB0)
#define DHT11_PIN RB6_bit
#define DHT11_PIN_DIR TRISB6_bit

// Define pins
sbit SOIL_SENSOR_PIN at RA0_bit; // Soil moisture sensor on analog pin AN0
sbit LDR_PIN at RA1_bit;         // LDR sensor on analog pin AN1
sbit PUMP_PIN at RC2_bit;        // Water pump on digital pin RC2

// LCD module connections
sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D7 at RB3_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D4 at RB0_bit;

// Pin direction
sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D7_Direction at TRISB3_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D4_Direction at TRISB0_bit;

// Define constants
#define SOIL_MOISTURE_THRESHOLD 500 // Threshold for dry soil (adjust as needed)
#define WATERING_DURATION 1000      // Watering duration in milliseconds
#define LOG_INTERVAL 10000          // Logging interval in milliseconds (1 hour)
#define BAUD_RATE 9600              // UART baud rate for communication with ESP32

// Function prototypes
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

// Global variables

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

    ADCON0 = 0x01;  // Enable ADC
    TRISC2_bit = 0; // Set RC2 as output (Water pump)
    autoMode = 1;

    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);

    initUART();
    initPWM();
    //    InitTimer1();

    while (1)
    {

        // Read sensor data

        readSensors();

        // read humidity (in rH%) and temperature (in ?C) from the DHT11 sensor
        dht11_read(&humidity, &temperature);

        temp[7] = temperature / 10 + '0';
        temp[8] = temperature % 10 + '0';
        humi[7] = humidity / 10 + '0';
        humi[8] = humidity % 10 + '0';
        temp[11] = 223; // put degree symbol (?)
                        // lcd_out(1, 1, temp);
                        // lcd_out(2, 1, humi);
        printToLCD("Logging data...");
        delay_ms(1000);
        logData(humidity, temperature);

        // Check if watering is needed
        if (soilMoistureValue < SOIL_MOISTURE_THRESHOLD && autoMode)
        {
            startWatering();
            printToLCD("Watering...");
        }

        //         Check if watering duration has elapsed
        if (wateringStartTime > 0 && (millis() - wateringStartTime >= WATERING_DURATION))
        {
            stopWatering();
            wateringStartTime = 0;
            printToLCD("Watering complete");
        }

        //         Log data at specified interval
        if (millis() - lastLogTime >= LOG_INTERVAL)
        {
            printToLCD("Logging data...");
            logData(humidity, temperature);
            lastLogTime = millis();
        }

        // Check for incoming commands from ESP32
        if (UART1_Data_Ready())
        {
            char command[20];
            char delimiter[3] = "\r\n";               // Delimiter is a carriage return and newline
            UART1_Read_Text(command, delimiter, 255); // Read until delimiter is found

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
    // Configure UART settings
    UART1_Init(BAUD_RATE);
}

void initPWM()
{
    // Configure PWM module
    PWM1_Init(5000); // Set PWM frequency to 5 kHz
    PWM1_Start();
    PWM1_Set_Duty(0); // Set initial duty cycle to 0%
}

void startWatering()
{
    // Start watering by setting PWM duty cycle
    PWM1_Set_Duty(75); // Set duty cycle to 75%
    wateringStartTime = millis();
}

void stopWatering()
{
    // Stop watering by setting PWM duty cycle to 0%
    PWM1_Set_Duty(0);
}

void readSensors()
{

    // Read soil moisture sensor
    soilMoistureValue = ADC_Read(SOIL_SENSOR_PIN);

    // Read DHT11 sensor

    // IDFK yet https://libstock.mikroe.com/projects/view/1229/dht11-humidity-and-temperature-sensor-mikroc-library

    // Read LDR sensor
    lightLevel = ADC_Read(LDR_PIN);

    // Lcd_Out(1, 1, temp);
}

void logData(char Temp, char hum)
{

    // Prepare log data string
    char Data[50];
    sprinti(Data, "Soil:%.2d,Temp:%.1f,Humid:%.1f,Light:%.2d", soilMoistureValue, temp, hum, lightLevel);
    printToLCD(Data);

    // Send log data to ESP32
    UART1_Write_Text(Data);
}

void printToLCD(char *message)
{
    // Print message to LCD
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1, 1, message);
}

void initTimer()
{

    // Enables all interrupts
    INTCON.GIE = 1;
    // Enables all peripheral interrupts
    INTCON.PEIE = 1;
    // Enables the TMR1 overflow interrupt
    PIE1.TMR1IE = 1;
    // 45536 inst to count 1 ms
    TMR1L = 0x3C;
    TMR1H = 0xB0;
    T1CON.T1CKPS0 = 0;
    T1CON.T1CKPS1 = 0;
    // Enables Timer1
    T1CON.TMR1ON = 1;
}

unsigned long millis()
{
    // Function to get the current time in milliseconds
    return millisCount;
}

unsigned short int dht11_read_byte()
{
    unsigned short int i = 8, dht11_byte = 0;

    while (i--)
    {
        while (!DHT11_PIN)
            ;

        delay_us(40);

        if (DHT11_PIN)
        {
            dht11_byte |= (1 << i); // set bit i
            while (DHT11_PIN)
                ;
        }
    }
    return (dht11_byte);
}

// read humidity (in rH%) and temperature (in ?Celsius) from sensor
void dht11_read(unsigned short int *dht11_humi, unsigned short int *dht11_temp)
{
    // send start signal
    DHT11_PIN = 0;     // connection pin output low
    DHT11_PIN_DIR = 0; // configure connection pin as output
    delay_ms(25);      // wait 25 ms
    DHT11_PIN = 1;     // connection pin output high
    delay_us(70);      // wait 30 us
    DHT11_PIN_DIR = 1; // configure connection pin as input

    // check sensor response
    while (DHT11_PIN)
        ;
    while (!DHT11_PIN)
        ;
    while (DHT11_PIN)
        ;

    // read data
    *dht11_humi = dht11_read_byte(); // read humidity byte 1
    dht11_read_byte();               // read humidity byte 2 (skipped)
    *dht11_temp = dht11_read_byte(); // read temperature byte 1
    dht11_read_byte();               // read temperature byte 2 (skipped)
    dht11_read_byte();               // read checksum (skipped)
}