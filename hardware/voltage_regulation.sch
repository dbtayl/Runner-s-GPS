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
Sheet 4 4
Title "LPC1788FBD144 Devboard"
Date "11 nov 2014"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 6950 3750 0    60   ~ 0
Not breaking out any analog pins;\ndon't care about noise on analog lines
Wire Wire Line
	7500 3950 8000 3950
Wire Wire Line
	8000 3550 7550 3550
Wire Wire Line
	2450 2500 2450 2550
Text GLabel 2450 2500 1    60   Input ~ 0
GND
$Comp
L C C5
U 1 1 5122993A
P 2450 2750
F 0 "C5" V 2500 2850 50  0000 L CNN
F 1 "4.7u" H 2500 2650 50  0000 L CNN
F 2 "" H 2450 2750 60  0001 C CNN
F 3 "" H 2450 2750 60  0001 C CNN
	1    2450 2750
	1    0    0    -1  
$EndComp
$Comp
L C C6
U 1 1 51229922
P 4950 3450
F 0 "C6" H 5000 3550 50  0000 L CNN
F 1 "10u" H 5000 3350 50  0000 L CNN
F 2 "" H 4950 3450 60  0001 C CNN
F 3 "" H 4950 3450 60  0001 C CNN
	1    4950 3450
	1    0    0    -1  
$EndComp
Text GLabel 2150 3000 0    60   Input ~ 0
V_BAT
$Comp
L INDUCTOR L1
U 1 1 5123B1A5
P 4450 3250
F 0 "L1" V 4400 3250 40  0000 C CNN
F 1 "2.2 uH" V 4550 3250 40  0000 C CNN
F 2 "" H 4450 3250 60  0001 C CNN
F 3 "" H 4450 3250 60  0001 C CNN
	1    4450 3250
	0    -1   -1   0   
$EndComp
Text GLabel 2250 3250 0    60   Input ~ 0
GND
Text GLabel 8000 3950 2    60   Input ~ 0
GND
Text GLabel 8000 3550 2    60   Input ~ 0
3V3
Text GLabel 7500 3950 0    60   Input ~ 0
GND_A
Text GLabel 7550 3550 0    60   Input ~ 0
3V3_A
Text GLabel 5300 3250 2    60   Input ~ 0
3V3
$Comp
L MCP73831 U4
U 1 1 52432FE4
P 4550 6200
F 0 "U4" H 4550 6100 50  0000 C CNN
F 1 "MCP73831" H 4550 6300 50  0000 C CNN
F 2 "SOT23-5" H 4550 5900 50  0001 C CNN
F 3 "DOCUMENTATION" H 4550 6000 50  0001 C CNN
	1    4550 6200
	1    0    0    -1  
$EndComp
$Comp
L R R_ref1
U 1 1 52432FF3
P 5300 6550
F 0 "R_ref1" V 5380 6550 40  0000 C CNN
F 1 "5.1k" V 5307 6551 40  0000 C CNN
F 2 "~" V 5230 6550 30  0000 C CNN
F 3 "~" H 5300 6550 30  0000 C CNN
	1    5300 6550
	1    0    0    -1  
$EndComp
$Comp
L LED D1
U 1 1 52433002
P 4000 5500
F 0 "D1" H 4000 5600 50  0000 C CNN
F 1 "LED" H 4000 5400 50  0000 C CNN
F 2 "~" H 4000 5500 60  0000 C CNN
F 3 "~" H 4000 5500 60  0000 C CNN
	1    4000 5500
	-1   0    0    1   
$EndComp
Text GLabel 3800 6400 0    60   Input ~ 0
V_BAT
Text GLabel 5550 6100 2    60   Input ~ 0
V_USB
Text GLabel 3800 6200 0    60   Input ~ 0
GND
Text GLabel 5300 6800 3    60   Input ~ 0
GND
$Comp
L C C1
U 1 1 5243304D
P 5400 5900
F 0 "C1" H 5400 6000 40  0000 L CNN
F 1 "4.7u" H 5406 5815 40  0000 L CNN
F 2 "~" H 5438 5750 30  0000 C CNN
F 3 "~" H 5400 5900 60  0000 C CNN
	1    5400 5900
	1    0    0    -1  
$EndComp
Text GLabel 5400 5700 1    60   Input ~ 0
GND
$Comp
L C C2
U 1 1 52433060
P 3800 6600
F 0 "C2" H 3800 6700 40  0000 L CNN
F 1 "4.7u" H 3806 6515 40  0000 L CNN
F 2 "~" H 3838 6450 30  0000 C CNN
F 3 "~" H 3800 6600 60  0000 C CNN
	1    3800 6600
	1    0    0    -1  
$EndComp
Text GLabel 3800 6800 3    60   Input ~ 0
GND
$Comp
L R R12
U 1 1 52433084
P 3800 5750
F 0 "R12" V 3880 5750 40  0000 C CNN
F 1 "470" V 3807 5751 40  0000 C CNN
F 2 "~" V 3730 5750 30  0000 C CNN
F 3 "~" H 3800 5750 30  0000 C CNN
	1    3800 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 6100 5550 6100
Connection ~ 5400 6100
Wire Wire Line
	5300 6100 5300 5500
Wire Wire Line
	5300 5500 4200 5500
$Comp
L CONN_2 P2
U 1 1 524334BC
P 6800 2300
F 0 "P2" V 6750 2300 40  0000 C CNN
F 1 "CONN_2" V 6850 2300 40  0000 C CNN
F 2 "" H 6800 2300 60  0000 C CNN
F 3 "" H 6800 2300 60  0000 C CNN
	1    6800 2300
	1    0    0    -1  
$EndComp
Text GLabel 6450 2200 0    60   Input ~ 0
V_BAT
Text GLabel 6450 2400 0    60   Input ~ 0
GND
Text GLabel 1300 4700 0    60   Input ~ 0
GND
Text GLabel 1300 4900 0    60   Input ~ 0
V_BAT
Text GLabel 2350 4800 2    60   Input ~ 0
REG_EN
Text GLabel 2150 3350 0    60   Input ~ 0
REG_EN
$Comp
L SWITCH_INV SW3
U 1 1 5333649F
P 1850 4800
F 0 "SW3" H 1650 4950 50  0000 C CNN
F 1 "SWITCH_INV" H 1700 4650 50  0000 C CNN
F 2 "~" H 1850 4800 60  0000 C CNN
F 3 "~" H 1850 4800 60  0000 C CNN
	1    1850 4800
	-1   0    0    1   
$EndComp
$Comp
L LM3671 U6
U 1 1 5333A10B
P 3200 3250
F 0 "U6" H 3200 3150 50  0000 C CNN
F 1 "LM3671" H 3200 3350 50  0000 C CNN
F 2 "MODULE" H 3200 3250 50  0001 C CNN
F 3 "DOCUMENTATION" H 3200 3250 50  0001 C CNN
	1    3200 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 3250 4150 3250
Wire Wire Line
	3950 3350 4750 3350
Wire Wire Line
	4750 3350 4750 3250
Text GLabel 4800 3650 0    60   Input ~ 0
GND
Wire Wire Line
	4800 3650 5200 3650
Wire Wire Line
	4750 3250 5300 3250
Wire Wire Line
	2150 3350 2450 3350
Wire Wire Line
	2250 3250 2450 3250
Connection ~ 4950 3250
Wire Wire Line
	2150 3000 2450 3000
Wire Wire Line
	2450 2950 2450 3150
Connection ~ 2450 3000
$Comp
L C C7
U 1 1 53ACAC91
P 5200 3450
F 0 "C7" H 5250 3550 50  0000 L CNN
F 1 "0.1u" H 5250 3350 50  0000 L CNN
F 2 "" H 5200 3450 60  0001 C CNN
F 3 "" H 5200 3450 60  0001 C CNN
	1    5200 3450
	1    0    0    -1  
$EndComp
Connection ~ 4950 3650
Wire Wire Line
	5150 3250 5200 3250
Connection ~ 5200 3250
Connection ~ 5150 3250
Wire Wire Line
	1300 4900 1350 4900
Wire Wire Line
	1300 4700 1350 4700
$EndSCHEMATC
