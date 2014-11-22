EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:Ghost
LIBS:CY62138FV30_2M_RAM
LIBS:nmos
LIBS:adafruit_gps
LIBS:open-project
LIBS:AS1325
LIBS:LM3671
LIBS:runnersgps-cache
EELAYER 27 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 4
Title ""
Date "11 nov 2014"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	1850 2450 2050 2450
Wire Wire Line
	2050 2450 2050 2550
Wire Wire Line
	2050 2550 3700 2550
Wire Wire Line
	1850 2250 2800 2250
Wire Wire Line
	1850 2050 3050 2050
Wire Wire Line
	1850 1850 3050 1850
Wire Wire Line
	6650 1450 6000 1450
Wire Wire Line
	6450 1050 6000 1050
Wire Wire Line
	4050 1050 4500 1050
Wire Wire Line
	4200 1450 4500 1450
Wire Wire Line
	4200 1250 4500 1250
Wire Wire Line
	8900 850  8250 850 
Wire Wire Line
	8900 1050 8250 1050
Wire Wire Line
	8450 1650 8900 1650
Wire Wire Line
	8500 1850 8900 1850
Wire Wire Line
	8800 4350 8900 4350
Wire Wire Line
	7350 4150 8900 4150
Connection ~ 2450 1950
Connection ~ 2600 2550
Connection ~ 2550 2650
Wire Wire Line
	2000 2650 2800 2650
Wire Wire Line
	8500 3550 8900 3550
Wire Wire Line
	8500 3350 8900 3350
Wire Wire Line
	8500 3150 8900 3150
Wire Wire Line
	8500 2950 8900 2950
Wire Wire Line
	8500 2750 8900 2750
Wire Wire Line
	8500 2550 8900 2550
Wire Wire Line
	8500 2350 8900 2350
Wire Wire Line
	8500 2150 8900 2150
Wire Wire Line
	1900 750  2450 750 
Wire Wire Line
	2450 750  2450 1950
Wire Wire Line
	1900 1050 2550 1050
Wire Wire Line
	2550 1050 2550 2650
Wire Wire Line
	1400 900  1150 900 
Wire Wire Line
	8900 1450 8650 1450
Connection ~ 8650 1350
Wire Wire Line
	8650 1450 8650 1350
Wire Wire Line
	8500 1350 8900 1350
Wire Wire Line
	8550 4550 8900 4550
Wire Wire Line
	8500 750  8900 750 
Connection ~ 1400 1050
Connection ~ 1400 900 
Wire Wire Line
	1400 600  1400 1200
Connection ~ 1400 750 
Wire Wire Line
	2600 2550 2600 1200
Wire Wire Line
	2600 1200 1900 1200
Wire Wire Line
	2500 2450 2500 900 
Wire Wire Line
	2500 900  1900 900 
Wire Wire Line
	2400 1850 2400 600 
Wire Wire Line
	2400 600  1900 600 
Wire Wire Line
	8500 2050 8900 2050
Wire Wire Line
	8900 2250 8500 2250
Wire Wire Line
	8900 2450 8500 2450
Wire Wire Line
	8900 2650 8500 2650
Wire Wire Line
	8900 2850 8500 2850
Wire Wire Line
	8900 3050 8500 3050
Wire Wire Line
	8900 3250 8500 3250
Wire Wire Line
	8900 3450 8500 3450
Wire Wire Line
	8750 3750 8900 3750
Wire Wire Line
	2100 2450 3250 2450
Connection ~ 2500 2450
Connection ~ 2400 1850
Wire Wire Line
	8800 4050 8900 4050
Wire Wire Line
	8800 4250 8900 4250
Wire Wire Line
	8100 1950 8900 1950
Wire Wire Line
	8050 1750 8900 1750
Wire Wire Line
	8650 1150 8900 1150
Wire Wire Line
	8650 950  8900 950 
Wire Wire Line
	8000 3650 8900 3650
Wire Wire Line
	3800 1150 4500 1150
Wire Wire Line
	3800 1350 4500 1350
Wire Wire Line
	3750 1550 4500 1550
Wire Wire Line
	6000 1150 6100 1150
Wire Wire Line
	6000 1350 6100 1350
Wire Wire Line
	6550 1250 6000 1250
Wire Wire Line
	6100 1550 6000 1550
Wire Wire Line
	6750 1650 6000 1650
Wire Wire Line
	1850 1950 3500 1950
Wire Wire Line
	1850 2150 3400 2150
Wire Wire Line
	2100 2450 2100 2350
Wire Wire Line
	2100 2350 1850 2350
Wire Wire Line
	2000 2650 2000 2550
Wire Wire Line
	2000 2550 1850 2550
$Comp
L MICRO_SD_SOCKET U2
U 1 1 512CDE4E
P 1100 2250
F 0 "U2" H 1200 2100 60  0000 C CNN
F 1 "MICRO_SD_SOCKET" H 1300 2250 60  0000 C CNN
F 2 "" H 1100 2250 60  0001 C CNN
F 3 "" H 1100 2250 60  0001 C CNN
	1    1100 2250
	-1   0    0    1   
$EndComp
NoConn ~ 6000 1750
Text GLabel 6100 1550 2    60   Input ~ 0
TS_PENIRQ
Text GLabel 6650 1450 2    60   Input ~ 0
TS_DOUT
Text GLabel 6100 1350 2    60   Input ~ 0
TS_BUSY
Text GLabel 6550 1250 2    60   Input ~ 0
TS_DIN
Text GLabel 6100 1150 2    60   Input ~ 0
TS_CS
Text GLabel 6450 1050 2    60   Input ~ 0
TS_CLK
NoConn ~ 8900 1550
NoConn ~ 8900 1250
Text GLabel 6750 1650 2    60   Input ~ 0
3V3
NoConn ~ 4500 1650
NoConn ~ 4500 1750
Text GLabel 4050 1050 0    60   Input ~ 0
3V3
Text GLabel 3750 1550 0    60   Input ~ 0
GND
$Comp
L TSC2046 U1
U 1 1 5123D19E
P 5250 1400
F 0 "U1" H 5250 1300 50  0000 C CNN
F 1 "TSC2046" H 5250 1500 50  0000 C CNN
F 2 "MODULE" H 5250 1400 50  0001 C CNN
F 3 "DOCUMENTATION" H 5250 1400 50  0001 C CNN
	1    5250 1400
	1    0    0    -1  
$EndComp
Text GLabel 4200 1450 0    60   Input ~ 0
LCD_Y-
Text GLabel 4200 1250 0    60   Input ~ 0
LCD_Y+
Text GLabel 3800 1350 0    60   Input ~ 0
LCD_X-
Text GLabel 3800 1150 0    60   Input ~ 0
LCD_X+
$Comp
L 40-PIN-LCD U3
U 1 1 5123B19F
P 9650 2700
F 0 "U3" H 9650 2600 50  0000 C CNN
F 1 "40-PIN-LCD" H 9650 2800 50  0000 C CNN
F 2 "MODULE" H 9650 2700 50  0001 C CNN
F 3 "DOCUMENTATION" H 9650 2700 50  0001 C CNN
	1    9650 2700
	1    0    0    -1  
$EndComp
Text GLabel 8000 3650 0    60   Input ~ 0
LCD_RESET
Text GLabel 8450 1650 0    60   Input ~ 0
LCD_CS
Text GLabel 8250 850  0    60   Input ~ 0
LCD_Y-
Text GLabel 8250 1050 0    60   Input ~ 0
LCD_Y+
Text GLabel 8650 1150 0    60   Input ~ 0
LCD_X-
Text GLabel 8650 950  0    60   Input ~ 0
LCD_X+
Text GLabel 8100 1950 0    60   Input ~ 0
LCD_RD
Text GLabel 8500 1850 0    60   Input ~ 0
LCD_WR
Text GLabel 8050 1750 0    60   Input ~ 0
LCD_RS
Text GLabel 2800 2250 2    60   Input ~ 0
SD_VDD
Text GLabel 3250 2450 2    60   Input ~ 0
SD_CMD
Text GLabel 3700 2550 2    60   Input ~ 0
SD_DAT3
Text GLabel 2800 2650 2    60   Input ~ 0
SD_DAT2
Text GLabel 3050 1850 2    60   Input ~ 0
SD_DAT1
Text GLabel 3500 1950 2    60   Input ~ 0
SD_DAT0
Text GLabel 3400 2150 2    60   Input ~ 0
SD_CLK
Text Notes 9550 3800 0    60   ~ 0
IM0 to GND to enable 16-bit mode
Text GLabel 8750 3750 0    60   Input ~ 0
GND
Text GLabel 8500 3550 0    60   Input ~ 0
LCD_D15
Text GLabel 8500 3450 0    60   Input ~ 0
LCD_D14
Text GLabel 8500 3350 0    60   Input ~ 0
LCD_D13
Text GLabel 8500 3250 0    60   Input ~ 0
LCD_D12
Text GLabel 8500 3150 0    60   Input ~ 0
LCD_D11
Text GLabel 8500 3050 0    60   Input ~ 0
LCD_D10
Text GLabel 8500 2950 0    60   Input ~ 0
LCD_D9
Text GLabel 8500 2850 0    60   Input ~ 0
LCD_D8
Text GLabel 8500 2750 0    60   Input ~ 0
LCD_D7
Text GLabel 8500 2650 0    60   Input ~ 0
LCD_D6
Text GLabel 8500 2550 0    60   Input ~ 0
LCD_D5
Text GLabel 8500 2450 0    60   Input ~ 0
LCD_D4
Text GLabel 8500 2350 0    60   Input ~ 0
LCD_D3
Text GLabel 8500 2250 0    60   Input ~ 0
LCD_D2
Text GLabel 8500 2150 0    60   Input ~ 0
LCD_D1
Text GLabel 8500 2050 0    60   Input ~ 0
LCD_D0
Text GLabel 3050 2050 2    60   Input ~ 0
GND
Text GLabel 1150 900  0    60   Input ~ 0
3V3
$Comp
L R R5
U 1 1 51199BA0
P 1650 1200
F 0 "R5" V 1730 1200 50  0000 C CNN
F 1 "10k" V 1650 1200 50  0000 C CNN
F 2 "" H 1650 1200 60  0001 C CNN
F 3 "" H 1650 1200 60  0001 C CNN
	1    1650 1200
	0    -1   -1   0   
$EndComp
$Comp
L R R4
U 1 1 51199B9F
P 1650 1050
F 0 "R4" V 1730 1050 50  0000 C CNN
F 1 "10k" V 1650 1050 50  0000 C CNN
F 2 "" H 1650 1050 60  0001 C CNN
F 3 "" H 1650 1050 60  0001 C CNN
	1    1650 1050
	0    -1   -1   0   
$EndComp
$Comp
L R R2
U 1 1 51199B9E
P 1650 750
F 0 "R2" V 1730 750 50  0000 C CNN
F 1 "10k" V 1650 750 50  0000 C CNN
F 2 "" H 1650 750 60  0001 C CNN
F 3 "" H 1650 750 60  0001 C CNN
	1    1650 750 
	0    -1   -1   0   
$EndComp
$Comp
L R R3
U 1 1 51199B9D
P 1650 900
F 0 "R3" V 1730 900 50  0000 C CNN
F 1 "10k" V 1650 900 50  0000 C CNN
F 2 "" H 1650 900 60  0001 C CNN
F 3 "" H 1650 900 60  0001 C CNN
	1    1650 900 
	0    -1   -1   0   
$EndComp
$Comp
L R R1
U 1 1 51199B9C
P 1650 600
F 0 "R1" V 1730 600 50  0000 C CNN
F 1 "10k" V 1650 600 50  0000 C CNN
F 2 "" H 1650 600 60  0001 C CNN
F 3 "" H 1650 600 60  0001 C CNN
	1    1650 600 
	0    -1   -1   0   
$EndComp
NoConn ~ 8900 4650
NoConn ~ 8900 3850
Text GLabel 7350 5000 3    60   Input ~ 0
GND
Text GLabel 8550 4550 0    60   Input ~ 0
GND
Text GLabel 8500 1350 0    60   Input ~ 0
3V3
Text GLabel 8500 750  0    60   Input ~ 0
GND
Connection ~ 8800 4150
Connection ~ 8800 4250
Connection ~ 8800 4050
$Comp
L NMOS U7
U 1 1 52433BF6
P 7350 4550
F 0 "U7" H 7600 4650 60  0000 C CNN
F 1 "NMOS" H 7600 4450 60  0000 C CNN
F 2 "~" H 7350 4550 60  0000 C CNN
F 3 "~" H 7350 4550 60  0000 C CNN
	1    7350 4550
	1    0    0    -1  
$EndComp
Text GLabel 6900 4550 0    60   Input ~ 0
LCD_BL
Text GLabel 8200 4450 0    60   Input ~ 0
3V3
Wire Wire Line
	8900 4450 8200 4450
$Comp
L ADAFRUIT_GPS U8
U 1 1 5247B8C0
P 3000 4250
F 0 "U8" H 3000 4150 50  0000 C CNN
F 1 "ADAFRUIT_GPS" H 3000 4350 50  0000 C CNN
F 2 "adafruit_gps" H 3000 4250 50  0001 C CNN
F 3 "DOCUMENTATION" H 3000 4250 50  0001 C CNN
	1    3000 4250
	1    0    0    -1  
$EndComp
NoConn ~ 3750 3800
NoConn ~ 3750 4000
NoConn ~ 3750 4100
NoConn ~ 3750 4200
NoConn ~ 3750 4300
NoConn ~ 2250 4400
NoConn ~ 2250 4300
Text GLabel 2250 3800 0    60   Input ~ 0
3V3
Text GLabel 2250 4500 0    60   Input ~ 0
GPS_GND
Text GLabel 3750 4600 2    60   Input ~ 0
GPS_GND
Text GLabel 3750 3900 2    60   Input ~ 0
GPS_GND
NoConn ~ 3750 4700
Text GLabel 2250 4000 0    60   Input ~ 0
GPS_GND
NoConn ~ 3750 4500
Text GLabel 2250 4100 0    60   Input ~ 0
V_BAT
NoConn ~ 3750 4400
Text Notes 4000 4300 0    60   ~ 0
RTCM requires contacting\nmanufacturer; leave floating\notherwise
Text GLabel 2250 3900 0    60   Input ~ 0
GPS_RESET
NoConn ~ 2250 4200
Text Notes 600  4150 0    60   ~ 0
Don't need 3D-Fix output;\nwill check in software
Text GLabel 2250 4600 0    60   Input ~ 0
GPS_TX
Text GLabel 2250 4700 0    60   Input ~ 0
GPS_RX
Text GLabel 8750 3950 0    60   Input ~ 0
GND
Wire Wire Line
	8750 3950 8900 3950
Wire Wire Line
	8800 4050 8800 4350
Text GLabel 1150 4600 0    60   Input ~ 0
GPS_TX
Text GLabel 1150 4750 0    60   Input ~ 0
GPS_RX
Text GLabel 1250 4600 2    60   Input ~ 0
U3_RX
Text GLabel 1250 4750 2    60   Input ~ 0
U3_TX
Wire Wire Line
	1150 4600 1250 4600
Wire Wire Line
	1250 4750 1150 4750
$Comp
L NMOS U10
U 1 1 5333576A
P 2200 5650
F 0 "U10" H 2450 5750 60  0000 C CNN
F 1 "NMOS" H 2450 5550 60  0000 C CNN
F 2 "~" H 2200 5650 60  0000 C CNN
F 3 "~" H 2200 5650 60  0000 C CNN
	1    2200 5650
	1    0    0    -1  
$EndComp
Text GLabel 2200 5250 0    60   Input ~ 0
GPS_GND
Text GLabel 2200 6100 0    60   Input ~ 0
GND
Text GLabel 1250 5650 0    60   Input ~ 0
GPS_ENABLE
$Comp
L R R7
U 1 1 53335D4E
P 1450 5950
F 0 "R7" V 1530 5950 40  0000 C CNN
F 1 "100k" V 1457 5951 40  0000 C CNN
F 2 "~" V 1380 5950 30  0000 C CNN
F 3 "~" H 1450 5950 30  0000 C CNN
	1    1450 5950
	1    0    0    -1  
$EndComp
Text GLabel 1450 6200 3    60   Input ~ 0
GND
Wire Wire Line
	1250 5650 1750 5650
Wire Wire Line
	1450 5700 1450 5650
Connection ~ 1450 5650
$Comp
L C C8
U 1 1 53AD7E8C
P 4250 800
F 0 "C8" H 4250 900 40  0000 L CNN
F 1 "0.1u" H 4256 715 40  0000 L CNN
F 2 "~" H 4288 650 30  0000 C CNN
F 3 "~" H 4250 800 60  0000 C CNN
	1    4250 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 1000 4250 1050
Connection ~ 4250 1050
Text GLabel 4000 600  0    60   Input ~ 0
GND
Wire Wire Line
	4000 600  4250 600 
$Comp
L C C9
U 1 1 53AD811F
P 2200 3000
F 0 "C9" H 2200 3100 40  0000 L CNN
F 1 "0.1u" H 2206 2915 40  0000 L CNN
F 2 "~" H 2238 2850 30  0000 C CNN
F 3 "~" H 2200 3000 60  0000 C CNN
	1    2200 3000
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 2800 2200 2250
Connection ~ 2200 2250
Text GLabel 2400 3200 2    60   Input ~ 0
GND
Wire Wire Line
	2400 3200 2200 3200
$EndSCHEMATC
