EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A3 11693 16535 portrait
encoding utf-8
Sheet 1 2
Title "CamTrol V3.6"
Date "2021-12-15"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:PWR_FLAG #FLG01
U 1 1 5E692212
P 1700 2200
F 0 "#FLG01" H 1700 2275 50  0001 C CNN
F 1 "PWR_FLAG" H 1700 2373 50  0000 C CNN
F 2 "" H 1700 2200 50  0001 C CNN
F 3 "~" H 1700 2200 50  0001 C CNN
	1    1700 2200
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR09
U 1 1 5E250FEE
P 2100 2200
F 0 "#PWR09" H 2100 2050 50  0001 C CNN
F 1 "+5V" H 2115 2373 50  0000 C CNN
F 2 "" H 2100 2200 50  0001 C CNN
F 3 "" H 2100 2200 50  0001 C CNN
	1    2100 2200
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG03
U 1 1 5E252E46
P 2500 2200
F 0 "#FLG03" H 2500 2275 50  0001 C CNN
F 1 "PWR_FLAG" H 2500 2373 50  0000 C CNN
F 2 "" H 2500 2200 50  0001 C CNN
F 3 "~" H 2500 2200 50  0001 C CNN
	1    2500 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 2200 1300 2400
Wire Wire Line
	1300 2400 1700 2400
Wire Wire Line
	1700 2400 1700 2200
Wire Wire Line
	2100 2200 2100 2400
Wire Wire Line
	2100 2400 2500 2400
Wire Wire Line
	2500 2400 2500 2200
$Comp
L power:GNDPWR #PWR017
U 1 1 5E25DF42
P 3000 2400
AR Path="/5E25DF42" Ref="#PWR017"  Part="1" 
AR Path="/5E3D0683/5E25DF42" Ref="#PWR?"  Part="1" 
F 0 "#PWR017" H 3000 2200 50  0001 C CNN
F 1 "GNDPWR" H 3004 2246 50  0000 C CNN
F 2 "" H 3000 2350 50  0001 C CNN
F 3 "" H 3000 2350 50  0001 C CNN
	1    3000 2400
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG04
U 1 1 5E25F701
P 3000 2200
F 0 "#FLG04" H 3000 2275 50  0001 C CNN
F 1 "PWR_FLAG" H 3000 2373 50  0000 C CNN
F 2 "" H 3000 2200 50  0001 C CNN
F 3 "~" H 3000 2200 50  0001 C CNN
	1    3000 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 2200 3000 2400
$Comp
L power:GNDPWR #PWR020
U 1 1 611BFDD2
P 5300 8800
AR Path="/611BFDD2" Ref="#PWR020"  Part="1" 
AR Path="/5E3D0683/611BFDD2" Ref="#PWR?"  Part="1" 
F 0 "#PWR020" H 5300 8600 50  0001 C CNN
F 1 "GNDPWR" H 5304 8646 50  0000 C CNN
F 2 "" H 5300 8750 50  0001 C CNN
F 3 "" H 5300 8750 50  0001 C CNN
	1    5300 8800
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 8750 5300 8750
Wire Wire Line
	5300 8750 5300 8800
Wire Wire Line
	3600 8850 3700 8850
Text Notes 3900 9150 0    50   ~ 0
*Current 40mA
Text Notes 3900 9250 0    50   ~ 0
*12Bit (4095)
Text GLabel 3700 8650 0    50   Input ~ 0
GEN_OUT_01
Text GLabel 3700 8550 0    50   Input ~ 0
GEN_OUT_02
Text GLabel 3700 8450 0    50   Input ~ 0
GEN_OUT_03
Text GLabel 3700 8350 0    50   Input ~ 0
GEN_OUT_04
Text GLabel 3700 8250 0    50   Input ~ 0
GEN_OUT_05
Text GLabel 3700 8150 0    50   Input ~ 0
GEN_OUT_06
$Comp
L power:+12P #PWR01
U 1 1 611BFE3D
P 1100 7000
F 0 "#PWR01" H 1100 6850 50  0001 C CNN
F 1 "+12P" H 1115 7173 50  0000 C CNN
F 2 "" H 1100 7000 50  0001 C CNN
F 3 "" H 1100 7000 50  0001 C CNN
	1    1100 7000
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 7950 1850 8000
Wire Wire Line
	1850 8000 1850 8100
Connection ~ 1850 8000
$Comp
L power:GNDPWR #PWR06
U 1 1 611BFE5C
P 2200 8100
AR Path="/611BFE5C" Ref="#PWR06"  Part="1" 
AR Path="/5E3D0683/611BFE5C" Ref="#PWR?"  Part="1" 
F 0 "#PWR06" H 2200 7900 50  0001 C CNN
F 1 "GNDPWR" H 2204 7946 50  0000 C CNN
F 2 "" H 2200 8050 50  0001 C CNN
F 3 "" H 2200 8050 50  0001 C CNN
	1    2200 8100
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR03
U 1 1 611BFE62
P 1850 8100
AR Path="/611BFE62" Ref="#PWR03"  Part="1" 
AR Path="/5E3D0683/611BFE62" Ref="#PWR?"  Part="1" 
F 0 "#PWR03" H 1850 7900 50  0001 C CNN
F 1 "GNDPWR" H 1854 7946 50  0000 C CNN
F 2 "" H 1850 8050 50  0001 C CNN
F 3 "" H 1850 8050 50  0001 C CNN
	1    1850 8100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 8000 1850 8000
Wire Wire Line
	2050 7800 2050 8000
Wire Wire Line
	2000 7800 2050 7800
Wire Wire Line
	1850 7650 1850 7550
Wire Wire Line
	1100 7150 1250 7150
Wire Wire Line
	1100 7000 1100 7150
$Comp
L power:GNDPWR #PWR016
U 1 1 611BFE84
P 3600 9700
AR Path="/611BFE84" Ref="#PWR016"  Part="1" 
AR Path="/5E3D0683/611BFE84" Ref="#PWR?"  Part="1" 
F 0 "#PWR016" H 3600 9500 50  0001 C CNN
F 1 "GNDPWR" H 3604 9546 50  0000 C CNN
F 2 "" H 3600 9650 50  0001 C CNN
F 3 "" H 3600 9650 50  0001 C CNN
	1    3600 9700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 3700 2100 3850
$Comp
L power:GNDPWR #PWR05
U 1 1 60B9B127
P 2100 3850
AR Path="/60B9B127" Ref="#PWR05"  Part="1" 
AR Path="/5E3D0683/60B9B127" Ref="#PWR?"  Part="1" 
F 0 "#PWR05" H 2100 3650 50  0001 C CNN
F 1 "GNDPWR" H 2104 3696 50  0000 C CNN
F 2 "" H 2100 3800 50  0001 C CNN
F 3 "" H 2100 3800 50  0001 C CNN
	1    2100 3850
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR034
U 1 1 5E2B81A1
P 6650 2450
F 0 "#PWR034" H 6650 2300 50  0001 C CNN
F 1 "+5V" H 6665 2623 50  0000 C CNN
F 2 "" H 6650 2450 50  0001 C CNN
F 3 "" H 6650 2450 50  0001 C CNN
	1    6650 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 4400 2700 4400
$Comp
L power:+12P #PWR02
U 1 1 60D979D0
P 1300 2200
F 0 "#PWR02" H 1300 2050 50  0001 C CNN
F 1 "+12P" H 1315 2373 50  0000 C CNN
F 2 "" H 1300 2200 50  0001 C CNN
F 3 "" H 1300 2200 50  0001 C CNN
	1    1300 2200
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR037
U 1 1 60F48B38
P 6850 4250
AR Path="/60F48B38" Ref="#PWR037"  Part="1" 
AR Path="/5E3D0683/60F48B38" Ref="#PWR?"  Part="1" 
F 0 "#PWR037" H 6850 4050 50  0001 C CNN
F 1 "GNDPWR" H 6854 4096 50  0000 C CNN
F 2 "" H 6850 4200 50  0001 C CNN
F 3 "" H 6850 4200 50  0001 C CNN
	1    6850 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 4250 6850 4200
Wire Wire Line
	6850 3900 6850 3850
$Comp
L power:GNDPWR #PWR012
U 1 1 611BFDDA
P 3000 9000
AR Path="/611BFDDA" Ref="#PWR012"  Part="1" 
AR Path="/5E3D0683/611BFDDA" Ref="#PWR?"  Part="1" 
F 0 "#PWR012" H 3000 8800 50  0001 C CNN
F 1 "GNDPWR" H 3004 8846 50  0000 C CNN
F 2 "" H 3000 8950 50  0001 C CNN
F 3 "" H 3000 8950 50  0001 C CNN
	1    3000 9000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3000 8750 3000 8850
Wire Wire Line
	3000 8750 3700 8750
$Comp
L power:+5V #PWR036
U 1 1 610B9BF7
P 6850 3350
F 0 "#PWR036" H 6850 3200 50  0001 C CNN
F 1 "+5V" H 6865 3523 50  0000 C CNN
F 2 "" H 6850 3350 50  0001 C CNN
F 3 "" H 6850 3350 50  0001 C CNN
	1    6850 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 3550 6850 3350
$Comp
L power:+5V #PWR021
U 1 1 64028349
P 5100 6650
F 0 "#PWR021" H 5100 6500 50  0001 C CNN
F 1 "+5V" H 5115 6823 50  0000 C CNN
F 2 "" H 5100 6650 50  0001 C CNN
F 3 "" H 5100 6650 50  0001 C CNN
	1    5100 6650
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR023
U 1 1 6402DF33
P 5300 6650
F 0 "#PWR023" H 5300 6500 50  0001 C CNN
F 1 "+5V" H 5315 6823 50  0000 C CNN
F 2 "" H 5300 6650 50  0001 C CNN
F 3 "" H 5300 6650 50  0001 C CNN
	1    5300 6650
	1    0    0    -1  
$EndComp
NoConn ~ 3700 7450
Text GLabel 5000 8050 2    50   Input ~ 0
LED_OUT_06
Text GLabel 5000 8250 2    50   Input ~ 0
LED_OUT_05
Text GLabel 5000 8350 2    50   Input ~ 0
LED_OUT_04
Text GLabel 5000 8450 2    50   Input ~ 0
LED_OUT_03
Text GLabel 5000 7950 2    50   Input ~ 0
LED_OUT_02
Text GLabel 5000 8650 2    50   Input ~ 0
LED_OUT_01
Wire Wire Line
	3250 10650 3250 10400
Wire Wire Line
	2750 10500 2750 10400
Wire Wire Line
	2750 10400 3250 10400
Wire Wire Line
	3250 10950 3250 11200
Wire Wire Line
	2750 11100 2750 11200
Wire Wire Line
	2750 11200 3250 11200
NoConn ~ 2450 10500
Wire Wire Line
	1550 10750 1850 10750
Wire Wire Line
	1850 10750 1850 10350
Wire Wire Line
	1850 10350 2250 10350
Wire Wire Line
	2250 10350 2250 10500
Wire Wire Line
	1550 10850 1850 10850
Wire Wire Line
	1850 10850 1850 11250
Wire Wire Line
	1850 11250 2350 11250
Wire Wire Line
	2350 11250 2350 11100
Text GLabel 3400 11200 2    50   Input ~ 0
INVERTOR
Text Notes 1100 11000 0    50   ~ 0
INVERTOR ONLY\n
Text Notes 950  4200 0    50   ~ 0
MAIN SWITCH\n
Text Notes 950  4700 0    55   ~ 0
1 : AUTO\n2 : 12V\n3 : MANUAL\n4 : LED +\n5 : LED -\n
$Comp
L power:GNDPWR #PWR024
U 1 1 64B10814
P 6250 3350
AR Path="/64B10814" Ref="#PWR024"  Part="1" 
AR Path="/5E3D0683/64B10814" Ref="#PWR?"  Part="1" 
F 0 "#PWR024" H 6250 3150 50  0001 C CNN
F 1 "GNDPWR" H 6254 3196 50  0000 C CNN
F 2 "" H 6250 3300 50  0001 C CNN
F 3 "" H 6250 3300 50  0001 C CNN
	1    6250 3350
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR022
U 1 1 64B10E29
P 4950 3350
AR Path="/64B10E29" Ref="#PWR022"  Part="1" 
AR Path="/5E3D0683/64B10E29" Ref="#PWR?"  Part="1" 
F 0 "#PWR022" H 4950 3150 50  0001 C CNN
F 1 "GNDPWR" H 4954 3196 50  0000 C CNN
F 2 "" H 4950 3300 50  0001 C CNN
F 3 "" H 4950 3300 50  0001 C CNN
	1    4950 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 3350 4950 3200
Text GLabel 4850 5300 2    50   Input ~ 0
INVERTOR
Wire Wire Line
	6250 3350 6250 3200
Wire Wire Line
	5000 7550 5100 7550
Wire Wire Line
	5000 7850 5300 7850
Wire Wire Line
	5100 7350 5100 7550
Connection ~ 5100 7550
Wire Wire Line
	5100 7550 6500 7550
Wire Wire Line
	5300 7350 5300 7850
Connection ~ 5300 7850
$Comp
L power:+5V #PWR0131
U 1 1 6439E77E
P 5750 6600
F 0 "#PWR0131" H 5750 6450 50  0001 C CNN
F 1 "+5V" H 5765 6773 50  0000 C CNN
F 2 "" H 5750 6600 50  0001 C CNN
F 3 "" H 5750 6600 50  0001 C CNN
	1    5750 6600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5750 6600 5750 6750
Wire Wire Line
	5750 6750 5900 6750
$Comp
L power:GNDPWR #PWR0132
U 1 1 643AB6FC
P 6200 7650
AR Path="/643AB6FC" Ref="#PWR0132"  Part="1" 
AR Path="/5E3D0683/643AB6FC" Ref="#PWR?"  Part="1" 
F 0 "#PWR0132" H 6200 7450 50  0001 C CNN
F 1 "GNDPWR" H 6204 7496 50  0000 C CNN
F 2 "" H 6200 7600 50  0001 C CNN
F 3 "" H 6200 7600 50  0001 C CNN
	1    6200 7650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 7650 6200 6750
Text Label 5400 7550 0    50   ~ 0
SCL
Text Label 5400 7850 0    50   ~ 0
SDA
Text Label 5400 7650 0    50   ~ 0
TxD
Text Label 5400 7750 0    50   ~ 0
RxD
Wire Wire Line
	6650 2750 6650 2450
Connection ~ 6250 2750
Wire Wire Line
	6250 2750 6650 2750
Wire Wire Line
	6250 2750 6250 2900
Wire Wire Line
	2200 8100 2200 7800
$Comp
L power:GNDPWR #PWR0153
U 1 1 615A10E1
P 2000 10100
AR Path="/615A10E1" Ref="#PWR0153"  Part="1" 
AR Path="/5E3D0683/615A10E1" Ref="#PWR?"  Part="1" 
F 0 "#PWR0153" H 2000 9900 50  0001 C CNN
F 1 "GNDPWR" H 2004 9946 50  0000 C CNN
F 2 "" H 2000 10050 50  0001 C CNN
F 3 "" H 2000 10050 50  0001 C CNN
	1    2000 10100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 9700 2000 9800
$Comp
L power:GNDPWR #PWR0154
U 1 1 615C9A60
P 1600 10100
AR Path="/615C9A60" Ref="#PWR0154"  Part="1" 
AR Path="/5E3D0683/615C9A60" Ref="#PWR?"  Part="1" 
F 0 "#PWR0154" H 1600 9900 50  0001 C CNN
F 1 "GNDPWR" H 1604 9946 50  0000 C CNN
F 2 "" H 1600 10050 50  0001 C CNN
F 3 "" H 1600 10050 50  0001 C CNN
	1    1600 10100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 9550 1600 9700
$Comp
L power:GNDPWR #PWR0157
U 1 1 6160593D
P 2000 9100
AR Path="/6160593D" Ref="#PWR0157"  Part="1" 
AR Path="/5E3D0683/6160593D" Ref="#PWR?"  Part="1" 
F 0 "#PWR0157" H 2000 8900 50  0001 C CNN
F 1 "GNDPWR" H 2004 8946 50  0000 C CNN
F 2 "" H 2000 9050 50  0001 C CNN
F 3 "" H 2000 9050 50  0001 C CNN
	1    2000 9100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 8700 2000 8800
$Comp
L power:GNDPWR #PWR0158
U 1 1 61605949
P 1600 9100
AR Path="/61605949" Ref="#PWR0158"  Part="1" 
AR Path="/5E3D0683/61605949" Ref="#PWR?"  Part="1" 
F 0 "#PWR0158" H 1600 8900 50  0001 C CNN
F 1 "GNDPWR" H 1604 8946 50  0000 C CNN
F 2 "" H 1600 9050 50  0001 C CNN
F 3 "" H 1600 9050 50  0001 C CNN
	1    1600 9100
	1    0    0    -1  
$EndComp
Connection ~ 2000 8700
Wire Wire Line
	2100 8700 2000 8700
Wire Wire Line
	2100 9700 2000 9700
Connection ~ 2000 9700
Wire Wire Line
	2400 8700 2550 8700
Wire Wire Line
	2550 8700 2550 9700
Wire Wire Line
	2550 9950 2950 9950
Wire Wire Line
	2400 9700 2550 9700
Connection ~ 2550 9700
Wire Wire Line
	2550 9700 2550 9950
Wire Wire Line
	2000 8700 2000 8450
Wire Wire Line
	2000 8450 2100 8450
Wire Wire Line
	2000 9700 2000 9450
Wire Wire Line
	2000 9450 2100 9450
Text GLabel 4200 3200 2    50   Input ~ 0
AUTO
$Comp
L power:GNDPWR #PWR0161
U 1 1 609F1EA5
P 2700 4850
AR Path="/609F1EA5" Ref="#PWR0161"  Part="1" 
AR Path="/5E3D0683/609F1EA5" Ref="#PWR?"  Part="1" 
F 0 "#PWR0161" H 2700 4650 50  0001 C CNN
F 1 "GNDPWR" H 2704 4696 50  0000 C CNN
F 2 "" H 2700 4800 50  0001 C CNN
F 3 "" H 2700 4800 50  0001 C CNN
	1    2700 4850
	1    0    0    -1  
$EndComp
Connection ~ 4700 6000
Wire Wire Line
	4700 6000 4700 6150
Wire Wire Line
	4700 6000 4550 6000
Wire Wire Line
	4700 5850 4700 6000
$Comp
L power:GNDPWR #PWR019
U 1 1 60CFA991
P 4700 6150
AR Path="/60CFA991" Ref="#PWR019"  Part="1" 
AR Path="/5E3D0683/60CFA991" Ref="#PWR?"  Part="1" 
F 0 "#PWR019" H 4700 5950 50  0001 C CNN
F 1 "GNDPWR" H 4704 5996 50  0000 C CNN
F 2 "" H 4700 6100 50  0001 C CNN
F 3 "" H 4700 6100 50  0001 C CNN
	1    4700 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 6100 4150 6000
Wire Wire Line
	4150 5650 4400 5650
Wire Wire Line
	4250 6000 4150 6000
Connection ~ 4150 6000
Wire Wire Line
	4150 6000 4150 5650
Wire Wire Line
	5000 7450 5000 6550
Wire Wire Line
	5000 6550 4150 6550
Wire Wire Line
	4150 6550 4150 6400
Wire Wire Line
	5300 7850 6400 7850
Wire Wire Line
	5100 7050 5100 6650
Wire Wire Line
	5300 7050 5300 6650
Wire Wire Line
	5000 7650 5750 7650
Wire Wire Line
	6100 7650 6100 6750
Wire Wire Line
	4200 3200 4150 3200
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R7
U 1 1 60CF8FD8
P 4150 6100
F 0 "R7" V 4050 5950 50  0000 C CNN
F 1 "102F" V 4150 5950 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 4080 6100 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000614" H 4150 6100 50  0001 C CNN
F 4 "20" H 4150 6100 50  0001 C CNN "Price"
F 5 "부품세상" H 4150 6100 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 4150 6100 50  0001 C CNN "Package"
F 7 "1KΩ" H 4150 6100 50  0001 C CNN "Value"
	1    4150 6100
	-1   0    0    -1  
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R8
U 1 1 60CFA2EB
P 4250 6000
F 0 "R8" V 4150 5850 50  0000 C CNN
F 1 "103F" V 4250 5850 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 4180 6000 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 4250 6000 50  0001 C CNN
F 4 "20" H 4250 6000 50  0001 C CNN "Price"
F 5 "부품세상" H 4250 6000 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 4250 6000 50  0001 C CNN "Package"
F 7 "10KΩ" H 4250 6000 50  0001 C CNN "Value"
	1    4250 6000
	0    -1   1    0   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R126
U 1 1 61605933
P 2400 8700
F 0 "R126" V 2300 8550 50  0000 C CNN
F 1 "330F" V 2400 8550 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 2330 8700 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 2400 8700 50  0001 C CNN
F 4 "20" H 2400 8700 50  0001 C CNN "Price"
F 5 "부품세상" H 2400 8700 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 2400 8700 50  0001 C CNN "Package"
F 7 "10KΩ" H 2400 8700 50  0001 C CNN "Value"
	1    2400 8700
	0    1    1    0   
$EndComp
$Comp
L Server_Used:C C28
U 1 1 61605925
P 2000 8950
F 0 "C28" H 2115 8996 50  0000 L CNN
F 1 "106" H 2115 8905 50  0000 L CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 2038 8800 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 2000 8950 50  0001 C CNN
F 4 "66" H 2000 8950 50  0001 C CNN "Price"
F 5 "부품세상" H 2000 8950 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 2000 8950 50  0001 C CNN "Package"
F 7 "106(10uF)" H 2000 8950 50  0001 C CNN "Value"
	1    2000 8950
	1    0    0    -1  
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R127
U 1 1 61583111
P 2400 9700
F 0 "R127" V 2300 9550 50  0000 C CNN
F 1 "330F" V 2400 9550 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 2330 9700 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 2400 9700 50  0001 C CNN
F 4 "20" H 2400 9700 50  0001 C CNN "Price"
F 5 "부품세상" H 2400 9700 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 2400 9700 50  0001 C CNN "Package"
F 7 "10KΩ" H 2400 9700 50  0001 C CNN "Value"
	1    2400 9700
	0    1    1    0   
$EndComp
$Comp
L Server_Used:C C29
U 1 1 61581E23
P 2000 9950
F 0 "C29" H 2115 9996 50  0000 L CNN
F 1 "106" H 2115 9905 50  0000 L CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 2038 9800 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 2000 9950 50  0001 C CNN
F 4 "66" H 2000 9950 50  0001 C CNN "Price"
F 5 "부품세상" H 2000 9950 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 2000 9950 50  0001 C CNN "Package"
F 7 "106(10uF)" H 2000 9950 50  0001 C CNN "Value"
	1    2000 9950
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x04_Male J?
U 1 1 6438EED4
P 6000 6550
AR Path="/60E3C840/6438EED4" Ref="J?"  Part="1" 
AR Path="/6438EED4" Ref="J5"  Part="1" 
F 0 "J5" V 5800 6600 50  0000 C CNN
F 1 "YMW025-04R" V 5900 6450 50  0000 C CNN
F 2 "Server.pretty:Molex_SPOX_5267-04A_1x04_P2.50mm_Vertical" H 6000 6550 50  0001 C CNN
F 3 "https://www.devicemart.co.kr/goods/view?no=419" H 6000 6550 50  0001 C CNN
F 4 "DEVICEMART" H 6000 6550 50  0001 C CNN "Manufacture"
F 5 "30" H 6000 6550 50  0001 C CNN "Price"
F 6 "5267-04P" H 6000 6550 50  0001 C CNN "Value"
	1    6000 6550
	0    -1   1    0   
$EndComp
$Comp
L Mechanical:MountingHole H25
U 1 1 6260F66C
P 1850 11950
F 0 "H25" H 1950 11996 50  0001 L CNN
F 1 "MH" H 1950 11905 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 1850 11950 50  0001 C CNN
F 3 "~" H 1850 11950 50  0001 C CNN
	1    1850 11950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H24
U 1 1 62460DE0
P 3050 11950
F 0 "H24" H 3150 11996 50  0001 L CNN
F 1 "MH" H 3150 11905 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 3050 11950 50  0001 C CNN
F 3 "~" H 3050 11950 50  0001 C CNN
	1    3050 11950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H23
U 1 1 62460DDA
P 2750 11950
F 0 "H23" H 2850 11996 50  0001 L CNN
F 1 "MH" H 2850 11905 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 2750 11950 50  0001 C CNN
F 3 "~" H 2750 11950 50  0001 C CNN
	1    2750 11950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H22
U 1 1 6242F295
P 2450 11950
F 0 "H22" H 2550 11996 50  0001 L CNN
F 1 "MH" H 2550 11905 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 2450 11950 50  0001 C CNN
F 3 "~" H 2450 11950 50  0001 C CNN
	1    2450 11950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H21
U 1 1 6242F28F
P 2150 11950
F 0 "H21" H 2250 11996 50  0001 L CNN
F 1 "MH" H 2250 11905 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 2150 11950 50  0001 C CNN
F 3 "~" H 2150 11950 50  0001 C CNN
	1    2150 11950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H20
U 1 1 6242F289
P 3650 11750
F 0 "H20" H 3750 11796 50  0001 L CNN
F 1 "MH" H 3750 11705 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 3650 11750 50  0001 C CNN
F 3 "~" H 3650 11750 50  0001 C CNN
	1    3650 11750
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H19
U 1 1 6242F283
P 1550 11950
F 0 "H19" H 1650 11996 50  0001 L CNN
F 1 "MH" H 1650 11905 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 1550 11950 50  0001 C CNN
F 3 "~" H 1550 11950 50  0001 C CNN
	1    1550 11950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H18
U 1 1 6242F27D
P 1250 11950
F 0 "H18" H 1350 11996 50  0001 L CNN
F 1 "MH" H 1350 11905 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 1250 11950 50  0001 C CNN
F 3 "~" H 1250 11950 50  0001 C CNN
	1    1250 11950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H15
U 1 1 6242F277
P 3050 11750
F 0 "H15" H 3150 11796 50  0001 L CNN
F 1 "MH" H 3150 11705 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 3050 11750 50  0001 C CNN
F 3 "~" H 3050 11750 50  0001 C CNN
	1    3050 11750
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x05_Male J?
U 1 1 60B4F58E
P 1500 4500
AR Path="/60E3C840/60B4F58E" Ref="J?"  Part="1" 
AR Path="/60B4F58E" Ref="J6"  Part="1" 
F 0 "J6" H 1600 4200 50  0000 C CNN
F 1 "MAIN SW" H 1550 4100 50  0000 C CNN
F 2 "Server.pretty:Molex_SPOX_5267-05A_1x05_P2.50mm_Vertical" H 1500 4500 50  0001 C CNN
F 3 "https://www.devicemart.co.kr/goods/view?no=419" H 1500 4500 50  0001 C CNN
F 4 "DEVICEMART" H 1500 4500 50  0001 C CNN "Manufacture"
F 5 "30" H 1500 4500 50  0001 C CNN "Price"
F 6 "5267-05P" H 1500 4500 50  0001 C CNN "Value"
	1    1500 4500
	1    0    0    -1  
$EndComp
$Comp
L Server_Used:C C4
U 1 1 64AF7440
P 6250 3050
F 0 "C4" H 6365 3096 50  0000 L CNN
F 1 "104" H 6365 3005 50  0000 L CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 6288 2900 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 6250 3050 50  0001 C CNN
F 4 "66" H 6250 3050 50  0001 C CNN "Price"
F 5 "부품세상" H 6250 3050 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 6250 3050 50  0001 C CNN "Package"
F 7 "106(10uF)" H 6250 3050 50  0001 C CNN "Value"
	1    6250 3050
	1    0    0    -1  
$EndComp
$Comp
L Server_Used:C C3
U 1 1 64AEBAA4
P 4950 3050
F 0 "C3" H 5065 3096 50  0000 L CNN
F 1 "106" H 5065 3005 50  0000 L CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 4988 2900 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 4950 3050 50  0001 C CNN
F 4 "66" H 4950 3050 50  0001 C CNN "Price"
F 5 "부품세상" H 4950 3050 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 4950 3050 50  0001 C CNN "Package"
F 7 "106(10uF)" H 4950 3050 50  0001 C CNN "Value"
	1    4950 3050
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H6
U 1 1 6567C937
P 2750 11750
F 0 "H6" H 2850 11796 50  0001 L CNN
F 1 "MH" H 2850 11705 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 2750 11750 50  0001 C CNN
F 3 "~" H 2750 11750 50  0001 C CNN
	1    2750 11750
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H5
U 1 1 65671A69
P 2450 11750
F 0 "H5" H 2550 11796 50  0001 L CNN
F 1 "MH" H 2550 11705 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 2450 11750 50  0001 C CNN
F 3 "~" H 2450 11750 50  0001 C CNN
	1    2450 11750
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 65666AE6
P 2150 11750
F 0 "H4" H 2250 11796 50  0001 L CNN
F 1 "MH" H 2250 11705 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 2150 11750 50  0001 C CNN
F 3 "~" H 2150 11750 50  0001 C CNN
	1    2150 11750
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 6565BA46
P 1850 11750
F 0 "H3" H 1950 11796 50  0001 L CNN
F 1 "MH" H 1950 11705 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 1850 11750 50  0001 C CNN
F 3 "~" H 1850 11750 50  0001 C CNN
	1    1850 11750
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 65650B4F
P 1550 11750
F 0 "H2" H 1650 11796 50  0001 L CNN
F 1 "MH" H 1650 11705 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 1550 11750 50  0001 C CNN
F 3 "~" H 1550 11750 50  0001 C CNN
	1    1550 11750
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H1
U 1 1 656471D2
P 1250 11750
F 0 "H1" H 1350 11796 50  0001 L CNN
F 1 "MH" H 1350 11705 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 1250 11750 50  0001 C CNN
F 3 "~" H 1250 11750 50  0001 C CNN
	1    1250 11750
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J1
U 1 1 6082E3CF
P 1350 10750
F 0 "J1" H 1450 10950 50  0000 C CNN
F 1 "INV" H 1500 10850 50  0000 C CNN
F 2 "Server.pretty:PhoenixContact_MCV_1,5_2-G-3.81_1x02_P3.81mm_Vertical" H 1350 10750 50  0001 C CNN
F 3 "https://www.devicemart.co.kr/goods/view?no=23260&market=naver&NaPm=ct%3Dkof8h100%7Cci%3D2b06ad3968d8adec0b2b30ae50256e40450772f8%7Ctr%3Dslsc%7Csn%3D876973%7Chk%3Dcbed85e5f251313af0535f600d98c0e674ebf3d9" H 1350 10750 50  0001 C CNN
F 4 "DEVICEMART" H 1350 10750 50  0001 C CNN "Manufacture"
F 5 "100" H 1350 10750 50  0001 C CNN "Price"
F 6 "ECH381V-2P" H 1350 10750 50  0001 C CNN "Value"
	1    1350 10750
	1    0    0    -1  
$EndComp
$Comp
L Simulation_SPICE:DIODE D?
U 1 1 6081DB10
P 3250 10800
AR Path="/5E3D0683/6081DB10" Ref="D?"  Part="1" 
AR Path="/6081DB10" Ref="D1"  Part="1" 
F 0 "D1" V 3300 10950 50  0000 C CNN
F 1 "M7" V 3200 10950 50  0000 C CNN
F 2 "Server.pretty:D_SMA" H 3250 10800 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000012566" H 3250 10800 50  0001 C CNN
F 4 "Y" H 3250 10800 50  0001 L CNN "Spice_Netlist_Enabled"
F 5 "D" H 3250 10800 50  0001 L CNN "Spice_Primitive"
F 6 "부품세상" H 3250 10800 50  0001 C CNN "Manufacture"
F 7 "DO-214AC(SMA)" H 3250 10800 50  0001 C CNN "Package"
F 8 "55" H 3250 10800 50  0001 C CNN "Price"
F 9 "4007" H 3250 10800 50  0001 C CNN "Value"
	1    3250 10800
	0    1    -1   0   
$EndComp
$Comp
L Server-cache:HJR1-2C-L-12VDC K?
U 1 1 607F78F2
P 2550 10800
AR Path="/5E3D0683/607F78F2" Ref="K?"  Part="1" 
AR Path="/607F78F2" Ref="K1"  Part="1" 
F 0 "K1" V 3117 10800 50  0000 C CNN
F 1 "JQC-T78-DC-12V" V 3026 10800 50  0000 C CNN
F 2 "Server.pretty:JQC-T78-DC12V-C" H 3000 10750 50  0001 L CNN
F 3 "https://www.devicemart.co.kr/goods/view?no=38804" H 2550 10800 50  0001 C CNN
F 4 "OPENIDEA" H 2550 10800 50  0001 C CNN "Manufacture"
F 5 "704" H 2550 10800 50  0001 C CNN "Price"
F 6 "JQC-T78-DC-12V" H 2550 10800 50  0001 C CNN "Value"
	1    2550 10800
	-1   0    0    -1  
$EndComp
$Comp
L Server_Used:ESP32-DEVKITC-32D U1
U 1 1 611BFE31
P 4500 8150
F 0 "U1" H 4350 9215 50  0000 C CNN
F 1 "ESP32-DEVKITC-32D" H 4350 9124 50  0000 C CNN
F 2 "Server.pretty:ESP32-DEVKITC-32D" H 3900 7050 50  0001 L BNN
F 3 "https://smartstore.naver.com/makerspace/products/2377048185" H 5000 7050 50  0001 L BNN
F 4 "Espressif Systems" H 3900 6950 50  0001 L BNN "Field4"
F 5 "ESP32-DEVKITC-32D" H 4500 8150 50  0001 C CNN "Value"
F 6 "송파메이커" H 4500 8150 50  0001 C CNN "Manufacture"
	1    4500 8150
	1    0    0    -1  
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R13
U 1 1 61823572
P 5300 7350
F 0 "R13" V 5200 7200 50  0000 C CNN
F 1 "103F" V 5300 7200 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 5230 7350 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 5300 7350 50  0001 C CNN
F 4 "20" H 5300 7350 50  0001 C CNN "Price"
F 5 "부품세상" H 5300 7350 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 5300 7350 50  0001 C CNN "Package"
F 7 "10KΩ" H 5300 7350 50  0001 C CNN "Value"
	1    5300 7350
	-1   0    0    1   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R18
U 1 1 60F48B66
P 6850 3550
AR Path="/60F48B66" Ref="R18"  Part="1" 
AR Path="/5E3D0683/60F48B66" Ref="R?"  Part="3" 
F 0 "R18" H 7050 3450 50  0000 R CNN
F 1 "103F" H 7100 3350 50  0000 R CNN
F 2 "Server.pretty:R_0805_HandSoldering" H 6850 3480 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" V 6850 3550 50  0001 C CNN
F 4 "20" H 6850 3550 50  0001 C CNN "Price"
F 5 "부품세상" H 6850 3550 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 6850 3550 50  0001 C CNN "Package"
F 7 "10KΩ" H 6850 3550 50  0001 C CNN "Value"
	1    6850 3550
	1    0    0    -1  
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:LED-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue D4
U 1 1 60F48B19
P 6850 4050
AR Path="/60F48B19" Ref="D4"  Part="1" 
AR Path="/5E3D0683/60F48B19" Ref="D?"  Part="1" 
F 0 "D4" V 6889 3932 50  0000 R CNN
F 1 "POWER" V 6798 3932 50  0000 R CNN
F 2 "Server.pretty:LED_0805" H 6850 4050 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000008652" H 6850 4050 50  0001 C CNN
F 4 "132" H 6850 4050 50  0001 C CNN "Price"
F 5 "부품세상" H 6850 4050 50  0001 C CNN "Manufacture"
F 6 "RED LED" H 6850 4050 50  0001 C CNN "Value"
F 7 "2012(0805)" H 6850 4050 50  0001 C CNN "Package"
	1    6850 4050
	0    -1   -1   0   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R12
U 1 1 60E7F791
P 5100 7350
F 0 "R12" V 5000 7200 50  0000 C CNN
F 1 "103F" V 5100 7200 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 5030 7350 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 5100 7350 50  0001 C CNN
F 4 "20" H 5100 7350 50  0001 C CNN "Price"
F 5 "부품세상" H 5100 7350 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 5100 7350 50  0001 C CNN "Package"
F 7 "10KΩ" H 5100 7350 50  0001 C CNN "Value"
	1    5100 7350
	-1   0    0    1   
$EndComp
$Comp
L Device:CP1 C2
U 1 1 611BFE7C
P 3600 9550
F 0 "C2" H 3715 9596 50  0000 L CNN
F 1 "100uF/25V" H 3715 9505 50  0000 L CNN
F 2 "Capacitor_SMD.pretty:CP_Elec_6.3x5.3" H 3600 9550 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000002852" H 3600 9550 50  0001 C CNN
F 4 "부품세상" H 3600 9550 50  0001 C CNN "Manufacture"
F 5 "6.3ΦX5.4" H 3600 9550 50  0001 C CNN "Package"
F 6 "66" H 3600 9550 50  0001 C CNN "Price"
F 7 "100uF" H 3600 9550 50  0001 C CNN "Value"
	1    3600 9550
	1    0    0    -1  
$EndComp
$Comp
L Server_Used:C C1
U 1 1 611BFE75
P 2200 7650
F 0 "C1" H 2315 7696 50  0000 L CNN
F 1 "106" H 2315 7605 50  0000 L CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 2238 7500 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 2200 7650 50  0001 C CNN
F 4 "66" H 2200 7650 50  0001 C CNN "Price"
F 5 "부품세상" H 2200 7650 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 2200 7650 50  0001 C CNN "Package"
F 7 "106(10uF)" H 2200 7650 50  0001 C CNN "Value"
	1    2200 7650
	1    0    0    -1  
$EndComp
$Comp
L Server_Used:3296Y AUX_V1
U 1 1 611BFE53
P 1850 7800
F 0 "AUX_V1" H 1782 7846 50  0000 R CNN
F 1 "202" H 1782 7755 50  0000 R CNN
F 2 "Server.pretty:Potentiometer_Bourns_3296Y_Vertical" H 1850 7800 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013040" H 1850 7800 50  0001 C CNN
F 4 "710" H 1850 7800 50  0001 C CNN "Price"
F 5 "B3296Y-2KΩ" H 1850 7800 50  0001 C CNN "Value"
F 6 "부품세상" H 1850 7800 50  0001 C CNN "Manufacture"
	1    1850 7800
	1    0    0    1   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R5
U 1 1 611BFE4B
P 2600 7150
F 0 "R5" V 2500 7000 50  0000 C CNN
F 1 "103F" V 2600 7000 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 2530 7150 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 2600 7150 50  0001 C CNN
F 4 "20" H 2600 7150 50  0001 C CNN "Price"
F 5 "부품세상" H 2600 7150 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 2600 7150 50  0001 C CNN "Package"
F 7 "10KΩ" H 2600 7150 50  0001 C CNN "Value"
	1    2600 7150
	0    1    1    0   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R1
U 1 1 611BFE44
P 1550 7150
F 0 "R1" V 1450 7000 50  0000 C CNN
F 1 "103" V 1550 7000 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 1480 7150 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 1550 7150 50  0001 C CNN
F 4 "20" H 1550 7150 50  0001 C CNN "Price"
F 5 "부품세상" H 1550 7150 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 1550 7150 50  0001 C CNN "Package"
F 7 "10KΩ" H 1550 7150 50  0001 C CNN "Value"
	1    1550 7150
	0    1    1    0   
$EndComp
$Sheet
S 7150 2000 600  600 
U 5E3D0683
F0 "OutPut" 50
F1 "file5E3D0682.sch" 50
$EndSheet
$Comp
L power:GNDPWR #PWR0114
U 1 1 60F11F52
P 6650 6450
AR Path="/60F11F52" Ref="#PWR0114"  Part="1" 
AR Path="/5E3D0683/60F11F52" Ref="#PWR?"  Part="1" 
F 0 "#PWR0114" H 6650 6250 50  0001 C CNN
F 1 "GNDPWR" H 6654 6296 50  0000 C CNN
F 2 "" H 6650 6400 50  0001 C CNN
F 3 "" H 6650 6400 50  0001 C CNN
	1    6650 6450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 5400 6950 5400
Wire Wire Line
	6400 5500 6950 5500
Wire Wire Line
	1600 8550 1600 8700
Wire Wire Line
	1450 9550 1600 9550
Wire Wire Line
	1450 8550 1600 8550
$Comp
L Connector:Conn_01x02_Male J25
U 1 1 61580C69
P 1250 9450
F 0 "J25" H 1350 9600 50  0000 C CNN
F 1 "WASTE" H 1100 9400 50  0000 C CNN
F 2 "Server.pretty:PhoenixContact_MCV_1,5_2-G-3.81_1x02_P3.81mm_Vertical" H 1250 9450 50  0001 C CNN
F 3 "https://www.devicemart.co.kr/goods/view?no=23260&market=naver&NaPm=ct%3Dkof8h100%7Cci%3D2b06ad3968d8adec0b2b30ae50256e40450772f8%7Ctr%3Dslsc%7Csn%3D876973%7Chk%3Dcbed85e5f251313af0535f600d98c0e674ebf3d9" H 1250 9450 50  0001 C CNN
F 4 "DEVICEMART" H 1250 9450 50  0001 C CNN "Manufacture"
F 5 "100" H 1250 9450 50  0001 C CNN "Price"
F 6 "ECH381V-2P" H 1250 9450 50  0001 C CNN "Value"
	1    1250 9450
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J24
U 1 1 61605905
P 1250 8450
F 0 "J24" H 1350 8600 50  0000 C CNN
F 1 "CLEAN" H 1100 8400 50  0000 C CNN
F 2 "Server.pretty:PhoenixContact_MCV_1,5_2-G-3.81_1x02_P3.81mm_Vertical" H 1250 8450 50  0001 C CNN
F 3 "https://www.devicemart.co.kr/goods/view?no=23260&market=naver&NaPm=ct%3Dkof8h100%7Cci%3D2b06ad3968d8adec0b2b30ae50256e40450772f8%7Ctr%3Dslsc%7Csn%3D876973%7Chk%3Dcbed85e5f251313af0535f600d98c0e674ebf3d9" H 1250 8450 50  0001 C CNN
F 4 "DEVICEMART" H 1250 8450 50  0001 C CNN "Manufacture"
F 5 "100" H 1250 8450 50  0001 C CNN "Price"
F 6 "ECH381V-2P" H 1250 8450 50  0001 C CNN "Value"
	1    1250 8450
	1    0    0    -1  
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R10
U 1 1 60C5B0B0
P 2400 8450
F 0 "R10" V 2300 8300 50  0000 C CNN
F 1 "103F" V 2400 8300 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 2330 8450 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 2400 8450 50  0001 C CNN
F 4 "20" H 2400 8450 50  0001 C CNN "Price"
F 5 "부품세상" H 2400 8450 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 2400 8450 50  0001 C CNN "Package"
F 7 "10KΩ" H 2400 8450 50  0001 C CNN "Value"
	1    2400 8450
	0    1    1    0   
$EndComp
$Comp
L power:+12P #PWR0116
U 1 1 62F3714D
P 2850 3150
F 0 "#PWR0116" H 2850 3000 50  0001 C CNN
F 1 "+12P" H 2865 3323 50  0000 C CNN
F 2 "" H 2850 3150 50  0001 C CNN
F 3 "" H 2850 3150 50  0001 C CNN
	1    2850 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	2850 3150 2850 3300
Connection ~ 2700 3450
Wire Wire Line
	2700 3450 2850 3450
$Comp
L Simulation_SPICE:DIODE D?
U 1 1 6321DCDF
P 3850 4000
AR Path="/5E3D0683/6321DCDF" Ref="D?"  Part="1" 
AR Path="/6321DCDF" Ref="D62"  Part="1" 
F 0 "D62" H 3850 4250 50  0000 C CNN
F 1 "M7" H 3850 4150 50  0000 C CNN
F 2 "Server.pretty:D_SMA" H 3850 4000 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000005570" H 3850 4000 50  0001 C CNN
F 4 "Y" H 3850 4000 50  0001 L CNN "Spice_Netlist_Enabled"
F 5 "D" H 3850 4000 50  0001 L CNN "Spice_Primitive"
F 6 "부품세상" H 3850 4000 50  0001 C CNN "Manufacture"
F 7 "DO-214AC(SMA)" H 3850 4000 50  0001 C CNN "Package"
F 8 "55" H 3850 4000 50  0001 C CNN "Price"
F 9 "4007" H 3850 4000 50  0001 C CNN "Value"
	1    3850 4000
	-1   0    0    1   
$EndComp
Wire Wire Line
	3700 4000 3400 4000
Wire Wire Line
	3400 4000 3400 3700
Wire Wire Line
	3400 3700 3550 3700
$Comp
L power:GNDPWR #PWR0118
U 1 1 63291E2B
P 4250 4100
AR Path="/63291E2B" Ref="#PWR0118"  Part="1" 
AR Path="/5E3D0683/63291E2B" Ref="#PWR?"  Part="1" 
F 0 "#PWR0118" H 4250 3900 50  0001 C CNN
F 1 "GNDPWR" H 4254 3946 50  0000 C CNN
F 2 "" H 4250 4050 50  0001 C CNN
F 3 "" H 4250 4050 50  0001 C CNN
	1    4250 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4250 3700 4150 3700
Wire Wire Line
	4000 4000 4250 4000
Wire Wire Line
	4250 4000 4250 3700
Connection ~ 4950 2750
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R16
U 1 1 60B9FC2D
P 1850 7550
F 0 "R16" V 1750 7400 50  0000 C CNN
F 1 "102" V 1850 7400 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 1780 7550 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 1850 7550 50  0001 C CNN
F 4 "20" H 1850 7550 50  0001 C CNN "Price"
F 5 "부품세상" H 1850 7550 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 1850 7550 50  0001 C CNN "Package"
F 7 "10KΩ" H 1850 7550 50  0001 C CNN "Value"
	1    1850 7550
	-1   0    0    1   
$EndComp
Wire Wire Line
	1550 7150 1850 7150
Wire Wire Line
	1850 7250 1850 7150
Connection ~ 1850 7150
Wire Wire Line
	1850 7150 2200 7150
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R17
U 1 1 60C5F26B
P 1600 9000
F 0 "R17" H 1450 8850 50  0000 C CNN
F 1 "100F" V 1600 8850 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 1530 9000 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 1600 9000 50  0001 C CNN
F 4 "20" H 1600 9000 50  0001 C CNN "Price"
F 5 "부품세상" H 1600 9000 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 1600 9000 50  0001 C CNN "Package"
F 7 "10KΩ" H 1600 9000 50  0001 C CNN "Value"
	1    1600 9000
	-1   0    0    1   
$EndComp
Wire Wire Line
	1600 9000 1600 9100
Wire Wire Line
	1600 10000 1600 10100
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R19
U 1 1 60C20D94
P 1600 10000
F 0 "R19" H 1450 9850 50  0000 C CNN
F 1 "100F" V 1600 9850 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 1530 10000 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 1600 10000 50  0001 C CNN
F 4 "20" H 1600 10000 50  0001 C CNN "Price"
F 5 "부품세상" H 1600 10000 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 1600 10000 50  0001 C CNN "Package"
F 7 "10KΩ" H 1600 10000 50  0001 C CNN "Value"
	1    1600 10000
	-1   0    0    1   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R15
U 1 1 60C5B70E
P 2400 9450
F 0 "R15" V 2300 9300 50  0000 C CNN
F 1 "103F" V 2400 9300 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 2330 9450 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 2400 9450 50  0001 C CNN
F 4 "20" H 2400 9450 50  0001 C CNN "Price"
F 5 "부품세상" H 2400 9450 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 2400 9450 50  0001 C CNN "Package"
F 7 "10KΩ" H 2400 9450 50  0001 C CNN "Value"
	1    2400 9450
	0    1    1    0   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R21
U 1 1 60C6F52C
P 5550 5950
F 0 "R21" V 5450 5800 50  0000 C CNN
F 1 "103F" V 5550 5800 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 5480 5950 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 5550 5950 50  0001 C CNN
F 4 "20" H 5550 5950 50  0001 C CNN "Price"
F 5 "부품세상" H 5550 5950 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 5550 5950 50  0001 C CNN "Package"
F 7 "10KΩ" H 5550 5950 50  0001 C CNN "Value"
	1    5550 5950
	0    -1   -1   0   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R20
U 1 1 60C70C57
P 5500 6100
F 0 "R20" V 5400 5950 50  0000 C CNN
F 1 "102F" V 5500 5950 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 5430 6100 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000614" H 5500 6100 50  0001 C CNN
F 4 "20" H 5500 6100 50  0001 C CNN "Price"
F 5 "부품세상" H 5500 6100 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 5500 6100 50  0001 C CNN "Package"
F 7 "1KΩ" H 5500 6100 50  0001 C CNN "Value"
	1    5500 6100
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5500 6100 5500 5950
Wire Wire Line
	5500 5650 5650 5650
Wire Wire Line
	5550 5950 5500 5950
Connection ~ 5500 5950
Wire Wire Line
	5500 5950 5500 5650
$Comp
L power:GNDPWR #PWR0113
U 1 1 60CD2F57
P 5950 6150
AR Path="/60CD2F57" Ref="#PWR0113"  Part="1" 
AR Path="/5E3D0683/60CD2F57" Ref="#PWR?"  Part="1" 
F 0 "#PWR0113" H 5950 5950 50  0001 C CNN
F 1 "GNDPWR" H 5954 5996 50  0000 C CNN
F 2 "" H 5950 6100 50  0001 C CNN
F 3 "" H 5950 6100 50  0001 C CNN
	1    5950 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 6150 5950 5950
Wire Wire Line
	5850 5950 5950 5950
Connection ~ 5950 5950
Wire Wire Line
	5950 5950 5950 5850
Connection ~ 3400 4000
$Comp
L Server_Used:C C5
U 1 1 60D0963B
P 3300 8850
F 0 "C5" V 3450 8800 50  0000 L CNN
F 1 "104" V 3550 8750 50  0000 L CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 3338 8700 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 3300 8850 50  0001 C CNN
F 4 "66" H 3300 8850 50  0001 C CNN "Price"
F 5 "부품세상" H 3300 8850 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 3300 8850 50  0001 C CNN "Package"
F 7 "106(10uF)" H 3300 8850 50  0001 C CNN "Value"
	1    3300 8850
	0    -1   1    0   
$EndComp
Wire Wire Line
	3450 8850 3600 8850
Connection ~ 3600 8850
Wire Wire Line
	3150 8850 3000 8850
Connection ~ 3000 8850
Wire Wire Line
	3000 8850 3000 9000
$Comp
L Server_Used:C C7
U 1 1 60D91DE9
P 3500 2150
F 0 "C7" H 3615 2196 50  0000 L CNN
F 1 "104" H 3615 2105 50  0000 L CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 3538 2000 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 3500 2150 50  0001 C CNN
F 4 "66" H 3500 2150 50  0001 C CNN "Price"
F 5 "부품세상" H 3500 2150 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 3500 2150 50  0001 C CNN "Package"
F 7 "106(10uF)" H 3500 2150 50  0001 C CNN "Value"
	1    3500 2150
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J4
U 1 1 60B9B101
P 1600 3700
F 0 "J4" H 1700 3800 50  0000 C CNN
F 1 "-12V" H 1450 3700 50  0000 C CNN
F 2 "Server.pretty:BR-120" H 1600 3700 50  0001 C CNN
F 3 "https://www.devicemart.co.kr/goods/view?no=1232631" H 1600 3700 50  0001 C CNN
F 4 "360" H 1600 3700 50  0001 C CNN "Price"
F 5 "DEVICEMART" H 1600 3700 50  0001 C CNN "Manufacture"
F 6 "BR-120" H 1600 3700 50  0001 C CNN "Value"
	1    1600 3700
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x01_Male J3
U 1 1 60B9B0FA
P 1600 3450
F 0 "J3" H 1700 3550 50  0000 C CNN
F 1 "+12V" H 1450 3450 50  0000 C CNN
F 2 "Server.pretty:BR-120" H 1600 3450 50  0001 C CNN
F 3 "https://www.devicemart.co.kr/goods/view?no=1232631" H 1600 3450 50  0001 C CNN
F 4 "360" H 1600 3450 50  0001 C CNN "Price"
F 5 "DEVICEMART" H 1600 3450 50  0001 C CNN "Manufacture"
F 6 "BR-120" H 1600 3450 50  0001 C CNN "Value"
	1    1600 3450
	1    0    0    -1  
$EndComp
$Comp
L Server_Used:C C8
U 1 1 60DBE72F
P 3850 2150
F 0 "C8" H 3965 2196 50  0000 L CNN
F 1 "104" H 3965 2105 50  0000 L CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 3888 2000 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 3850 2150 50  0001 C CNN
F 4 "66" H 3850 2150 50  0001 C CNN "Price"
F 5 "부품세상" H 3850 2150 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 3850 2150 50  0001 C CNN "Package"
F 7 "106(10uF)" H 3850 2150 50  0001 C CNN "Value"
	1    3850 2150
	1    0    0    -1  
$EndComp
$Comp
L Server_Used:C C9
U 1 1 60DBEB1C
P 4200 2150
F 0 "C9" H 4315 2196 50  0000 L CNN
F 1 "104" H 4315 2105 50  0000 L CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 4238 2000 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 4200 2150 50  0001 C CNN
F 4 "66" H 4200 2150 50  0001 C CNN "Price"
F 5 "부품세상" H 4200 2150 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 4200 2150 50  0001 C CNN "Package"
F 7 "106(10uF)" H 4200 2150 50  0001 C CNN "Value"
	1    4200 2150
	1    0    0    -1  
$EndComp
$Comp
L Server_Used:C C10
U 1 1 60DBF071
P 4550 2150
F 0 "C10" H 4665 2196 50  0000 L CNN
F 1 "104" H 4665 2105 50  0000 L CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 4588 2000 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 4550 2150 50  0001 C CNN
F 4 "66" H 4550 2150 50  0001 C CNN "Price"
F 5 "부품세상" H 4550 2150 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 4550 2150 50  0001 C CNN "Package"
F 7 "106(10uF)" H 4550 2150 50  0001 C CNN "Value"
	1    4550 2150
	1    0    0    -1  
$EndComp
$Comp
L Server_Used:C C11
U 1 1 60DBF3E9
P 4900 2150
F 0 "C11" H 5015 2196 50  0000 L CNN
F 1 "104" H 5015 2105 50  0000 L CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 4938 2000 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 4900 2150 50  0001 C CNN
F 4 "66" H 4900 2150 50  0001 C CNN "Price"
F 5 "부품세상" H 4900 2150 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 4900 2150 50  0001 C CNN "Package"
F 7 "106(10uF)" H 4900 2150 50  0001 C CNN "Value"
	1    4900 2150
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR0111
U 1 1 60DD6809
P 3500 2400
AR Path="/60DD6809" Ref="#PWR0111"  Part="1" 
AR Path="/5E3D0683/60DD6809" Ref="#PWR?"  Part="1" 
F 0 "#PWR0111" H 3500 2200 50  0001 C CNN
F 1 "GNDPWR" H 3504 2246 50  0000 C CNN
F 2 "" H 3500 2350 50  0001 C CNN
F 3 "" H 3500 2350 50  0001 C CNN
	1    3500 2400
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR0115
U 1 1 60DD6C3E
P 3850 2400
AR Path="/60DD6C3E" Ref="#PWR0115"  Part="1" 
AR Path="/5E3D0683/60DD6C3E" Ref="#PWR?"  Part="1" 
F 0 "#PWR0115" H 3850 2200 50  0001 C CNN
F 1 "GNDPWR" H 3854 2246 50  0000 C CNN
F 2 "" H 3850 2350 50  0001 C CNN
F 3 "" H 3850 2350 50  0001 C CNN
	1    3850 2400
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR0117
U 1 1 60DD6F7B
P 4200 2400
AR Path="/60DD6F7B" Ref="#PWR0117"  Part="1" 
AR Path="/5E3D0683/60DD6F7B" Ref="#PWR?"  Part="1" 
F 0 "#PWR0117" H 4200 2200 50  0001 C CNN
F 1 "GNDPWR" H 4204 2246 50  0000 C CNN
F 2 "" H 4200 2350 50  0001 C CNN
F 3 "" H 4200 2350 50  0001 C CNN
	1    4200 2400
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR0119
U 1 1 60DD71E7
P 4550 2400
AR Path="/60DD71E7" Ref="#PWR0119"  Part="1" 
AR Path="/5E3D0683/60DD71E7" Ref="#PWR?"  Part="1" 
F 0 "#PWR0119" H 4550 2200 50  0001 C CNN
F 1 "GNDPWR" H 4554 2246 50  0000 C CNN
F 2 "" H 4550 2350 50  0001 C CNN
F 3 "" H 4550 2350 50  0001 C CNN
	1    4550 2400
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR0121
U 1 1 60DD74A1
P 4900 2400
AR Path="/60DD74A1" Ref="#PWR0121"  Part="1" 
AR Path="/5E3D0683/60DD74A1" Ref="#PWR?"  Part="1" 
F 0 "#PWR0121" H 4900 2200 50  0001 C CNN
F 1 "GNDPWR" H 4904 2246 50  0000 C CNN
F 2 "" H 4900 2350 50  0001 C CNN
F 3 "" H 4900 2350 50  0001 C CNN
	1    4900 2400
	1    0    0    -1  
$EndComp
$Comp
L power:+12P #PWR0123
U 1 1 60DD78B8
P 4200 1900
F 0 "#PWR0123" H 4200 1750 50  0001 C CNN
F 1 "+12P" H 4215 2073 50  0000 C CNN
F 2 "" H 4200 1900 50  0001 C CNN
F 3 "" H 4200 1900 50  0001 C CNN
	1    4200 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 2000 3500 1950
Wire Wire Line
	4900 1950 4900 2000
Wire Wire Line
	4550 2000 4550 1950
Connection ~ 4550 1950
Wire Wire Line
	4550 1950 4900 1950
Wire Wire Line
	4200 1900 4200 1950
Wire Wire Line
	3500 1950 3850 1950
Wire Wire Line
	3850 2000 3850 1950
Connection ~ 3850 1950
Wire Wire Line
	3850 1950 4200 1950
Wire Wire Line
	3500 2400 3500 2300
Wire Wire Line
	4900 2400 4900 2300
Wire Wire Line
	4550 2300 4550 2400
Wire Wire Line
	4200 2400 4200 2300
Wire Wire Line
	3850 2300 3850 2400
Connection ~ 4200 1950
Wire Wire Line
	4200 1950 4200 2000
Wire Wire Line
	4200 1950 4550 1950
$Comp
L power:+5V #PWR0124
U 1 1 60D4A003
P 6500 5000
F 0 "#PWR0124" H 6500 4850 50  0001 C CNN
F 1 "+5V" H 6515 5173 50  0000 C CNN
F 2 "" H 6500 5000 50  0001 C CNN
F 3 "" H 6500 5000 50  0001 C CNN
	1    6500 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 5000 6500 5200
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R2
U 1 1 60D8EB44
P 6000 8450
F 0 "R2" V 5900 8300 50  0000 C CNN
F 1 "202F" V 6000 8300 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 5930 8450 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 6000 8450 50  0001 C CNN
F 4 "20" H 6000 8450 50  0001 C CNN "Price"
F 5 "부품세상" H 6000 8450 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 6000 8450 50  0001 C CNN "Package"
F 7 "10KΩ" H 6000 8450 50  0001 C CNN "Value"
	1    6000 8450
	1    0    0    1   
$EndComp
Wire Wire Line
	6000 8150 6000 7750
Wire Wire Line
	6000 7350 6000 7750
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R3
U 1 1 60EFDCEE
P 6000 7350
F 0 "R3" V 5900 7200 50  0000 C CNN
F 1 "102F" V 6000 7200 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 5930 7350 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 6000 7350 50  0001 C CNN
F 4 "20" H 6000 7350 50  0001 C CNN "Price"
F 5 "부품세상" H 6000 7350 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 6000 7350 50  0001 C CNN "Package"
F 7 "10KΩ" H 6000 7350 50  0001 C CNN "Value"
	1    6000 7350
	1    0    0    1   
$EndComp
$Comp
L power:GNDPWR #PWR0125
U 1 1 60F4C555
P 6000 8800
AR Path="/60F4C555" Ref="#PWR0125"  Part="1" 
AR Path="/5E3D0683/60F4C555" Ref="#PWR?"  Part="1" 
F 0 "#PWR0125" H 6000 8600 50  0001 C CNN
F 1 "GNDPWR" H 6004 8646 50  0000 C CNN
F 2 "" H 6000 8750 50  0001 C CNN
F 3 "" H 6000 8750 50  0001 C CNN
	1    6000 8800
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 8800 6000 8450
Wire Wire Line
	2850 3450 2850 3300
NoConn ~ 4150 3400
Connection ~ 2850 3300
$Comp
L Server_Used:MP1584 MO1
U 1 1 61987B57
P 5150 2200
F 0 "MO1" H 5878 2095 60  0000 L CNN
F 1 "MP1584" H 5878 1989 60  0000 L CNN
F 2 "Server.pretty:MP1584" H 5150 2200 60  0001 C CNN
F 3 "" H 5150 2200 60  0001 C CNN
	1    5150 2200
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0120
U 1 1 61988143
P 3750 9200
F 0 "#PWR0120" H 3750 9050 50  0001 C CNN
F 1 "+5V" H 3765 9373 50  0000 C CNN
F 2 "" H 3750 9200 50  0001 C CNN
F 3 "" H 3750 9200 50  0001 C CNN
	1    3750 9200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3600 8850 3600 9300
Wire Wire Line
	3750 9200 3750 9300
Wire Wire Line
	3750 9300 3600 9300
Connection ~ 3600 9300
Wire Wire Line
	3600 9300 3600 9400
Wire Wire Line
	4950 2750 4950 2900
$Comp
L power:GNDPWR #PWR0133
U 1 1 61AAD6AF
P 5450 3350
AR Path="/61AAD6AF" Ref="#PWR0133"  Part="1" 
AR Path="/5E3D0683/61AAD6AF" Ref="#PWR?"  Part="1" 
F 0 "#PWR0133" H 5450 3150 50  0001 C CNN
F 1 "GNDPWR" H 5454 3196 50  0000 C CNN
F 2 "" H 5450 3300 50  0001 C CNN
F 3 "" H 5450 3300 50  0001 C CNN
	1    5450 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5450 2600 5450 3000
Wire Wire Line
	5550 2750 5550 2600
Wire Wire Line
	4950 2750 5550 2750
Wire Wire Line
	5750 2600 5750 2750
Wire Wire Line
	5750 2750 6250 2750
Wire Wire Line
	5650 2600 5650 3000
Wire Wire Line
	5650 3000 5450 3000
Connection ~ 5450 3000
Wire Wire Line
	5450 3000 5450 3350
Wire Wire Line
	6400 5500 6400 6000
Wire Wire Line
	6650 5300 6650 5800
Connection ~ 3250 10400
Wire Wire Line
	3400 4000 3400 4500
$Comp
L Simulation_SPICE:DIODE D?
U 1 1 615686E8
P 4750 4300
AR Path="/5E3D0683/615686E8" Ref="D?"  Part="1" 
AR Path="/615686E8" Ref="D3"  Part="1" 
F 0 "D3" H 4750 4500 50  0000 C CNN
F 1 "M7" H 4750 4400 50  0000 C CNN
F 2 "Server.pretty:D_SMA" H 4750 4300 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000005570" H 4750 4300 50  0001 C CNN
F 4 "Y" H 4750 4300 50  0001 L CNN "Spice_Netlist_Enabled"
F 5 "D" H 4750 4300 50  0001 L CNN "Spice_Primitive"
F 6 "부품세상" H 4750 4300 50  0001 C CNN "Manufacture"
F 7 "DO-214AC(SMA)" H 4750 4300 50  0001 C CNN "Package"
F 8 "55" H 4750 4300 50  0001 C CNN "Price"
F 9 "4007" H 4750 4300 50  0001 C CNN "Value"
	1    4750 4300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4600 2750 4950 2750
$Comp
L CamTrol-12ChOne-Board-cache:AZ1117-3.3 U2
U 1 1 615A5996
P 4600 9950
F 0 "U2" H 4600 10192 50  0000 C CNN
F 1 "AZ1117-3.3" H 4600 10101 50  0000 C CNN
F 2 "TO_SOT_Packages_SMD.pretty:SOT-223" H 4600 10200 50  0001 C CIN
F 3 "" H 4600 9950 50  0001 C CNN
	1    4600 9950
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4900 9950 5050 9950
$Comp
L Server_Used:C C12
U 1 1 615A7031
P 2900 9700
F 0 "C12" V 3150 9700 50  0000 C CNN
F 1 "106" V 3050 9700 50  0000 C CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 2938 9550 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 2900 9700 50  0001 C CNN
F 4 "66" H 2900 9700 50  0001 C CNN "Price"
F 5 "부품세상" H 2900 9700 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 2900 9700 50  0001 C CNN "Package"
F 7 "106(10uF)" H 2900 9700 50  0001 C CNN "Value"
	1    2900 9700
	0    -1   -1   0   
$EndComp
$Comp
L Server_Used:3296Y ADC_5V1
U 1 1 615A7C21
P 4000 10250
F 0 "ADC_5V1" V 3800 10250 50  0000 C CNN
F 1 "202" V 3900 10250 50  0000 C CNN
F 2 "Server.pretty:Potentiometer_Bourns_3296Y_Vertical" H 4000 10250 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013040" H 4000 10250 50  0001 C CNN
F 4 "710" H 4000 10250 50  0001 C CNN "Price"
F 5 "B3296Y-2KΩ" H 4000 10250 50  0001 C CNN "Value"
F 6 "부품세상" H 4000 10250 50  0001 C CNN "Manufacture"
	1    4000 10250
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDPWR #PWR0134
U 1 1 615A9BC1
P 4600 10900
AR Path="/615A9BC1" Ref="#PWR0134"  Part="1" 
AR Path="/5E3D0683/615A9BC1" Ref="#PWR?"  Part="1" 
F 0 "#PWR0134" H 4600 10700 50  0001 C CNN
F 1 "GNDPWR" H 4604 10746 50  0000 C CNN
F 2 "" H 4600 10850 50  0001 C CNN
F 3 "" H 4600 10850 50  0001 C CNN
	1    4600 10900
	1    0    0    -1  
$EndComp
NoConn ~ 5000 8850
Wire Wire Line
	4250 4100 4250 4000
Wire Wire Line
	6650 5300 6950 5300
Connection ~ 6650 5800
Wire Wire Line
	6650 5800 6650 6100
Wire Wire Line
	6500 5400 6500 5900
Connection ~ 6500 5900
Wire Wire Line
	6500 5900 6500 7550
Wire Wire Line
	8500 6000 7950 6000
Connection ~ 6400 6000
Wire Wire Line
	8500 6100 6650 6100
Connection ~ 6650 6100
Wire Wire Line
	6650 6100 6650 6450
NoConn ~ 8500 6200
Wire Wire Line
	1500 6500 1500 6550
Wire Wire Line
	1500 6550 1500 6650
Connection ~ 1500 6550
$Comp
L power:GNDPWR #PWR0135
U 1 1 6154A5BA
P 1850 6650
AR Path="/6154A5BA" Ref="#PWR0135"  Part="1" 
AR Path="/5E3D0683/6154A5BA" Ref="#PWR?"  Part="1" 
F 0 "#PWR0135" H 1850 6450 50  0001 C CNN
F 1 "GNDPWR" H 1854 6496 50  0000 C CNN
F 2 "" H 1850 6600 50  0001 C CNN
F 3 "" H 1850 6600 50  0001 C CNN
	1    1850 6650
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR0137
U 1 1 6154A5C0
P 1500 6650
AR Path="/6154A5C0" Ref="#PWR0137"  Part="1" 
AR Path="/5E3D0683/6154A5C0" Ref="#PWR?"  Part="1" 
F 0 "#PWR0137" H 1500 6450 50  0001 C CNN
F 1 "GNDPWR" H 1504 6496 50  0000 C CNN
F 2 "" H 1500 6600 50  0001 C CNN
F 3 "" H 1500 6600 50  0001 C CNN
	1    1500 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 6550 1500 6550
Wire Wire Line
	1700 6350 1700 6550
Wire Wire Line
	1650 6350 1700 6350
Wire Wire Line
	1500 6200 1500 6100
Wire Wire Line
	1850 6650 1850 6350
$Comp
L Server_Used:C C13
U 1 1 6154A5D3
P 1850 6200
F 0 "C13" H 1965 6246 50  0000 L CNN
F 1 "106" H 1965 6155 50  0000 L CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 1888 6050 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 1850 6200 50  0001 C CNN
F 4 "66" H 1850 6200 50  0001 C CNN "Price"
F 5 "부품세상" H 1850 6200 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 1850 6200 50  0001 C CNN "Package"
F 7 "106(10uF)" H 1850 6200 50  0001 C CNN "Value"
	1    1850 6200
	1    0    0    -1  
$EndComp
$Comp
L Server_Used:3296Y MAIN_V1
U 1 1 6154A5DC
P 1500 6350
F 0 "MAIN_V1" H 1432 6396 50  0000 R CNN
F 1 "202" H 1432 6305 50  0000 R CNN
F 2 "Server.pretty:Potentiometer_Bourns_3296Y_Vertical" H 1500 6350 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013040" H 1500 6350 50  0001 C CNN
F 4 "710" H 1500 6350 50  0001 C CNN "Price"
F 5 "B3296Y-2KΩ" H 1500 6350 50  0001 C CNN "Value"
F 6 "부품세상" H 1500 6350 50  0001 C CNN "Manufacture"
	1    1500 6350
	1    0    0    1   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R4
U 1 1 6154A5E6
P 1400 5700
F 0 "R4" V 1300 5550 50  0000 C CNN
F 1 "103" V 1400 5550 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 1330 5700 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 1400 5700 50  0001 C CNN
F 4 "20" H 1400 5700 50  0001 C CNN "Price"
F 5 "부품세상" H 1400 5700 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 1400 5700 50  0001 C CNN "Package"
F 7 "10KΩ" H 1400 5700 50  0001 C CNN "Value"
	1    1400 5700
	0    1    1    0   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R6
U 1 1 6154A5F0
P 1500 6100
F 0 "R6" V 1400 5950 50  0000 C CNN
F 1 "102" V 1500 5950 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 1430 6100 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 1500 6100 50  0001 C CNN
F 4 "20" H 1500 6100 50  0001 C CNN "Price"
F 5 "부품세상" H 1500 6100 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 1500 6100 50  0001 C CNN "Package"
F 7 "10KΩ" H 1500 6100 50  0001 C CNN "Value"
	1    1500 6100
	-1   0    0    1   
$EndComp
Wire Wire Line
	1400 5700 1500 5700
Wire Wire Line
	1500 5800 1500 5700
Connection ~ 1500 5700
Wire Wire Line
	1500 5700 1850 5700
Wire Wire Line
	1850 5700 1850 6050
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R22
U 1 1 615C792D
P 1900 5500
F 0 "R22" V 1800 5350 50  0000 C CNN
F 1 "103F" V 1900 5350 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 1830 5500 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 1900 5500 50  0001 C CNN
F 4 "20" H 1900 5500 50  0001 C CNN "Price"
F 5 "부품세상" H 1900 5500 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 1900 5500 50  0001 C CNN "Package"
F 7 "10KΩ" H 1900 5500 50  0001 C CNN "Value"
	1    1900 5500
	0    1    1    0   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R23
U 1 1 615DEB57
P 4600 10450
F 0 "R23" V 4700 10300 50  0000 C CNN
F 1 "120F" V 4600 10300 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 4530 10450 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 4600 10450 50  0001 C CNN
F 4 "20" H 4600 10450 50  0001 C CNN "Price"
F 5 "부품세상" H 4600 10450 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 4600 10450 50  0001 C CNN "Package"
F 7 "10KΩ" H 4600 10450 50  0001 C CNN "Value"
	1    4600 10450
	1    0    0    -1  
$EndComp
Connection ~ 4250 4000
Wire Wire Line
	6500 5200 6950 5200
Wire Wire Line
	4600 10900 4600 10750
Wire Wire Line
	4600 10250 4600 10350
$Comp
L Connector:TestPoint_Small TP1
U 1 1 61764C33
P 2550 9950
F 0 "TP1" H 2500 9900 50  0000 L TNN
F 1 "ADC 5V" H 2500 9800 50  0000 L CNN
F 2 "Measurement_Points.pretty:Test_Point_Keystone_5000-5004_Miniature" H 2750 9950 50  0001 C CNN
F 3 "~" H 2750 9950 50  0001 C CNN
	1    2550 9950
	1    0    0    -1  
$EndComp
Connection ~ 2550 9950
Wire Wire Line
	4150 10250 4300 10250
Wire Wire Line
	4300 10250 4300 10350
Wire Wire Line
	4300 10350 4600 10350
Connection ~ 4600 10350
Wire Wire Line
	4600 10350 4600 10450
Wire Wire Line
	4000 10100 4000 10050
Wire Wire Line
	4000 10050 4300 10050
Wire Wire Line
	4300 10050 4300 10250
Connection ~ 4300 10250
Wire Wire Line
	3850 10250 3650 10250
Wire Wire Line
	3650 10250 3650 9950
Connection ~ 3650 9950
Wire Wire Line
	3650 9950 4300 9950
$Comp
L power:GNDPWR #PWR0138
U 1 1 617A83FD
P 3200 9700
AR Path="/617A83FD" Ref="#PWR0138"  Part="1" 
AR Path="/5E3D0683/617A83FD" Ref="#PWR?"  Part="1" 
F 0 "#PWR0138" H 3200 9500 50  0001 C CNN
F 1 "GNDPWR" H 3204 9546 50  0000 C CNN
F 2 "" H 3200 9650 50  0001 C CNN
F 3 "" H 3200 9650 50  0001 C CNN
	1    3200 9700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 9700 3050 9700
Wire Wire Line
	2750 9700 2550 9700
Text GLabel 3150 10100 2    50   Input ~ 0
ADC_5V
Wire Wire Line
	3150 10100 2950 10100
Wire Wire Line
	2950 10100 2950 9950
Connection ~ 2950 9950
Wire Wire Line
	2950 9950 3650 9950
Text GLabel 8300 5700 0    50   Input ~ 0
ADC_5V
Wire Wire Line
	8500 5700 8300 5700
Connection ~ 6000 7750
Wire Wire Line
	5500 6400 5500 6700
Wire Wire Line
	5500 6700 5650 6700
Wire Wire Line
	5650 6700 5650 8150
Wire Wire Line
	5650 8150 5000 8150
Text GLabel 5000 8550 2    50   Input ~ 0
WiFi
$Comp
L power:GNDPWR #PWR0139
U 1 1 618844F2
P 7400 4250
AR Path="/618844F2" Ref="#PWR0139"  Part="1" 
AR Path="/5E3D0683/618844F2" Ref="#PWR?"  Part="1" 
F 0 "#PWR0139" H 7400 4050 50  0001 C CNN
F 1 "GNDPWR" H 7404 4096 50  0000 C CNN
F 2 "" H 7400 4200 50  0001 C CNN
F 3 "" H 7400 4200 50  0001 C CNN
	1    7400 4250
	1    0    0    -1  
$EndComp
Wire Wire Line
	7400 4250 7400 4200
Wire Wire Line
	7400 3900 7400 3850
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R36
U 1 1 61884505
P 7400 3850
AR Path="/61884505" Ref="R36"  Part="1" 
AR Path="/5E3D0683/61884505" Ref="R?"  Part="3" 
F 0 "R36" H 7350 3650 50  0000 R CNN
F 1 "103F" H 7350 3750 50  0000 R CNN
F 2 "Server.pretty:R_0805_HandSoldering" H 7400 3780 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" V 7400 3850 50  0001 C CNN
F 4 "20" H 7400 3850 50  0001 C CNN "Price"
F 5 "부품세상" H 7400 3850 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 7400 3850 50  0001 C CNN "Package"
F 7 "10KΩ" H 7400 3850 50  0001 C CNN "Value"
	1    7400 3850
	-1   0    0    1   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:LED-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue D5
U 1 1 6188450F
P 7400 4050
AR Path="/6188450F" Ref="D5"  Part="1" 
AR Path="/5E3D0683/6188450F" Ref="D?"  Part="1" 
F 0 "D5" V 7439 3932 50  0000 R CNN
F 1 "WiFi" V 7348 3932 50  0000 R CNN
F 2 "Server.pretty:LED_0805" H 7400 4050 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000008652" H 7400 4050 50  0001 C CNN
F 4 "132" H 7400 4050 50  0001 C CNN "Price"
F 5 "부품세상" H 7400 4050 50  0001 C CNN "Manufacture"
F 6 "RED LED" H 7400 4050 50  0001 C CNN "Value"
F 7 "2012(0805)" H 7400 4050 50  0001 C CNN "Package"
	1    7400 4050
	0    -1   -1   0   
$EndComp
Text GLabel 7500 3200 2    50   Input ~ 0
WiFi
Wire Wire Line
	7400 3200 7500 3200
Wire Wire Line
	7400 3200 7400 3550
Text GLabel 2800 7150 2    50   Input ~ 0
AUX-V
Wire Wire Line
	2300 7150 2200 7150
Connection ~ 2200 7150
Text GLabel 2000 5500 2    50   Input ~ 0
MAIN_DC
Wire Wire Line
	2000 5500 1950 5500
Wire Wire Line
	2200 7150 2200 7500
Wire Wire Line
	1600 5500 1500 5500
Text GLabel 8400 6400 0    50   Input ~ 0
MAIN_DC
Text GLabel 8400 6300 0    50   Input ~ 0
AUX-V
Text GLabel 2750 8450 2    50   Input ~ 0
CLEAN
Wire Wire Line
	2400 9450 2650 9450
Text GLabel 8400 6500 0    50   Input ~ 0
CLEAN
Text GLabel 8400 6600 0    50   Input ~ 0
WASTE
Wire Wire Line
	8400 6300 8500 6300
Wire Wire Line
	8500 6400 8400 6400
Wire Wire Line
	8400 6500 8500 6500
Wire Wire Line
	8500 6600 8400 6600
Wire Wire Line
	1000 5700 1100 5700
Wire Wire Line
	2850 3300 3150 3300
Connection ~ 3150 4300
Wire Wire Line
	3150 4300 4600 4300
Wire Wire Line
	3150 3400 3150 3300
Connection ~ 3150 3300
Wire Wire Line
	3150 3300 3550 3300
$Comp
L Server_Used:ADS1115 MO2
U 1 1 61401D4B
P 8500 5900
F 0 "MO2" H 8700 5050 60  0000 L CNN
F 1 "ADS1115" H 8700 4950 60  0000 L CNN
F 2 "Server.pretty:ADS1115" H 8500 5900 60  0001 C CNN
F 3 "" H 8500 5900 60  0001 C CNN
	1    8500 5900
	1    0    0    -1  
$EndComp
$Comp
L Server_Used:AHT10 U5
U 1 1 60BE9677
P 7150 5200
F 0 "U5" H 6872 5096 50  0000 R CNN
F 1 "AHT10" H 6872 5005 50  0000 R CNN
F 2 "Server.pretty:AHT10" H 6900 4550 50  0001 L BNN
F 3 "4" H 7250 4650 50  0001 L BNN
	1    7150 5200
	-1   0    0    -1  
$EndComp
Text Notes 8700 7000 0    50   ~ 0
ADDRESS : 0x48
$Comp
L Switch:SW_Push SW1
U 1 1 620BEAE8
P 5050 10400
F 0 "SW1" V 5004 10352 50  0000 R CNN
F 1 "FACTORY" V 5095 10352 50  0000 R CNN
F 2 "Button_Switch_SMD:SW_SPST_CK_RS282G05A3" H 5050 10600 50  0001 C CNN
F 3 "~" H 5050 10600 50  0001 C CNN
	1    5050 10400
	0    -1   1    0   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R37
U 1 1 620C123D
P 5350 9900
F 0 "R37" V 5250 9750 50  0000 C CNN
F 1 "103F" V 5350 9750 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 5280 9900 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 5350 9900 50  0001 C CNN
F 4 "20" H 5350 9900 50  0001 C CNN "Price"
F 5 "부품세상" H 5350 9900 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 5350 9900 50  0001 C CNN "Package"
F 7 "10KΩ" H 5350 9900 50  0001 C CNN "Value"
	1    5350 9900
	-1   0    0    1   
$EndComp
$Comp
L Server_Used:C C14
U 1 1 620C187E
P 5550 10400
F 0 "C14" H 5665 10446 50  0000 L CNN
F 1 "106" H 5665 10355 50  0000 L CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 5588 10250 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 5550 10400 50  0001 C CNN
F 4 "66" H 5550 10400 50  0001 C CNN "Price"
F 5 "부품세상" H 5550 10400 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 5550 10400 50  0001 C CNN "Package"
F 7 "106(10uF)" H 5550 10400 50  0001 C CNN "Value"
	1    5550 10400
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR0140
U 1 1 62120E62
P 5350 9600
F 0 "#PWR0140" H 5350 9450 50  0001 C CNN
F 1 "+5V" H 5365 9773 50  0000 C CNN
F 2 "" H 5350 9600 50  0001 C CNN
F 3 "" H 5350 9600 50  0001 C CNN
	1    5350 9600
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR0141
U 1 1 62120F9F
P 5350 10900
AR Path="/62120F9F" Ref="#PWR0141"  Part="1" 
AR Path="/5E3D0683/62120F9F" Ref="#PWR?"  Part="1" 
F 0 "#PWR0141" H 5350 10700 50  0001 C CNN
F 1 "GNDPWR" H 5354 10746 50  0000 C CNN
F 2 "" H 5350 10850 50  0001 C CNN
F 3 "" H 5350 10850 50  0001 C CNN
	1    5350 10900
	1    0    0    -1  
$EndComp
Text GLabel 5500 10000 2    50   Input ~ 0
FACTORY
Wire Wire Line
	5500 10000 5350 10000
Connection ~ 5350 10000
Wire Wire Line
	5350 10000 5350 9900
Wire Wire Line
	5050 10200 5050 10150
Wire Wire Line
	5050 10150 5350 10150
Wire Wire Line
	5550 10150 5550 10250
Wire Wire Line
	5350 10000 5350 10150
Connection ~ 5350 10150
Wire Wire Line
	5350 10150 5550 10150
Wire Wire Line
	5050 10600 5050 10750
Wire Wire Line
	5050 10750 5350 10750
Wire Wire Line
	5550 10750 5550 10550
Wire Wire Line
	5350 10900 5350 10750
Connection ~ 5350 10750
Wire Wire Line
	5350 10750 5550 10750
Text GLabel 3700 7850 0    50   Input ~ 0
FACTORY
$Comp
L Connector:Conn_01x02_Male J2
U 1 1 61556596
P 950 5400
F 0 "J2" H 1050 5150 50  0000 C CNN
F 1 "VOLT" H 800 5350 50  0001 C CNN
F 2 "Server.pretty:PhoenixContact_MCV_1,5_2-G-3.81_1x02_P3.81mm_Vertical" H 950 5400 50  0001 C CNN
F 3 "https://www.devicemart.co.kr/goods/view?no=23260&market=naver&NaPm=ct%3Dkof8h100%7Cci%3D2b06ad3968d8adec0b2b30ae50256e40450772f8%7Ctr%3Dslsc%7Csn%3D876973%7Chk%3Dcbed85e5f251313af0535f600d98c0e674ebf3d9" H 950 5400 50  0001 C CNN
F 4 "DEVICEMART" H 950 5400 50  0001 C CNN "Manufacture"
F 5 "100" H 950 5400 50  0001 C CNN "Price"
F 6 "ECH381V-2P" H 950 5400 50  0001 C CNN "Value"
	1    950  5400
	1    0    0    1   
$EndComp
Wire Wire Line
	1000 5700 1000 5500
Wire Wire Line
	1000 5500 1300 5500
Wire Wire Line
	1300 5500 1300 5400
Wire Wire Line
	1300 5400 1150 5400
Wire Wire Line
	1500 5500 1500 5700
Wire Wire Line
	2500 6500 2500 6550
Wire Wire Line
	2500 6550 2500 6650
Connection ~ 2500 6550
$Comp
L power:GNDPWR #PWR0142
U 1 1 61606C40
P 2850 6650
AR Path="/61606C40" Ref="#PWR0142"  Part="1" 
AR Path="/5E3D0683/61606C40" Ref="#PWR?"  Part="1" 
F 0 "#PWR0142" H 2850 6450 50  0001 C CNN
F 1 "GNDPWR" H 2854 6496 50  0000 C CNN
F 2 "" H 2850 6600 50  0001 C CNN
F 3 "" H 2850 6600 50  0001 C CNN
	1    2850 6650
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR0143
U 1 1 61606C46
P 2500 6650
AR Path="/61606C46" Ref="#PWR0143"  Part="1" 
AR Path="/5E3D0683/61606C46" Ref="#PWR?"  Part="1" 
F 0 "#PWR0143" H 2500 6450 50  0001 C CNN
F 1 "GNDPWR" H 2504 6496 50  0000 C CNN
F 2 "" H 2500 6600 50  0001 C CNN
F 3 "" H 2500 6600 50  0001 C CNN
	1    2500 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 6550 2500 6550
Wire Wire Line
	2700 6350 2700 6550
Wire Wire Line
	2650 6350 2700 6350
Wire Wire Line
	2500 6200 2500 6100
Wire Wire Line
	2850 6650 2850 6350
$Comp
L Server_Used:C C15
U 1 1 61606C55
P 2850 6200
F 0 "C15" H 2965 6246 50  0000 L CNN
F 1 "106" H 2965 6155 50  0000 L CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 2888 6050 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 2850 6200 50  0001 C CNN
F 4 "66" H 2850 6200 50  0001 C CNN "Price"
F 5 "부품세상" H 2850 6200 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 2850 6200 50  0001 C CNN "Package"
F 7 "106(10uF)" H 2850 6200 50  0001 C CNN "Value"
	1    2850 6200
	1    0    0    -1  
$EndComp
$Comp
L Server_Used:3296Y SOLAR_V1
U 1 1 61606C5E
P 2500 6350
F 0 "SOLAR_V1" H 2432 6396 50  0000 R CNN
F 1 "202" H 2432 6305 50  0000 R CNN
F 2 "Server.pretty:Potentiometer_Bourns_3296Y_Vertical" H 2500 6350 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013040" H 2500 6350 50  0001 C CNN
F 4 "710" H 2500 6350 50  0001 C CNN "Price"
F 5 "B3296Y-2KΩ" H 2500 6350 50  0001 C CNN "Value"
F 6 "부품세상" H 2500 6350 50  0001 C CNN "Manufacture"
	1    2500 6350
	1    0    0    1   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R38
U 1 1 61606C68
P 2500 5650
F 0 "R38" V 2400 5500 50  0000 C CNN
F 1 "103" V 2500 5500 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 2430 5650 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 2500 5650 50  0001 C CNN
F 4 "20" H 2500 5650 50  0001 C CNN "Price"
F 5 "부품세상" H 2500 5650 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 2500 5650 50  0001 C CNN "Package"
F 7 "10KΩ" H 2500 5650 50  0001 C CNN "Value"
	1    2500 5650
	-1   0    0    1   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R39
U 1 1 61606C72
P 2500 6100
F 0 "R39" V 2400 5950 50  0000 C CNN
F 1 "102" V 2500 5950 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 2430 6100 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 2500 6100 50  0001 C CNN
F 4 "20" H 2500 6100 50  0001 C CNN "Price"
F 5 "부품세상" H 2500 6100 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 2500 6100 50  0001 C CNN "Package"
F 7 "10KΩ" H 2500 6100 50  0001 C CNN "Value"
	1    2500 6100
	-1   0    0    1   
$EndComp
Wire Wire Line
	2500 5800 2500 5700
Wire Wire Line
	2850 5700 2850 6050
Wire Wire Line
	2500 5700 2500 5650
Connection ~ 2500 5700
Wire Wire Line
	1150 5300 2500 5300
Wire Wire Line
	2500 5300 2500 5350
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R40
U 1 1 616AD503
P 3250 5700
F 0 "R40" V 3150 5550 50  0000 C CNN
F 1 "103F" V 3250 5550 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 3180 5700 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 3250 5700 50  0001 C CNN
F 4 "20" H 3250 5700 50  0001 C CNN "Price"
F 5 "부품세상" H 3250 5700 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 3250 5700 50  0001 C CNN "Package"
F 7 "10KΩ" H 3250 5700 50  0001 C CNN "Value"
	1    3250 5700
	0    1    1    0   
$EndComp
$Comp
L Connector:TestPoint_Small TP3
U 1 1 617D7DB3
P 1950 5500
F 0 "TP3" H 1900 5450 50  0000 L TNN
F 1 "MAIN_BT" H 1900 5350 50  0000 L CNN
F 2 "Measurement_Points.pretty:Test_Point_Keystone_5000-5004_Miniature" H 2150 5500 50  0001 C CNN
F 3 "~" H 2150 5500 50  0001 C CNN
	1    1950 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2400 8450 2750 8450
Wire Wire Line
	2000 9450 1450 9450
Connection ~ 2000 9450
Wire Wire Line
	2500 5700 2850 5700
Connection ~ 2850 5700
Wire Wire Line
	2850 5700 2950 5700
Text GLabel 3700 7950 0    50   Input ~ 0
SP_01
Text GLabel 3700 8050 0    50   Input ~ 0
SP_02
Wire Wire Line
	1800 3700 2100 3700
Wire Wire Line
	1800 3450 2700 3450
$Comp
L power:+5V #PWR0150
U 1 1 616065AD
P 3750 6050
F 0 "#PWR0150" H 3750 5900 50  0001 C CNN
F 1 "+5V" H 3765 6223 50  0000 C CNN
F 2 "" H 3750 6050 50  0001 C CNN
F 3 "" H 3750 6050 50  0001 C CNN
	1    3750 6050
	1    0    0    -1  
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R57
U 1 1 61606EB0
P 3750 6350
F 0 "R57" V 3650 6200 50  0000 C CNN
F 1 "103F" V 3750 6200 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 3680 6350 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 3750 6350 50  0001 C CNN
F 4 "20" H 3750 6350 50  0001 C CNN "Price"
F 5 "부품세상" H 3750 6350 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 3750 6350 50  0001 C CNN "Package"
F 7 "10KΩ" H 3750 6350 50  0001 C CNN "Value"
	1    3750 6350
	-1   0    0    1   
$EndComp
$Comp
L power:GNDPWR #PWR0151
U 1 1 6160896C
P 3200 6650
AR Path="/6160896C" Ref="#PWR0151"  Part="1" 
AR Path="/5E3D0683/6160896C" Ref="#PWR?"  Part="1" 
F 0 "#PWR0151" H 3200 6450 50  0001 C CNN
F 1 "GNDPWR" H 3204 6496 50  0000 C CNN
F 2 "" H 3200 6600 50  0001 C CNN
F 3 "" H 3200 6600 50  0001 C CNN
	1    3200 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 6350 3750 6500
Wire Wire Line
	3750 6500 3700 6500
Wire Wire Line
	3300 6500 3200 6500
Wire Wire Line
	3200 6500 3200 6650
$Comp
L Switch:SW_SPST SW3
U 1 1 6160A5C0
P 3500 6500
F 0 "SW3" H 3500 6735 50  0000 C CNN
F 1 "SW_SPST" H 3500 6644 50  0000 C CNN
F 2 "Server.pretty:NW3-SK-02" H 3500 6500 50  0001 C CNN
F 3 "~" H 3500 6500 50  0001 C CNN
	1    3500 6500
	-1   0    0    -1  
$EndComp
Connection ~ 2700 4400
Text GLabel 4450 2750 0    50   Input ~ 0
12V-IN
Wire Wire Line
	4450 2750 4600 2750
Connection ~ 4600 2750
Text GLabel 5150 9150 2    50   Input ~ 0
12V-IN
$Comp
L Server_Used:C C6
U 1 1 6174CA5E
P 4850 9400
F 0 "C6" H 5050 9350 50  0000 C CNN
F 1 "106" H 5050 9450 50  0000 C CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 4888 9250 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 4850 9400 50  0001 C CNN
F 4 "66" H 4850 9400 50  0001 C CNN "Price"
F 5 "부품세상" H 4850 9400 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 4850 9400 50  0001 C CNN "Package"
F 7 "106(10uF)" H 4850 9400 50  0001 C CNN "Value"
	1    4850 9400
	-1   0    0    1   
$EndComp
$Comp
L power:GNDPWR #PWR0136
U 1 1 6174D95E
P 4850 9550
AR Path="/6174D95E" Ref="#PWR0136"  Part="1" 
AR Path="/5E3D0683/6174D95E" Ref="#PWR?"  Part="1" 
F 0 "#PWR0136" H 4850 9350 50  0001 C CNN
F 1 "GNDPWR" H 4854 9396 50  0000 C CNN
F 2 "" H 4850 9500 50  0001 C CNN
F 3 "" H 4850 9500 50  0001 C CNN
	1    4850 9550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 9150 5150 9150
Wire Wire Line
	5050 9150 5050 9950
Wire Wire Line
	5050 9150 4850 9150
Wire Wire Line
	4850 9150 4850 9250
Connection ~ 5050 9150
$Comp
L Server_Used:2SC1815_SMD Q2
U 1 1 6181E8E0
P 4600 5650
AR Path="/6181E8E0" Ref="Q2"  Part="1" 
AR Path="/5E3D0683/6181E8E0" Ref="Q?"  Part="2" 
F 0 "Q2" H 4850 5700 50  0000 L CNN
F 1 "MMBTA42L" H 4850 5600 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD.pretty:SOT-23" H 4800 5650 50  0001 C CNN
F 3 "" H 4800 5650 50  0001 C CNN
F 4 "" H 4600 5650 50  0001 C CNN "Price"
	1    4600 5650
	1    0    0    -1  
$EndComp
$Comp
L Server_Used:2SC1815_SMD Q6
U 1 1 618201E2
P 5850 5650
AR Path="/618201E2" Ref="Q6"  Part="1" 
AR Path="/5E3D0683/618201E2" Ref="Q?"  Part="2" 
F 0 "Q6" H 6050 5700 50  0000 L CNN
F 1 "2SC1815" H 6050 5600 50  0000 L CNN
F 2 "TO_SOT_Packages_SMD.pretty:SOT-23" H 6050 5650 50  0001 C CNN
F 3 "" H 6050 5650 50  0001 C CNN
F 4 "" H 5850 5650 50  0001 C CNN "Price"
	1    5850 5650
	1    0    0    -1  
$EndComp
Text Notes 5750 5600 2    39   ~ 0
HIGH -> Relay ON
Wire Wire Line
	5950 5350 5950 5250
Wire Wire Line
	5500 5350 5950 5350
Wire Wire Line
	5500 5300 5500 5350
Connection ~ 5950 5350
Wire Wire Line
	5950 5450 5950 5350
Wire Wire Line
	5500 4950 5500 4800
Wire Wire Line
	5750 4950 5500 4950
Connection ~ 5500 4950
Wire Wire Line
	5500 5000 5500 4950
$Comp
L power:+12P #PWR0112
U 1 1 60C851E9
P 5500 4800
F 0 "#PWR0112" H 5500 4650 50  0001 C CNN
F 1 "+12P" H 5515 4973 50  0000 C CNN
F 2 "" H 5500 4800 50  0001 C CNN
F 3 "" H 5500 4800 50  0001 C CNN
	1    5500 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	3400 4500 5100 4500
$Comp
L Simulation_SPICE:DIODE D?
U 1 1 60D4DE66
P 5750 4500
AR Path="/5E3D0683/60D4DE66" Ref="D?"  Part="1" 
AR Path="/60D4DE66" Ref="D2"  Part="1" 
F 0 "D2" H 5750 4750 50  0000 C CNN
F 1 "M7" H 5750 4650 50  0000 C CNN
F 2 "Server.pretty:D_SMA" H 5750 4500 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000005570" H 5750 4500 50  0001 C CNN
F 4 "Y" H 5750 4500 50  0001 L CNN "Spice_Netlist_Enabled"
F 5 "D" H 5750 4500 50  0001 L CNN "Spice_Primitive"
F 6 "부품세상" H 5750 4500 50  0001 C CNN "Manufacture"
F 7 "DO-214AC(SMA)" H 5750 4500 50  0001 C CNN "Package"
F 8 "55" H 5750 4500 50  0001 C CNN "Price"
F 9 "4007" H 5750 4500 50  0001 C CNN "Value"
	1    5750 4500
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6300 4500 6300 4950
Wire Wire Line
	6300 4950 6150 4950
Wire Wire Line
	5900 4500 6300 4500
$Comp
L Server_Used:AOD4185 Q4
U 1 1 60C593F5
P 5950 5050
AR Path="/60C593F5" Ref="Q4"  Part="1" 
AR Path="/5E3D0683/60C593F5" Ref="Q?"  Part="1" 
F 0 "Q4" V 5900 5150 50  0000 L CNN
F 1 "AOD4185" V 6175 4875 50  0000 L CNN
F 2 "Server.pretty:AOD1485" H 6150 4975 50  0001 L CIN
F 3 "https://www.devicemart.co.kr/goods/view?no=6603223" H 5950 5050 50  0001 L CNN
F 4 "1050" H 5950 5050 50  0001 C CNN "Price"
F 5 "DEVICEMART" H 5950 5050 50  0001 C CNN "Manufacture"
F 6 "TO-252-3" H 5950 5050 50  0001 C CNN "Package"
F 7 "AOD4185" H 5950 5050 50  0001 C CNN "Value"
	1    5950 5050
	0    1    -1   0   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R14
U 1 1 60C6EA57
P 5500 5300
F 0 "R14" V 5400 5150 50  0000 C CNN
F 1 "103F" V 5500 5150 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 5430 5300 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 5500 5300 50  0001 C CNN
F 4 "20" H 5500 5300 50  0001 C CNN "Price"
F 5 "부품세상" H 5500 5300 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 5500 5300 50  0001 C CNN "Package"
F 7 "10KΩ" H 5500 5300 50  0001 C CNN "Value"
	1    5500 5300
	1    0    0    1   
$EndComp
Wire Wire Line
	4700 5450 4700 5300
Wire Wire Line
	4700 5300 4850 5300
Wire Wire Line
	3400 11200 3250 11200
Connection ~ 3250 11200
$Comp
L power:+12P #PWR0152
U 1 1 61C3F5AD
P 3250 10400
F 0 "#PWR0152" H 3250 10250 50  0001 C CNN
F 1 "+12P" H 3265 10573 50  0000 C CNN
F 2 "" H 3250 10400 50  0001 C CNN
F 3 "" H 3250 10400 50  0001 C CNN
	1    3250 10400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2700 4400 2700 4600
Text GLabel 2750 8600 2    50   Input ~ 0
WASTE
$Comp
L Server_Used:ADS1115 MO3
U 1 1 61F2651B
P 8500 4350
F 0 "MO3" H 8700 4900 60  0000 L CNN
F 1 "ADS1115" H 8700 4800 60  0000 L CNN
F 2 "Server.pretty:ADS1115" H 8500 4350 60  0001 C CNN
F 3 "" H 8500 4350 60  0001 C CNN
	1    8500 4350
	1    0    0    -1  
$EndComp
NoConn ~ 8500 4650
Text GLabel 3350 5700 2    50   Input ~ 0
SOLAR
Wire Wire Line
	3350 5700 3300 5700
Text GLabel 8300 4750 0    50   Input ~ 0
SOLAR
Wire Wire Line
	8500 4750 8300 4750
Wire Wire Line
	6650 5800 8500 5800
Wire Wire Line
	6500 5900 7900 5900
Wire Wire Line
	8500 4450 7950 4450
Wire Wire Line
	7950 4450 7950 6000
Connection ~ 7950 6000
Wire Wire Line
	7950 6000 6400 6000
Wire Wire Line
	8500 4350 7900 4350
Wire Wire Line
	7900 4350 7900 5900
Connection ~ 7900 5900
Wire Wire Line
	7900 5900 8500 5900
$Comp
L power:GNDPWR #PWR0155
U 1 1 620C0137
P 7700 4450
AR Path="/620C0137" Ref="#PWR0155"  Part="1" 
AR Path="/5E3D0683/620C0137" Ref="#PWR?"  Part="1" 
F 0 "#PWR0155" H 7700 4250 50  0001 C CNN
F 1 "GNDPWR" H 7704 4296 50  0000 C CNN
F 2 "" H 7700 4400 50  0001 C CNN
F 3 "" H 7700 4400 50  0001 C CNN
	1    7700 4450
	1    0    0    -1  
$EndComp
Text GLabel 8350 4150 0    50   Input ~ 0
ADC_5V
Wire Wire Line
	8500 4150 8400 4150
Wire Wire Line
	8500 4250 7700 4250
Wire Wire Line
	7700 4250 7700 4450
Wire Wire Line
	8500 4550 8400 4550
Wire Wire Line
	8400 4550 8400 4150
Connection ~ 8400 4150
Wire Wire Line
	8400 4150 8350 4150
Text Notes 8700 4050 0    50   ~ 0
ADDRESS : 0x49
$Comp
L Server_Used:C C16
U 1 1 62174FAB
P 8100 3750
F 0 "C16" V 8350 3700 50  0000 L CNN
F 1 "106" V 8250 3750 50  0000 C CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 8138 3600 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 8100 3750 50  0001 C CNN
F 4 "66" H 8100 3750 50  0001 C CNN "Price"
F 5 "부품세상" H 8100 3750 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 8100 3750 50  0001 C CNN "Package"
F 7 "106(10uF)" H 8100 3750 50  0001 C CNN "Value"
	1    8100 3750
	0    -1   -1   0   
$EndComp
$Comp
L power:GNDPWR #PWR0156
U 1 1 62175FC8
P 7850 3850
AR Path="/62175FC8" Ref="#PWR0156"  Part="1" 
AR Path="/5E3D0683/62175FC8" Ref="#PWR?"  Part="1" 
F 0 "#PWR0156" H 7850 3650 50  0001 C CNN
F 1 "GNDPWR" H 7854 3696 50  0000 C CNN
F 2 "" H 7850 3800 50  0001 C CNN
F 3 "" H 7850 3800 50  0001 C CNN
	1    7850 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7850 3850 7850 3750
Wire Wire Line
	7850 3750 7950 3750
Wire Wire Line
	8250 3750 8400 3750
Wire Wire Line
	8400 3750 8400 4150
Connection ~ 2000 8450
Wire Wire Line
	1450 8450 2000 8450
Wire Wire Line
	3700 7550 3550 7550
Wire Wire Line
	3550 7550 3550 6650
Wire Wire Line
	3550 6650 3750 6650
Wire Wire Line
	3750 6650 3750 6500
Connection ~ 3750 6500
Wire Wire Line
	2650 9450 2650 8600
Wire Wire Line
	2650 8600 2750 8600
$Comp
L Connector:TestPoint_Small TP4
U 1 1 6184A882
P 3300 5700
F 0 "TP4" H 3250 5650 50  0000 L TNN
F 1 "SOLAR" H 3250 5550 50  0000 L CNN
F 2 "Measurement_Points.pretty:Test_Point_Keystone_5000-5004_Miniature" H 3500 5700 50  0001 C CNN
F 3 "~" H 3500 5700 50  0001 C CNN
	1    3300 5700
	1    0    0    -1  
$EndComp
Connection ~ 1950 5500
Wire Wire Line
	1950 5500 1900 5500
Connection ~ 3300 5700
Wire Wire Line
	3300 5700 3250 5700
$Comp
L Connector:TestPoint_Small TP5
U 1 1 619E0A2B
P 2450 2850
F 0 "TP5" H 2400 2800 50  0001 L TNN
F 1 "GND" H 2400 2700 50  0000 L CNN
F 2 "Measurement_Points.pretty:Test_Point_Keystone_5000-5004_Miniature" H 2650 2850 50  0001 C CNN
F 3 "~" H 2650 2850 50  0001 C CNN
	1    2450 2850
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR0122
U 1 1 619E1900
P 2200 2850
AR Path="/619E1900" Ref="#PWR0122"  Part="1" 
AR Path="/5E3D0683/619E1900" Ref="#PWR?"  Part="1" 
F 0 "#PWR0122" H 2200 2650 50  0001 C CNN
F 1 "GNDPWR" H 2204 2696 50  0000 C CNN
F 2 "" H 2200 2800 50  0001 C CNN
F 3 "" H 2200 2800 50  0001 C CNN
	1    2200 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 2850 2450 2700
Wire Wire Line
	2450 2700 2200 2700
Wire Wire Line
	2200 2700 2200 2850
Text GLabel 5750 7450 1    50   Input ~ 0
SP_03
Text GLabel 5750 8000 3    50   Input ~ 0
SP_04
Wire Wire Line
	5750 7450 5750 7650
Connection ~ 5750 7650
Wire Wire Line
	5750 7650 6100 7650
Wire Wire Line
	5000 7750 5750 7750
Wire Wire Line
	2500 4600 2700 4600
Wire Wire Line
	1700 4700 2700 4700
Wire Wire Line
	2700 4700 2700 4850
Wire Wire Line
	6000 6750 6000 7050
Wire Wire Line
	5750 8000 5750 7750
Connection ~ 5750 7750
Wire Wire Line
	5750 7750 6000 7750
Text GLabel 8300 4850 0    50   Input ~ 0
AI_01
Text GLabel 8300 4950 0    50   Input ~ 0
AI_02
Text GLabel 8300 5050 0    50   Input ~ 0
AI_03
Wire Wire Line
	8300 4850 8500 4850
Wire Wire Line
	8500 4950 8300 4950
Wire Wire Line
	8300 5050 8500 5050
Wire Wire Line
	6400 6000 6400 7850
Wire Wire Line
	2600 7150 2800 7150
$Comp
L Server-cache:HJR1-2C-L-12VDC K?
U 1 1 6321C357
P 3850 3500
AR Path="/5E3D0683/6321C357" Ref="K?"  Part="1" 
AR Path="/6321C357" Ref="K20"  Part="1" 
F 0 "K20" V 4417 3500 50  0000 C CNN
F 1 "JQC-T78-DC-12V" V 4326 3500 50  0000 C CNN
F 2 "Server.pretty:JQC-T78-DC12V-C" H 4300 3450 50  0001 L CNN
F 3 "https://www.devicemart.co.kr/goods/view?no=38804" H 3850 3500 50  0001 C CNN
F 4 "DEVICEMART" H 3850 3500 50  0001 C CNN "Manufacture"
F 5 "704" H 3850 3500 50  0001 C CNN "Price"
F 6 "JQC-T78-DC-12V" H 3850 3500 50  0001 C CNN "Value"
	1    3850 3500
	0    1    -1   0   
$EndComp
$Comp
L power:GNDPWR #PWR0172
U 1 1 61DC3315
P 3650 11900
AR Path="/61DC3315" Ref="#PWR0172"  Part="1" 
AR Path="/5E3D0683/61DC3315" Ref="#PWR?"  Part="1" 
F 0 "#PWR0172" H 3650 11700 50  0001 C CNN
F 1 "GNDPWR" H 3654 11746 50  0000 C CNN
F 2 "" H 3650 11850 50  0001 C CNN
F 3 "" H 3650 11850 50  0001 C CNN
	1    3650 11900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3650 11850 3650 11900
$Comp
L Connector:TestPoint_Small TP9
U 1 1 61F519BA
P 4700 5300
F 0 "TP9" H 4700 5250 50  0000 C TNN
F 1 "INV" H 4700 5150 50  0000 C CNN
F 2 "Measurement_Points.pretty:Test_Point_Keystone_5000-5004_Miniature" H 4900 5300 50  0001 C CNN
F 3 "~" H 4900 5300 50  0001 C CNN
	1    4700 5300
	1    0    0    1   
$EndComp
Wire Wire Line
	1700 4600 2050 4600
Wire Wire Line
	2100 4200 2050 4200
Wire Wire Line
	2050 4200 2050 4600
Connection ~ 2050 4600
Wire Wire Line
	2050 4600 2200 4600
Wire Wire Line
	2700 3450 2700 4400
Wire Wire Line
	2400 4200 2450 4200
Wire Wire Line
	1700 4300 2450 4300
Connection ~ 2450 4300
Wire Wire Line
	2450 4200 2450 4300
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R67
U 1 1 61941C37
P 2100 4200
F 0 "R67" V 2200 4050 50  0000 C CNN
F 1 "103F" V 2100 4050 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 2030 4200 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000607" H 2100 4200 50  0001 C CNN
F 4 "50" H 2100 4200 50  0001 C CNN "Price"
F 5 "부품세상" H 2100 4200 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 2100 4200 50  0001 C CNN "Package"
F 7 "500Ω" H 2100 4200 50  0001 C CNN "Value"
	1    2100 4200
	0    -1   -1   0   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R11
U 1 1 60B453C4
P 2200 4600
F 0 "R11" V 2300 4450 50  0000 C CNN
F 1 "473F" V 2200 4450 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 2130 4600 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000607" H 2200 4600 50  0001 C CNN
F 4 "50" H 2200 4600 50  0001 C CNN "Price"
F 5 "부품세상" H 2200 4600 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 2200 4600 50  0001 C CNN "Package"
F 7 "500Ω" H 2200 4600 50  0001 C CNN "Value"
	1    2200 4600
	0    -1   -1   0   
$EndComp
$Comp
L Switch:SW_SPST SW4
U 1 1 61AB96EE
P 3150 3600
F 0 "SW4" V 3200 3400 50  0000 C CNN
F 1 "CPU" V 3100 3400 50  0000 C CNN
F 2 "Server.pretty:NW3-SK-02" H 3150 3600 50  0001 C CNN
F 3 "~" H 3150 3600 50  0001 C CNN
	1    3150 3600
	0    1    1    0   
$EndComp
Wire Wire Line
	3150 3800 3150 4300
Wire Wire Line
	3300 4500 3400 4500
Connection ~ 3400 4500
Wire Wire Line
	2450 4300 2850 4300
Wire Wire Line
	2900 4400 2850 4400
Wire Wire Line
	2850 4400 2850 4300
Connection ~ 2850 4300
Wire Wire Line
	2850 4300 3150 4300
$Comp
L Switch:SW_SPDT SW2
U 1 1 61B15159
P 3100 4500
F 0 "SW2" H 3000 4650 50  0000 C CNN
F 1 "RELAY" H 3000 4750 50  0000 C CNN
F 2 "Server.pretty:NW3-SK-02" H 3100 4500 50  0001 C CNN
F 3 "~" H 3100 4500 50  0001 C CNN
	1    3100 4500
	-1   0    0    1   
$EndComp
Wire Wire Line
	4900 4300 5050 4300
Wire Wire Line
	4600 3950 5050 3950
Wire Wire Line
	4600 2750 4600 3950
Connection ~ 4700 5300
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R68
U 1 1 61ABF2E1
P 3550 7650
F 0 "R68" V 3450 7500 50  0000 C CNN
F 1 "103F" V 3550 7500 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 3480 7650 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 3550 7650 50  0001 C CNN
F 4 "20" H 3550 7650 50  0001 C CNN "Price"
F 5 "부품세상" H 3550 7650 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 3550 7650 50  0001 C CNN "Package"
F 7 "10KΩ" H 3550 7650 50  0001 C CNN "Value"
	1    3550 7650
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR0173
U 1 1 61AED591
P 3150 7500
F 0 "#PWR0173" H 3150 7350 50  0001 C CNN
F 1 "+5V" H 3165 7673 50  0000 C CNN
F 2 "" H 3150 7500 50  0001 C CNN
F 3 "" H 3150 7500 50  0001 C CNN
	1    3150 7500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 7650 3550 7650
Wire Wire Line
	3250 7650 3150 7650
Wire Wire Line
	3150 7650 3150 7500
Wire Wire Line
	6450 10800 6600 10800
Text GLabel 6450 10800 0    50   Input ~ 0
SP_03
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R9
U 1 1 6193F5FA
P 6900 10800
F 0 "R9" V 6800 10650 50  0000 C CNN
F 1 "102F" V 6900 10650 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 6830 10800 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000614" H 6900 10800 50  0001 C CNN
F 4 "20" H 6900 10800 50  0001 C CNN "Price"
F 5 "부품세상" H 6900 10800 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 6900 10800 50  0001 C CNN "Package"
F 7 "1KΩ" H 6900 10800 50  0001 C CNN "Value"
	1    6900 10800
	0    1    -1   0   
$EndComp
Wire Wire Line
	6450 9250 6600 9250
Text GLabel 6450 9250 0    50   Input ~ 0
SP_01
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R41
U 1 1 61599056
P 6900 9250
F 0 "R41" V 6800 9100 50  0000 C CNN
F 1 "102F" V 6900 9100 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 6830 9250 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000614" H 6900 9250 50  0001 C CNN
F 4 "20" H 6900 9250 50  0001 C CNN "Price"
F 5 "부품세상" H 6900 9250 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 6900 9250 50  0001 C CNN "Package"
F 7 "1KΩ" H 6900 9250 50  0001 C CNN "Value"
	1    6900 9250
	0    1    -1   0   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R66
U 1 1 61DF2D65
P 9500 7550
F 0 "R66" V 9400 7400 50  0000 C CNN
F 1 "103F" V 9500 7400 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 9430 7550 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000614" H 9500 7550 50  0001 C CNN
F 4 "20" H 9500 7550 50  0001 C CNN "Price"
F 5 "부품세상" H 9500 7550 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 9500 7550 50  0001 C CNN "Package"
F 7 "1KΩ" H 9500 7550 50  0001 C CNN "Value"
	1    9500 7550
	1    0    0    1   
$EndComp
Wire Wire Line
	9500 7550 9500 7800
Wire Wire Line
	9500 7150 9500 7250
$Comp
L Connector:TestPoint_Small TP8
U 1 1 61F20648
P 9350 10800
F 0 "TP8" H 9300 10750 50  0000 L TNN
F 1 "SP4" H 9300 10650 50  0000 L CNN
F 2 "Measurement_Points.pretty:Test_Point_Keystone_5000-5004_Miniature" H 9550 10800 50  0001 C CNN
F 3 "~" H 9550 10800 50  0001 C CNN
	1    9350 10800
	1    0    0    1   
$EndComp
$Comp
L Connector:TestPoint_Small TP7
U 1 1 61F202A2
P 9350 9250
F 0 "TP7" H 9300 9200 50  0000 L TNN
F 1 "SP2" H 9300 9100 50  0000 L CNN
F 2 "Measurement_Points.pretty:Test_Point_Keystone_5000-5004_Miniature" H 9550 9250 50  0001 C CNN
F 3 "~" H 9550 9250 50  0001 C CNN
	1    9350 9250
	1    0    0    1   
$EndComp
$Comp
L Connector:TestPoint_Small TP2
U 1 1 61F1FD44
P 7000 9250
F 0 "TP2" H 6950 9200 50  0000 L TNN
F 1 "SP1" H 6950 9100 50  0000 L CNN
F 2 "Measurement_Points.pretty:Test_Point_Keystone_5000-5004_Miniature" H 7200 9250 50  0001 C CNN
F 3 "~" H 7200 9250 50  0001 C CNN
	1    7000 9250
	1    0    0    1   
$EndComp
$Comp
L Connector:TestPoint_Small TP6
U 1 1 61F1EF55
P 7000 10800
F 0 "TP6" H 6950 10750 50  0000 L TNN
F 1 "SP3" H 6950 10650 50  0000 L CNN
F 2 "Measurement_Points.pretty:Test_Point_Keystone_5000-5004_Miniature" H 7200 10800 50  0001 C CNN
F 3 "~" H 7200 10800 50  0001 C CNN
	1    7000 10800
	1    0    0    1   
$EndComp
Wire Wire Line
	7950 10400 8100 10400
$Comp
L Device:Fuse_Small F14
U 1 1 61B8C2D6
P 7850 10400
F 0 "F14" H 7850 10585 50  0000 C CNN
F 1 "Fuse" H 7850 10494 50  0000 C CNN
F 2 "Fuse.pretty:Fuse_1210_3225Metric_Castellated" H 7850 10400 50  0001 C CNN
F 3 "~" H 7850 10400 50  0001 C CNN
	1    7850 10400
	1    0    0    -1  
$EndComp
Wire Wire Line
	10300 10400 10450 10400
$Comp
L Device:Fuse_Small F16
U 1 1 61B8BB13
P 10200 10400
F 0 "F16" H 10200 10585 50  0000 C CNN
F 1 "Fuse" H 10200 10494 50  0000 C CNN
F 2 "Fuse.pretty:Fuse_1210_3225Metric_Castellated" H 10200 10400 50  0001 C CNN
F 3 "~" H 10200 10400 50  0001 C CNN
	1    10200 10400
	1    0    0    -1  
$EndComp
Wire Wire Line
	10300 8850 10450 8850
$Comp
L Device:Fuse_Small F15
U 1 1 61B8B5E4
P 10200 8850
F 0 "F15" H 10200 9035 50  0000 C CNN
F 1 "Fuse" H 10200 8944 50  0000 C CNN
F 2 "Fuse.pretty:Fuse_1210_3225Metric_Castellated" H 10200 8850 50  0001 C CNN
F 3 "~" H 10200 8850 50  0001 C CNN
	1    10200 8850
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 8850 7550 8850
$Comp
L Device:Fuse_Small F13
U 1 1 61A923EE
P 7850 8850
F 0 "F13" H 7850 9035 50  0000 C CNN
F 1 "Fuse" H 7850 8944 50  0000 C CNN
F 2 "Fuse.pretty:Fuse_1210_3225Metric_Castellated" H 7850 8850 50  0001 C CNN
F 3 "~" H 7850 8850 50  0001 C CNN
	1    7850 8850
	1    0    0    -1  
$EndComp
Wire Wire Line
	8700 8300 8700 8150
Wire Wire Line
	9100 8150 9100 8300
Wire Wire Line
	9500 8300 9500 8150
Wire Wire Line
	9500 7850 9500 7800
Wire Wire Line
	9100 7850 9100 7700
Wire Wire Line
	8700 7850 8700 7600
Wire Wire Line
	8400 7800 9500 7800
Wire Wire Line
	8700 7150 9100 7150
Connection ~ 8700 7150
Wire Wire Line
	8700 7250 8700 7150
Wire Wire Line
	9100 7150 9500 7150
Connection ~ 9100 7150
Wire Wire Line
	9100 7250 9100 7150
Wire Wire Line
	7900 7150 8700 7150
Text GLabel 7900 7150 0    50   Input ~ 0
ADC_5V
$Comp
L power:GNDPWR #PWR0171
U 1 1 623D204D
P 9500 8300
AR Path="/623D204D" Ref="#PWR0171"  Part="1" 
AR Path="/5E3D0683/623D204D" Ref="#PWR?"  Part="1" 
F 0 "#PWR0171" H 9500 8100 50  0001 C CNN
F 1 "GNDPWR" H 9504 8146 50  0000 C CNN
F 2 "" H 9500 8250 50  0001 C CNN
F 3 "" H 9500 8250 50  0001 C CNN
	1    9500 8300
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR0170
U 1 1 623D1D6E
P 9100 8300
AR Path="/623D1D6E" Ref="#PWR0170"  Part="1" 
AR Path="/5E3D0683/623D1D6E" Ref="#PWR?"  Part="1" 
F 0 "#PWR0170" H 9100 8100 50  0001 C CNN
F 1 "GNDPWR" H 9104 8146 50  0000 C CNN
F 2 "" H 9100 8250 50  0001 C CNN
F 3 "" H 9100 8250 50  0001 C CNN
	1    9100 8300
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR0169
U 1 1 623D1B69
P 8700 8300
AR Path="/623D1B69" Ref="#PWR0169"  Part="1" 
AR Path="/5E3D0683/623D1B69" Ref="#PWR?"  Part="1" 
F 0 "#PWR0169" H 8700 8100 50  0001 C CNN
F 1 "GNDPWR" H 8704 8146 50  0000 C CNN
F 2 "" H 8700 8250 50  0001 C CNN
F 3 "" H 8700 8250 50  0001 C CNN
	1    8700 8300
	1    0    0    -1  
$EndComp
$Comp
L Server_Used:C C19
U 1 1 62223D91
P 9500 8000
F 0 "C19" H 9600 7900 50  0000 C CNN
F 1 "106" H 9600 8100 50  0000 C CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 9538 7850 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 9500 8000 50  0001 C CNN
F 4 "66" H 9500 8000 50  0001 C CNN "Price"
F 5 "부품세상" H 9500 8000 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 9500 8000 50  0001 C CNN "Package"
F 7 "106(10uF)" H 9500 8000 50  0001 C CNN "Value"
	1    9500 8000
	-1   0    0    -1  
$EndComp
$Comp
L Server_Used:C C18
U 1 1 62223A22
P 9100 8000
F 0 "C18" H 9200 7900 50  0000 C CNN
F 1 "106" H 9200 8100 50  0000 C CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 9138 7850 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 9100 8000 50  0001 C CNN
F 4 "66" H 9100 8000 50  0001 C CNN "Price"
F 5 "부품세상" H 9100 8000 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 9100 8000 50  0001 C CNN "Package"
F 7 "106(10uF)" H 9100 8000 50  0001 C CNN "Value"
	1    9100 8000
	-1   0    0    -1  
$EndComp
$Comp
L Server_Used:C C17
U 1 1 622221B0
P 8700 8000
F 0 "C17" H 8800 7900 50  0000 C CNN
F 1 "106" H 8800 8100 50  0000 C CNN
F 2 "Server.pretty:C_0805_HandSoldering" H 8738 7850 50  0001 C CNN
F 3 "http://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000013173" H 8700 8000 50  0001 C CNN
F 4 "66" H 8700 8000 50  0001 C CNN "Price"
F 5 "부품세상" H 8700 8000 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 8700 8000 50  0001 C CNN "Package"
F 7 "106(10uF)" H 8700 8000 50  0001 C CNN "Value"
	1    8700 8000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	10300 8200 10450 8200
Wire Wire Line
	10300 8300 10300 8200
Wire Wire Line
	10300 7750 10300 7850
Wire Wire Line
	10450 7750 10300 7750
Wire Wire Line
	10300 7300 10300 7400
Wire Wire Line
	10450 7300 10300 7300
$Comp
L power:GNDPWR #PWR0168
U 1 1 621678CB
P 10300 7400
AR Path="/621678CB" Ref="#PWR0168"  Part="1" 
AR Path="/5E3D0683/621678CB" Ref="#PWR?"  Part="1" 
F 0 "#PWR0168" H 10300 7200 50  0001 C CNN
F 1 "GNDPWR" H 10304 7246 50  0000 C CNN
F 2 "" H 10300 7350 50  0001 C CNN
F 3 "" H 10300 7350 50  0001 C CNN
	1    10300 7400
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR0167
U 1 1 62167382
P 10300 7850
AR Path="/62167382" Ref="#PWR0167"  Part="1" 
AR Path="/5E3D0683/62167382" Ref="#PWR?"  Part="1" 
F 0 "#PWR0167" H 10300 7650 50  0001 C CNN
F 1 "GNDPWR" H 10304 7696 50  0000 C CNN
F 2 "" H 10300 7800 50  0001 C CNN
F 3 "" H 10300 7800 50  0001 C CNN
	1    10300 7850
	1    0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR0166
U 1 1 621667EC
P 10300 8300
AR Path="/621667EC" Ref="#PWR0166"  Part="1" 
AR Path="/5E3D0683/621667EC" Ref="#PWR?"  Part="1" 
F 0 "#PWR0166" H 10300 8100 50  0001 C CNN
F 1 "GNDPWR" H 10304 8146 50  0000 C CNN
F 2 "" H 10300 8250 50  0001 C CNN
F 3 "" H 10300 8250 50  0001 C CNN
	1    10300 8300
	1    0    0    -1  
$EndComp
Connection ~ 9500 7800
Wire Wire Line
	9100 7700 9100 7550
Connection ~ 9100 7700
Wire Wire Line
	8400 7700 9100 7700
Connection ~ 8700 7600
Wire Wire Line
	8400 7600 8700 7600
Wire Wire Line
	8100 7800 7900 7800
Wire Wire Line
	7900 7700 8100 7700
Wire Wire Line
	8100 7600 7900 7600
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R61
U 1 1 61FFE2F4
P 8400 7800
F 0 "R61" V 8350 7850 50  0000 C CNN
F 1 "102F" V 8400 7650 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 8330 7800 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000614" H 8400 7800 50  0001 C CNN
F 4 "20" H 8400 7800 50  0001 C CNN "Price"
F 5 "부품세상" H 8400 7800 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 8400 7800 50  0001 C CNN "Package"
F 7 "1KΩ" H 8400 7800 50  0001 C CNN "Value"
	1    8400 7800
	0    1    1    0   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R60
U 1 1 61FFE076
P 8400 7700
F 0 "R60" V 8350 7750 50  0000 C CNN
F 1 "102F" V 8400 7550 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 8330 7700 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000614" H 8400 7700 50  0001 C CNN
F 4 "20" H 8400 7700 50  0001 C CNN "Price"
F 5 "부품세상" H 8400 7700 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 8400 7700 50  0001 C CNN "Package"
F 7 "1KΩ" H 8400 7700 50  0001 C CNN "Value"
	1    8400 7700
	0    1    1    0   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R59
U 1 1 61FFD5EC
P 8400 7600
F 0 "R59" V 8350 7650 50  0000 C CNN
F 1 "102F" V 8400 7450 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 8330 7600 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000614" H 8400 7600 50  0001 C CNN
F 4 "20" H 8400 7600 50  0001 C CNN "Price"
F 5 "부품세상" H 8400 7600 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 8400 7600 50  0001 C CNN "Package"
F 7 "1KΩ" H 8400 7600 50  0001 C CNN "Value"
	1    8400 7600
	0    1    1    0   
$EndComp
Text GLabel 7900 7800 0    50   Input ~ 0
AI_03
Text GLabel 7900 7700 0    50   Input ~ 0
AI_02
Text GLabel 7900 7600 0    50   Input ~ 0
AI_01
Wire Wire Line
	8700 7550 8700 7600
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R63
U 1 1 61D9AFA7
P 9100 7550
F 0 "R63" V 9000 7400 50  0000 C CNN
F 1 "103F" V 9100 7400 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 9030 7550 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000614" H 9100 7550 50  0001 C CNN
F 4 "20" H 9100 7550 50  0001 C CNN "Price"
F 5 "부품세상" H 9100 7550 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 9100 7550 50  0001 C CNN "Package"
F 7 "1KΩ" H 9100 7550 50  0001 C CNN "Value"
	1    9100 7550
	1    0    0    1   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R62
U 1 1 61D17302
P 8700 7550
F 0 "R62" V 8600 7400 50  0000 C CNN
F 1 "103F" V 8700 7400 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 8630 7550 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000614" H 8700 7550 50  0001 C CNN
F 4 "20" H 8700 7550 50  0001 C CNN "Price"
F 5 "부품세상" H 8700 7550 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 8700 7550 50  0001 C CNN "Package"
F 7 "1KΩ" H 8700 7550 50  0001 C CNN "Value"
	1    8700 7550
	1    0    0    1   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R64
U 1 1 6196E78E
P 9250 10800
F 0 "R64" V 9150 10650 50  0000 C CNN
F 1 "102F" V 9250 10650 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 9180 10800 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000614" H 9250 10800 50  0001 C CNN
F 4 "20" H 9250 10800 50  0001 C CNN "Price"
F 5 "부품세상" H 9250 10800 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 9250 10800 50  0001 C CNN "Package"
F 7 "1KΩ" H 9250 10800 50  0001 C CNN "Value"
	1    9250 10800
	0    1    -1   0   
$EndComp
Wire Wire Line
	9350 10800 9250 10800
Connection ~ 9350 10800
Wire Wire Line
	8800 10800 8950 10800
$Comp
L Connector:Conn_01x02_Male J26
U 1 1 61B32720
P 10650 8100
F 0 "J26" H 10650 7900 50  0000 C CNN
F 1 "AI3" H 10500 8050 50  0001 C CNN
F 2 "Server.pretty:PhoenixContact_MCV_1,5_2-G-3.81_1x02_P3.81mm_Vertical" H 10650 8100 50  0001 C CNN
F 3 "https://www.devicemart.co.kr/goods/view?no=23260&market=naver&NaPm=ct%3Dkof8h100%7Cci%3D2b06ad3968d8adec0b2b30ae50256e40450772f8%7Ctr%3Dslsc%7Csn%3D876973%7Chk%3Dcbed85e5f251313af0535f600d98c0e674ebf3d9" H 10650 8100 50  0001 C CNN
F 4 "DEVICEMART" H 10650 8100 50  0001 C CNN "Manufacture"
F 5 "100" H 10650 8100 50  0001 C CNN "Price"
F 6 "ECH381V-2P" H 10650 8100 50  0001 C CNN "Value"
	1    10650 8100
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J9
U 1 1 61B31F95
P 10650 7200
F 0 "J9" H 10650 7000 50  0000 C CNN
F 1 "AI1" H 10500 7150 50  0001 C CNN
F 2 "Server.pretty:PhoenixContact_MCV_1,5_2-G-3.81_1x02_P3.81mm_Vertical" H 10650 7200 50  0001 C CNN
F 3 "https://www.devicemart.co.kr/goods/view?no=23260&market=naver&NaPm=ct%3Dkof8h100%7Cci%3D2b06ad3968d8adec0b2b30ae50256e40450772f8%7Ctr%3Dslsc%7Csn%3D876973%7Chk%3Dcbed85e5f251313af0535f600d98c0e674ebf3d9" H 10650 7200 50  0001 C CNN
F 4 "DEVICEMART" H 10650 7200 50  0001 C CNN "Manufacture"
F 5 "100" H 10650 7200 50  0001 C CNN "Price"
F 6 "ECH381V-2P" H 10650 7200 50  0001 C CNN "Value"
	1    10650 7200
	-1   0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J23
U 1 1 61B313DE
P 10650 7650
F 0 "J23" H 10650 7450 50  0000 C CNN
F 1 "AI2" H 10500 7600 50  0001 C CNN
F 2 "Server.pretty:PhoenixContact_MCV_1,5_2-G-3.81_1x02_P3.81mm_Vertical" H 10650 7650 50  0001 C CNN
F 3 "https://www.devicemart.co.kr/goods/view?no=23260&market=naver&NaPm=ct%3Dkof8h100%7Cci%3D2b06ad3968d8adec0b2b30ae50256e40450772f8%7Ctr%3Dslsc%7Csn%3D876973%7Chk%3Dcbed85e5f251313af0535f600d98c0e674ebf3d9" H 10650 7650 50  0001 C CNN
F 4 "DEVICEMART" H 10650 7650 50  0001 C CNN "Manufacture"
F 5 "100" H 10650 7650 50  0001 C CNN "Price"
F 6 "ECH381V-2P" H 10650 7650 50  0001 C CNN "Value"
	1    10650 7650
	-1   0    0    -1  
$EndComp
$Comp
L Server_Used:FDS6990A Q3
U 2 1 6196E83C
P 7450 10800
F 0 "Q3" H 7656 10846 50  0000 L CNN
F 1 "FDS6990A" H 7656 10755 50  0000 L CNN
F 2 "Server.pretty:6990A" H 7650 10800 50  0001 C CNN
F 3 "" H 7650 10800 50  0001 C CNN
	2    7450 10800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 10300 9500 10500
Wire Wire Line
	9500 10500 9600 10500
$Comp
L power:+12P #PWR0165
U 1 1 6196E830
P 9500 10300
F 0 "#PWR0165" H 9500 10150 50  0001 C CNN
F 1 "+12P" H 9515 10473 50  0000 C CNN
F 2 "" H 9500 10300 50  0001 C CNN
F 3 "" H 9500 10300 50  0001 C CNN
	1    9500 10300
	1    0    0    -1  
$EndComp
Connection ~ 9900 10500
$Comp
L Simulation_SPICE:DIODE D?
U 1 1 6196E809
P 9750 10500
AR Path="/5E3D0683/6196E809" Ref="D?"  Part="1" 
AR Path="/6196E809" Ref="D9"  Part="1" 
F 0 "D9" H 9750 10650 50  0000 C CNN
F 1 "M7" H 9750 10600 50  0000 C CNN
F 2 "Server.pretty:D_SMA" H 9750 10500 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000005570" H 9750 10500 50  0001 C CNN
F 4 "Y" H 9750 10500 50  0001 L CNN "Spice_Netlist_Enabled"
F 5 "D" H 9750 10500 50  0001 L CNN "Spice_Primitive"
F 6 "부품세상" H 9750 10500 50  0001 C CNN "Manufacture"
F 7 "DO-214AC(SMA)" H 9750 10500 50  0001 C CNN "Package"
F 8 "55" H 9750 10500 50  0001 C CNN "Price"
F 9 "4007" H 9750 10500 50  0001 C CNN "Value"
	1    9750 10500
	-1   0    0    1   
$EndComp
Wire Wire Line
	9900 10400 10100 10400
Wire Wire Line
	9900 10400 9900 10300
$Comp
L power:+12P #PWR0164
U 1 1 6196E7F7
P 9900 10300
F 0 "#PWR0164" H 9900 10150 50  0001 C CNN
F 1 "+12P" H 9915 10473 50  0000 C CNN
F 2 "" H 9900 10300 50  0001 C CNN
F 3 "" H 9900 10300 50  0001 C CNN
	1    9900 10300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 10500 10450 10500
Wire Wire Line
	9900 10600 9900 10500
$Comp
L Connector:Conn_01x02_Male J27
U 1 1 6196E7CD
P 10650 10400
F 0 "J27" H 10650 10500 50  0000 C CNN
F 1 "SP4" H 10500 10350 50  0001 C CNN
F 2 "Server.pretty:PhoenixContact_MCV_1,5_2-G-3.81_1x02_P3.81mm_Vertical" H 10650 10400 50  0001 C CNN
F 3 "https://www.devicemart.co.kr/goods/view?no=23260&market=naver&NaPm=ct%3Dkof8h100%7Cci%3D2b06ad3968d8adec0b2b30ae50256e40450772f8%7Ctr%3Dslsc%7Csn%3D876973%7Chk%3Dcbed85e5f251313af0535f600d98c0e674ebf3d9" H 10650 10400 50  0001 C CNN
F 4 "DEVICEMART" H 10650 10400 50  0001 C CNN "Manufacture"
F 5 "100" H 10650 10400 50  0001 C CNN "Price"
F 6 "ECH381V-2P" H 10650 10400 50  0001 C CNN "Value"
	1    10650 10400
	-1   0    0    -1  
$EndComp
Text GLabel 8800 10800 0    50   Input ~ 0
SP_04
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R65
U 1 1 6196E7BC
P 9450 11150
F 0 "R65" V 9350 11000 50  0000 C CNN
F 1 "103F" V 9450 11000 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 9380 11150 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 9450 11150 50  0001 C CNN
F 4 "20" H 9450 11150 50  0001 C CNN "Price"
F 5 "부품세상" H 9450 11150 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 9450 11150 50  0001 C CNN "Package"
F 7 "10KΩ" H 9450 11150 50  0001 C CNN "Value"
	1    9450 11150
	0    -1   1    0   
$EndComp
Wire Wire Line
	9350 11150 9350 10800
Wire Wire Line
	9450 11150 9350 11150
Wire Wire Line
	9350 10800 9600 10800
$Comp
L power:GNDPWR #PWR0163
U 1 1 6196E77D
P 9900 11300
AR Path="/6196E77D" Ref="#PWR0163"  Part="1" 
AR Path="/5E3D0683/6196E77D" Ref="#PWR?"  Part="1" 
F 0 "#PWR0163" H 9900 11100 50  0001 C CNN
F 1 "GNDPWR" H 9904 11146 50  0000 C CNN
F 2 "" H 9900 11250 50  0001 C CNN
F 3 "" H 9900 11250 50  0001 C CNN
	1    9900 11300
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 11000 9900 11150
Wire Wire Line
	9900 11150 9750 11150
Wire Wire Line
	9900 11150 9900 11300
Connection ~ 9900 11150
$Comp
L Server_Used:FDS6990A Q3
U 1 1 6193F6A8
P 9800 10800
F 0 "Q3" H 10006 10846 50  0000 L CNN
F 1 "FDS6990A" H 10006 10755 50  0000 L CNN
F 2 "Server.pretty:6990A" H 10000 10800 50  0001 C CNN
F 3 "" H 10000 10800 50  0001 C CNN
	1    9800 10800
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 10300 7150 10500
Wire Wire Line
	7150 10500 7250 10500
$Comp
L power:+12P #PWR0162
U 1 1 6193F69C
P 7150 10300
F 0 "#PWR0162" H 7150 10150 50  0001 C CNN
F 1 "+12P" H 7165 10473 50  0000 C CNN
F 2 "" H 7150 10300 50  0001 C CNN
F 3 "" H 7150 10300 50  0001 C CNN
	1    7150 10300
	1    0    0    -1  
$EndComp
Connection ~ 7550 10500
$Comp
L Simulation_SPICE:DIODE D?
U 1 1 6193F675
P 7400 10500
AR Path="/5E3D0683/6193F675" Ref="D?"  Part="1" 
AR Path="/6193F675" Ref="D8"  Part="1" 
F 0 "D8" H 7400 10650 50  0000 C CNN
F 1 "M7" H 7400 10600 50  0000 C CNN
F 2 "Server.pretty:D_SMA" H 7400 10500 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000005570" H 7400 10500 50  0001 C CNN
F 4 "Y" H 7400 10500 50  0001 L CNN "Spice_Netlist_Enabled"
F 5 "D" H 7400 10500 50  0001 L CNN "Spice_Primitive"
F 6 "부품세상" H 7400 10500 50  0001 C CNN "Manufacture"
F 7 "DO-214AC(SMA)" H 7400 10500 50  0001 C CNN "Package"
F 8 "55" H 7400 10500 50  0001 C CNN "Price"
F 9 "4007" H 7400 10500 50  0001 C CNN "Value"
	1    7400 10500
	-1   0    0    1   
$EndComp
Wire Wire Line
	7550 10400 7750 10400
Wire Wire Line
	7550 10400 7550 10300
$Comp
L power:+12P #PWR0160
U 1 1 6193F663
P 7550 10300
F 0 "#PWR0160" H 7550 10150 50  0001 C CNN
F 1 "+12P" H 7565 10473 50  0000 C CNN
F 2 "" H 7550 10300 50  0001 C CNN
F 3 "" H 7550 10300 50  0001 C CNN
	1    7550 10300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 10500 8100 10500
Wire Wire Line
	7550 10600 7550 10500
$Comp
L Connector:Conn_01x02_Male J8
U 1 1 6193F645
P 8300 10400
F 0 "J8" H 8300 10500 50  0000 C CNN
F 1 "SP3" H 8150 10350 50  0001 C CNN
F 2 "Server.pretty:PhoenixContact_MCV_1,5_2-G-3.81_1x02_P3.81mm_Vertical" H 8300 10400 50  0001 C CNN
F 3 "https://www.devicemart.co.kr/goods/view?no=23260&market=naver&NaPm=ct%3Dkof8h100%7Cci%3D2b06ad3968d8adec0b2b30ae50256e40450772f8%7Ctr%3Dslsc%7Csn%3D876973%7Chk%3Dcbed85e5f251313af0535f600d98c0e674ebf3d9" H 8300 10400 50  0001 C CNN
F 4 "DEVICEMART" H 8300 10400 50  0001 C CNN "Manufacture"
F 5 "100" H 8300 10400 50  0001 C CNN "Price"
F 6 "ECH381V-2P" H 8300 10400 50  0001 C CNN "Value"
	1    8300 10400
	-1   0    0    -1  
$EndComp
Connection ~ 7000 10800
Wire Wire Line
	7000 10800 6900 10800
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R58
U 1 1 6193F608
P 7100 11150
F 0 "R58" V 7000 11000 50  0000 C CNN
F 1 "103F" V 7100 11000 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 7030 11150 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 7100 11150 50  0001 C CNN
F 4 "20" H 7100 11150 50  0001 C CNN "Price"
F 5 "부품세상" H 7100 11150 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 7100 11150 50  0001 C CNN "Package"
F 7 "10KΩ" H 7100 11150 50  0001 C CNN "Value"
	1    7100 11150
	0    -1   1    0   
$EndComp
Wire Wire Line
	7000 11150 7000 10800
Wire Wire Line
	7100 11150 7000 11150
Wire Wire Line
	7000 10800 7250 10800
$Comp
L power:GNDPWR #PWR0159
U 1 1 6193F5E9
P 7550 11300
AR Path="/6193F5E9" Ref="#PWR0159"  Part="1" 
AR Path="/5E3D0683/6193F5E9" Ref="#PWR?"  Part="1" 
F 0 "#PWR0159" H 7550 11100 50  0001 C CNN
F 1 "GNDPWR" H 7554 11146 50  0000 C CNN
F 2 "" H 7550 11250 50  0001 C CNN
F 3 "" H 7550 11250 50  0001 C CNN
	1    7550 11300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 11000 7550 11150
Wire Wire Line
	7550 11150 7400 11150
Wire Wire Line
	7550 11150 7550 11300
Connection ~ 7550 11150
Connection ~ 7550 9600
$Comp
L Server_Used:FDS6990A Q1
U 1 1 618DD56B
P 9800 9250
F 0 "Q1" H 10006 9296 50  0000 L CNN
F 1 "FDS6990A" H 10006 9205 50  0000 L CNN
F 2 "Server.pretty:6990A" H 10000 9250 50  0001 C CNN
F 3 "" H 10000 9250 50  0001 C CNN
	1    9800 9250
	1    0    0    -1  
$EndComp
$Comp
L Server_Used:FDS6990A Q1
U 2 1 618DBD7F
P 7450 9250
F 0 "Q1" H 7656 9296 50  0000 L CNN
F 1 "FDS6990A" H 7656 9205 50  0000 L CNN
F 2 "Server.pretty:6990A" H 7650 9250 50  0001 C CNN
F 3 "" H 7650 9250 50  0001 C CNN
	2    7450 9250
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x02_Male J7
U 1 1 6173E6DA
P 8300 8850
F 0 "J7" H 8300 8950 50  0000 C CNN
F 1 "SP1" H 8150 8800 50  0001 C CNN
F 2 "Server.pretty:PhoenixContact_MCV_1,5_2-G-3.81_1x02_P3.81mm_Vertical" H 8300 8850 50  0001 C CNN
F 3 "https://www.devicemart.co.kr/goods/view?no=23260&market=naver&NaPm=ct%3Dkof8h100%7Cci%3D2b06ad3968d8adec0b2b30ae50256e40450772f8%7Ctr%3Dslsc%7Csn%3D876973%7Chk%3Dcbed85e5f251313af0535f600d98c0e674ebf3d9" H 8300 8850 50  0001 C CNN
F 4 "DEVICEMART" H 8300 8850 50  0001 C CNN "Manufacture"
F 5 "100" H 8300 8850 50  0001 C CNN "Price"
F 6 "ECH381V-2P" H 8300 8850 50  0001 C CNN "Value"
	1    8300 8850
	-1   0    0    -1  
$EndComp
Connection ~ 7550 8950
Wire Wire Line
	7550 8950 8100 8950
Wire Wire Line
	7550 8950 7550 9050
Wire Wire Line
	7500 8950 7550 8950
$Comp
L Simulation_SPICE:DIODE D?
U 1 1 61931A85
P 7350 8950
AR Path="/5E3D0683/61931A85" Ref="D?"  Part="1" 
AR Path="/61931A85" Ref="D6"  Part="1" 
F 0 "D6" H 7350 9100 50  0000 C CNN
F 1 "M7" H 7350 9050 50  0000 C CNN
F 2 "Server.pretty:D_SMA" H 7350 8950 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000005570" H 7350 8950 50  0001 C CNN
F 4 "Y" H 7350 8950 50  0001 L CNN "Spice_Netlist_Enabled"
F 5 "D" H 7350 8950 50  0001 L CNN "Spice_Primitive"
F 6 "부품세상" H 7350 8950 50  0001 C CNN "Manufacture"
F 7 "DO-214AC(SMA)" H 7350 8950 50  0001 C CNN "Package"
F 8 "55" H 7350 8950 50  0001 C CNN "Price"
F 9 "4007" H 7350 8950 50  0001 C CNN "Value"
	1    7350 8950
	-1   0    0    1   
$EndComp
Wire Wire Line
	7150 8750 7150 8950
Wire Wire Line
	7150 8950 7200 8950
$Comp
L power:+12P #PWR0149
U 1 1 618884A0
P 7150 8750
F 0 "#PWR0149" H 7150 8600 50  0001 C CNN
F 1 "+12P" H 7165 8923 50  0000 C CNN
F 2 "" H 7150 8750 50  0001 C CNN
F 3 "" H 7150 8750 50  0001 C CNN
	1    7150 8750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9500 8750 9500 8950
Wire Wire Line
	9500 8950 9600 8950
$Comp
L power:+12P #PWR0148
U 1 1 61846025
P 9500 8750
F 0 "#PWR0148" H 9500 8600 50  0001 C CNN
F 1 "+12P" H 9515 8923 50  0000 C CNN
F 2 "" H 9500 8750 50  0001 C CNN
F 3 "" H 9500 8750 50  0001 C CNN
	1    9500 8750
	1    0    0    -1  
$EndComp
Connection ~ 9900 8950
$Comp
L Simulation_SPICE:DIODE D?
U 1 1 61843DF6
P 9750 8950
AR Path="/5E3D0683/61843DF6" Ref="D?"  Part="1" 
AR Path="/61843DF6" Ref="D7"  Part="1" 
F 0 "D7" H 9750 9100 50  0000 C CNN
F 1 "M7" H 9750 9050 50  0000 C CNN
F 2 "Server.pretty:D_SMA" H 9750 8950 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000005570" H 9750 8950 50  0001 C CNN
F 4 "Y" H 9750 8950 50  0001 L CNN "Spice_Netlist_Enabled"
F 5 "D" H 9750 8950 50  0001 L CNN "Spice_Primitive"
F 6 "부품세상" H 9750 8950 50  0001 C CNN "Manufacture"
F 7 "DO-214AC(SMA)" H 9750 8950 50  0001 C CNN "Package"
F 8 "55" H 9750 8950 50  0001 C CNN "Price"
F 9 "4007" H 9750 8950 50  0001 C CNN "Value"
	1    9750 8950
	-1   0    0    1   
$EndComp
Wire Wire Line
	9900 8850 10100 8850
Wire Wire Line
	9900 8850 9900 8750
$Comp
L power:+12P #PWR0147
U 1 1 617E3127
P 9900 8750
F 0 "#PWR0147" H 9900 8600 50  0001 C CNN
F 1 "+12P" H 9915 8923 50  0000 C CNN
F 2 "" H 9900 8750 50  0001 C CNN
F 3 "" H 9900 8750 50  0001 C CNN
	1    9900 8750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 8850 7550 8750
Wire Wire Line
	8100 8850 7950 8850
$Comp
L power:+12P #PWR0146
U 1 1 617C0FB6
P 7550 8750
F 0 "#PWR0146" H 7550 8600 50  0001 C CNN
F 1 "+12P" H 7565 8923 50  0000 C CNN
F 2 "" H 7550 8750 50  0001 C CNN
F 3 "" H 7550 8750 50  0001 C CNN
	1    7550 8750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 8950 10450 8950
Wire Wire Line
	9900 9050 9900 8950
$Comp
L Connector:Conn_01x02_Male J22
U 1 1 617614D4
P 10650 8850
F 0 "J22" H 10650 8950 50  0000 C CNN
F 1 "SP2" H 10500 8800 50  0001 C CNN
F 2 "Server.pretty:PhoenixContact_MCV_1,5_2-G-3.81_1x02_P3.81mm_Vertical" H 10650 8850 50  0001 C CNN
F 3 "https://www.devicemart.co.kr/goods/view?no=23260&market=naver&NaPm=ct%3Dkof8h100%7Cci%3D2b06ad3968d8adec0b2b30ae50256e40450772f8%7Ctr%3Dslsc%7Csn%3D876973%7Chk%3Dcbed85e5f251313af0535f600d98c0e674ebf3d9" H 10650 8850 50  0001 C CNN
F 4 "DEVICEMART" H 10650 8850 50  0001 C CNN "Manufacture"
F 5 "100" H 10650 8850 50  0001 C CNN "Price"
F 6 "ECH381V-2P" H 10650 8850 50  0001 C CNN "Value"
	1    10650 8850
	-1   0    0    -1  
$EndComp
Wire Wire Line
	8800 9250 8950 9250
Text GLabel 8800 9250 0    50   Input ~ 0
SP_02
Connection ~ 9350 9250
Wire Wire Line
	9350 9250 9250 9250
Connection ~ 7000 9250
Wire Wire Line
	7000 9250 6900 9250
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R56
U 1 1 6161C30F
P 9450 9600
F 0 "R56" V 9350 9450 50  0000 C CNN
F 1 "103F" V 9450 9450 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 9380 9600 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 9450 9600 50  0001 C CNN
F 4 "20" H 9450 9600 50  0001 C CNN "Price"
F 5 "부품세상" H 9450 9600 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 9450 9600 50  0001 C CNN "Package"
F 7 "10KΩ" H 9450 9600 50  0001 C CNN "Value"
	1    9450 9600
	0    -1   1    0   
$EndComp
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R54
U 1 1 6161C305
P 9250 9250
F 0 "R54" V 9150 9100 50  0000 C CNN
F 1 "102F" V 9250 9100 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 9180 9250 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000614" H 9250 9250 50  0001 C CNN
F 4 "20" H 9250 9250 50  0001 C CNN "Price"
F 5 "부품세상" H 9250 9250 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 9250 9250 50  0001 C CNN "Package"
F 7 "1KΩ" H 9250 9250 50  0001 C CNN "Value"
	1    9250 9250
	0    1    -1   0   
$EndComp
Wire Wire Line
	9350 9600 9350 9250
Wire Wire Line
	9450 9600 9350 9600
Wire Wire Line
	9350 9250 9600 9250
$Comp
L power:GNDPWR #PWR0145
U 1 1 6161C2F6
P 9900 9750
AR Path="/6161C2F6" Ref="#PWR0145"  Part="1" 
AR Path="/5E3D0683/6161C2F6" Ref="#PWR?"  Part="1" 
F 0 "#PWR0145" H 9900 9550 50  0001 C CNN
F 1 "GNDPWR" H 9904 9596 50  0000 C CNN
F 2 "" H 9900 9700 50  0001 C CNN
F 3 "" H 9900 9700 50  0001 C CNN
	1    9900 9750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9900 9450 9900 9600
Wire Wire Line
	9900 9600 9750 9600
Wire Wire Line
	9900 9600 9900 9750
Connection ~ 9900 9600
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R55
U 1 1 61599060
P 7100 9600
F 0 "R55" V 7000 9450 50  0000 C CNN
F 1 "103F" V 7100 9450 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 7030 9600 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 7100 9600 50  0001 C CNN
F 4 "20" H 7100 9600 50  0001 C CNN "Price"
F 5 "부품세상" H 7100 9600 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 7100 9600 50  0001 C CNN "Package"
F 7 "10KΩ" H 7100 9600 50  0001 C CNN "Value"
	1    7100 9600
	0    -1   1    0   
$EndComp
Wire Wire Line
	7000 9600 7000 9250
Wire Wire Line
	7100 9600 7000 9600
Wire Wire Line
	7000 9250 7250 9250
$Comp
L power:GNDPWR #PWR0144
U 1 1 61599047
P 7550 9750
AR Path="/61599047" Ref="#PWR0144"  Part="1" 
AR Path="/5E3D0683/61599047" Ref="#PWR?"  Part="1" 
F 0 "#PWR0144" H 7550 9550 50  0001 C CNN
F 1 "GNDPWR" H 7554 9596 50  0000 C CNN
F 2 "" H 7550 9700 50  0001 C CNN
F 3 "" H 7550 9700 50  0001 C CNN
	1    7550 9750
	1    0    0    -1  
$EndComp
Wire Wire Line
	7550 9450 7550 9600
Wire Wire Line
	7550 9600 7400 9600
Wire Wire Line
	7550 9600 7550 9750
Wire Wire Line
	9500 7800 9800 7800
Wire Wire Line
	9800 8100 10450 8100
Wire Wire Line
	9800 7800 9800 8100
Wire Wire Line
	9800 7650 10450 7650
Wire Wire Line
	9800 7700 9800 7650
Wire Wire Line
	9100 7700 9800 7700
Wire Wire Line
	9700 7200 10450 7200
Wire Wire Line
	9700 7600 9700 7200
Wire Wire Line
	8700 7600 9700 7600
$Comp
L Server-cache:SmartCon_Water_Conn_01x02_Male CON1
U 1 1 61CED1EF
P 5250 3950
F 0 "CON1" H 5350 4200 50  0000 C CNN
F 1 "01x02" H 5350 4100 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm.pretty:PinHeader_1x02_P2.54mm_Vertical" H 5250 3950 50  0001 C CNN
F 3 "" H 5250 3950 50  0001 C CNN
	1    5250 3950
	-1   0    0    -1  
$EndComp
Wire Wire Line
	5050 4050 5050 4300
Text Notes 5300 4000 0    50   ~ 0
CPU
Text Notes 5300 4100 0    50   ~ 0
+12V
$Comp
L Connector:TestPoint_Small TP10
U 1 1 61B61A13
P 2600 2850
F 0 "TP10" H 2550 2800 50  0001 L TNN
F 1 "GND" H 2550 2700 50  0000 L CNN
F 2 "Measurement_Points.pretty:Test_Point_Keystone_5000-5004_Miniature" H 2800 2850 50  0001 C CNN
F 3 "~" H 2800 2850 50  0001 C CNN
	1    2600 2850
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 2850 2600 2700
Wire Wire Line
	2600 2700 2450 2700
Connection ~ 2450 2700
Text Notes 3600 6850 0    50   ~ 0
OPEN : WiFi\nCLOSE : Hot Spot
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R70
U 1 1 61BC9786
P 5000 4700
F 0 "R70" V 5100 4550 50  0000 C CNN
F 1 "473F" V 5000 4550 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 4930 4700 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000607" H 5000 4700 50  0001 C CNN
F 4 "50" H 5000 4700 50  0001 C CNN "Price"
F 5 "부품세상" H 5000 4700 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 5000 4700 50  0001 C CNN "Package"
F 7 "500Ω" H 5000 4700 50  0001 C CNN "Value"
	1    5000 4700
	0    1    1    0   
$EndComp
Text GLabel 4600 4700 0    50   Input ~ 0
RELAY
$Comp
L CamTrol-12Ch-Main-Board-rescue:R-Server_Used-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue-CamTrol-12Ch-Main-Board-rescue R69
U 1 1 61BCA8B1
P 2950 8150
F 0 "R69" V 2850 8000 50  0000 C CNN
F 1 "103F" V 2950 8000 50  0000 C CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 2880 8150 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 2950 8150 50  0001 C CNN
F 4 "20" H 2950 8150 50  0001 C CNN "Price"
F 5 "부품세상" H 2950 8150 50  0001 C CNN "Manufacture"
F 6 "2012(0805)" H 2950 8150 50  0001 C CNN "Package"
F 7 "10KΩ" H 2950 8150 50  0001 C CNN "Value"
	1    2950 8150
	-1   0    0    1   
$EndComp
$Comp
L power:GNDPWR #PWR0174
U 1 1 61BCB57B
P 2950 8150
AR Path="/61BCB57B" Ref="#PWR0174"  Part="1" 
AR Path="/5E3D0683/61BCB57B" Ref="#PWR?"  Part="1" 
F 0 "#PWR0174" H 2950 7950 50  0001 C CNN
F 1 "GNDPWR" H 2954 7996 50  0000 C CNN
F 2 "" H 2950 8100 50  0001 C CNN
F 3 "" H 2950 8100 50  0001 C CNN
	1    2950 8150
	1    0    0    -1  
$EndComp
Text GLabel 2800 7750 0    50   Input ~ 0
RELAY
Wire Wire Line
	3700 7750 2950 7750
Wire Wire Line
	2950 7750 2950 7850
Connection ~ 2950 7750
Wire Wire Line
	2950 7750 2800 7750
Text Notes 3400 4750 0    50   ~ 0
1-2 : WiFi\n3-2 : Switch
Wire Wire Line
	4600 4700 4700 4700
Wire Wire Line
	5000 4700 5100 4700
Wire Wire Line
	5100 4700 5100 4500
Connection ~ 5100 4500
Wire Wire Line
	5100 4500 5600 4500
Wire Wire Line
	2900 4600 2900 4900
Wire Wire Line
	2900 4900 3050 4900
$Comp
L Connector:TestPoint_Small TP11
U 1 1 61C17495
P 3050 4900
F 0 "TP11" H 3000 4850 50  0000 L TNN
F 1 "MANUAL" H 3000 4750 50  0000 L CNN
F 2 "Measurement_Points.pretty:Test_Point_Keystone_5000-5004_Miniature" H 3250 4900 50  0001 C CNN
F 3 "~" H 3250 4900 50  0001 C CNN
	1    3050 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 4500 1900 4500
Wire Wire Line
	1900 4500 1900 5100
Wire Wire Line
	1900 5100 2900 5100
Wire Wire Line
	2900 5100 2900 4900
Connection ~ 2900 4900
$EndSCHEMATC
