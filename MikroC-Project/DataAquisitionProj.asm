
_Interrupt:
	MOVWF      R15+0
	SWAPF      STATUS+0, 0
	CLRF       STATUS+0
	MOVWF      ___saveSTATUS+0
	MOVF       PCLATH+0, 0
	MOVWF      ___savePCLATH+0
	CLRF       PCLATH+0

;DataAquisitionProj.c,59 :: 		void Interrupt()
;DataAquisitionProj.c,62 :: 		if (TMR1IF_bit)
	BTFSS      TMR1IF_bit+0, BitPos(TMR1IF_bit+0)
	GOTO       L_Interrupt0
;DataAquisitionProj.c,64 :: 		TMR1IF_bit = 0;
	BCF        TMR1IF_bit+0, BitPos(TMR1IF_bit+0)
;DataAquisitionProj.c,65 :: 		TMR1H = 0xF8;
	MOVLW      248
	MOVWF      TMR1H+0
;DataAquisitionProj.c,66 :: 		TMR1L = 0x30;
	MOVLW      48
	MOVWF      TMR1L+0
;DataAquisitionProj.c,67 :: 		millisCount++;
	MOVF       _millisCount+0, 0
	MOVWF      R0+0
	MOVF       _millisCount+1, 0
	MOVWF      R0+1
	MOVF       _millisCount+2, 0
	MOVWF      R0+2
	MOVF       _millisCount+3, 0
	MOVWF      R0+3
	INCF       R0+0, 1
	BTFSC      STATUS+0, 2
	INCF       R0+1, 1
	BTFSC      STATUS+0, 2
	INCF       R0+2, 1
	BTFSC      STATUS+0, 2
	INCF       R0+3, 1
	MOVF       R0+0, 0
	MOVWF      _millisCount+0
	MOVF       R0+1, 0
	MOVWF      _millisCount+1
	MOVF       R0+2, 0
	MOVWF      _millisCount+2
	MOVF       R0+3, 0
	MOVWF      _millisCount+3
;DataAquisitionProj.c,68 :: 		}
L_Interrupt0:
;DataAquisitionProj.c,69 :: 		}
L_end_Interrupt:
L__Interrupt39:
	MOVF       ___savePCLATH+0, 0
	MOVWF      PCLATH+0
	SWAPF      ___saveSTATUS+0, 0
	MOVWF      STATUS+0
	SWAPF      R15+0, 1
	SWAPF      R15+0, 0
	RETFIE
; end of _Interrupt

_InitTimer1:

;DataAquisitionProj.c,70 :: 		void InitTimer1()
;DataAquisitionProj.c,72 :: 		T1CON = 0x01;
	MOVLW      1
	MOVWF      T1CON+0
;DataAquisitionProj.c,73 :: 		TMR1IF_bit = 0;
	BCF        TMR1IF_bit+0, BitPos(TMR1IF_bit+0)
;DataAquisitionProj.c,74 :: 		TMR1H = 0xF8;
	MOVLW      248
	MOVWF      TMR1H+0
;DataAquisitionProj.c,75 :: 		TMR1L = 0x30;
	MOVLW      48
	MOVWF      TMR1L+0
;DataAquisitionProj.c,76 :: 		TMR1IE_bit = 1;
	BSF        TMR1IE_bit+0, BitPos(TMR1IE_bit+0)
;DataAquisitionProj.c,77 :: 		INTCON = 0xC0;
	MOVLW      192
	MOVWF      INTCON+0
;DataAquisitionProj.c,78 :: 		}
L_end_InitTimer1:
	RETURN
; end of _InitTimer1

_main:

;DataAquisitionProj.c,80 :: 		void main()
;DataAquisitionProj.c,83 :: 		ADCON0 = 0x01;  // Enable ADC
	MOVLW      1
	MOVWF      ADCON0+0
;DataAquisitionProj.c,84 :: 		TRISC2_bit = 0; // Set RC2 as output (Water pump)
	BCF        TRISC2_bit+0, BitPos(TRISC2_bit+0)
;DataAquisitionProj.c,85 :: 		autoMode = 1;
	BSF        _autoMode+0, BitPos(_autoMode+0)
;DataAquisitionProj.c,87 :: 		Lcd_Init();
	CALL       _Lcd_Init+0
;DataAquisitionProj.c,88 :: 		Lcd_Cmd(_LCD_CLEAR);
	MOVLW      1
	MOVWF      FARG_Lcd_Cmd_out_char+0
	CALL       _Lcd_Cmd+0
;DataAquisitionProj.c,90 :: 		initUART();
	CALL       _initUART+0
;DataAquisitionProj.c,91 :: 		initPWM();
	CALL       _initPWM+0
;DataAquisitionProj.c,94 :: 		while (1)
L_main1:
;DataAquisitionProj.c,99 :: 		readSensors();
	CALL       _readSensors+0
;DataAquisitionProj.c,102 :: 		dht11_read(&humidity, &temperature);
	MOVLW      _humidity+0
	MOVWF      FARG_dht11_read_dht11_humi+0
	MOVLW      _temperature+0
	MOVWF      FARG_dht11_read_dht11_temp+0
	CALL       _dht11_read+0
;DataAquisitionProj.c,104 :: 		temp[7] = temperature / 10 + '0';
	MOVLW      10
	MOVWF      R4+0
	MOVF       _temperature+0, 0
	MOVWF      R0+0
	CALL       _Div_8X8_U+0
	MOVLW      48
	ADDWF      R0+0, 0
	MOVWF      _temp+7
;DataAquisitionProj.c,105 :: 		temp[8] = temperature % 10 + '0';
	MOVLW      10
	MOVWF      R4+0
	MOVF       _temperature+0, 0
	MOVWF      R0+0
	CALL       _Div_8X8_U+0
	MOVF       R8+0, 0
	MOVWF      R0+0
	MOVLW      48
	ADDWF      R0+0, 0
	MOVWF      _temp+8
;DataAquisitionProj.c,106 :: 		humi[7] = humidity / 10 + '0';
	MOVLW      10
	MOVWF      R4+0
	MOVF       _humidity+0, 0
	MOVWF      R0+0
	CALL       _Div_8X8_U+0
	MOVLW      48
	ADDWF      R0+0, 0
	MOVWF      _humi+7
;DataAquisitionProj.c,107 :: 		humi[8] = humidity % 10 + '0';
	MOVLW      10
	MOVWF      R4+0
	MOVF       _humidity+0, 0
	MOVWF      R0+0
	CALL       _Div_8X8_U+0
	MOVF       R8+0, 0
	MOVWF      R0+0
	MOVLW      48
	ADDWF      R0+0, 0
	MOVWF      _humi+8
;DataAquisitionProj.c,108 :: 		temp[11] = 223; // put degree symbol (?)
	MOVLW      223
	MOVWF      _temp+11
;DataAquisitionProj.c,111 :: 		printToLCD("Logging data...");
	MOVLW      ?lstr1_DataAquisitionProj+0
	MOVWF      FARG_printToLCD_message+0
	CALL       _printToLCD+0
;DataAquisitionProj.c,112 :: 		delay_ms(1000);
	MOVLW      11
	MOVWF      R11+0
	MOVLW      38
	MOVWF      R12+0
	MOVLW      93
	MOVWF      R13+0
L_main3:
	DECFSZ     R13+0, 1
	GOTO       L_main3
	DECFSZ     R12+0, 1
	GOTO       L_main3
	DECFSZ     R11+0, 1
	GOTO       L_main3
	NOP
	NOP
;DataAquisitionProj.c,113 :: 		logData(humidity, temperature);
	MOVF       _humidity+0, 0
	MOVWF      FARG_logData_temp+0
	MOVF       _temperature+0, 0
	MOVWF      FARG_logData_hum+0
	CALL       _logData+0
;DataAquisitionProj.c,116 :: 		if (soilMoistureValue < SOIL_MOISTURE_THRESHOLD && autoMode)
	MOVLW      1
	SUBWF      _soilMoistureValue+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main42
	MOVLW      244
	SUBWF      _soilMoistureValue+0, 0
L__main42:
	BTFSC      STATUS+0, 0
	GOTO       L_main6
	BTFSS      _autoMode+0, BitPos(_autoMode+0)
	GOTO       L_main6
L__main37:
;DataAquisitionProj.c,118 :: 		startWatering();
	CALL       _startWatering+0
;DataAquisitionProj.c,119 :: 		printToLCD("Watering...");
	MOVLW      ?lstr2_DataAquisitionProj+0
	MOVWF      FARG_printToLCD_message+0
	CALL       _printToLCD+0
;DataAquisitionProj.c,120 :: 		}
L_main6:
;DataAquisitionProj.c,123 :: 		if (wateringStartTime > 0 && (millis() - wateringStartTime >= WATERING_DURATION))
	MOVF       _wateringStartTime+3, 0
	SUBLW      0
	BTFSS      STATUS+0, 2
	GOTO       L__main43
	MOVF       _wateringStartTime+2, 0
	SUBLW      0
	BTFSS      STATUS+0, 2
	GOTO       L__main43
	MOVF       _wateringStartTime+1, 0
	SUBLW      0
	BTFSS      STATUS+0, 2
	GOTO       L__main43
	MOVF       _wateringStartTime+0, 0
	SUBLW      0
L__main43:
	BTFSC      STATUS+0, 0
	GOTO       L_main9
	CALL       _millis+0
	MOVF       R0+0, 0
	MOVWF      R4+0
	MOVF       R0+1, 0
	MOVWF      R4+1
	MOVF       R0+2, 0
	MOVWF      R4+2
	MOVF       R0+3, 0
	MOVWF      R4+3
	MOVF       _wateringStartTime+0, 0
	SUBWF      R4+0, 1
	MOVF       _wateringStartTime+1, 0
	BTFSS      STATUS+0, 0
	INCFSZ     _wateringStartTime+1, 0
	SUBWF      R4+1, 1
	MOVF       _wateringStartTime+2, 0
	BTFSS      STATUS+0, 0
	INCFSZ     _wateringStartTime+2, 0
	SUBWF      R4+2, 1
	MOVF       _wateringStartTime+3, 0
	BTFSS      STATUS+0, 0
	INCFSZ     _wateringStartTime+3, 0
	SUBWF      R4+3, 1
	MOVLW      0
	SUBWF      R4+3, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main44
	MOVLW      0
	SUBWF      R4+2, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main44
	MOVLW      3
	SUBWF      R4+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main44
	MOVLW      232
	SUBWF      R4+0, 0
L__main44:
	BTFSS      STATUS+0, 0
	GOTO       L_main9
L__main36:
;DataAquisitionProj.c,125 :: 		stopWatering();
	CALL       _stopWatering+0
;DataAquisitionProj.c,126 :: 		wateringStartTime = 0;
	CLRF       _wateringStartTime+0
	CLRF       _wateringStartTime+1
	CLRF       _wateringStartTime+2
	CLRF       _wateringStartTime+3
;DataAquisitionProj.c,127 :: 		printToLCD("Watering complete");
	MOVLW      ?lstr3_DataAquisitionProj+0
	MOVWF      FARG_printToLCD_message+0
	CALL       _printToLCD+0
;DataAquisitionProj.c,128 :: 		}
L_main9:
;DataAquisitionProj.c,131 :: 		if (millis() - lastLogTime >= LOG_INTERVAL)
	CALL       _millis+0
	MOVF       R0+0, 0
	MOVWF      R4+0
	MOVF       R0+1, 0
	MOVWF      R4+1
	MOVF       R0+2, 0
	MOVWF      R4+2
	MOVF       R0+3, 0
	MOVWF      R4+3
	MOVF       _lastLogTime+0, 0
	SUBWF      R4+0, 1
	MOVF       _lastLogTime+1, 0
	BTFSS      STATUS+0, 0
	INCFSZ     _lastLogTime+1, 0
	SUBWF      R4+1, 1
	MOVF       _lastLogTime+2, 0
	BTFSS      STATUS+0, 0
	INCFSZ     _lastLogTime+2, 0
	SUBWF      R4+2, 1
	MOVF       _lastLogTime+3, 0
	BTFSS      STATUS+0, 0
	INCFSZ     _lastLogTime+3, 0
	SUBWF      R4+3, 1
	MOVLW      0
	SUBWF      R4+3, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main45
	MOVLW      0
	SUBWF      R4+2, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main45
	MOVLW      39
	SUBWF      R4+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main45
	MOVLW      16
	SUBWF      R4+0, 0
L__main45:
	BTFSS      STATUS+0, 0
	GOTO       L_main10
;DataAquisitionProj.c,133 :: 		printToLCD("Logging data...");
	MOVLW      ?lstr4_DataAquisitionProj+0
	MOVWF      FARG_printToLCD_message+0
	CALL       _printToLCD+0
;DataAquisitionProj.c,134 :: 		logData(humidity, temperature);
	MOVF       _humidity+0, 0
	MOVWF      FARG_logData_temp+0
	MOVF       _temperature+0, 0
	MOVWF      FARG_logData_hum+0
	CALL       _logData+0
;DataAquisitionProj.c,135 :: 		lastLogTime = millis();
	CALL       _millis+0
	MOVF       R0+0, 0
	MOVWF      _lastLogTime+0
	MOVF       R0+1, 0
	MOVWF      _lastLogTime+1
	MOVF       R0+2, 0
	MOVWF      _lastLogTime+2
	MOVF       R0+3, 0
	MOVWF      _lastLogTime+3
;DataAquisitionProj.c,136 :: 		}
L_main10:
;DataAquisitionProj.c,139 :: 		if (UART1_Data_Ready())
	CALL       _UART1_Data_Ready+0
	MOVF       R0+0, 0
	BTFSC      STATUS+0, 2
	GOTO       L_main11
;DataAquisitionProj.c,142 :: 		char delimiter[3] = "\r\n";               // Delimiter is a carriage return and newline
	MOVLW      13
	MOVWF      main_delimiter_L2+0
	MOVLW      10
	MOVWF      main_delimiter_L2+1
	CLRF       main_delimiter_L2+2
;DataAquisitionProj.c,143 :: 		UART1_Read_Text(command, delimiter, 255); // Read until delimiter is found
	MOVLW      main_command_L2+0
	MOVWF      FARG_UART1_Read_Text_Output+0
	MOVLW      main_delimiter_L2+0
	MOVWF      FARG_UART1_Read_Text_Delimiter+0
	MOVLW      255
	MOVWF      FARG_UART1_Read_Text_Attempts+0
	CALL       _UART1_Read_Text+0
;DataAquisitionProj.c,145 :: 		if (strcmp(command, "water") == 0)
	MOVLW      main_command_L2+0
	MOVWF      FARG_strcmp_s1+0
	MOVLW      ?lstr5_DataAquisitionProj+0
	MOVWF      FARG_strcmp_s2+0
	CALL       _strcmp+0
	MOVLW      0
	XORWF      R0+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main46
	MOVLW      0
	XORWF      R0+0, 0
L__main46:
	BTFSS      STATUS+0, 2
	GOTO       L_main12
;DataAquisitionProj.c,147 :: 		startWatering();
	CALL       _startWatering+0
;DataAquisitionProj.c,148 :: 		}
	GOTO       L_main13
L_main12:
;DataAquisitionProj.c,149 :: 		else if (strcmp(command, "stop") == 0)
	MOVLW      main_command_L2+0
	MOVWF      FARG_strcmp_s1+0
	MOVLW      ?lstr6_DataAquisitionProj+0
	MOVWF      FARG_strcmp_s2+0
	CALL       _strcmp+0
	MOVLW      0
	XORWF      R0+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main47
	MOVLW      0
	XORWF      R0+0, 0
L__main47:
	BTFSS      STATUS+0, 2
	GOTO       L_main14
;DataAquisitionProj.c,151 :: 		stopWatering();
	CALL       _stopWatering+0
;DataAquisitionProj.c,152 :: 		}
	GOTO       L_main15
L_main14:
;DataAquisitionProj.c,153 :: 		else if (strcmp(command, "auto") == 0)
	MOVLW      main_command_L2+0
	MOVWF      FARG_strcmp_s1+0
	MOVLW      ?lstr7_DataAquisitionProj+0
	MOVWF      FARG_strcmp_s2+0
	CALL       _strcmp+0
	MOVLW      0
	XORWF      R0+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main48
	MOVLW      0
	XORWF      R0+0, 0
L__main48:
	BTFSS      STATUS+0, 2
	GOTO       L_main16
;DataAquisitionProj.c,155 :: 		autoMode = 1;
	BSF        _autoMode+0, BitPos(_autoMode+0)
;DataAquisitionProj.c,156 :: 		}
	GOTO       L_main17
L_main16:
;DataAquisitionProj.c,157 :: 		else if (strcmp(command, "manual") == 0)
	MOVLW      main_command_L2+0
	MOVWF      FARG_strcmp_s1+0
	MOVLW      ?lstr8_DataAquisitionProj+0
	MOVWF      FARG_strcmp_s2+0
	CALL       _strcmp+0
	MOVLW      0
	XORWF      R0+1, 0
	BTFSS      STATUS+0, 2
	GOTO       L__main49
	MOVLW      0
	XORWF      R0+0, 0
L__main49:
	BTFSS      STATUS+0, 2
	GOTO       L_main18
;DataAquisitionProj.c,159 :: 		autoMode = 0;
	BCF        _autoMode+0, BitPos(_autoMode+0)
;DataAquisitionProj.c,160 :: 		}
L_main18:
L_main17:
L_main15:
L_main13:
;DataAquisitionProj.c,161 :: 		}
L_main11:
;DataAquisitionProj.c,163 :: 		delay_ms(100);
	MOVLW      2
	MOVWF      R11+0
	MOVLW      4
	MOVWF      R12+0
	MOVLW      186
	MOVWF      R13+0
L_main19:
	DECFSZ     R13+0, 1
	GOTO       L_main19
	DECFSZ     R12+0, 1
	GOTO       L_main19
	DECFSZ     R11+0, 1
	GOTO       L_main19
	NOP
;DataAquisitionProj.c,164 :: 		}
	GOTO       L_main1
;DataAquisitionProj.c,165 :: 		}
L_end_main:
	GOTO       $+0
; end of _main

_initUART:

;DataAquisitionProj.c,167 :: 		void initUART()
;DataAquisitionProj.c,170 :: 		UART1_Init(BAUD_RATE);
	MOVLW      51
	MOVWF      SPBRG+0
	BSF        TXSTA+0, 2
	CALL       _UART1_Init+0
;DataAquisitionProj.c,171 :: 		}
L_end_initUART:
	RETURN
; end of _initUART

_initPWM:

;DataAquisitionProj.c,173 :: 		void initPWM()
;DataAquisitionProj.c,176 :: 		PWM1_Init(5000); // Set PWM frequency to 5 kHz
	BSF        T2CON+0, 0
	BCF        T2CON+0, 1
	MOVLW      99
	MOVWF      PR2+0
	CALL       _PWM1_Init+0
;DataAquisitionProj.c,177 :: 		PWM1_Start();
	CALL       _PWM1_Start+0
;DataAquisitionProj.c,178 :: 		PWM1_Set_Duty(0); // Set initial duty cycle to 0%
	CLRF       FARG_PWM1_Set_Duty_new_duty+0
	CALL       _PWM1_Set_Duty+0
;DataAquisitionProj.c,179 :: 		}
L_end_initPWM:
	RETURN
; end of _initPWM

_startWatering:

;DataAquisitionProj.c,181 :: 		void startWatering()
;DataAquisitionProj.c,184 :: 		PWM1_Set_Duty(75); // Set duty cycle to 75%
	MOVLW      75
	MOVWF      FARG_PWM1_Set_Duty_new_duty+0
	CALL       _PWM1_Set_Duty+0
;DataAquisitionProj.c,185 :: 		wateringStartTime = millis();
	CALL       _millis+0
	MOVF       R0+0, 0
	MOVWF      _wateringStartTime+0
	MOVF       R0+1, 0
	MOVWF      _wateringStartTime+1
	MOVF       R0+2, 0
	MOVWF      _wateringStartTime+2
	MOVF       R0+3, 0
	MOVWF      _wateringStartTime+3
;DataAquisitionProj.c,186 :: 		}
L_end_startWatering:
	RETURN
; end of _startWatering

_stopWatering:

;DataAquisitionProj.c,188 :: 		void stopWatering()
;DataAquisitionProj.c,191 :: 		PWM1_Set_Duty(0);
	CLRF       FARG_PWM1_Set_Duty_new_duty+0
	CALL       _PWM1_Set_Duty+0
;DataAquisitionProj.c,192 :: 		}
L_end_stopWatering:
	RETURN
; end of _stopWatering

_readSensors:

;DataAquisitionProj.c,194 :: 		void readSensors()
;DataAquisitionProj.c,198 :: 		soilMoistureValue = ADC_Read(SOIL_SENSOR_PIN);
	MOVLW      0
	BTFSC      RA0_bit+0, BitPos(RA0_bit+0)
	MOVLW      1
	MOVWF      FARG_ADC_Read_channel+0
	CALL       _ADC_Read+0
	MOVF       R0+0, 0
	MOVWF      _soilMoistureValue+0
	MOVF       R0+1, 0
	MOVWF      _soilMoistureValue+1
;DataAquisitionProj.c,205 :: 		lightLevel = ADC_Read(LDR_PIN);
	MOVLW      0
	BTFSC      RA1_bit+0, BitPos(RA1_bit+0)
	MOVLW      1
	MOVWF      FARG_ADC_Read_channel+0
	CALL       _ADC_Read+0
	MOVF       R0+0, 0
	MOVWF      _lightLevel+0
	MOVF       R0+1, 0
	MOVWF      _lightLevel+1
;DataAquisitionProj.c,208 :: 		}
L_end_readSensors:
	RETURN
; end of _readSensors

_logData:

;DataAquisitionProj.c,210 :: 		void logData(char Temp, char hum)
;DataAquisitionProj.c,215 :: 		sprinti(Data, "Soil:%.2d,Temp:%.1f,Humid:%.1f,Light:%.2d", soilMoistureValue, temp, hum, lightLevel);
	MOVLW      logData_Data_L0+0
	MOVWF      FARG_sprinti_wh+0
	MOVLW      ?lstr_9_DataAquisitionProj+0
	MOVWF      FARG_sprinti_f+0
	MOVLW      hi_addr(?lstr_9_DataAquisitionProj+0)
	MOVWF      FARG_sprinti_f+1
	MOVF       _soilMoistureValue+0, 0
	MOVWF      FARG_sprinti_wh+3
	MOVF       _soilMoistureValue+1, 0
	MOVWF      FARG_sprinti_wh+4
	MOVF       FARG_logData_Temp+0, 0
	MOVWF      FARG_sprinti_wh+5
	MOVF       FARG_logData_hum+0, 0
	MOVWF      FARG_sprinti_wh+6
	MOVF       _lightLevel+0, 0
	MOVWF      FARG_sprinti_wh+7
	MOVF       _lightLevel+1, 0
	MOVWF      FARG_sprinti_wh+8
	CALL       _sprinti+0
;DataAquisitionProj.c,216 :: 		printToLCD(Data);
	MOVLW      logData_Data_L0+0
	MOVWF      FARG_printToLCD_message+0
	CALL       _printToLCD+0
;DataAquisitionProj.c,219 :: 		UART1_Write_Text(Data);
	MOVLW      logData_Data_L0+0
	MOVWF      FARG_UART1_Write_Text_uart_text+0
	CALL       _UART1_Write_Text+0
;DataAquisitionProj.c,220 :: 		}
L_end_logData:
	RETURN
; end of _logData

_printToLCD:

;DataAquisitionProj.c,222 :: 		void printToLCD(char *message)
;DataAquisitionProj.c,225 :: 		Lcd_Cmd(_LCD_CLEAR);
	MOVLW      1
	MOVWF      FARG_Lcd_Cmd_out_char+0
	CALL       _Lcd_Cmd+0
;DataAquisitionProj.c,226 :: 		Lcd_Out(1, 1, message);
	MOVLW      1
	MOVWF      FARG_Lcd_Out_row+0
	MOVLW      1
	MOVWF      FARG_Lcd_Out_column+0
	MOVF       FARG_printToLCD_message+0, 0
	MOVWF      FARG_Lcd_Out_text+0
	CALL       _Lcd_Out+0
;DataAquisitionProj.c,227 :: 		}
L_end_printToLCD:
	RETURN
; end of _printToLCD

_initTimer:

;DataAquisitionProj.c,229 :: 		void initTimer()
;DataAquisitionProj.c,233 :: 		INTCON.GIE = 1;
	BSF        INTCON+0, 7
;DataAquisitionProj.c,235 :: 		INTCON.PEIE = 1;
	BSF        INTCON+0, 6
;DataAquisitionProj.c,237 :: 		PIE1.TMR1IE = 1;
	BSF        PIE1+0, 0
;DataAquisitionProj.c,239 :: 		TMR1L = 0x3C;
	MOVLW      60
	MOVWF      TMR1L+0
;DataAquisitionProj.c,240 :: 		TMR1H = 0xB0;
	MOVLW      176
	MOVWF      TMR1H+0
;DataAquisitionProj.c,241 :: 		T1CON.T1CKPS0 = 0;
	BCF        T1CON+0, 4
;DataAquisitionProj.c,242 :: 		T1CON.T1CKPS1 = 0;
	BCF        T1CON+0, 5
;DataAquisitionProj.c,244 :: 		T1CON.TMR1ON = 1;
	BSF        T1CON+0, 0
;DataAquisitionProj.c,245 :: 		}
L_end_initTimer:
	RETURN
; end of _initTimer

_millis:

;DataAquisitionProj.c,247 :: 		unsigned long millis()
;DataAquisitionProj.c,250 :: 		return millisCount;
	MOVF       _millisCount+0, 0
	MOVWF      R0+0
	MOVF       _millisCount+1, 0
	MOVWF      R0+1
	MOVF       _millisCount+2, 0
	MOVWF      R0+2
	MOVF       _millisCount+3, 0
	MOVWF      R0+3
;DataAquisitionProj.c,251 :: 		}
L_end_millis:
	RETURN
; end of _millis

_dht11_read_byte:

;DataAquisitionProj.c,253 :: 		unsigned short int dht11_read_byte()
;DataAquisitionProj.c,255 :: 		unsigned short int i = 8, dht11_byte = 0;
	MOVLW      8
	MOVWF      dht11_read_byte_i_L0+0
	CLRF       dht11_read_byte_dht11_byte_L0+0
;DataAquisitionProj.c,257 :: 		while (i--)
L_dht11_read_byte20:
	MOVF       dht11_read_byte_i_L0+0, 0
	MOVWF      R0+0
	DECF       dht11_read_byte_i_L0+0, 1
	MOVF       R0+0, 0
	BTFSC      STATUS+0, 2
	GOTO       L_dht11_read_byte21
;DataAquisitionProj.c,259 :: 		while (!DHT11_PIN)
L_dht11_read_byte22:
	BTFSC      RB6_bit+0, BitPos(RB6_bit+0)
	GOTO       L_dht11_read_byte23
;DataAquisitionProj.c,260 :: 		;
	GOTO       L_dht11_read_byte22
L_dht11_read_byte23:
;DataAquisitionProj.c,262 :: 		delay_us(40);
	MOVLW      26
	MOVWF      R13+0
L_dht11_read_byte24:
	DECFSZ     R13+0, 1
	GOTO       L_dht11_read_byte24
	NOP
;DataAquisitionProj.c,264 :: 		if (DHT11_PIN)
	BTFSS      RB6_bit+0, BitPos(RB6_bit+0)
	GOTO       L_dht11_read_byte25
;DataAquisitionProj.c,266 :: 		dht11_byte |= (1 << i); // set bit i
	MOVF       dht11_read_byte_i_L0+0, 0
	MOVWF      R1+0
	MOVLW      1
	MOVWF      R0+0
	MOVF       R1+0, 0
L__dht11_read_byte60:
	BTFSC      STATUS+0, 2
	GOTO       L__dht11_read_byte61
	RLF        R0+0, 1
	BCF        R0+0, 0
	ADDLW      255
	GOTO       L__dht11_read_byte60
L__dht11_read_byte61:
	MOVF       R0+0, 0
	IORWF      dht11_read_byte_dht11_byte_L0+0, 1
;DataAquisitionProj.c,267 :: 		while (DHT11_PIN)
L_dht11_read_byte26:
	BTFSS      RB6_bit+0, BitPos(RB6_bit+0)
	GOTO       L_dht11_read_byte27
;DataAquisitionProj.c,268 :: 		;
	GOTO       L_dht11_read_byte26
L_dht11_read_byte27:
;DataAquisitionProj.c,269 :: 		}
L_dht11_read_byte25:
;DataAquisitionProj.c,270 :: 		}
	GOTO       L_dht11_read_byte20
L_dht11_read_byte21:
;DataAquisitionProj.c,271 :: 		return (dht11_byte);
	MOVF       dht11_read_byte_dht11_byte_L0+0, 0
	MOVWF      R0+0
;DataAquisitionProj.c,272 :: 		}
L_end_dht11_read_byte:
	RETURN
; end of _dht11_read_byte

_dht11_read:

;DataAquisitionProj.c,275 :: 		void dht11_read(unsigned short int *dht11_humi, unsigned short int *dht11_temp)
;DataAquisitionProj.c,278 :: 		DHT11_PIN = 0;     // connection pin output low
	BCF        RB6_bit+0, BitPos(RB6_bit+0)
;DataAquisitionProj.c,279 :: 		DHT11_PIN_DIR = 0; // configure connection pin as output
	BCF        TRISB6_bit+0, BitPos(TRISB6_bit+0)
;DataAquisitionProj.c,280 :: 		delay_ms(25);      // wait 25 ms
	MOVLW      65
	MOVWF      R12+0
	MOVLW      238
	MOVWF      R13+0
L_dht11_read28:
	DECFSZ     R13+0, 1
	GOTO       L_dht11_read28
	DECFSZ     R12+0, 1
	GOTO       L_dht11_read28
	NOP
;DataAquisitionProj.c,281 :: 		DHT11_PIN = 1;     // connection pin output high
	BSF        RB6_bit+0, BitPos(RB6_bit+0)
;DataAquisitionProj.c,282 :: 		delay_us(70);      // wait 30 us
	MOVLW      46
	MOVWF      R13+0
L_dht11_read29:
	DECFSZ     R13+0, 1
	GOTO       L_dht11_read29
	NOP
;DataAquisitionProj.c,283 :: 		DHT11_PIN_DIR = 1; // configure connection pin as input
	BSF        TRISB6_bit+0, BitPos(TRISB6_bit+0)
;DataAquisitionProj.c,286 :: 		while (DHT11_PIN)
L_dht11_read30:
	BTFSS      RB6_bit+0, BitPos(RB6_bit+0)
	GOTO       L_dht11_read31
;DataAquisitionProj.c,287 :: 		;
	GOTO       L_dht11_read30
L_dht11_read31:
;DataAquisitionProj.c,288 :: 		while (!DHT11_PIN)
L_dht11_read32:
	BTFSC      RB6_bit+0, BitPos(RB6_bit+0)
	GOTO       L_dht11_read33
;DataAquisitionProj.c,289 :: 		;
	GOTO       L_dht11_read32
L_dht11_read33:
;DataAquisitionProj.c,290 :: 		while (DHT11_PIN)
L_dht11_read34:
	BTFSS      RB6_bit+0, BitPos(RB6_bit+0)
	GOTO       L_dht11_read35
;DataAquisitionProj.c,291 :: 		;
	GOTO       L_dht11_read34
L_dht11_read35:
;DataAquisitionProj.c,294 :: 		*dht11_humi = dht11_read_byte(); // read humidity byte 1
	CALL       _dht11_read_byte+0
	MOVF       FARG_dht11_read_dht11_humi+0, 0
	MOVWF      FSR
	MOVF       R0+0, 0
	MOVWF      INDF+0
;DataAquisitionProj.c,295 :: 		dht11_read_byte();               // read humidity byte 2 (skipped)
	CALL       _dht11_read_byte+0
;DataAquisitionProj.c,296 :: 		*dht11_temp = dht11_read_byte(); // read temperature byte 1
	CALL       _dht11_read_byte+0
	MOVF       FARG_dht11_read_dht11_temp+0, 0
	MOVWF      FSR
	MOVF       R0+0, 0
	MOVWF      INDF+0
;DataAquisitionProj.c,297 :: 		dht11_read_byte();               // read temperature byte 2 (skipped)
	CALL       _dht11_read_byte+0
;DataAquisitionProj.c,298 :: 		dht11_read_byte();               // read checksum (skipped)
	CALL       _dht11_read_byte+0
;DataAquisitionProj.c,299 :: 		}
L_end_dht11_read:
	RETURN
; end of _dht11_read
