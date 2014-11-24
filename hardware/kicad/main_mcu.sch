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
Sheet 3 4
Title "LPC1788FBD144 Devboard"
Date "11 nov 2014"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text GLabel 4000 7100 0    60   Input ~ 0
P2-10
Text GLabel 5150 950  1    60   Input ~ 0
3V3
Text GLabel 3200 7250 0    60   Input ~ 0
3V3
$Comp
L R R11
U 1 1 512E7554
P 3600 7250
F 0 "R11" V 3680 7250 50  0000 C CNN
F 1 "10k" V 3600 7250 50  0000 C CNN
F 2 "" H 3600 7250 60  0001 C CNN
F 3 "" H 3600 7250 60  0001 C CNN
	1    3600 7250
	0    -1   -1   0   
$EndComp
$Comp
L R R10
U 1 1 00000000
P 5150 1350
F 0 "R10" V 5230 1350 50  0000 C CNN
F 1 "10k" V 5150 1350 50  0000 C CNN
F 2 "" H 5150 1350 60  0001 C CNN
F 3 "" H 5150 1350 60  0001 C CNN
	1    5150 1350
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5123B1A1
P 5600 1100
F 0 "C3" H 5650 1200 50  0000 L CNN
F 1 "100n" H 5650 1000 50  0000 L CNN
F 2 "" H 5600 1100 60  0001 C CNN
F 3 "" H 5600 1100 60  0001 C CNN
	1    5600 1100
	1    0    0    -1  
$EndComp
Text GLabel 8600 5400 2    60   Input ~ 0
GND
$Comp
L CONN_3 K1
U 1 1 512E7564
P 10750 2950
F 0 "K1" V 10700 2950 50  0000 C CNN
F 1 "ISP_HEADER" V 10800 2950 40  0000 C CNN
F 2 "" H 10750 2950 60  0001 C CNN
F 3 "" H 10750 2950 60  0001 C CNN
	1    10750 2950
	1    0    0    -1  
$EndComp
Text Notes 9900 650  0    60   ~ 0
Host/device
Text Notes 10700 600  0    60   ~ 0
Power
Text GLabel 5600 800  1    60   Input ~ 0
GND
Text GLabel 4500 2100 1    60   Input ~ 0
P1-31
Text GLabel 4550 1300 1    60   Input ~ 0
P1-30
Text GLabel 2600 5400 0    60   Input ~ 0
P1-29
Text GLabel 2200 5300 0    60   Input ~ 0
P1-28
Text GLabel 2200 5100 0    60   Input ~ 0
P1-27
Text GLabel 2550 4700 0    60   Input ~ 0
P1-26
Text GLabel 2150 4600 0    60   Input ~ 0
P1-25
Text GLabel 2550 4400 0    60   Input ~ 0
P1-24
Text GLabel 2150 4300 0    60   Input ~ 0
P1-23
Text GLabel 2150 4100 0    60   Input ~ 0
P1-22
Text GLabel 2550 4000 0    60   Input ~ 0
P1-21
Text GLabel 2150 3900 0    60   Input ~ 0
P1-20
Text GLabel 2550 3700 0    60   Input ~ 0
P1-19
Text GLabel 2150 3600 0    60   Input ~ 0
P1-18
Text GLabel 8300 4800 2    60   Input ~ 0
P1-17
Text GLabel 8650 4600 2    60   Input ~ 0
P1-16
Text GLabel 1400 1600 2    60   Input ~ 0
P1-21
Text GLabel 1400 1500 2    60   Input ~ 0
P1-20
Text GLabel 1400 1400 2    60   Input ~ 0
P1-19
Text GLabel 1400 1300 2    60   Input ~ 0
P1-18
Text GLabel 1400 1200 2    60   Input ~ 0
P1-17
Text GLabel 1400 1100 2    60   Input ~ 0
P1-16
Text GLabel 1400 1700 2    60   Input ~ 0
P1-22
Text GLabel 1400 1800 2    60   Input ~ 0
P1-23
Text GLabel 1400 1900 2    60   Input ~ 0
P1-24
Text GLabel 1400 2000 2    60   Input ~ 0
P1-25
Text GLabel 1400 2100 2    60   Input ~ 0
P1-26
Text GLabel 1400 2200 2    60   Input ~ 0
P1-27
Text GLabel 1400 2300 2    60   Input ~ 0
P1-28
Text GLabel 1400 2400 2    60   Input ~ 0
P1-29
Text GLabel 1400 2500 2    60   Input ~ 0
P1-30
Text GLabel 1400 2600 2    60   Input ~ 0
P1-31
Text GLabel 1600 7700 2    60   Input ~ 0
GND
Text GLabel 1500 7300 2    60   Input ~ 0
3V3
$Comp
L C C4
U 1 1 5123B1A0
P 1400 7500
F 0 "C4" H 1450 7600 50  0000 L CNN
F 1 "4.7u" H 1450 7400 50  0000 L CNN
F 2 "" H 1400 7500 60  0001 C CNN
F 3 "" H 1400 7500 60  0001 C CNN
	1    1400 7500
	1    0    0    -1  
$EndComp
Text GLabel 1250 650  0    60   Input ~ 0
LCD_RS
Text GLabel 1250 750  0    60   Input ~ 0
LCD_WR
Text GLabel 1250 850  0    60   Input ~ 0
LCD_RD
Text GLabel 1250 550  0    60   Input ~ 0
LCD_CS
Text GLabel 1350 7300 0    60   Input ~ 0
SD_VDD
Text GLabel 1350 7150 0    60   Input ~ 0
SD_CMD
Text GLabel 1250 1100 0    60   Input ~ 0
LCD_D0
Text GLabel 1250 1200 0    60   Input ~ 0
LCD_D1
Text GLabel 1250 1300 0    60   Input ~ 0
LCD_D2
Text GLabel 1250 1400 0    60   Input ~ 0
LCD_D3
Text GLabel 1250 1500 0    60   Input ~ 0
LCD_D4
Text GLabel 1250 1600 0    60   Input ~ 0
LCD_D5
Text GLabel 1250 1700 0    60   Input ~ 0
LCD_D6
Text GLabel 1250 1800 0    60   Input ~ 0
LCD_D7
Text GLabel 1250 1900 0    60   Input ~ 0
LCD_D8
Text GLabel 1250 2000 0    60   Input ~ 0
LCD_D9
Text GLabel 1250 2100 0    60   Input ~ 0
LCD_D10
Text GLabel 1250 2200 0    60   Input ~ 0
LCD_D11
Text GLabel 1250 2300 0    60   Input ~ 0
LCD_D12
Text GLabel 1250 2400 0    60   Input ~ 0
LCD_D13
Text GLabel 1250 2500 0    60   Input ~ 0
LCD_D14
Text GLabel 1250 2600 0    60   Input ~ 0
LCD_D15
Text GLabel 1350 6850 0    60   Input ~ 0
SD_DAT3
Text GLabel 1350 6700 0    60   Input ~ 0
SD_DAT2
Text GLabel 1350 6550 0    60   Input ~ 0
SD_DAT1
Text GLabel 1350 6400 0    60   Input ~ 0
SD_DAT0
Text GLabel 1350 7000 0    60   Input ~ 0
SD_CLK
Text GLabel 10200 3200 3    60   Input ~ 0
GND
Text Notes 9250 3100 0    60   ~ 0
U0_TXD
Text Notes 9250 2850 0    60   ~ 0
U0_RXD
Text GLabel 3050 7550 0    60   Input ~ 0
GND
$Comp
L SW_PUSH SW2
U 1 1 510F1D2B
P 3600 7550
F 0 "SW2" H 3750 7660 50  0000 C CNN
F 1 "SW_PUSH" H 3600 7470 50  0000 C CNN
F 2 "" H 3600 7550 60  0001 C CNN
F 3 "" H 3600 7550 60  0001 C CNN
F 4 "ISP switch" H 3600 7550 60  0001 C CNN "Description"
	1    3600 7550
	1    0    0    -1  
$EndComp
Text GLabel 5800 1650 1    60   Input ~ 0
GND_A
Text GLabel 5900 2000 1    60   Input ~ 0
3V3_A
Text GLabel 8600 5200 2    60   Input ~ 0
GND
Text GLabel 2050 4900 0    60   Input ~ 0
GND
Text GLabel 5100 2000 1    60   Input ~ 0
GND
Text GLabel 5500 2000 1    60   Input ~ 0
3V3
Text GLabel 10800 1150 3    60   Input ~ 0
USB_D-
Text GLabel 10700 1150 3    60   Input ~ 0
USB_D+
Text GLabel 10500 1150 3    60   Input ~ 0
GND
Text GLabel 10900 1150 3    60   Input ~ 0
V_USB
$Comp
L LPC1788FBD144 U5
U 1 1 510F16BC
P 5450 4450
F 0 "U5" H 5450 4350 50  0000 C CNN
F 1 "LPC1788FBD144" H 5450 4550 50  0000 C CNN
F 2 "MODULE" H 5450 4450 50  0001 C CNN
F 3 "DOCUMENTATION" H 5450 4450 50  0001 C CNN
	1    5450 4450
	1    0    0    -1  
$EndComp
Text GLabel 1800 3400 0    60   Input ~ 0
GND
Text GLabel 1800 5500 0    60   Input ~ 0
GND
Text GLabel 4300 7200 3    60   Input ~ 0
GND
Text GLabel 6700 7200 3    60   Input ~ 0
GND
Text GLabel 8600 3200 2    60   Input ~ 0
GND
Text GLabel 2150 3100 0    60   Input ~ 0
3V3
Text GLabel 4100 7200 3    60   Input ~ 0
3V3
Text GLabel 6600 7450 3    60   Input ~ 0
3V3
Text GLabel 9900 5300 2    60   Input ~ 0
3V3
Text GLabel 8850 3300 2    60   Input ~ 0
3V3
$Comp
L SW_PUSH SW1
U 1 1 510F16BB
P 4900 1350
F 0 "SW1" H 5050 1460 50  0000 C CNN
F 1 "SW_PUSH" H 4900 1270 50  0000 C CNN
F 2 "" H 4900 1350 60  0001 C CNN
F 3 "" H 4900 1350 60  0001 C CNN
F 4 "Reset switch" H 4900 1350 60  0001 C CNN "Description"
	1    4900 1350
	0    -1   -1   0   
$EndComp
Text GLabel 4900 800  1    60   Input ~ 0
GND
Text GLabel 6500 2200 1    60   Input ~ 0
LCD_BL
Connection ~ 4000 7250
Wire Wire Line
	3850 7250 4000 7250
Connection ~ 4900 1750
Wire Wire Line
	5150 1600 5150 1750
Wire Wire Line
	5150 1750 4900 1750
Wire Wire Line
	10400 3050 10200 3050
Wire Wire Line
	10200 3050 10200 3200
Wire Wire Line
	8600 5400 8000 5400
Wire Wire Line
	10400 2850 9600 2850
Wire Wire Line
	9600 2850 9600 2900
Wire Wire Line
	2900 3000 2500 3000
Wire Wire Line
	2500 3000 2500 750 
Wire Wire Line
	2500 750  1250 750 
Wire Wire Line
	2900 2800 2700 2800
Wire Wire Line
	2700 2800 2700 550 
Wire Wire Line
	2700 550  1250 550 
Wire Wire Line
	5600 800  5600 900 
Wire Wire Line
	4300 1450 4300 2200
Wire Wire Line
	2200 5300 2900 5300
Wire Wire Line
	2550 4700 2900 4700
Wire Wire Line
	2550 4400 2900 4400
Wire Wire Line
	2150 4100 2900 4100
Wire Wire Line
	2150 3900 2900 3900
Wire Wire Line
	2150 3600 2900 3600
Wire Wire Line
	8000 4600 8650 4600
Wire Wire Line
	1250 2600 1400 2600
Wire Wire Line
	1250 2400 1400 2400
Wire Wire Line
	1250 2200 1400 2200
Wire Wire Line
	1250 2000 1400 2000
Wire Wire Line
	1250 1800 1400 1800
Wire Wire Line
	1250 1600 1400 1600
Wire Wire Line
	1400 1400 1250 1400
Wire Wire Line
	1400 1200 1250 1200
Wire Wire Line
	1350 7000 1600 7000
Wire Wire Line
	1600 7000 1600 6900
Wire Wire Line
	1600 6900 4900 6900
Wire Wire Line
	4900 6900 4900 6700
Wire Wire Line
	1350 6400 2100 6400
Wire Wire Line
	2100 6400 2100 6800
Wire Wire Line
	2100 6800 4400 6800
Wire Wire Line
	4400 6800 4400 6700
Wire Wire Line
	1350 6850 1850 6850
Wire Wire Line
	1850 6850 1850 6100
Wire Wire Line
	1850 6100 2900 6100
Wire Wire Line
	1400 7700 1600 7700
Wire Wire Line
	9600 2900 8000 2900
Wire Wire Line
	3900 7550 4000 7550
Wire Wire Line
	4000 7550 4000 6700
Wire Wire Line
	5900 2000 5900 2200
Wire Wire Line
	2050 4900 2900 4900
Wire Wire Line
	8000 5000 9550 5000
Wire Wire Line
	5500 2000 5500 2200
Wire Wire Line
	4900 1050 4900 800 
Wire Wire Line
	8000 3300 8850 3300
Wire Wire Line
	8000 3200 8600 3200
Wire Wire Line
	4100 7200 4100 6700
Wire Wire Line
	4300 6700 4300 7200
Wire Wire Line
	1800 3400 2900 3400
Wire Wire Line
	1800 5500 2900 5500
Wire Wire Line
	6700 7200 6700 6700
Wire Wire Line
	1250 5200 2900 5200
Wire Wire Line
	6600 7450 6600 6700
Wire Wire Line
	8000 5700 9100 5700
Wire Wire Line
	4900 2200 4900 1650
Wire Wire Line
	1250 5000 2900 5000
Wire Wire Line
	5100 2000 5100 2200
Wire Wire Line
	8600 5200 8000 5200
Wire Wire Line
	5800 1650 5800 2200
Wire Wire Line
	3050 7550 3300 7550
Wire Wire Line
	8000 3000 9600 3000
Wire Wire Line
	1500 7300 1350 7300
Connection ~ 1400 7300
Wire Wire Line
	3700 6700 1350 6700
Wire Wire Line
	3900 6700 3900 6850
Wire Wire Line
	3900 6850 2000 6850
Wire Wire Line
	2000 6850 2000 6550
Wire Wire Line
	2000 6550 1350 6550
Wire Wire Line
	1350 7150 2950 7150
Wire Wire Line
	2950 7150 2950 6950
Wire Wire Line
	2950 6950 4700 6950
Wire Wire Line
	4700 6950 4700 6700
Wire Wire Line
	1250 1100 1400 1100
Wire Wire Line
	1250 1300 1400 1300
Wire Wire Line
	1250 1500 1400 1500
Wire Wire Line
	1250 1700 1400 1700
Wire Wire Line
	1250 1900 1400 1900
Wire Wire Line
	1250 2100 1400 2100
Wire Wire Line
	1400 2300 1250 2300
Wire Wire Line
	1250 2500 1400 2500
Wire Wire Line
	8300 4800 8000 4800
Wire Wire Line
	2550 3700 2900 3700
Wire Wire Line
	2550 4000 2900 4000
Wire Wire Line
	2150 4300 2900 4300
Wire Wire Line
	2150 4600 2900 4600
Wire Wire Line
	2200 5100 2900 5100
Wire Wire Line
	2600 5400 2900 5400
Wire Wire Line
	4500 2100 4500 2200
Wire Wire Line
	5600 1300 5600 2200
Wire Wire Line
	1250 650  2600 650 
Wire Wire Line
	2600 650  2600 2900
Wire Wire Line
	2600 2900 2900 2900
Wire Wire Line
	2900 3500 2400 3500
Wire Wire Line
	2400 3500 2400 850 
Wire Wire Line
	2400 850  1250 850 
Wire Wire Line
	10400 2950 9600 2950
Wire Wire Line
	9600 2950 9600 3000
Wire Wire Line
	5150 950  5150 1100
Connection ~ 4900 950 
Wire Wire Line
	3350 7250 3200 7250
$Comp
L MICRO-B_USB U9
U 1 1 5248540C
P 10700 800
F 0 "U9" H 10700 500 50  0000 C CNN
F 1 "MICRO-B_USB" H 10700 1100 50  0000 C CNN
F 2 "" H 10700 800 60  0000 C CNN
F 3 "" H 10700 800 60  0000 C CNN
	1    10700 800 
	0    1    1    0   
$EndComp
Text GLabel 8900 4900 2    60   Input ~ 0
U3_RX
Text GLabel 8250 5300 2    60   Input ~ 0
U3_TX
Wire Wire Line
	8000 4900 8900 4900
Wire Wire Line
	8000 5300 8250 5300
Text GLabel 9300 2600 2    60   Input ~ 0
GPS_ENABLE
Wire Wire Line
	9300 2600 8700 2600
Wire Wire Line
	8700 2600 8700 2800
Wire Wire Line
	8700 2800 8000 2800
Wire Wire Line
	4300 1450 4550 1450
Wire Wire Line
	4550 1450 4550 1300
Wire Wire Line
	2150 3100 2900 3100
Wire Wire Line
	10500 1000 10500 1150
Wire Wire Line
	10700 1000 10700 1150
Wire Wire Line
	10800 1000 10800 1150
Wire Wire Line
	10900 1000 10900 1150
$Comp
L C C13
U 1 1 53AD83B3
P 8550 3650
F 0 "C13" H 8550 3750 40  0000 L CNN
F 1 "0.1u" H 8556 3565 40  0000 L CNN
F 2 "~" H 8588 3500 30  0000 C CNN
F 3 "~" H 8550 3650 60  0000 C CNN
	1    8550 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 3450 8550 3300
Connection ~ 8550 3300
Text GLabel 8750 3850 2    60   Input ~ 0
GND
Wire Wire Line
	8750 3850 8550 3850
$Comp
L C C10
U 1 1 53AD8535
P 5400 1500
F 0 "C10" H 5400 1600 40  0000 L CNN
F 1 "0.1u" H 5406 1415 40  0000 L CNN
F 2 "~" H 5438 1350 30  0000 C CNN
F 3 "~" H 5400 1500 60  0000 C CNN
	1    5400 1500
	1    0    0    -1  
$EndComp
$Comp
L C C11
U 1 1 53AD853B
P 6250 1700
F 0 "C11" H 6250 1800 40  0000 L CNN
F 1 "0.1u" H 6256 1615 40  0000 L CNN
F 2 "~" H 6288 1550 30  0000 C CNN
F 3 "~" H 6250 1700 60  0000 C CNN
	1    6250 1700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5900 2100 6450 2100
Connection ~ 5900 2100
Wire Wire Line
	6450 2100 6450 1700
Wire Wire Line
	6050 1700 5800 1700
Wire Wire Line
	5800 1700 5800 1800
Connection ~ 5800 1800
Wire Wire Line
	5400 1700 5400 2100
Wire Wire Line
	5400 2100 5500 2100
Connection ~ 5500 2100
Wire Wire Line
	5400 1300 5400 850 
Wire Wire Line
	5400 850  5600 850 
Connection ~ 5600 850 
$Comp
L C C12
U 1 1 53AD8A6A
P 2250 2850
F 0 "C12" H 2250 2950 40  0000 L CNN
F 1 "0.1u" H 2256 2765 40  0000 L CNN
F 2 "~" H 2288 2700 30  0000 C CNN
F 3 "~" H 2250 2850 60  0000 C CNN
	1    2250 2850
	-1   0    0    1   
$EndComp
Text GLabel 2250 2550 1    60   Input ~ 0
GND
Wire Wire Line
	2250 2550 2250 2650
Wire Wire Line
	2250 3050 2250 3100
Connection ~ 2250 3100
$Comp
L C C14
U 1 1 53AD8DB7
P 1250 5400
F 0 "C14" H 1250 5500 40  0000 L CNN
F 1 "0.1u" H 1256 5315 40  0000 L CNN
F 2 "~" H 1288 5250 30  0000 C CNN
F 3 "~" H 1250 5400 60  0000 C CNN
	1    1250 5400
	-1   0    0    1   
$EndComp
Text GLabel 1150 5600 0    60   Input ~ 0
GND
Wire Wire Line
	1250 5000 1250 5200
Wire Wire Line
	1150 5600 1250 5600
$Comp
L C C16
U 1 1 53AD90AD
P 6400 7100
F 0 "C16" H 6400 7200 40  0000 L CNN
F 1 "0.1u" H 6406 7015 40  0000 L CNN
F 2 "~" H 6438 6950 30  0000 C CNN
F 3 "~" H 6400 7100 60  0000 C CNN
	1    6400 7100
	-1   0    0    1   
$EndComp
Wire Wire Line
	6400 7300 6600 7300
Connection ~ 6600 7300
Wire Wire Line
	6400 6900 6700 6900
Connection ~ 6700 6900
$Comp
L C C15
U 1 1 53AD922F
P 9550 5650
F 0 "C15" H 9550 5750 40  0000 L CNN
F 1 "0.1u" H 9556 5565 40  0000 L CNN
F 2 "~" H 9588 5500 30  0000 C CNN
F 3 "~" H 9550 5650 60  0000 C CNN
	1    9550 5650
	-1   0    0    1   
$EndComp
Text GLabel 9750 5850 2    60   Input ~ 0
GND
Wire Wire Line
	9750 5850 9550 5850
Text GLabel 1050 5050 0    60   Input ~ 0
3V3
Wire Wire Line
	1050 5050 1250 5050
Connection ~ 1250 5050
Wire Wire Line
	9550 5000 9550 5450
Wire Wire Line
	9900 5300 9550 5300
Connection ~ 9550 5300
Wire Wire Line
	9100 5700 9100 5350
Wire Wire Line
	9100 5350 9550 5350
Connection ~ 9550 5350
Wire Wire Line
	5500 2050 5600 2050
Connection ~ 5600 2050
Connection ~ 5500 2050
Text GLabel 2700 3200 0    60   Input ~ 0
TS_PENIRQ
Wire Wire Line
	2700 3200 2900 3200
Text GLabel 1700 3800 0    60   Input ~ 0
TS_CS
Wire Wire Line
	1700 3800 2900 3800
Text GLabel 8700 5900 2    60   Input ~ 0
TS_CLK
Wire Wire Line
	8700 5900 8000 5900
Text GLabel 4100 1850 1    60   Input ~ 0
TS_DIN
Wire Wire Line
	4100 1850 4100 2200
Text GLabel 4400 1950 1    60   Input ~ 0
TS_DOUT
Wire Wire Line
	4400 1950 4400 2200
Text GLabel 3700 2050 1    60   Input ~ 0
TS_BUSY
Wire Wire Line
	3700 2050 3700 2200
Text GLabel 10100 2800 1    60   Input ~ 0
U0_RXD
Text GLabel 9750 3000 3    60   Input ~ 0
U0_TXD
Wire Wire Line
	9750 3000 9750 2950
Connection ~ 9750 2950
Wire Wire Line
	10100 2800 10100 2850
Connection ~ 10100 2850
Text GLabel 4750 1800 1    60   Input ~ 0
RST
Wire Wire Line
	4750 1800 4900 1800
Wire Wire Line
	4900 1800 4900 1850
Connection ~ 4900 1850
Text GLabel 3300 6550 0    60   Input ~ 0
LCD_RESET
Wire Wire Line
	3800 6700 3800 6750
Wire Wire Line
	3800 6750 3450 6750
Wire Wire Line
	3450 6750 3450 6550
Wire Wire Line
	3450 6550 3300 6550
$EndSCHEMATC
