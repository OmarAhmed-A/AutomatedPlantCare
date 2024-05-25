// Define pins

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
#define BAUD_RATE 9600 // UART baud rate for communication with ESP32

// Function prototypes
void initUART();
void initPWM();
void startWatering();
void stopWatering();
void printToLCD(char *message);
void StartSignal();
unsigned short CheckResponse();
unsigned short ReadByte();
void ReadDHT11();

// globals
bit wateringState; // Variable to track watering state
char command;
char message[] = "00.0";
unsigned short TOUT = 0, CheckSum, i;
unsigned int T_Byte1, T_Byte2, RH_Byte1, RH_Byte2;
int moist, light;
char buff[45];

sbit DHT at RB6_bit;
sbit DataDir at TRISB6_bit;



//STRING REQUIRED FOR UARTTTT "temp:40 moist:90 humid:80 light:889"

void main()
{
    ADCON0 = 0x01;  // Enable ADC
    TRISC2_bit = 0; // Set RC2 as output (Water pump)
    TRISB1_bit = 1; // Set RB1 as input (DHT11)

    // Set up LCD
    TRISB &= ~0xF0; // Set RB4-RB7 as outputs for LCD data lines
    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);

    initUART();
    initPWM();
    // InitADC();
    ADC_Init();
    wateringState = 0;

    INTCON.GIE = 1;  // Enable global interrupt
    INTCON.PEIE = 1; // Enable peripheral interrupt
    // Configure Timer2 module
    PIE1.TMR2IE = 1; // Enable Timer2 interrupt
    T2CON = 0;       // Prescaler 1:1, and Timer2 is off initially
    PIR1.TMR2IF = 0; // Clear TMR INT Flag bit
    TMR2 = 0;
    
    while (1)
    {
        // Check for incoming commands from ESP32
        INTCON.GIE = 0;
        ReadDHT11();
        INTCON.GIE = 1;

        printToLCD("Reading sensors...");
        moist = ADC_Get_Sample(2);
        // buff[16] = moist / 10 + 48;
        // buff[17] = moist % 10 + 48;
        light = ADC_Get_Sample(3);
        // buff[36] = light / 10 + 48;
        // buff[37] = light % 10 + 48;
        //LDR to LUX
        light = light * 0.48875855327;
        for (i = 0; i < 5; i++)
        {
            message[i] = 0;
        }

        printToLCD("Moisture:");
        IntToStr(moist, message);
        Lcd_Out(1, 11, message);
        Lcd_Out(1, 16, "%");


        uart1_write_text(" ");
        uart1_write_text("moist:");
        delay_ms(20);
        uart1_write_text(message);
        delay_ms(20);

        for (i = 0; i < 5; i++)
        {
            message[i] = 0;
        }
        // delay_ms(1000);

        Lcd_Out(2, 1, "Light:");
        IntToStr(light, message);
        Lcd_Out(2, 11, message);
        Lcd_Out(2, 16, "%");



        uart1_write_text(" ");
        uart1_write_text("light:");
        delay_ms(20);
        uart1_write_text(message);
        delay_ms(20);

        UART1_Write_Text("\r\n");



        delay_ms(200);

        //Clear "message"
        for (i = 0; i < 5; i++)
        {
            message[i] = 0;
        }




        

        

        if (UART1_Data_Ready())
        {

            command = UART1_Read(); // Read a single character and wait for carriage return

            if (command == '1')
            {
                startWatering();
                wateringState = 1;
                printToLCD("Watering...");
            }
            else if (command == '2')
            {
                stopWatering();
                wateringState = 0;
                printToLCD("Watering stopped");
            }
        }

        // printToLCD("Uart msg");

        // //printToLCD(buff);

        // // sprinti(buff, "temp:%s moist:%d humid:%s light:%d ", tempstr , moist, humidstr, light);
        // printToLCD("sprintf done");
        // delay_ms(200);

        
        // uart1_write_text(buff);
        // printToLCD("Uart msg sent");
        // delay_ms(200);

        // UART1_Write_Text("\r\n");
        // printToLCD("rn msg sent");
        // delay_ms(200);

        delay_ms(200);

        
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
    PWM1_Set_Duty(192); // Set duty cycle to 75%
}

void stopWatering()
{
    // Stop watering by setting PWM duty cycle to 0%
    PWM1_Set_Duty(0);
}

void printToLCD(char *message)
{
    // Print message to LCD
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Out(1, 1, message);
}
void StartSignal()
{
    DataDir = 0; // Data port is output
    DHT = 0;
    Delay_ms(25);
    DHT = 1;
    Delay_us(30);
    DataDir = 1; // Data port is input
}

unsigned short CheckResponse()
{
    TOUT = 0;
    TMR2 = 0;
    T2CON.TMR2ON = 1; // start timer
    while (!DHT && !TOUT)
        ;
    if (TOUT)
        return 0;
    else
    {
        TMR2 = 0;
        while (DHT && !TOUT)
            ;
        if (TOUT)
            return 0;
        else
        {
            T2CON.TMR2ON = 0;
            return 1;
        }
    }
}

unsigned short ReadByte()
{
    unsigned short num = 0, t;
    DataDir = 1;
    for (i = 0; i < 8; i++)
    {
        while (!DHT)
            ;
        Delay_us(40);
        if (DHT)
            num |= 1 << (7 - i);
        while (DHT)
            ;
    }
    return num;
}
void ReadDHT11()
{
    unsigned short check;
    PIE1.TMR2IE = 1; // Enable Timer2 interrupt
    StartSignal();
    check = CheckResponse();

    RH_Byte1 = ReadByte();
    RH_Byte2 = ReadByte();
    T_Byte1 = ReadByte();
    T_Byte2 = ReadByte();
    CheckSum = ReadByte();
    // Check for error in Data reception

    lcd_cmd(_lcd_clear);
    lcd_out(1, 1, "Read");
    delay_ms(1000);

    if (CheckSum == ((RH_Byte1 + RH_Byte2 + T_Byte1 + T_Byte2) & 0xFF))
    {
        Lcd_Out(1, 1, "HUMIDITY:");

        message[0] = RH_Byte1 / 10 + 48;
        message[1] = RH_Byte1 % 10 + 48;
        // buff[26] = RH_Byte1 / 10 + 48;
        // buff[27] = RH_Byte1 % 10 + 48;
        Lcd_Out(1, 11, message);
        Lcd_Out(1, 16, "%");

        UART1_Write_Text("humid:");
        delay_ms(20);
        UART1_Write_Text(message);
        delay_ms(20);

       // uart1_write('H');
        //uart1_write_text(message);
        //UART1_Write_Text("\r\n");
        delay_ms(100);
        Lcd_Out(2, 1, "TEMP:");
        message[0] = T_Byte1 / 10 + 48;
        message[1] = T_Byte1 % 10 + 48;
        // buff[6] = T_Byte1 / 10 + 48;
        // buff[7] = T_Byte1 % 10 + 48;
        Lcd_Out(2, 11, message);
        Lcd_Chr_CP(223);
        Lcd_Out(2, 16, "C");

        uart1_write_text(" ");
        UART1_Write_Text("temp:");
        delay_ms(20);
        UART1_Write_Text(message);
        delay_ms(20);

       // uart1_write('T');
        //uart1_write_text(message);
        //UART1_Write_Text("\r\n");
        Delay_ms(200);
    }
    lcd_cmd(_lcd_clear);
    lcd_out(1, 1, "finish");
    delay_ms(100);

    PIE1.TMR2IE = 0; // disable Timer2 interrupt
}