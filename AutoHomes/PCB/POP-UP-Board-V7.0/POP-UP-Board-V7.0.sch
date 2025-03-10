EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Clean, Waste water Valve Circuit"
Date "2021-11-29"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Server-cache:HR702-NH-DC12V K1
U 1 1 61A4AA4C
P 5000 2900
F 0 "K1" V 5567 2900 50  0000 C CNN
F 1 "HR702-NH-DC12V" V 5476 2900 50  0000 C CNN
F 2 "Server.pretty:RELAY_10A" H 5450 2850 50  0001 L CNN
F 3 "https://standexelectronics.com/wp-content/uploads/datasheet_reed_relay_DIP.pdf" H 5000 2900 50  0001 C CNN
	1    5000 2900
	0    1    -1   0   
$EndComp
$Comp
L CamTrol-12ChOne-Board-cache:Simulation_SPICE_DIODE D1
U 1 1 61A4B85E
P 5000 3500
F 0 "D1" H 5000 3300 50  0000 C CNN
F 1 "M7" H 5000 3400 50  0000 C CNN
F 2 "Server.pretty:D_SMA" H 5000 3500 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000012566" H 5000 3500 50  0001 C CNN
F 4 "Y" H 5000 3500 50  0001 L CNN "Spice_Netlist_Enabled"
F 5 "D" H 5000 3500 50  0001 L CNN "Spice_Primitive"
F 6 "DO-214AC(SMA)" H 5000 3500 50  0001 C CNN "Package"
	1    5000 3500
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Female J3
U 1 1 61A4E076
P 7100 2750
F 0 "J3" H 7050 2450 50  0000 L CNN
F 1 "Con" H 7050 2550 50  0000 L CNN
F 2 "Server.pretty:USL-5VB1L3-3P" H 7100 2750 50  0001 C CNN
F 3 "~" H 7100 2750 50  0001 C CNN
	1    7100 2750
	1    0    0    1   
$EndComp
$Comp
L Connector:Conn_01x03_Female J1
U 1 1 61A5226B
P 3550 2700
F 0 "J1" H 3450 3000 50  0000 L CNN
F 1 "Con" H 3450 2900 50  0000 L CNN
F 2 "Server.pretty:USL-5VB1L3-3P" H 3550 2700 50  0001 C CNN
F 3 "~" H 3550 2700 50  0001 C CNN
	1    3550 2700
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3750 2600 3950 2600
Wire Wire Line
	4100 2600 4100 2200
Wire Wire Line
	4100 2200 5700 2200
Wire Wire Line
	5700 2200 5700 2650
Wire Wire Line
	5700 2650 6900 2650
Wire Wire Line
	5300 2600 5600 2600
Wire Wire Line
	5600 2600 5600 2750
Wire Wire Line
	5600 2750 6000 2750
Wire Wire Line
	5300 2800 5500 2800
Wire Wire Line
	5500 2800 5500 2850
Wire Wire Line
	5500 2850 6500 2850
Wire Wire Line
	4100 3500 4850 3500
Wire Wire Line
	4700 3100 4100 3100
Wire Wire Line
	4100 3100 4100 3500
Wire Wire Line
	5700 2650 5700 3100
Wire Wire Line
	5700 3500 5150 3500
Connection ~ 5700 2650
Wire Wire Line
	5300 3100 5700 3100
Connection ~ 5700 3100
Wire Wire Line
	5700 3100 5700 3500
$Comp
L Server_Used:LED D3
U 1 1 61A579B6
P 6000 2300
F 0 "D3" V 6039 2183 50  0000 R CNN
F 1 "OPEN" V 5948 2183 50  0000 R CNN
F 2 "Server.pretty:LED_0805" H 6000 2300 50  0001 C CNN
F 3 "http://www.devicemart.co.kr/goods/view?no=30195" H 6000 2300 50  0001 C CNN
	1    6000 2300
	0    -1   -1   0   
$EndComp
$Comp
L Server_Used:R R1
U 1 1 61A59769
P 6000 1850
F 0 "R1" H 6070 1746 50  0000 L CNN
F 1 "203F" H 6070 1655 50  0000 L CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 5930 1700 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 6000 1700 50  0001 C CNN
	1    6000 1850
	1    0    0    -1  
$EndComp
$Comp
L Server_Used:LED D7
U 1 1 61A59E6A
P 6500 2300
F 0 "D7" V 6539 2183 50  0000 R CNN
F 1 "CLOSE" V 6448 2183 50  0000 R CNN
F 2 "Server.pretty:LED_0805" H 6500 2300 50  0001 C CNN
F 3 "http://www.devicemart.co.kr/goods/view?no=30195" H 6500 2300 50  0001 C CNN
	1    6500 2300
	0    -1   -1   0   
$EndComp
$Comp
L Server_Used:R R3
U 1 1 61A59F2C
P 6500 1850
F 0 "R3" H 6570 1746 50  0000 L CNN
F 1 "473F" H 6570 1655 50  0000 L CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 6430 1700 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 6500 1700 50  0001 C CNN
	1    6500 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 2450 6000 2750
Connection ~ 6000 2750
Wire Wire Line
	6000 2750 6900 2750
Wire Wire Line
	6500 2450 6500 2850
Connection ~ 6500 2850
Wire Wire Line
	6500 2850 6900 2850
$Comp
L CamTrol-12ChOne-Board-cache:Simulation_SPICE_DIODE D4
U 1 1 61A5B14A
P 6000 3200
F 0 "D4" V 5950 3050 50  0000 C CNN
F 1 "M7" V 6050 3050 50  0000 C CNN
F 2 "Server.pretty:D_SMA" H 6000 3200 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000012566" H 6000 3200 50  0001 C CNN
F 4 "Y" H 6000 3200 50  0001 L CNN "Spice_Netlist_Enabled"
F 5 "D" H 6000 3200 50  0001 L CNN "Spice_Primitive"
F 6 "DO-214AC(SMA)" H 6000 3200 50  0001 C CNN "Package"
	1    6000 3200
	0    1    1    0   
$EndComp
$Comp
L CamTrol-12ChOne-Board-cache:Simulation_SPICE_DIODE D8
U 1 1 61A5C843
P 6500 3200
F 0 "D8" V 6450 3350 50  0000 C CNN
F 1 "M7" V 6550 3350 50  0000 C CNN
F 2 "Server.pretty:D_SMA" H 6500 3200 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000012566" H 6500 3200 50  0001 C CNN
F 4 "Y" H 6500 3200 50  0001 L CNN "Spice_Netlist_Enabled"
F 5 "D" H 6500 3200 50  0001 L CNN "Spice_Primitive"
F 6 "DO-214AC(SMA)" H 6500 3200 50  0001 C CNN "Package"
	1    6500 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	6000 3050 6000 2750
Wire Wire Line
	6500 3050 6500 2850
Wire Wire Line
	6500 3350 6500 3500
Wire Wire Line
	6500 3500 6000 3500
Connection ~ 5700 3500
Wire Wire Line
	6000 3350 6000 3500
Connection ~ 6000 3500
Wire Wire Line
	6000 3500 5700 3500
Wire Wire Line
	5700 2200 5700 1750
Wire Wire Line
	5700 1750 6000 1750
Wire Wire Line
	6500 1750 6500 1850
Connection ~ 5700 2200
Wire Wire Line
	6000 1850 6000 1750
Connection ~ 6000 1750
$Comp
L Server-cache:HR702-NH-DC12V K2
U 1 1 61A5F52F
P 5000 5000
F 0 "K2" V 5567 5000 50  0000 C CNN
F 1 "HR702-NH-DC12V" V 5476 5000 50  0000 C CNN
F 2 "Server.pretty:RELAY_10A" H 5450 4950 50  0001 L CNN
F 3 "https://standexelectronics.com/wp-content/uploads/datasheet_reed_relay_DIP.pdf" H 5000 5000 50  0001 C CNN
	1    5000 5000
	0    1    -1   0   
$EndComp
$Comp
L CamTrol-12ChOne-Board-cache:Simulation_SPICE_DIODE D2
U 1 1 61A5F65C
P 5000 5600
F 0 "D2" H 5000 5400 50  0000 C CNN
F 1 "M7" H 5000 5500 50  0000 C CNN
F 2 "Server.pretty:D_SMA" H 5000 5600 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000012566" H 5000 5600 50  0001 C CNN
F 4 "Y" H 5000 5600 50  0001 L CNN "Spice_Netlist_Enabled"
F 5 "D" H 5000 5600 50  0001 L CNN "Spice_Primitive"
F 6 "DO-214AC(SMA)" H 5000 5600 50  0001 C CNN "Package"
	1    5000 5600
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Female J4
U 1 1 61A5F666
P 7100 4850
F 0 "J4" H 7050 4550 50  0000 L CNN
F 1 "Con" H 7050 4650 50  0000 L CNN
F 2 "Server.pretty:USL-5VB1L3-3P" H 7100 4850 50  0001 C CNN
F 3 "~" H 7100 4850 50  0001 C CNN
	1    7100 4850
	1    0    0    1   
$EndComp
$Comp
L Connector:Conn_01x03_Female J2
U 1 1 61A5F670
P 3550 4800
F 0 "J2" H 3450 5100 50  0000 L CNN
F 1 "Con" H 3450 5000 50  0000 L CNN
F 2 "Server.pretty:USL-5VB1L3-3P" H 3550 4800 50  0001 C CNN
F 3 "~" H 3550 4800 50  0001 C CNN
	1    3550 4800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3750 4700 4100 4700
Wire Wire Line
	4100 4700 4100 4300
Wire Wire Line
	4100 4300 5700 4300
Wire Wire Line
	5700 4300 5700 4750
Wire Wire Line
	5700 4750 6900 4750
Wire Wire Line
	5300 4700 5600 4700
Wire Wire Line
	5600 4700 5600 4850
Wire Wire Line
	5600 4850 6000 4850
Wire Wire Line
	5300 4900 5500 4900
Wire Wire Line
	5500 4900 5500 4950
Wire Wire Line
	5500 4950 6500 4950
Wire Wire Line
	4100 5600 4850 5600
Wire Wire Line
	4700 5200 4100 5200
Wire Wire Line
	4100 5200 4100 5600
Wire Wire Line
	5700 4750 5700 5200
Wire Wire Line
	5700 5600 5150 5600
Connection ~ 5700 4750
Wire Wire Line
	5300 5200 5700 5200
Connection ~ 5700 5200
Wire Wire Line
	5700 5200 5700 5600
$Comp
L Server_Used:LED D5
U 1 1 61A5F6D5
P 6000 4400
F 0 "D5" V 6039 4283 50  0000 R CNN
F 1 "OPEN" V 5948 4283 50  0000 R CNN
F 2 "Server.pretty:LED_0805" H 6000 4400 50  0001 C CNN
F 3 "http://www.devicemart.co.kr/goods/view?no=30195" H 6000 4400 50  0001 C CNN
	1    6000 4400
	0    -1   -1   0   
$EndComp
$Comp
L Server_Used:R R2
U 1 1 61A5F6DF
P 6000 3950
F 0 "R2" H 6070 3846 50  0000 L CNN
F 1 "203F" H 6070 3755 50  0000 L CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 5930 3800 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 6000 3800 50  0001 C CNN
	1    6000 3950
	1    0    0    -1  
$EndComp
$Comp
L Server_Used:LED D9
U 1 1 61A5F6E9
P 6500 4400
F 0 "D9" V 6539 4283 50  0000 R CNN
F 1 "CLOSE" V 6448 4283 50  0000 R CNN
F 2 "Server.pretty:LED_0805" H 6500 4400 50  0001 C CNN
F 3 "http://www.devicemart.co.kr/goods/view?no=30195" H 6500 4400 50  0001 C CNN
	1    6500 4400
	0    -1   -1   0   
$EndComp
$Comp
L Server_Used:R R4
U 1 1 61A5F6F3
P 6500 3950
F 0 "R4" H 6570 3846 50  0000 L CNN
F 1 "473F" H 6570 3755 50  0000 L CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 6430 3800 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 6500 3800 50  0001 C CNN
	1    6500 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6000 4550 6000 4850
Connection ~ 6000 4850
Wire Wire Line
	6000 4850 6900 4850
Wire Wire Line
	6500 4550 6500 4950
Connection ~ 6500 4950
Wire Wire Line
	6500 4950 6900 4950
$Comp
L CamTrol-12ChOne-Board-cache:Simulation_SPICE_DIODE D6
U 1 1 61A5F706
P 6000 5300
F 0 "D6" V 5950 5150 50  0000 C CNN
F 1 "M7" V 6050 5150 50  0000 C CNN
F 2 "Server.pretty:D_SMA" H 6000 5300 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000012566" H 6000 5300 50  0001 C CNN
F 4 "Y" H 6000 5300 50  0001 L CNN "Spice_Netlist_Enabled"
F 5 "D" H 6000 5300 50  0001 L CNN "Spice_Primitive"
F 6 "DO-214AC(SMA)" H 6000 5300 50  0001 C CNN "Package"
	1    6000 5300
	0    1    1    0   
$EndComp
$Comp
L CamTrol-12ChOne-Board-cache:Simulation_SPICE_DIODE D10
U 1 1 61A5F763
P 6500 5300
F 0 "D10" V 6450 5450 50  0000 C CNN
F 1 "M7" V 6550 5450 50  0000 C CNN
F 2 "Server.pretty:D_SMA" H 6500 5300 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000012566" H 6500 5300 50  0001 C CNN
F 4 "Y" H 6500 5300 50  0001 L CNN "Spice_Netlist_Enabled"
F 5 "D" H 6500 5300 50  0001 L CNN "Spice_Primitive"
F 6 "DO-214AC(SMA)" H 6500 5300 50  0001 C CNN "Package"
	1    6500 5300
	0    1    1    0   
$EndComp
Wire Wire Line
	6000 5150 6000 4850
Wire Wire Line
	6500 5150 6500 4950
Wire Wire Line
	6500 5450 6500 5600
Wire Wire Line
	6500 5600 6000 5600
Connection ~ 5700 5600
Wire Wire Line
	6000 5450 6000 5600
Connection ~ 6000 5600
Wire Wire Line
	6000 5600 5700 5600
Wire Wire Line
	5700 4300 5700 3850
Wire Wire Line
	5700 3850 6000 3850
Wire Wire Line
	6500 3850 6500 3950
Connection ~ 5700 4300
Wire Wire Line
	6000 3950 6000 3850
Connection ~ 6000 3850
Wire Wire Line
	6000 3850 6500 3850
$Comp
L Mechanical:MountingHole H1
U 1 1 61A920C9
P 4200 1700
F 0 "H1" H 4300 1746 50  0000 L CNN
F 1 "MH" H 4300 1655 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 4200 1700 50  0001 C CNN
F 3 "~" H 4200 1700 50  0001 C CNN
	1    4200 1700
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 61A92A0F
P 4550 1700
F 0 "H2" H 4650 1746 50  0000 L CNN
F 1 "MH" H 4650 1655 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 4550 1700 50  0001 C CNN
F 3 "~" H 4550 1700 50  0001 C CNN
	1    4550 1700
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 61A92C9F
P 4900 1700
F 0 "H3" H 5000 1746 50  0000 L CNN
F 1 "MH" H 5000 1655 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 4900 1700 50  0001 C CNN
F 3 "~" H 4900 1700 50  0001 C CNN
	1    4900 1700
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 61A92E9F
P 5250 1700
F 0 "H4" H 5350 1746 50  0000 L CNN
F 1 "MH" H 5350 1655 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 5250 1700 50  0001 C CNN
F 3 "~" H 5250 1700 50  0001 C CNN
	1    5250 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 5200 4100 4800
Wire Wire Line
	4100 4800 3750 4800
Connection ~ 4100 5200
Wire Wire Line
	3750 4900 4350 4900
Wire Wire Line
	4350 4900 4350 4800
Wire Wire Line
	4350 4800 4700 4800
Wire Wire Line
	4100 3100 4100 2700
Wire Wire Line
	4100 2700 3750 2700
Connection ~ 4100 3100
Wire Wire Line
	3750 2800 4350 2800
Wire Wire Line
	4350 2800 4350 2700
Wire Wire Line
	4350 2700 4700 2700
$Comp
L power:GNDPWR #PWR0101
U 1 1 61ACFC91
P 4350 2900
F 0 "#PWR0101" H 4350 2700 50  0001 C CNN
F 1 "GNDPWR" H 4354 2746 50  0000 C CNN
F 2 "" H 4350 2850 50  0001 C CNN
F 3 "" H 4350 2850 50  0001 C CNN
	1    4350 2900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 2800 4350 2900
Connection ~ 4350 2800
$Comp
L power:GNDPWR #PWR0102
U 1 1 61AD2DD3
P 4350 5000
F 0 "#PWR0102" H 4350 4800 50  0001 C CNN
F 1 "GNDPWR" H 4354 4846 50  0000 C CNN
F 2 "" H 4350 4950 50  0001 C CNN
F 3 "" H 4350 4950 50  0001 C CNN
	1    4350 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 4900 4350 5000
Connection ~ 4350 4900
$Comp
L Mechanical:MountingHole H5
U 1 1 61AD609F
P 4200 1950
F 0 "H5" H 4300 1996 50  0000 L CNN
F 1 "MH" H 4300 1905 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 4200 1950 50  0001 C CNN
F 3 "~" H 4200 1950 50  0001 C CNN
	1    4200 1950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H6
U 1 1 61AD60A5
P 4550 1950
F 0 "H6" H 4650 1996 50  0000 L CNN
F 1 "MH" H 4650 1905 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 4550 1950 50  0001 C CNN
F 3 "~" H 4550 1950 50  0001 C CNN
	1    4550 1950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H7
U 1 1 61AD60AB
P 4900 1950
F 0 "H7" H 5000 1996 50  0000 L CNN
F 1 "MH" H 5000 1905 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 4900 1950 50  0001 C CNN
F 3 "~" H 4900 1950 50  0001 C CNN
	1    4900 1950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H8
U 1 1 61AD60B1
P 5250 1950
F 0 "H8" H 5350 1996 50  0000 L CNN
F 1 "MH" H 5350 1905 50  0000 L CNN
F 2 "Server.pretty:MountingHole_3.2mm_M3" H 5250 1950 50  0001 C CNN
F 3 "~" H 5250 1950 50  0001 C CNN
	1    5250 1950
	1    0    0    -1  
$EndComp
$Comp
L Server_Used:LED D11
U 1 1 61DC5F5F
P 3650 3800
F 0 "D11" V 3689 3683 50  0000 R CNN
F 1 "POWER" V 3598 3683 50  0000 R CNN
F 2 "Server.pretty:LED_0805" H 3650 3800 50  0001 C CNN
F 3 "http://www.devicemart.co.kr/goods/view?no=30195" H 3650 3800 50  0001 C CNN
	1    3650 3800
	0    1    -1   0   
$EndComp
$Comp
L Server_Used:R R5
U 1 1 61DC5F65
P 3650 3350
F 0 "R5" H 3720 3246 50  0000 L CNN
F 1 "203F" H 3720 3155 50  0000 L CNN
F 2 "Server.pretty:R_0805_HandSoldering" V 3580 3200 50  0001 C CNN
F 3 "https://www.partsworld.co.kr/goods/goods_view.php?goodsNo=1000000637" H 3650 3200 50  0001 C CNN
	1    3650 3350
	-1   0    0    -1  
$EndComp
$Comp
L power:GNDPWR #PWR?
U 1 1 61DDC5A9
P 3650 4100
F 0 "#PWR?" H 3650 3900 50  0001 C CNN
F 1 "GNDPWR" H 3654 3946 50  0000 C CNN
F 2 "" H 3650 4050 50  0001 C CNN
F 3 "" H 3650 4050 50  0001 C CNN
	1    3650 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 3850 7350 3850
Wire Wire Line
	7350 3850 7350 1750
Wire Wire Line
	6000 1750 6500 1750
Connection ~ 6500 3850
Connection ~ 6500 1750
Wire Wire Line
	6500 1750 7350 1750
Wire Wire Line
	3650 3100 3950 3100
Wire Wire Line
	3950 3100 3950 2600
Wire Wire Line
	3650 3100 3650 3350
Connection ~ 3950 2600
Wire Wire Line
	3950 2600 4100 2600
Wire Wire Line
	3650 4100 3650 3950
$EndSCHEMATC
