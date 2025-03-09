// ignore_for_file: prefer_const_constructors

import 'package:flutter_analog_clock/flutter_analog_clock.dart';
import 'package:flutter_iot_wifi/flutter_iot_wifi.dart';
import 'package:permission_handler/permission_handler.dart';
import 'dart:developer';
import 'dart:io';
import 'package:qr_code_scanner/qr_code_scanner.dart';
import 'variable.dart';
import 'dart:convert';
import 'dart:async';
import 'package:audioplayers/audioplayers.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:get/get.dart';
import 'package:dio/dio.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:url_launcher/url_launcher.dart';
import 'package:slide_digital_clock/slide_digital_clock.dart';
import 'package:intl/intl.dart';

late Timer timer;

String stationIp = '192.168.0.100'; // Hot Spot Mode 일때 IP Address...
String aPIp = 'http://192.168.4.1/'; // AP Mode 일때 IP Address...
String s = ' ';

String voltPng = 'assets/images/car-battery.png';
String tempPng = 'assets/images/temp.png';

String voltText = ' ';
String tempText = ' ';
dynamic recvMessage;
dynamic message;

List<int> pwm = [0, 0, 0, 0, 0, 0, 0, 0, 0];

late double dispHeight;
late double dispWidth;
late double pixelRatio;
late double txtScale;

bool wifiSelect = true;
bool statusFlag = false;
bool mobile = true; // true : 휴대폰, false : 태블릿
bool lteFlag = false;
bool sleepModeFlag = true;
bool sleepFlag = false;
bool sleepFlagReset = true;
bool wifiConFlag = false;
bool networkSelect = false; // true : Hot Spot, False : WiFi
bool qrOkFlag = false; // QR 코드로 정상 접속 되면 true...

// bool carFlag = false;

bool displayDir = true;
// bool sensorChgFlag = false;

bool netFlag = false;
// bool factoryMode = false;

// bool isSwitched = false;

bool remoteStatus = false;
bool allLedStatus = false;
bool invertorStatus = false;
bool tvStatus = false;
bool frigeStatus = false;
bool popUpStatus = false;
bool popDownStatus = false;
bool stopStatus = true;

Server server = Server();

Color portraitBackColor = const Color(0xff333333);

Color activeBoarderOutLineColor = Colors.white;
Color inActiveBoarderOutLineColor = Colors.white;

Color activeColor = Colors.white;
Color inActiveColor = Color.fromARGB(255, 171, 171, 171);

const Color appBarColor = Color(0x552C1FFF); //FF39F1
const Color titleColor = Colors.white;

const Color slideFontColor = Colors.white;
const Color portraitSlideColor = Colors.white;

const Color ledOnColor = Color(0xFF6B56F5);
const Color ledOffColor = Colors.lime;

const Color outOnColor = Color(0xFF6B56F5);
const Color outOffColor = Colors.lime; //6F58FF;

// const Color cleanBarColor = Colors.blue;
// const Color cleanBackColor = Color(0x80F8FFFE);
// const Color wasteBarColor = Colors.redAccent;
// const Color wasteBackColor = Color(0x80F8FFFE);

bool beep = true;

int wifiCount = 10;
int wifiConCount = 0;

double boardLine = 0.0026;
double borderRadius = 0.01;
double buttonRadius = 0.006;
double appBarHeight = 0.078;

Color batColor = Colors.white;

bool wiFiCheck = false;

AudioPlayer advancedPlayer = AudioPlayer();

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  @override
  Widget build(BuildContext context) {
    return GetMaterialApp(
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.indigo),
        useMaterial3: true,
      ),
      initialRoute: "/",
      getPages: [
        GetPage(
            name: "/", page: () => MyHomePage(), transition: Transition.fadeIn),
        GetPage(
            name: '/setting',
            page: () => Setting(),
            transition: Transition.fadeIn),
        GetPage(
            name: '/wifichange',
            page: () => WiFiChange(),
            transition: Transition.fadeIn),
        GetPage(
            name: '/sleepmode',
            page: () => SleepMode(),
            transition: Transition.fadeIn),
        GetPage(
            name: '/phoneNumber',
            page: () => PhoneNumber(),
            transition: Transition.fadeIn),
        GetPage(
            name: '/qrCodeRead',
            page: () => QrCode(),
            transition: Transition.fadeIn),
      ],
      debugShowCheckedModeBanner: false,
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({Key? key}) : super(key: key);

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  Future<bool> _checkPermissions() async {
    if (await Permission.location.request().isGranted) {
      return true;
    }
    return false;
  }

  void _connect() async {
    if (await _checkPermissions()) {
      FlutterIotWifi.connect(V.hotSpotSSID, V.hotSpotPW, prefix: true)
          .then((value) => print("connect initiated: $value"));
      if (networkSelect) {
      } else {
        FlutterIotWifi.connect(V.conSSID, V.conPW, prefix: true)
            .then((value) => print("connect initiated: $value"));
      }
    } else {
      print("don't have permission");
    }
  }

  @override
  void initState() {
    _loadPrefs();
    SystemChrome.setEnabledSystemUIMode(SystemUiMode.manual, overlays: []);

    timer = Timer.periodic(const Duration(milliseconds: 500), (timer) {
      wiFiCheck = true;
      wifiCount++;

      if (!netFlag) {
        wifiConCount++;
      }

      if (wifiConCount > 50) {
        wifiConCount = 0;
        wifiConFlag = false;
      }

      if (networkSelect) {
        if (!wifiConFlag &&
            !netFlag &&
            V.hotSpotSSID != " " &&
            V.hotSpotPW != " ") {
          if (wifiConCount == 20) {
            _connect();
            wifiConFlag = true;
          }
        }
      } else {
        if (!wifiConFlag && !netFlag && V.conSSID != " " && V.conPW != " ") {
          if (wifiConCount == 20) {
            _connect();
            wifiConFlag = true;
          }
        }
      }

// 휴대폰에서 Sleep Button 이 눌리면 실행...
      if (sleepFlagReset == true && sleepFlag == true && mobile == false) {
        sleepFlagReset = false;
        Get.toNamed('/sleepmode');
      } else if (sleepFlagReset == false &&
          sleepFlag == false &&
          mobile == false) {
        sleepFlagReset = true;
        Get.back();
      }

      if (wifiCount > 3) {
        netFlag = false;
        // wifiConFlag = false;
        remoteStatus = false;
        allLedStatus = false;
        invertorStatus = false;
        tvStatus = false;
        frigeStatus = false;
        popUpStatus = false;
        popDownStatus = false;
        stopStatus = false;
        V.ledStatus[1] = false;
        V.ledStatus[2] = false;
        V.ledStatus[3] = false;
        V.ledStatus[4] = false;
        V.ledStatus[5] = false;
        V.ledStatus[6] = false;
        V.ledStatus[7] = false;
        V.ledStatus[8] = false;
        setState(() {});
        if (wifiCount > 100) {
          wifiCount = 10;
        }
      } else {
        netFlag = true;
        // wifiConFlag = true;
      }

      print("wifiCount : $wifiCount");

      print("netFlag : $netFlag");
      print("V.btVolt : ${(V.btVolt).toString()} V");

      voltText = '${(V.btVolt).toString()} V';
      tempText = '${V.temperature.toString()} ℃ ';
      server.getReq("request", 0);
      if (netFlag && qrOkFlag) {
        server.getReq("QR_OK", 1);
        qrOkFlag = false;
      }
      setState(() {});
    });
    super.initState();
  }

  void _loadPrefs() async {
    print("_loadPrefs Page in");
    if (beep) await advancedPlayer.play(AssetSource('sound/intro.mp3'));
    final SharedPreferences prefs = await SharedPreferences.getInstance();

    // File에 최초 저장시 null 대응...
    print('');
    print('ledName[1] : ${V.ledName[1]}');
    print('ledName[2] : ${V.ledName[2]}');
    print('ledName[3] : ${V.ledName[3]}');
    print('ledName[4] : ${V.ledName[4]}');
    print('ledName[5] : ${V.ledName[5]}');
    print('ledName[6] : ${V.ledName[6]}');
    print('ledName[7] : ${V.ledName[7]}');
    print('ledName[8] : ${V.ledName[8]}');
    print('');
    // carFlag = prefs.getBool('carFlag') ?? false;

    V.ledName[1] = prefs.getString('led1SaveName') ?? V.ledPushName[1];
    V.ledName[2] = prefs.getString('led2SaveName') ?? V.ledPushName[2];
    V.ledName[3] = prefs.getString('led3SaveName') ?? V.ledPushName[3];
    V.ledName[4] = prefs.getString('led4SaveName') ?? V.ledPushName[4];
    V.ledName[5] = prefs.getString('led5SaveName') ?? V.ledPushName[5];
    V.ledName[6] = prefs.getString('led6SaveName') ?? V.ledPushName[6];
    V.ledName[7] = prefs.getString('led7SaveName') ?? V.ledPushName[7];
    V.ledName[8] = prefs.getString('led8SaveName') ?? V.ledPushName[8];

    print('ledName[1] : ${V.ledName[1]}');
    print('ledName[2] : ${V.ledName[2]}');
    print('ledName[3] : ${V.ledName[3]}');
    print('ledName[4] : ${V.ledName[4]}');
    print('ledName[5] : ${V.ledName[5]}');
    print('ledName[6] : ${V.ledName[6]}');
    print('ledName[7] : ${V.ledName[7]}');
    print('ledName[8] : ${V.ledName[8]}');

    networkSelect = prefs.getBool('networkSelect') ?? false;

    // if (networkSelect) {
    V.hotSpotSSID = (prefs.getString('ssidPush') ?? "CamTrol V6.0-A001");
    V.hotSpotPW = (prefs.getString('pwPush') ?? "01234567");
    // } else {
    V.conSSID = (prefs.getString('ssidPush') ?? "CamTrol V6.0-A001");
    V.conPW = (prefs.getString('pwPush') ?? "01234567");
    // }

    beep = prefs.getBool('beep') ?? true;
    mobile = prefs.getBool('mobile') ?? true;
    displayDir = prefs.getBool('displayDir') ?? true;
    sleepModeFlag = prefs.getBool('sleepModeFlag') ?? true;
    stationIp = prefs.getString('stationIp') ?? '192.168.0.100';

    // isSwitched = prefs.getBool('isSwitched') ?? isSwitched;

    V.isPhoneTable[1] = prefs.getBool('isPhoneTableFlag1') ?? true;
    V.isPhoneTable[2] = prefs.getBool('isPhoneTableFlag2') ?? false;
    V.isPhoneTable[3] = prefs.getBool('isPhoneTableFlag3') ?? false;
    V.isPhoneTable[4] = prefs.getBool('isPhoneTableFlag4') ?? false;
    V.isPhoneTable[5] = prefs.getBool('isPhoneTableFlag5') ?? false;

    V.phoneName[1] = prefs.getString('phoneName1') ?? '연락처 1 입력';
    V.phoneName[2] = prefs.getString('phoneName2') ?? '연락처 2 입력';
    V.phoneName[3] = prefs.getString('phoneName3') ?? '연락처 3 입력';
    V.phoneName[4] = prefs.getString('phoneName4') ?? '연락처 4 입력';
    V.phoneName[5] = prefs.getString('phoneName5') ?? '연락처 5 입력';

    V.phoneNumber[1] = prefs.getString('phoneNumber1') ?? '전화번호 입력';
    V.phoneNumber[2] = prefs.getString('phoneNumber2') ?? '전화번호 입력';
    V.phoneNumber[3] = prefs.getString('phoneNumber3') ?? '전화번호 입력';
    V.phoneNumber[4] = prefs.getString('phoneNumber4') ?? '전화번호 입력';
    V.phoneNumber[5] = prefs.getString('phoneNumber5') ?? '전화번호 입력';
    print("_loadPrefs Page Out");
  }

  void handleTimeout() async {
    print("handleTimeout Empty...");
  }

  @override
  Widget build(BuildContext context) {
    dispHeight = MediaQuery.of(context).size.height;
    dispWidth = MediaQuery.of(context).size.width;
    pixelRatio = MediaQuery.of(context).devicePixelRatio;
    txtScale = MediaQuery.of(context).textScaleFactor;

    var rowSize = MediaQuery.of(context).textScaleFactor;

    if (rowSize <= 1.0) {
      V.textScaleRatio = 1.0;
    } else if (rowSize > 1.0) {
      V.textScaleRatio = 0.7;
    }

    if (pixelRatio > 2.3) {
      mobile = true;
    } else {
      mobile = false;
    }

    mobile
        ? SystemChrome.setPreferredOrientations([DeviceOrientation.portraitUp])
        : displayDir
            ? SystemChrome.setPreferredOrientations(
                [DeviceOrientation.landscapeRight])
            : SystemChrome.setPreferredOrientations(
                [DeviceOrientation.landscapeLeft]);

    return mobile ? PortScreen() : LandScreen();
  }
}

class QrCode extends StatefulWidget {
  const QrCode({Key? key}) : super(key: key);

  @override
  State<QrCode> createState() => _QrCodeState();
}

class _QrCodeState extends State<QrCode> {
  Barcode? result;
  late String? qrString = ' ';
  late List<String> convertString;
  QRViewController? controller;
  final GlobalKey qrKey = GlobalKey(debugLabel: 'QR');

  // In order to get hot reload to work we need to pause the camera if the platform
  // is android, or resume the camera if the platform is iOS.
  @override
  void reassemble() {
    super.reassemble();
    if (Platform.isAndroid) {
      controller!.pauseCamera();
    }
    controller!.resumeCamera();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: PreferredSize(
        preferredSize: Size.fromHeight(
          mobile
              ? (dispHeight * appBarHeight) * 0.6
              : (dispHeight * appBarHeight),
        ),
        child: AppBar(
          backgroundColor: appBarColor,
          iconTheme: const IconThemeData(
            color: Color(0xD0574B4C), //change your color here
          ),
          title: Text(
            'QR Code Scan ...',
            style: TextStyle(
              fontWeight: FontWeight.bold,
              fontStyle: FontStyle.italic,
              color: Colors.black,
              fontSize: mobile
                  ? ((dispWidth * 0.03) / txtScale)
                  : ((dispHeight * 0.04) / txtScale),
            ),
          ),
        ), // 공통으로 적용...
      ),
      body: Column(
        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
        children: <Widget>[
          SizedBox(
              height: dispHeight * 0.6, // 첫번째 Container 높이
              child: _buildQrView(context)),
          (result != null)
              ? Text('SSID : ${convertString[3]}\n PW : ${convertString[4]}',
                  style: TextStyle(
                    color: Colors.black,
                    fontWeight: FontWeight.bold,
                    fontSize: mobile
                        ? (dispHeight * 0.03) / txtScale
                        : (dispHeight * 0.03) / txtScale,
                  ))
              : Text('QR 코드를 스캔 해 주세요.',
                  style: TextStyle(
                    color: Colors.black,
                    fontWeight: FontWeight.bold,
                    fontSize: mobile
                        ? (dispHeight * 0.03) / txtScale
                        : (dispHeight * 0.04) / txtScale,
                  )),
          SizedBox(
            width: mobile ? dispWidth * 0.35 : dispWidth * 0.15,
            height: mobile ? dispHeight * 0.05 : dispHeight * 0.06,
            child: ElevatedButton(
              onPressed: () async {
                if (beep) {
                  await advancedPlayer.play(AssetSource('sound/iconClick.mp3'));
                }
                await controller?.toggleFlash();
                setState(() {});
              },
              child: FutureBuilder(
                future: controller?.getFlashStatus(),
                builder: (context, snapshot) {
                  // return Text('Flash: ${snapshot.data}');
                  return Text(
                    snapshot.data == false ? '조명 : 켜기' : '조명 : 끄기',
                    style: TextStyle(
                      color: Colors.black,
                      fontWeight: FontWeight.bold,
                      fontSize: mobile
                          ? (dispWidth * 0.04) / txtScale
                          : (dispWidth * 0.02) / txtScale,
                    ),
                  );
                },
              ),
            ),
          ),
          SizedBox(
            width: mobile ? dispWidth * 0.35 : dispWidth * 0.1,
            height: mobile ? dispHeight * 0.05 : dispHeight * 0.06,
            child: ElevatedButton(
                child: Text(
                  "저장",
                  style: TextStyle(
                    color: Colors.black,
                    fontWeight: FontWeight.bold,
                    fontSize: mobile
                        ? (dispWidth * 0.04) / txtScale
                        : (dispWidth * 0.02) / txtScale,
                  ),
                ),
                onPressed: () async {
                  qrOkFlag = true;
                  showDialog(
                    context: context,
                    builder: (context) => AlertDialog(
                      backgroundColor: const Color(0xC0D4AA39),
                      title: Text(
                        result != null ? "변경된 내용을 저장 합니다." : "변경된 내용이 없습니다.",
                        style: TextStyle(
                          color: Colors.white,
                          fontSize: mobile
                              ? (dispWidth * 0.045) / txtScale
                              : (dispHeight * 0.035) / txtScale,
                        ),
                      ),
                      actions: <Widget>[
                        // ignore: deprecated_member_use
                        SizedBox(
                          width: mobile ? dispWidth * 0.20 : dispWidth * 0.09,
                          height:
                              mobile ? dispHeight * 0.045 : dispHeight * 0.07,
                          child: ElevatedButton(
                              child: Text(
                                "예",
                                style: TextStyle(
                                  color: Colors.black,
                                  fontWeight: FontWeight.bold,
                                  fontSize: mobile
                                      ? (dispWidth * 0.04) / txtScale
                                      : (dispHeight * 0.03) / txtScale,
                                ),
                              ),
                              onPressed: () async {
                                SharedPreferences prefs =
                                    await SharedPreferences.getInstance();
                                setState(() {
                                  if (result != null) {
                                    if (networkSelect) {
                                      V.hotSpotSSID = convertString[3];
                                    } else {
                                      V.conSSID = convertString[3];
                                    }
                                    prefs.setString(
                                        'ssidPush', convertString[3]);
                                    if (networkSelect) {
                                      V.hotSpotPW = convertString[4];
                                    } else {
                                      V.conPW = convertString[4];
                                    }
                                    prefs.setString('pwPush', convertString[4]);
                                  }
                                });
                                if (beep) {
                                  advancedPlayer
                                      .play(AssetSource('sound/iconClick.mp3'));
                                }
                                Get.back();
                              }),
                        ),
                        SizedBox(
                            width:
                                mobile ? dispHeight * 0.04 : dispWidth * 0.04),
                        // ignore: deprecated_member_use
                        SizedBox(
                          width: mobile ? dispWidth * 0.25 : dispWidth * 0.13,
                          height:
                              mobile ? dispHeight * 0.045 : dispHeight * 0.07,
                          child: ElevatedButton(
                              child: Text(
                                "아니요",
                                style: TextStyle(
                                  color: Colors.black,
                                  fontWeight: FontWeight.bold,
                                  fontSize: mobile
                                      ? (dispWidth * 0.04) / txtScale
                                      : (dispHeight * 0.03) / txtScale,
                                ),
                              ),
                              onPressed: () {
                                if (beep) {
                                  advancedPlayer
                                      .play(AssetSource('sound/iconClick.mp3'));
                                }
                                Get.back();
                              }),
                        ),
                      ],
                    ),
                  );
                  if (beep) {
                    await advancedPlayer.play(AssetSource('sound/intro.mp3'));
                  }
                }),
          )
        ],
      ),
    );
  }

  Widget _buildQrView(BuildContext context) {
    // For this example we check how width or tall the device is and change the scanArea and overlay accordingly.
    var scanArea = (MediaQuery.of(context).size.width < 400 ||
            MediaQuery.of(context).size.height < 400)
        ? 150.0
        : 300.0;
    // To ensure the Scanner view is properly sizes after rotation
    // we need to listen for Flutter SizeChanged notification and update controller
    return QRView(
      key: qrKey,
      onQRViewCreated: _onQRViewCreated,
      overlay: QrScannerOverlayShape(
          borderColor: Colors.red,
          borderRadius: 10,
          borderLength: 30,
          borderWidth: 10,
          cutOutSize: scanArea),
      onPermissionSet: (ctrl, p) => _onPermissionSet(context, ctrl, p),
    );
  }

  void _onQRViewCreated(QRViewController controller) {
    setState(() {
      this.controller = controller;
    });
    controller.scannedDataStream.listen((scanData) {
      setState(() {
        result = scanData;
        qrString = result!.code as String;
        convertString = qrString!.split(":");
        convertString[3] =
            convertString[3].substring(0, (convertString[3].length) - 2);
        if (convertString[3] == "WPA") {
          convertString[3] =
              convertString[2].substring(0, (convertString[2].length) - 2);
        }
        convertString[4] =
            convertString[4].substring(0, (convertString[4].length) - 2);
      });
    });
  }

  void _onPermissionSet(BuildContext context, QRViewController ctrl, bool p) {
    log('${DateTime.now().toIso8601String()}_onPermissionSet $p');
    if (!p) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('no Permission')),
      );
    }
  }

  @override
  void dispose() {
    controller?.dispose();
    super.dispose();
  }
}

class SleepMode extends StatefulWidget {
  const SleepMode({Key? key}) : super(key: key);

  @override
  State<SleepMode> createState() => _SleepModeState();
}

class _SleepModeState extends State<SleepMode> {
  @override
  void initState() {
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0x80574B4C),
      appBar: PreferredSize(
        preferredSize: Size.fromHeight(
          dispHeight * appBarHeight,
        ),
        child: AppBar(
          backgroundColor: const Color(0x80574B4C),
          iconTheme: const IconThemeData(
            color: Color(0xD0574B4C), //change your color here
          ),
          title: Text(
            '취침 모드...',
            style: TextStyle(
              fontWeight: FontWeight.bold,
              fontStyle: FontStyle.italic,
              color: Colors.black,
              fontSize: mobile
                  ? ((dispWidth * 0.03) / txtScale)
                  : ((dispHeight * 0.04) / txtScale),
            ),
          ),
          actions: <Widget>[
            IconButton(
              icon: Icon(Icons.settings,
                  size: mobile ? dispWidth * 0.045 : dispHeight * 0.05,
                  color: Colors.black),
              tooltip: '모드 설정',
              onPressed: () async {
                SharedPreferences prefs = await SharedPreferences.getInstance();
                sleepModeFlag = !sleepModeFlag;
                prefs.setBool('sleepModeFlag', sleepModeFlag);
                setState(() {});
              },
            ),
            SizedBox(
              width: dispHeight * 0.02,
            ),
          ],
        ), // 공통으로 적용...
      ),
      body: InkWell(
        onTap: () async {
          if (beep) {
            await advancedPlayer.play(AssetSource('sound/dongdong.mp3'));
          }
          await server.getReq("SLEEP", 0);
          sleepFlag == false;
          Get.back();
        },
        child: Padding(
          padding: sleepModeFlag
              ? const EdgeInsets.all(20.0)
              : const EdgeInsets.all(0.0),
          child: sleepModeFlag
              ? FlutterAnalogClock(
                  dateTime: DateTime.now(),
                  dialPlateColor: Color(0x40574B4C),
                  hourHandColor: Color(0x803B5C4F), // 시침 색상...
                  minuteHandColor: Color(0x803B5C4F), // 분침 색상...
                  secondHandColor: Color(0x80D11F22), // 초침 색상
                  numberColor: Colors.black, // 시간 색상...
                  borderColor: Colors.black,
                  tickColor: Colors.black,
                  centerPointColor: Colors.black,
                  showBorder: true,
                  showTicks: true,
                  showMinuteHand: true,
                  showSecondHand: true,
                  showNumber: true,
                  borderWidth: 5.0,
                  hourNumberScale: 0.6,
                  hourNumbers: const [
                    '1',
                    '2',
                    '3',
                    '4',
                    '5',
                    '6',
                    '7',
                    '8',
                    '9',
                    '10',
                    '11',
                    '12'
                  ],
                )
              : Container(color: Colors.black),
        ),
      ),
    );
  }
}

class PhoneNumber extends StatefulWidget {
  const PhoneNumber({Key? key}) : super(key: key);

  @override
  State<PhoneNumber> createState() => _PhoneNumberState();
}

class _PhoneNumberState extends State<PhoneNumber> {
  bool isPhoneCheck = true;

  final List<String> _phoneName = [' ', ' ', ' ', ' ', ' ', ' '];
  final List<String> _phoneNumber = [' ', ' ', ' ', ' ', ' ', ' '];

  final List<bool> _nameFlag = [false, false, false, false, false, false];
  final List<bool> _numberFlag = [false, false, false, false, false, false];

  @override
  void initState() {
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0x8045FFA2),
      appBar: PreferredSize(
        preferredSize: Size.fromHeight(
          mobile
              ? (dispHeight * appBarHeight) * 0.6
              : (dispHeight * appBarHeight),
        ),
        child: AppBar(
          backgroundColor: appBarColor,
          title: Text(
            '긴급 연락처 등록...',
            style: TextStyle(
              color: titleColor,
              fontWeight: FontWeight.bold,
              fontSize: mobile
                  ? (dispHeight * 0.02) / txtScale
                  : (dispHeight * 0.04) / txtScale,
            ),
          ),
        ),
      ),
      body: GestureDetector(
        onTap: () => FocusScope.of(context).unfocus(),
        child: SingleChildScrollView(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.start,
            children: <Widget>[
              SizedBox(
                height: (mobile ? dispHeight : dispWidth) * 0.01,
              ),
              Container(
                width: dispWidth * 0.95,
                height: dispHeight * 0.4,
                decoration: BoxDecoration(
                  borderRadius: BorderRadius.circular(mobile
                      ? dispHeight * borderRadius
                      : dispWidth * borderRadius),
                  border: Border.all(
                      width: mobile
                          ? dispWidth * boardLine
                          : dispHeight * boardLine,
                      color: Colors.black), //Container 테두리 값, 색상
                ),
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                  children: [
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      // 1 번 Table...
                      children: [
                        Transform.scale(
                          scale: dispWidth * 0.003,
                          child: Checkbox(
                            activeColor: Colors.orange,
                            splashRadius: 20,
                            value: V.isPhoneTable[1],
                            onChanged: (value) async {
                              SharedPreferences prefs =
                                  await SharedPreferences.getInstance();
                              setState(() {
                                V.isPhoneTable[1] = value!;
                                prefs.setBool('isPhoneTableFlag1', value);
                              });
                            },
                          ),
                        ),
                        SizedBox(
                          width: dispWidth * 0.32,
                          height: dispHeight * 0.05,
                          child: TextField(
                            maxLines: 1,
                            style: TextStyle(
                                fontSize: dispWidth * 0.04,
                                height: 0.8,
                                color: Colors.black),
                            decoration: InputDecoration(
                              // hintText: '이름 입력',
                              isDense: true,
                              enabled: V.isPhoneTable[1] ? true : false,
                              focusedBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.white, width: 1.5),
                              ),
                              enabledBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.black, width: 1.0),
                              ),
                              border: const OutlineInputBorder(),
                              labelText: V.phoneName[1],
                              labelStyle: const TextStyle(
                                color: Colors.black,
                              ),
                            ),
                            onChanged: (text) {
                              _nameFlag[1] = true;
                              _phoneName[1] = text;
                            },
                          ),
                        ),
                        SizedBox(
                          width: dispWidth * 0.43,
                          height: dispHeight * 0.05,
                          child: TextField(
                            style: TextStyle(
                                fontSize: dispWidth * 0.04,
                                height: 0.8,
                                color: Colors.black),
                            decoration: InputDecoration(
                              // hintText: '전화번호 입력',
                              isDense: true,
                              enabled: V.isPhoneTable[1] ? true : false,
                              focusedBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.white, width: 1.5),
                              ),
                              enabledBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.black, width: 1.0),
                              ),
                              border: const OutlineInputBorder(),
                              labelText: V.phoneNumber[1],
                              labelStyle: const TextStyle(
                                color: Colors.black,
                              ),
                            ),
                            onChanged: (text) {
                              _numberFlag[1] = true;
                              _phoneNumber[1] = text;
                            },
                            keyboardType: TextInputType.number,
                          ),
                        ),
                      ],
                    ),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      // 2 번 Table...
                      children: [
                        Transform.scale(
                          scale: dispWidth * 0.003,
                          child: Checkbox(
                            activeColor: Colors.orange,
                            // shape: CircleBorder(),
                            splashRadius: 20,
                            value: V.isPhoneTable[2],
                            onChanged: (value) async {
                              SharedPreferences prefs =
                                  await SharedPreferences.getInstance();
                              setState(() {
                                V.isPhoneTable[2] = value!;
                                prefs.setBool('isPhoneTableFlag2', value);
                              });
                            },
                          ),
                        ),
                        SizedBox(
                          width: dispWidth * 0.32,
                          height: dispHeight * 0.05,
                          child: TextField(
                            style: TextStyle(
                                fontSize: dispWidth * 0.04,
                                height: 0.8,
                                color: Colors.black),
                            decoration: InputDecoration(
                              // hintText: '이름 입력',
                              enabled: V.isPhoneTable[2] ? true : false,
                              focusedBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.white, width: 1.5),
                              ),
                              enabledBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.black, width: 1.0),
                              ),
                              border: const OutlineInputBorder(),
                              labelText: V.phoneName[2],
                              labelStyle: const TextStyle(
                                color: Colors.black,
                              ),
                            ),
                            onChanged: (text) {
                              _nameFlag[2] = true;
                              _phoneName[2] = text;
                            },
                          ),
                        ),
                        SizedBox(
                          width: dispWidth * 0.43,
                          height: dispHeight * 0.05,
                          child: TextField(
                            style: TextStyle(
                                fontSize: dispWidth * 0.04,
                                height: 0.8,
                                color: Colors.black),
                            decoration: InputDecoration(
                              // hintText: '전화번호 입력',
                              enabled: V.isPhoneTable[2] ? true : false,
                              focusedBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.white, width: 1.5),
                              ),
                              enabledBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.black, width: 1.0),
                              ),
                              border: const OutlineInputBorder(),
                              labelText: V.phoneNumber[2],
                              labelStyle: const TextStyle(
                                color: Colors.black,
                              ),
                            ),
                            onChanged: (text) {
                              _numberFlag[2] = true;
                              _phoneNumber[2] = text;
                            },
                            keyboardType: TextInputType.number,
                          ),
                        ),
                      ],
                    ),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      // 3 번 Table...
                      children: [
                        Transform.scale(
                          scale: dispWidth * 0.003,
                          child: Checkbox(
                            activeColor: Colors.orange,
                            splashRadius: 20,
                            value: V.isPhoneTable[3],
                            onChanged: (value) async {
                              SharedPreferences prefs =
                                  await SharedPreferences.getInstance();
                              setState(() {
                                V.isPhoneTable[3] = value!;
                                prefs.setBool('isPhoneTableFlag3', value);
                              });
                            },
                          ),
                        ),
                        SizedBox(
                          width: dispWidth * 0.32,
                          height: dispHeight * 0.05,
                          child: TextField(
                            style: TextStyle(
                                fontSize: dispWidth * 0.04,
                                height: 0.8,
                                color: Colors.black),
                            decoration: InputDecoration(
                              // hintText: '이름 입력',
                              enabled: V.isPhoneTable[3] ? true : false,
                              focusedBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.white, width: 1.5),
                              ),
                              enabledBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.black, width: 1.0),
                              ),
                              border: const OutlineInputBorder(),
                              labelText: V.phoneName[3],
                              labelStyle: const TextStyle(
                                color: Colors.black,
                              ),
                            ),
                            onChanged: (text) {
                              _nameFlag[3] = true;
                              _phoneName[3] = text;
                            },
                          ),
                        ),
                        SizedBox(
                          width: dispWidth * 0.43,
                          height: dispHeight * 0.05,
                          child: TextField(
                            style: TextStyle(
                                fontSize: dispWidth * 0.04,
                                height: 0.8,
                                color: Colors.black),
                            decoration: InputDecoration(
                              // hintText: '전화번호 입력',
                              enabled: V.isPhoneTable[3] ? true : false,
                              focusedBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.white, width: 1.5),
                              ),
                              enabledBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.black, width: 1.0),
                              ),
                              border: const OutlineInputBorder(),
                              labelText: V.phoneNumber[3],
                              labelStyle: const TextStyle(
                                color: Colors.black,
                              ),
                            ),
                            onChanged: (text) {
                              _nameFlag[3] = true;
                              _phoneNumber[3] = text;
                            },
                            keyboardType: TextInputType.number,
                          ),
                        ),
                      ],
                    ),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      // 4 번 Table...
                      children: [
                        Transform.scale(
                          scale: dispWidth * 0.003,
                          child: Checkbox(
                            activeColor: Colors.orange,
                            splashRadius: 20,
                            value: V.isPhoneTable[4],
                            onChanged: (value) async {
                              SharedPreferences prefs =
                                  await SharedPreferences.getInstance();
                              setState(() {
                                V.isPhoneTable[4] = value!;
                                prefs.setBool('isPhoneTableFlag4', value);
                              });
                            },
                          ),
                        ),
                        SizedBox(
                          width: dispWidth * 0.32,
                          height: dispHeight * 0.05,
                          child: TextField(
                            style: TextStyle(
                                fontSize: dispWidth * 0.04,
                                height: 0.8,
                                color: Colors.black),
                            decoration: InputDecoration(
                              // hintText: '이름 입력',
                              enabled: V.isPhoneTable[4] ? true : false,
                              focusedBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.white, width: 1.5),
                              ),
                              enabledBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.black, width: 1.0),
                              ),
                              border: const OutlineInputBorder(),
                              labelText: V.phoneName[4],
                              labelStyle: const TextStyle(
                                color: Colors.black,
                              ),
                            ),
                            onChanged: (text) {
                              _nameFlag[4] = true;
                              _phoneName[4] = text;
                            },
                          ),
                        ),
                        SizedBox(
                          width: dispWidth * 0.43,
                          height: dispHeight * 0.05,
                          child: TextField(
                            style: TextStyle(
                                fontSize: dispWidth * 0.04,
                                height: 0.8,
                                color: Colors.black),
                            decoration: InputDecoration(
                              // hintText: '전화번호 입력',
                              enabled: V.isPhoneTable[4] ? true : false,
                              focusedBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.white, width: 1.5),
                              ),
                              enabledBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.black, width: 1.0),
                              ),
                              border: const OutlineInputBorder(),
                              labelText: V.phoneNumber[4],
                              labelStyle: const TextStyle(
                                color: Colors.black,
                              ),
                            ),
                            onChanged: (text) {
                              _numberFlag[4] = true;
                              _phoneNumber[4] = text;
                            },
                            keyboardType: TextInputType.number,
                          ),
                        ),
                      ],
                    ),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      // 5 번 Table...
                      children: [
                        Transform.scale(
                          scale: dispWidth * 0.003,
                          child: Checkbox(
                            activeColor: Colors.orange,
                            splashRadius: 20,
                            value: V.isPhoneTable[5],
                            onChanged: (value) async {
                              SharedPreferences prefs =
                                  await SharedPreferences.getInstance();
                              setState(() {
                                V.isPhoneTable[5] = value!;
                                prefs.setBool('isPhoneTableFlag5', value);
                              });
                            },
                          ),
                        ),
                        SizedBox(
                          width: dispWidth * 0.32,
                          height: dispHeight * 0.05,
                          child: TextField(
                            style: TextStyle(
                                fontSize: dispWidth * 0.04,
                                height: 0.8,
                                color: Colors.black),
                            decoration: InputDecoration(
                              // hintText: '이름 입력',
                              enabled: V.isPhoneTable[5] ? true : false,
                              focusedBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.white, width: 1.5),
                              ),
                              enabledBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.black, width: 1.0),
                              ),
                              border: const OutlineInputBorder(),
                              labelText: V.phoneName[5],
                              labelStyle: const TextStyle(
                                color: Colors.black,
                              ),
                            ),
                            onChanged: (text) {
                              _nameFlag[5] = true;
                              _phoneName[5] = text;
                            },
                          ),
                        ),
                        SizedBox(
                          width: dispWidth * 0.43,
                          height: dispHeight * 0.05,
                          child: TextField(
                            style: TextStyle(
                                fontSize: dispWidth * 0.04,
                                height: 0.8,
                                color: Colors.black),
                            decoration: InputDecoration(
                              // hintText: '전화번호 입력',
                              enabled: V.isPhoneTable[5] ? true : false,
                              focusedBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.white, width: 1.5),
                              ),
                              enabledBorder: const OutlineInputBorder(
                                borderSide:
                                    BorderSide(color: Colors.black, width: 1.0),
                              ),
                              border: const OutlineInputBorder(),
                              labelText: V.phoneNumber[5],
                              labelStyle: const TextStyle(
                                color: Colors.black,
                              ),
                            ),
                            onChanged: (text) {
                              _numberFlag[5] = true;
                              _phoneNumber[5] = text;
                            },
                            keyboardType: TextInputType.number,
                          ),
                        ),
                      ],
                    ),
                  ],
                ),
              ),
              SizedBox(
                height: dispHeight * 0.03,
              ),
              SizedBox(
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: <Widget>[
                    SizedBox(
                      width: dispWidth * 0.20,
                      height: dispHeight * 0.045,
                      child: ElevatedButton(
                        child: Text(
                          "저장",
                          style: TextStyle(
                            color: Colors.black,
                            fontWeight: FontWeight.bold,
                            fontSize: (dispWidth * 0.035) / txtScale,
                          ),
                        ),
                        onPressed: () async {
                          SharedPreferences prefs =
                              await SharedPreferences.getInstance();
                          setState(() {
                            if (!(_nameFlag[1] |
                                _nameFlag[2] |
                                _nameFlag[3] |
                                _nameFlag[4] |
                                _nameFlag[5] |
                                _numberFlag[1] |
                                _numberFlag[2] |
                                _numberFlag[3] |
                                _numberFlag[4] |
                                _numberFlag[5])) {
                              isPhoneCheck = false;
                            } else {
                              isPhoneCheck = true;
                            }

                            if (_nameFlag[1]) {
                              prefs.setString('phoneName1', _phoneName[1]);
                              V.phoneName[1] = _phoneName[1];
                            }
                            if (_numberFlag[1]) {
                              prefs.setString('phoneNumber1', _phoneNumber[1]);
                              V.phoneNumber[1] = _phoneNumber[1];
                            }

                            if (_nameFlag[2]) {
                              prefs.setString('phoneName2', _phoneName[2]);
                              V.phoneName[2] = _phoneName[2];
                            }
                            if (_numberFlag[2]) {
                              prefs.setString('phoneNumber2', _phoneNumber[2]);
                              V.phoneNumber[2] = _phoneNumber[2];
                            }

                            if (_nameFlag[3]) {
                              prefs.setString('phoneName3', _phoneName[3]);
                              V.phoneName[3] = _phoneName[3];
                            }
                            if (_numberFlag[3]) {
                              prefs.setString('phoneNumber3', _phoneNumber[3]);
                              V.phoneNumber[3] = _phoneNumber[3];
                            }

                            if (_nameFlag[4]) {
                              prefs.setString('phoneName4', _phoneName[4]);
                              V.phoneName[4] = _phoneName[4];
                            }
                            if (_numberFlag[4]) {
                              prefs.setString('phoneNumber4', _phoneNumber[4]);
                              V.phoneNumber[4] = _phoneNumber[4];
                            }

                            if (_nameFlag[5]) {
                              prefs.setString('phoneName5', _phoneName[5]);
                              V.phoneName[5] = _phoneName[5];
                            }
                            if (_numberFlag[5]) {
                              prefs.setString('phoneNumber5', _phoneNumber[5]);
                              V.phoneNumber[5] = _phoneNumber[5];
                            }
                          });
                          if (beep) {
                            advancedPlayer.play(AssetSource('sound/intro.mp3'));
                          }
                          // ignore: use_build_context_synchronously
                          showDialog(
                            context: context,
                            builder: (context) => AlertDialog(
                              backgroundColor: const Color(0xC0D4AA39),
                              title: Text(
                                isPhoneCheck ? "저장 되었습니다." : "변경된 내용이 없습니다.",
                                style: TextStyle(
                                  color: Colors.white,
                                  fontSize: (dispWidth * 0.05) / txtScale,
                                ),
                              ),
                              actions: <Widget>[
                                // ignore: deprecated_member_use
                                SizedBox(
                                  width: dispWidth * 0.20,
                                  height: dispHeight * 0.045,
                                  child: ElevatedButton(
                                      child: Text(
                                        "확인",
                                        style: TextStyle(
                                          color: Colors.black,
                                          fontWeight: FontWeight.bold,
                                          fontSize:
                                              (dispWidth * 0.035) / txtScale,
                                        ),
                                      ),
                                      onPressed: () {
                                        // sensorChgFlag = false;
                                        Get.back();
                                      }),
                                ),
                              ],
                            ),
                          );
                        },
                      ),
                    ),
                    SizedBox(width: dispWidth * 0.1),
                    SizedBox(
                      width: dispWidth * 0.20,
                      height: dispHeight * 0.045,
                      child: ElevatedButton(
                        child: Text(
                          "취소",
                          style: TextStyle(
                            color: Colors.black,
                            fontWeight: FontWeight.bold,
                            fontSize: (dispWidth * 0.035) / txtScale,
                          ),
                        ),
                        onPressed: () {
                          if (beep) {
                            advancedPlayer
                                .play(AssetSource('sound/iconClick.mp3'));
                          }
                          Get.back();
                        },
                      ),
                    ),
                  ],
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}

class WiFiChange extends StatefulWidget {
  const WiFiChange({Key? key}) : super(key: key);

  @override
  State<WiFiChange> createState() => WiFiChangeState();
}

class WiFiChangeState extends State<WiFiChange> {
  String ssidPush = " ";
  String pwPush = " ";

  bool ssidFlag = false;
  bool pwFlag = false;
  // bool _isChecked = false;
  // bool _isChecked = networkSelect;

  final _ssidTextController = TextEditingController(text: V.conSSID);
  final _pwTextController = TextEditingController(text: V.conPW);

  final _hotSpotSsidTextController = TextEditingController(text: V.hotSpotSSID);
  final _hotSpotPwTextController = TextEditingController(text: V.hotSpotPW);

  final _hotSpotTextController = TextEditingController(text: stationIp);

  @override
  void initState() {
    super.initState();
  }

  @override
  void dispose() {
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0x8045FFA2),
      appBar: PreferredSize(
        preferredSize: Size.fromHeight(
          mobile
              ? (dispHeight * appBarHeight) * 0.6
              : (dispHeight * appBarHeight),
        ),
        child: AppBar(
          backgroundColor: appBarColor,
          title: Text(
            'WiFi 네트워크 변경...',
            style: TextStyle(
              color: titleColor,
              fontWeight: FontWeight.bold,
              fontSize: mobile
                  ? (dispHeight * 0.02) / txtScale
                  : (dispHeight * 0.04) / txtScale,
            ),
          ),
        ),
      ),
      body: GestureDetector(
        onTap: () => FocusScope.of(context).unfocus(),
        child: SingleChildScrollView(
          child: Center(
            child: Column(
              mainAxisAlignment: MainAxisAlignment.start,
              children: <Widget>[
                SizedBox(
                  // appbar 와 간격 띄우기...
                  height: (mobile ? dispHeight : dispWidth) * 0.01,
                ),
                Container(
                  width: mobile // WiFi Mode 일 때 가로넓이...
                      ? dispWidth * 0.85
                      : dispHeight * 0.7,
                  decoration: BoxDecoration(
                    borderRadius: BorderRadius.circular(mobile
                        ? dispHeight * borderRadius
                        : dispWidth * borderRadius),
                    border: Border.all(
                        width: mobile
                            ? dispWidth * boardLine
                            : dispHeight * boardLine,
                        color: Colors.black), //Container 테두리 값, 색상
                  ),
                  child: Column(
                    children: <Widget>[
                      SizedBox(
                        height: mobile ? dispHeight * 0.03 : dispWidth * 0.03,
                      ),
                      SizedBox(
                        width: mobile ? dispWidth * 0.6 : dispWidth * 0.3,
                        height: mobile ? dispHeight * 0.07 : dispWidth * 0.045,
                        child: TextField(
                          // SSID 입력창...
                          controller: networkSelect
                              ? _hotSpotSsidTextController
                              : _ssidTextController,
                          keyboardType: TextInputType.url,
                          cursorColor: Colors.white,
                          textAlign: TextAlign.center,
                          onChanged: (text) {
                            ssidPush = text;
                            ssidFlag = true;
                          },
                          style: TextStyle(
                            fontSize: mobile
                                ? (dispWidth * 0.05) / txtScale
                                : (dispHeight * 0.035) / txtScale,
                            height: 0.8,
                            fontWeight: FontWeight.w400,
                            color: Colors.black,
                          ),
                          decoration: InputDecoration(
                            labelText:
                                networkSelect ? V.hotSpotSSID : V.conSSID,
                            labelStyle: const TextStyle(
                              color: Colors.white,
                            ),
                            focusedBorder: const OutlineInputBorder(
                              borderRadius:
                                  BorderRadius.all(Radius.circular(5.0)),
                              borderSide: BorderSide(
                                width: 2,
                                color: Color(0xE0D4AA39),
                              ),
                            ),
                            enabledBorder: const OutlineInputBorder(
                              borderSide:
                                  BorderSide(color: Colors.black, width: 1.0),
                            ),
                            border: const OutlineInputBorder(
                              borderRadius:
                                  BorderRadius.all(Radius.circular(5.0)),
                            ),
                          ),
                        ),
                      ),
                      Text(
                        '콘트롤러의 SSID를 입력 합니다.',
                        style: TextStyle(
                          fontSize: mobile
                              ? (dispWidth * 0.04) / txtScale
                              : (dispHeight * 0.035) / txtScale,
                        ),
                      ),
                      mobile
                          ? SizedBox(
                              height: mobile
                                  ? (dispHeight * 0.03) / 2
                                  : (dispWidth * 0.04) / 2)
                          : const SizedBox.shrink(),
                      SizedBox(
                        height: mobile
                            ? (dispHeight * 0.03) / 2
                            : (dispWidth * 0.04) / 2,
                      ),
                      SizedBox(
                        width: mobile ? dispWidth * 0.6 : dispWidth * 0.3,
                        height: mobile ? dispHeight * 0.07 : dispWidth * 0.045,
                        child: TextField(
                          // 비밀번호 입력 창...
                          controller: networkSelect
                              ? _hotSpotPwTextController
                              : _pwTextController,
                          keyboardType: TextInputType.number,
                          cursorColor: Colors.white,
                          textAlign: TextAlign.center,
                          onChanged: (text) {
                            pwPush = text;
                            pwFlag = true;
                          },
                          style: TextStyle(
                            fontSize: mobile
                                ? (dispWidth * 0.05) / txtScale
                                : (dispHeight * 0.035) / txtScale,
                            height: 0.8,
                            fontWeight: FontWeight.w400,
                            color: Colors.black,
                          ),
                          decoration: InputDecoration(
                            labelText: networkSelect ? V.hotSpotPW : V.conPW,
                            labelStyle: const TextStyle(
                              color: Colors.white,
                              height: 0.8,
                            ),
                            focusedBorder: const OutlineInputBorder(
                              borderRadius:
                                  BorderRadius.all(Radius.circular(5.0)),
                              borderSide: BorderSide(
                                width: 2,
                                color: Color(0xE0D4AA39),
                              ),
                            ),
                            enabledBorder: const OutlineInputBorder(
                              borderSide:
                                  BorderSide(color: Colors.black, width: 1.0),
                            ),
                            border: const OutlineInputBorder(
                              borderRadius:
                                  BorderRadius.all(Radius.circular(5.0)),
                            ),
                          ),
                        ),
                      ),
                      Text(
                        '콘트롤러의 비밀번호를 입력 합니다.',
                        style: TextStyle(
                          fontSize: mobile
                              ? (dispWidth * 0.04) / txtScale
                              : (dispHeight * 0.035) / txtScale,
                        ),
                      ),
                      SizedBox(
                        height: mobile ? dispHeight * 0.05 : dispWidth * 0.03,
                      ),
                      IconButton(
                        icon: Icon(Icons.qr_code_scanner,
                            size: mobile ? dispWidth * 0.3 : dispHeight * 0.2,
                            color: Colors.black),
                        tooltip: 'QR 코드',
                        onPressed: () async {
                          if (beep) {
                            await advancedPlayer
                                .play(AssetSource('sound/iconClick.mp3'));
                          }
                          await Get.toNamed('/qrCodeRead');
                        },
                      ),
                      SizedBox(
                        height: mobile ? dispHeight * 0.02 : dispWidth * 0.03,
                      ),
                      SizedBox(
                        child: Row(
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: [
                            Text(
                              '와이파이 연결',
                              style: TextStyle(
                                fontSize: mobile
                                    ? (dispWidth * 0.04) / txtScale
                                    : (dispHeight * 0.035) / txtScale,
                              ),
                            ),
                            Switch(
                              // value: networkSelect,
                              value: networkSelect,
                              onChanged: (value) {
                                setState(() {
                                  networkSelect = value;
                                  // SharedPreferences prefs =
                                  //     await SharedPreferences.getInstance();
                                  // networkSelect = value;
                                  // prefs.setBool('networkSelect', value);
                                });
                              },
                            ),
                            Text(
                              '핫스팟 연결',
                              style: TextStyle(
                                fontSize: mobile
                                    ? (dispWidth * 0.04) / txtScale
                                    : (dispHeight * 0.035) / txtScale,
                              ),
                            ),
                          ],
                        ),
                      ),
                      networkSelect
                          ? SizedBox(
                              width: mobile ? dispWidth * 0.6 : dispWidth * 0.3,
                              height: mobile
                                  ? dispHeight * 0.07
                                  : dispWidth * 0.045,
                              child: TextField(
                                // Hot Spot IP 입력창...
                                controller: _hotSpotTextController,
                                keyboardType: TextInputType.number,
                                cursorColor: Colors.white,
                                textAlign: TextAlign.center,
                                onChanged: (text) {
                                  stationIp = text;
                                },
                                style: TextStyle(
                                  fontSize: mobile
                                      ? (dispWidth * 0.05) / txtScale
                                      : (dispHeight * 0.035) / txtScale,
                                  height: 0.8,
                                  fontWeight: FontWeight.w400,
                                  color: Colors.black,
                                ),
                                decoration: InputDecoration(
                                  labelText: stationIp,
                                  labelStyle: const TextStyle(
                                    color: Colors.white,
                                  ),
                                  focusedBorder: const OutlineInputBorder(
                                    borderRadius:
                                        BorderRadius.all(Radius.circular(5.0)),
                                    borderSide: BorderSide(
                                      width: 2,
                                      color: Color(0xE0D4AA39),
                                    ),
                                  ),
                                  enabledBorder: const OutlineInputBorder(
                                    borderSide: BorderSide(
                                        color: Colors.black, width: 1.0),
                                  ),
                                  border: const OutlineInputBorder(
                                    borderRadius:
                                        BorderRadius.all(Radius.circular(5.0)),
                                  ),
                                ),
                              ),
                            )
                          : SizedBox.shrink(),
                      SizedBox(
                        height: mobile ? dispHeight * 0.05 : dispWidth * 0.03,
                      ),
                      SizedBox(
                        child: Row(
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: <Widget>[
                            SizedBox(
                              width:
                                  mobile ? dispWidth * 0.21 : dispWidth * 0.09,
                              height: mobile
                                  ? dispHeight * 0.045
                                  : dispHeight * 0.07,
                              child: ElevatedButton(
                                style: ButtonStyle(
                                  backgroundColor:
                                      MaterialStateProperty.all(Colors.orange),
                                ),
                                child: Text(
                                  "저장",
                                  style: TextStyle(
                                    color: Colors.black,
                                    fontWeight: FontWeight.bold,
                                    fontSize: mobile
                                        ? (dispWidth * 0.035) / txtScale
                                        : (dispHeight * 0.03) / txtScale,
                                  ),
                                ),
                                onPressed: () async {
                                  showDialog(
                                    context: context,
                                    builder: (context) => AlertDialog(
                                      backgroundColor: const Color(0xC0D4AA39),
                                      title: Text(
                                        "변경된 내용을 저장 합니다.",
                                        style: TextStyle(
                                          color: Colors.white,
                                          fontSize: mobile
                                              ? (dispWidth * 0.045) / txtScale
                                              : (dispHeight * 0.035) / txtScale,
                                        ),
                                      ),
                                      actions: <Widget>[
                                        // ignore: deprecated_member_use
                                        SizedBox(
                                          width: mobile
                                              ? dispWidth * 0.20
                                              : dispWidth * 0.09,
                                          height: mobile
                                              ? dispHeight * 0.045
                                              : dispHeight * 0.07,
                                          child: ElevatedButton(
                                              child: Text(
                                                "예",
                                                style: TextStyle(
                                                  color: Colors.black,
                                                  fontWeight: FontWeight.bold,
                                                  fontSize: mobile
                                                      ? (dispWidth * 0.04) /
                                                          txtScale
                                                      : (dispHeight * 0.03) /
                                                          txtScale,
                                                ),
                                              ),
                                              onPressed: () async {
                                                SharedPreferences prefs =
                                                    await SharedPreferences
                                                        .getInstance();
                                                setState(() {
                                                  if (ssidFlag) {
                                                    if (networkSelect) {
                                                      V.hotSpotSSID = ssidPush;
                                                      prefs.setString(
                                                          'hotSpotSSID',
                                                          ssidPush);
                                                    } else {
                                                      V.conSSID = ssidPush;
                                                      prefs.setString(
                                                          'ssidPush', ssidPush);
                                                    }
                                                  }
                                                  if (pwFlag) {
                                                    if (networkSelect) {
                                                      V.hotSpotPW = pwPush;
                                                      prefs.setString(
                                                          'pwPush', pwPush);
                                                    } else {
                                                      V.conPW = pwPush;
                                                      prefs.setString(
                                                          'pwPush', pwPush);
                                                    }
                                                  }

                                                  if (networkSelect) {
                                                    prefs.setString(
                                                        'stationIp', stationIp);
                                                  }

                                                  print('ssidPush : $ssidPush');
                                                  print('pwPush : $pwPush');
                                                });
                                                if (beep) {
                                                  advancedPlayer.play(AssetSource(
                                                      'sound/iconClick.mp3'));
                                                }
                                                Get.back();
                                              }),
                                        ),
                                        SizedBox(
                                            width: mobile
                                                ? dispHeight * 0.04
                                                : dispWidth * 0.04),
                                        // ignore: deprecated_member_use
                                        SizedBox(
                                          width: mobile
                                              ? dispWidth * 0.25
                                              : dispWidth * 0.13,
                                          height: mobile
                                              ? dispHeight * 0.045
                                              : dispHeight * 0.07,
                                          child: ElevatedButton(
                                              child: Text(
                                                "아니요",
                                                style: TextStyle(
                                                  color: Colors.black,
                                                  fontWeight: FontWeight.bold,
                                                  fontSize: mobile
                                                      ? (dispWidth * 0.04) /
                                                          txtScale
                                                      : (dispHeight * 0.03) /
                                                          txtScale,
                                                ),
                                              ),
                                              onPressed: () {
                                                if (beep) {
                                                  advancedPlayer.play(AssetSource(
                                                      'sound/iconClick.mp3'));
                                                }
                                                Get.back();
                                              }),
                                        ),
                                      ],
                                    ),
                                  );
                                  if (beep) {
                                    await advancedPlayer
                                        .play(AssetSource('sound/intro.mp3'));
                                  }
                                },
                              ),
                            ),
                            SizedBox(
                                width: mobile
                                    ? dispHeight * 0.04
                                    : dispWidth * 0.04),
                            SizedBox(
                              width:
                                  mobile ? dispWidth * 0.21 : dispWidth * 0.09,
                              height: mobile
                                  ? dispHeight * 0.045
                                  : dispHeight * 0.07,
                              child: ElevatedButton(
                                style: ButtonStyle(
                                  backgroundColor:
                                      MaterialStateProperty.all(Colors.orange),
                                ),
                                child: Text(
                                  "취소",
                                  style: TextStyle(
                                    color: Colors.black,
                                    fontWeight: FontWeight.bold,
                                    fontSize: mobile
                                        ? (dispWidth * 0.035) / txtScale
                                        : (dispHeight * 0.03) / txtScale,
                                  ),
                                ),
                                onPressed: () {
                                  if (beep) {
                                    advancedPlayer.play(
                                        AssetSource('sound/iconClick.mp3'));
                                  }
                                  Get.back();
                                },
                              ),
                            ),
                          ],
                        ),
                      ),
                      SizedBox(
                          height:
                              mobile ? dispHeight * 0.05 : dispWidth * 0.03),
                    ],
                  ),
                ),
                //   // 컨테이너 하단부 간격 띄우기 간격 띄우기...
              ],
            ),
          ),
        ),
      ),
    );
  }
}

class Setting extends StatefulWidget {
  const Setting({Key? key}) : super(key: key);

  @override
  State<Setting> createState() => _SettingState();
}

enum WiFi { A, B }

class _SettingState extends State<Setting> {
  List<bool> ledNameFlag = [
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false
  ];
  final _led1NameController = TextEditingController(text: V.ledName[1]);
  final _led2NameController = TextEditingController(text: V.ledName[2]);
  final _led3NameController = TextEditingController(text: V.ledName[3]);
  final _led4NameController = TextEditingController(text: V.ledName[4]);
  final _led5NameController = TextEditingController(text: V.ledName[5]);
  final _led6NameController = TextEditingController(text: V.ledName[6]);
  final _led7NameController = TextEditingController(text: V.ledName[7]);
  final _led8NameController = TextEditingController(text: V.ledName[8]);
  @override
  void initState() {
    super.initState();
    (() async {
      SharedPreferences prefs = await SharedPreferences.getInstance();
      setState(() {
        // File에 최초 저장시 null 대응...
        print("led1SaveName : ${prefs.getString('led1SaveName')}");
        print("led2SaveName : ${prefs.getString('led2SaveName')}");
        print("led3SaveName : ${prefs.getString('led3SaveName')}");
        print("led4SaveName : ${prefs.getString('led4SaveName')}");
        print("led5SaveName : ${prefs.getString('led5SaveName')}");
        print("led6SaveName : ${prefs.getString('led6SaveName')}");
        print("led7SaveName : ${prefs.getString('led7SaveName')}");
        print("led8SaveName : ${prefs.getString('led8SaveName')}");
      });
    })();
    // initSpeechRecognizer();
    //  permission(); //프로그램 최초 mic 권한 얻기..
  }

  Widget _ledNameChange(int ch) {
    return SizedBox(
      width: mobile ? dispWidth * 0.3 : dispWidth * 0.16,
      height: mobile ? dispHeight * 0.09 : dispHeight * 0.08,
      child: TextField(
        controller: ch == 1
            ? _led1NameController
            : ch == 2
                ? _led2NameController
                : ch == 3
                    ? _led3NameController
                    : ch == 4
                        ? _led4NameController
                        : ch == 5
                            ? _led5NameController
                            : ch == 6
                                ? _led6NameController
                                : ch == 7
                                    ? _led7NameController
                                    : ch == 8
                                        ? _led8NameController
                                        : null,
        cursorColor: Colors.white,
        style: TextStyle(
            fontSize: mobile
                ? (dispWidth * 0.055) / txtScale
                : (dispHeight * 0.04) / txtScale,
            height: 0.8,
            color: Colors.black),
        decoration: InputDecoration(
          focusedBorder: const OutlineInputBorder(
            borderSide: BorderSide(color: Color(0xE0D4AA39), width: 1.5),
          ),
          enabledBorder: const OutlineInputBorder(
            borderSide: BorderSide(color: Colors.black, width: 1.0),
          ),
          border: const OutlineInputBorder(),
          labelText: V.ledName[ch],
          labelStyle: const TextStyle(
            color: Colors.white,
          ),
        ),
        onChanged: (text) {
          setState(() {
            V.ledSaveName[ch] = text;
            ledNameFlag[ch] = true;
          });
        },
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      backgroundColor: const Color(0x8045FFA2),
      appBar: PreferredSize(
        preferredSize: Size.fromHeight(
          mobile
              ? (dispHeight * appBarHeight) * 0.6 // AppBar 높이
              : (dispHeight * appBarHeight),
        ),
        child: AppBar(
          backgroundColor: appBarColor,
          title: Text(
            '조명 이름 변경...',
            style: TextStyle(
              color: Colors.white,
              fontWeight: FontWeight.bold,
              fontSize: mobile
                  ? (dispHeight * 0.02) / txtScale
                  : (dispHeight * 0.04) / txtScale,
            ),
          ),
          actions: <Widget>[
            mobile
                ? const SizedBox.shrink()
                : IconButton(
                    icon: Icon(Icons.menu_book, size: dispHeight * 0.05),
                    tooltip: '설명서',
                    onPressed: () async {
                      if (beep) {
                        advancedPlayer.play(AssetSource('sound/iconClick.mp3'));
                      }
                    },
                  ),
            SizedBox(
                width: mobile
                    ? (pixelRatio > 1.5)
                        ? dispWidth * 0.0015
                        : dispWidth * 0.03
                    : (pixelRatio > 1.5)
                        ? dispWidth * 0.015
                        : dispWidth * 0.03),
            IconButton(
              icon: Icon(Icons.settings_backup_restore,
                  size: mobile ? dispWidth * 0.06 : dispHeight * 0.05,
                  color: activeColor),
              tooltip: '초기화',
              onPressed: () async {
                if (beep) advancedPlayer.play(AssetSource('sound/intro.mp3'));
                await showDialog(
                  context: context,
                  builder: (context) => AlertDialog(
                    backgroundColor: const Color(0xC0D4AA39),
                    title: Text(
                      "조명 이름을 초기화 합니다.",
                      style: TextStyle(
                        color: Colors.white,
                        fontSize: mobile
                            ? (dispWidth * 0.05) / txtScale
                            : (dispHeight * 0.035) / txtScale,
                      ),
                    ),
                    actions: <Widget>[
                      // ignore: deprecated_member_use
                      SizedBox(
                        width: mobile ? dispWidth * 0.20 : dispWidth * 0.09,
                        height: mobile ? dispHeight * 0.045 : dispHeight * 0.07,
                        child: ElevatedButton(
                            child: Text(
                              "예",
                              style: TextStyle(
                                color: Colors.black,
                                fontWeight: FontWeight.bold,
                                fontSize: mobile
                                    ? (dispWidth * 0.04) / txtScale
                                    : (dispHeight * 0.03) / txtScale,
                              ),
                            ),
                            onPressed: () async {
                              SharedPreferences prefs =
                                  await SharedPreferences.getInstance();
                              if (beep) {
                                advancedPlayer
                                    .play(AssetSource('sound/iconClick.mp3'));
                              }

                              for (int i = 1; i < 9; i++) {
                                V.ledName[i] = V.ledPushName[i];
                                V.ledSaveName[i] = V.ledPushName[i];
                                prefs.setString(
                                    'led${i}SaveName', V.ledPushName[i]);
                              }
                              setState(() {});
                              await server.getReq("LED_NAME", 0);
                              Get.back();
                            }),
                      ),
                      SizedBox(
                          width: mobile ? dispHeight * 0.04 : dispWidth * 0.04),
                      // ignore: deprecated_member_use
                      SizedBox(
                        width: mobile ? dispWidth * 0.27 : dispWidth * 0.13,
                        height: mobile ? dispHeight * 0.045 : dispHeight * 0.07,
                        child: ElevatedButton(
                            child: Text(
                              "아니요",
                              style: TextStyle(
                                color: Colors.black,
                                fontWeight: FontWeight.bold,
                                fontSize: mobile
                                    ? (dispWidth * 0.04) / txtScale
                                    : (dispHeight * 0.03) / txtScale,
                              ),
                            ),
                            onPressed: () {
                              if (beep) {
                                advancedPlayer
                                    .play(AssetSource('sound/iconClick.mp3'));
                              }
                              Get.back();
                            }),
                      ),
                    ],
                  ),
                );
                setState(() {});
              },
            ),
            SizedBox(
                width: mobile
                    ? (pixelRatio > 1.5)
                        ? dispWidth * 0.0015
                        : dispWidth * 0.03
                    : (pixelRatio > 1.5)
                        ? dispWidth * 0.001
                        : dispWidth * 0.03),
            IconButton(
              icon: Icon(
                Icons.save_alt,
                size: mobile ? dispWidth * 0.06 : dispHeight * 0.05,
                color: activeColor,
              ),
              tooltip: '저장',
              onPressed: () /*async*/ {
                if (beep) advancedPlayer.play(AssetSource('sound/intro.mp3'));
                showDialog(
                  context: context,
                  builder: (context) => AlertDialog(
                    backgroundColor: const Color(0xC0D4AA39),
                    title: Text(
                      "변경된 내용을 저장 합니다.",
                      style: TextStyle(
                        color: Colors.white,
                        fontSize: mobile
                            ? (dispWidth * 0.05) / txtScale
                            : (dispHeight * 0.035) / txtScale,
                      ),
                    ),
                    actions: <Widget>[
                      // ignore: deprecated_member_use
                      SizedBox(
                        width: mobile ? dispWidth * 0.20 : dispWidth * 0.09,
                        height: mobile ? dispHeight * 0.045 : dispHeight * 0.07,
                        child: ElevatedButton(
                            child: Text(
                              "예",
                              style: TextStyle(
                                color: Colors.black,
                                fontWeight: FontWeight.bold,
                                fontSize: mobile
                                    ? (dispWidth * 0.04) / txtScale
                                    : (dispHeight * 0.03) / txtScale,
                              ),
                            ),
                            onPressed: () async {
                              SharedPreferences prefs =
                                  await SharedPreferences.getInstance();
                              setState(() {
                                for (int i = 1; i < 9; i++) {
                                  if (ledNameFlag[i]) {
                                    V.ledName[i] = V.ledSaveName[i];
                                    prefs.setString(
                                        'led${i}SaveName', V.ledSaveName[i]);
                                  }
                                }

                                print('led1 SaveName : ${V.ledSaveName[1]}');
                                print('led2 SaveName : ${V.ledSaveName[2]}');
                                print('led3 SaveName : ${V.ledSaveName[3]}');
                                print('led4 SaveName : ${V.ledSaveName[4]}');
                                print('led5 SaveName : ${V.ledSaveName[5]}');
                                print('led6 SaveName : ${V.ledSaveName[6]}');
                                print('led7 SaveName : ${V.ledSaveName[7]}');
                                print('led8 SaveName : ${V.ledSaveName[8]}');
                                print("");

                                print('led1 name : ${V.ledName[1]}');
                                print('led2 name : ${V.ledName[2]}');
                                print('led3 name : ${V.ledName[3]}');
                                print('led4 name : ${V.ledName[4]}');
                                print('led5 name : ${V.ledName[5]}');
                                print('led6 name : ${V.ledName[6]}');
                                print('led7 name : ${V.ledName[7]}');
                                print('led8 name : ${V.ledName[8]}');
                              });
                              if (beep) {
                                advancedPlayer
                                    .play(AssetSource('sound/iconClick.mp3'));
                              }
                              // LED 1 Name 전달...
                              await server.getReq("LED_NAME", 1);
                              // LED 2 Name 전달...
                              await server.getReq("LED_NAME", 2);
                              // LED 3 Name 전달...
                              await server.getReq("LED_NAME", 3);
                              // LED 4 Name 전달...
                              await server.getReq("LED_NAME", 4);
                              // LED 5 Name 전달...
                              await server.getReq("LED_NAME", 5);
                              // LED 6 Name 전달...
                              await server.getReq("LED_NAME", 6);
                              // LED 7 Name 전달...
                              await server.getReq("LED_NAME", 7);
                              // LED 8 Name 전달...
                              await server.getReq("LED_NAME", 8);
                              // 모두 저장...
                              await server.getReq("LED_NAME", 10);
                              Get.back();
                            }),
                      ),
                      SizedBox(
                          width: mobile ? dispHeight * 0.04 : dispWidth * 0.04),
                      // ignore: deprecated_member_use
                      SizedBox(
                        width: mobile ? dispWidth * 0.27 : dispWidth * 0.13,
                        height: mobile ? dispHeight * 0.045 : dispHeight * 0.07,
                        child: ElevatedButton(
                            child: Text(
                              "아니요",
                              style: TextStyle(
                                color: Colors.black,
                                fontWeight: FontWeight.bold,
                                fontSize: mobile
                                    ? (dispWidth * 0.04) / txtScale
                                    : (dispHeight * 0.03) / txtScale,
                              ),
                            ),
                            onPressed: () {
                              if (beep) {
                                advancedPlayer
                                    .play(AssetSource('sound/iconClick.mp3'));
                              }
                              Get.back();
                            }),
                      ),
                    ],
                  ),
                );
              },
            ),
            SizedBox(
                width: mobile
                    ? dispWidth * 0.01
                    : (pixelRatio > 1.5)
                        ? dispWidth * 0.001
                        : dispWidth * 0.001),
          ],
        ),
      ),
      body: GestureDetector(
        onTap: () => FocusScope.of(context).unfocus(),
        child: SingleChildScrollView(
          child: Padding(
            padding: const EdgeInsets.all(8.0),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: <Widget>[
                Container(
                  // 조명 이름 설정 Container...
                  width: mobile ? dispWidth * 0.46 : dispWidth * 0.2,
                  height: mobile ? dispHeight * 0.8 : dispHeight * 0.88,
                  decoration: BoxDecoration(
                    borderRadius: BorderRadius.circular(
                        mobile ? dispWidth * 0.02 : dispWidth * borderRadius),
                    border: Border.all(
                        width: mobile
                            ? dispWidth * boardLine
                            : dispHeight * boardLine,
                        color: Colors.black), //Container 테두리 값, 색상
                  ),
                  child: Column(
                      mainAxisAlignment: MainAxisAlignment.spaceAround,
                      children: <Widget>[
                        SizedBox(
                            height:
                                mobile ? dispHeight * 0.02 : dispHeight * 0.01),
                        _ledNameChange(1),
                        _ledNameChange(2),
                        _ledNameChange(3),
                        _ledNameChange(4),
                        _ledNameChange(5),
                        _ledNameChange(6),
                        _ledNameChange(7),
                        _ledNameChange(8),
                      ]),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }
}

class AppBarScreen extends StatefulWidget {
  const AppBarScreen({Key? key}) : super(key: key);

  @override
  State<AppBarScreen> createState() => _AppBarScreenState();
}

class _AppBarScreenState extends State<AppBarScreen> {
  late Timer _timer;

  Future<bool> _checkPermissions() async {
    if (await Permission.location.request().isGranted) {
      return true;
    }
    return false;
  }

  void _connect() async {
    if (await _checkPermissions()) {
      if (networkSelect) {
        FlutterIotWifi.connect(V.hotSpotSSID, V.hotSpotPW, prefix: true)
            .then((value) => print("connect initiated: $value"));
      } else {
        FlutterIotWifi.connect(V.conSSID, V.conPW, prefix: true)
            .then((value) => print("connect initiated: $value"));
      }
    } else {
      print("don't have permission");
    }
  }

  @override
  void dispose() {
    _timer.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return AppBar(
      backgroundColor: appBarColor,
      title: Row(
        crossAxisAlignment: CrossAxisAlignment.center,
        children: [
          // logoFlag // true 면 logo 삽입...
          SizedBox(
            width: mobile ? dispWidth * 0.08 : dispWidth * 0.04, // 고정...
            height: mobile ? 30 : dispWidth * 0.035, // 고정...
            child: Image.asset(
              'assets/images/camperVan.png',
              color: activeColor,
            ),
          ),
          InkWell(
            onTap: () async {
              setState(() {});
            },
            onLongPress: () async {
              SharedPreferences prefs = await SharedPreferences.getInstance();
              displayDir = !displayDir;
              prefs.setBool('displayDir', displayDir);
            },
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                SizedBox(
                  height: mobile ? dispHeight * 0.015 : dispHeight * 0.0,
                ),
                SizedBox(
                  width: mobile ? dispWidth * 0.265 : dispWidth * 0.26,
                  height: mobile ? dispHeight * 0.04 : dispHeight * 0.045,
                  child: Text(
                    logoFlag ? logoText : 'CamTrol V3.5',
                    style: TextStyle(
                      fontWeight: FontWeight.bold,
                      fontStyle: FontStyle.italic,
                      fontSize: mobile
                          ? (dispHeight * 0.02) / txtScale
                          : (dispHeight * 0.04) / txtScale,
                      color: activeColor,
                    ),
                  ),
                ),
              ],
            ),
          ),
        ],
      ),
      actions: <Widget>[
        mobile
            ? const SizedBox.shrink()
            : DigitalClock(
                is24HourTimeFormat: false,
                colon: Text(
                  ":",
                  style: TextStyle(
                    fontSize: dispHeight * 0.025,
                    color: activeColor,
                    fontWeight: FontWeight.bold,
                  ),
                ),
                digitAnimationStyle: Curves.fastLinearToSlowEaseIn,
                areaAligment: AlignmentDirectional.center,
                amPmDigitTextStyle: TextStyle(
                    fontSize: dispHeight * 0.025,
                    color: activeColor,
                    fontWeight: FontWeight.bold),
                hourMinuteDigitTextStyle: TextStyle(
                    fontSize: dispHeight * 0.045,
                    color: activeColor,
                    fontWeight: FontWeight.bold),
                secondDigitTextStyle: TextStyle(
                  fontSize: dispHeight * 0.025,
                  color: activeColor,
                ),
              ),
        mobile ? const SizedBox.shrink() : SizedBox(width: dispWidth * 0.1),
        IconButton(
          icon: Icon(Icons.hotel,
              size: mobile ? dispWidth * 0.06 : dispHeight * 0.05,
              color: sleepFlag
                  ? Colors.black
                  : netFlag
                      ? activeColor
                      : inActiveColor),
          tooltip: '취침모드',
          onPressed: () async {
            if (beep) {
              await advancedPlayer.play(AssetSource('sound/iconClick.mp3'));
            }
            if (mobile == true) {
              if (sleepFlag == false) {
                netFlag
                    ? await server.getReq("SLEEP", 1)
                    : server.getReq("SLEEP", 1);
              } else {
                netFlag
                    ? await server.getReq("SLEEP", 0)
                    : server.getReq("SLEEP", 0);
              }
            } else {
              sleepFlagReset = true;
              netFlag
                  ? await server.getReq("SLEEP", 1)
                  : server.getReq("SLEEP", 1);
            }
            // server.getReq("TIME", 1);
          },
        ),
        SizedBox(width: mobile ? dispWidth * 0.03 : dispWidth * 0.03),
        InkWell(
          onTap: () async {
            if (!netFlag && beep) {
              wifiConFlag = true;
              if (V.conSSID == " " ||
                  V.conPW == " " ||
                  V.hotSpotSSID == " " ||
                  V.hotSpotPW == " ") {
                ScaffoldMessenger.of(context).showSnackBar(
                  const SnackBar(content: Text('네트워크 정보를 확인 해 주세요.')),
                );
                await advancedPlayer.play(AssetSource('sound/dongdong.mp3'));
              } else {
                await advancedPlayer.play(AssetSource('sound/iconClick.mp3'));
                _connect();
              }
            }
          },
          onLongPress: () async {
            if (!netFlag && beep) {
              await advancedPlayer.play(AssetSource('sound/dongdong.mp3'));
              await Get.toNamed("/wifichange");
            }
          },
          child: netFlag
              ? Icon(Icons.wifi,
                  size: mobile ? dispWidth * 0.06 : dispHeight * 0.05,
                  color: netFlag ? activeColor : inActiveColor)
              : Icon(Icons.wifi_off,
                  size: mobile ? dispWidth * 0.06 : dispHeight * 0.05,
                  color: netFlag ? activeColor : Colors.amber),
        ),
        SizedBox(width: mobile ? dispWidth * 0.03 : dispWidth * 0.03),
        IconButton(
          icon: Icon(Icons.change_circle_outlined,
              size: mobile ? dispWidth * 0.06 : dispHeight * 0.05,
              color: netFlag ? activeColor : inActiveColor),
          tooltip: '설정',
          onPressed: () async {
            if (beep) {
              await advancedPlayer.play(AssetSource('sound/iconClick.mp3'));
            }
            await Get.toNamed("/setting");
          },
        ),
        SizedBox(
            width: mobile
                ? (pixelRatio > 1.5)
                    ? dispWidth * 0.0015
                    : dispWidth * 0.03
                : (pixelRatio > 1.5)
                    ? dispWidth * 0.001
                    : dispWidth * 0.03),
      ],
    );
  }
}

class PortScreen extends StatefulWidget {
  const PortScreen({Key? key}) : super(key: key);
  @override
  State<PortScreen> createState() => _PortScreenState();
}

class _PortScreenState extends State<PortScreen> {
  @override
  Widget build(BuildContext context) {
    return Container(
      color: portraitBackColor,
      child: Scaffold(
        // Portrait 디자인...
        backgroundColor: Colors.transparent,
        appBar: PreferredSize(
          preferredSize: Size.fromHeight((dispHeight * appBarHeight) * 0.65),
          child: AppBarScreen(), // 공통으로 적용...
        ),
        body: SingleChildScrollView(
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: <Widget>[
              // Appbar와의 상하간격 늘림...
              SizedBox(
                height: dispHeight * 0.003,
              ),
              Container(
                // Display 창...
                height: dispHeight * 0.12, // 첫번째 Container 높이
                decoration: BoxDecoration(
                  borderRadius:
                      BorderRadius.circular(dispHeight * borderRadius),
                  border: Border.all(
                      width: dispHeight * boardLine,
                      color: netFlag
                          ? Colors.white
                          : Colors.white54), //Container 테두리 값, 색상
                ),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceAround,
                  children: <Widget>[
                    // Battery Icon + 전압표시 BOX
                    SizedBox(
                      // Battery Icon 및 전압 표시...
                      height: dispHeight * 0.1,
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.center,
                        mainAxisAlignment: MainAxisAlignment.spaceAround,
                        children: <Widget>[
                          InkWell(
                            onDoubleTap: () async {
                              server.getReq("TIME", 1);
                            },
                            child: Container(
                              width: dispWidth * 0.13, //Battery Icon 폭...
                              height: dispHeight * 0.05, //Battery Icon 높이...
                              decoration: BoxDecoration(
                                image: DecorationImage(
                                  image: AssetImage(voltPng),
                                  colorFilter: ColorFilter.mode(
                                      netFlag
                                          // ? activeColor
                                          ? batColor
                                          : const Color(0xff808080),
                                      BlendMode.srcIn),
                                  fit: BoxFit.fill,
                                ),
                              ),
                            ),
                          ),
                          SizedBox(
                            width: dispWidth * 0.03,
                          ),
                          Text(
                            netFlag ? voltText : '--.- V',
                            textAlign: TextAlign.right,
                            style: TextStyle(
                              fontWeight: FontWeight.w900,
                              fontSize: (dispWidth * 0.045) /
                                  txtScale, //Battery 전압 Size 기존 : 38
                              fontStyle: FontStyle.italic,
                              // color: netFlag ? batColor : Color(0xff808080)),
                              color: netFlag ? activeColor : Color(0xff808080),
                            ),
                          ),
                        ],
                      ),
                    ),
                    // Battery Icon + 전압표시 BOX
                    SizedBox(
                      // Battery Icon 및 전압 표시...
                      height: dispHeight * 0.1,
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.center,
                        mainAxisAlignment: MainAxisAlignment.spaceAround,
                        children: <Widget>[
                          Container(
                            width: dispWidth * 0.13, //Battery Icon 폭...
                            height: dispHeight * 0.05, //Battery Icon 높이...
                            decoration: BoxDecoration(
                              image: DecorationImage(
                                image: AssetImage(tempPng),
                                colorFilter: ColorFilter.mode(
                                    netFlag
                                        ? activeColor
                                        : const Color(0xff808080),
                                    BlendMode.srcIn),
                                fit: BoxFit.fill,
                              ),
                            ),
                          ),
                          SizedBox(
                            width: dispWidth * 0.03,
                          ),
                          Text(
                            netFlag ? tempText : '--.- ℃',
                            textAlign: TextAlign.right,
                            style: TextStyle(
                              fontWeight: FontWeight.w900,
                              fontSize: (dispWidth * 0.045) /
                                  txtScale, //Battery 전압 Size 기존 : 38
                              fontStyle: FontStyle.italic,
                              color: netFlag ? activeColor : Color(0xff808080),
                            ),
                          ),
                        ],
                      ),
                    ),
                    // Battery Icon + 전압표시 BOX
                    SizedBox(
                      height: dispHeight * 0.1,
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.center,
                        mainAxisAlignment: MainAxisAlignment.spaceAround,
                        children: <Widget>[
                          Container(
                            width: dispWidth * 0.13, //Battery Icon 폭...
                            height: dispHeight * 0.05, //Battery Icon 높이...
                            decoration: BoxDecoration(
                              image: DecorationImage(
                                image: AssetImage('assets/images/humidity.png'),
                                colorFilter: ColorFilter.mode(
                                    netFlag
                                        ? activeColor
                                        : const Color(0xff808080),
                                    BlendMode.srcIn),
                                fit: BoxFit.fill,
                              ),
                            ),
                          ),
                          SizedBox(
                            width: dispWidth * 0.03,
                          ),
                          Text(
                            netFlag
                                ? '${V.humidity.toString()} %  '
                                : '--.- %  ',
                            textAlign: TextAlign.right,
                            style: TextStyle(
                              fontWeight: FontWeight.w900,
                              fontSize: (dispWidth * 0.045) /
                                  txtScale, //Battery 전압 Size 기존 : 38
                              fontStyle: FontStyle.italic,
                              color: netFlag ? activeColor : Color(0xff808080),
                            ),
                          ),
                        ],
                      ),
                    ),
                  ],
                ),
              ),
              // display 창과의 간격 늘림...
              SizedBox(
                height: dispHeight * 0.008,
              ),
              Container(
                height: dispHeight * 0.09,
                decoration: BoxDecoration(
                    borderRadius:
                        BorderRadius.circular(dispHeight * borderRadius),
                    border: Border.all(
                        width: dispHeight * boardLine,
                        color: netFlag ? Colors.white : Colors.white54)),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                  // ignore: prefer_const_literals_to_create_immutables
                  children: <Widget>[
                    // RemoteProcess(),
                    AllLedProcess(),
                    InvertorProcess(),
                    TVProcess(),
                    FRIGEProcess(),
                  ],
                ),
              ),
              SizedBox(
                height: dispHeight * 0.008,
              ),
              // LED 출력용 Container..
              Container(
                // Button 및 Slide Bar 표시...
                height: dispHeight * 0.6,
                decoration: BoxDecoration(
                    borderRadius:
                        BorderRadius.circular(dispHeight * borderRadius),
                    border: Border.all(
                        width: dispHeight * boardLine,
                        color: netFlag ? Colors.white : Colors.white54)),
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.spaceAround,
                  // ignore: prefer_const_literals_to_create_immutables
                  children: <Widget>[
                    LedProcess(1),
                    LedProcess(2),
                    LedProcess(3),
                    LedProcess(4),
                    LedProcess(5),
                    LedProcess(6),
                    LedProcess(7),
                    LedProcess(8),
                  ],
                ),
              ),
              SizedBox(
                height: dispHeight * 0.008,
              ),
              Column(
                children: <Widget>[
                  Container(
                    height: dispHeight * 0.10,
                    decoration: BoxDecoration(
                        borderRadius:
                            BorderRadius.circular(dispHeight * borderRadius),
                        border: Border.all(
                            width: dispHeight * boardLine,
                            color: netFlag ? Colors.white : Colors.white54)),
                    child: Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: <Widget>[
                        InkWell(
                          onTap: () async {
                            stopStatus = false;
                            popUpStatus = true;
                            server.getReq("POP_UP", 1); // 팝업 상승
                            popDownStatus = false;
                            if (beep) {
                              await advancedPlayer
                                  .play(AssetSource('sound/switchOn.mp3'));
                            }
                          },
                          child: SizedBox(
                            width: dispWidth * 0.09, // 고정...
                            height: dispHeight * 0.5, // 고정...
                            child: Image.asset(
                              'assets/images/PopUp.png',
                              color: popUpStatus
                                  ? Colors.amber
                                  : netFlag
                                      ? activeColor
                                      : inActiveColor,
                            ),
                          ),
                        ),
                        InkWell(
                          child: Container(
                            color: Colors.transparent,
                            child: AnimatedContainer(
                              width: dispWidth * 0.18,
                              height: dispHeight * 0.05,
                              decoration: BoxDecoration(
                                  color: const Color(0xff292d32),
                                  borderRadius: BorderRadius.circular(mobile
                                      ? (dispWidth * buttonRadius * 3)
                                      : dispWidth * buttonRadius),
                                  gradient: const LinearGradient(
                                    begin: Alignment.topLeft,
                                    end: Alignment.bottomRight,
                                    colors: [
                                      Color(0xff292d32),
                                      Color(0xff292d32),
                                    ],
                                  ),
                                  boxShadow: stopStatus
                                      ? [
                                          BoxShadow(
                                            color: Colors.amber,
                                            offset: mobile
                                                ? Offset(-2.0, -2.0)
                                                : Offset(-4.0, -4.0),
                                            blurRadius: 6,
                                            spreadRadius: 0.0,
                                          ),
                                          BoxShadow(
                                            color: Colors.amber,
                                            offset: mobile
                                                ? Offset(2.0, 2.0)
                                                : Offset(4.0, 4.0),
                                            blurRadius: 6,
                                            spreadRadius: 0.0,
                                          ),
                                          BoxShadow(
                                            color: Colors.amber,
                                            offset: mobile
                                                ? Offset(2.0, -2.0)
                                                : Offset(4.0, -4.0),
                                            blurRadius: 6,
                                            spreadRadius: 0.0,
                                          ),
                                          BoxShadow(
                                            color: Colors.amber,
                                            offset: mobile
                                                ? Offset(-2.0, 2.0)
                                                : Offset(-4.0, 4.0),
                                            blurRadius: 6,
                                            spreadRadius: 0.0,
                                          ),
                                        ]
                                      : [
                                          BoxShadow(
                                            color: netFlag
                                                ? activeColor
                                                : inActiveColor,
                                            offset: Offset(-0.1, -0.1),
                                            blurRadius: 1,
                                            spreadRadius: 0.0,
                                          ),
                                          BoxShadow(
                                            color: netFlag
                                                ? activeColor
                                                : inActiveColor,
                                            offset: Offset(0.1, 0.1),
                                            blurRadius: 1,
                                            spreadRadius: 0.0,
                                          ),
                                        ]),
                              duration: const Duration(seconds: 1),
                              curve: Curves.fastOutSlowIn,
                              child: Center(
                                child: Text(
                                  '정 지',
                                  style: TextStyle(
                                      fontSize: mobile
                                          ? (dispWidth * 0.04) / txtScale
                                          : (dispWidth * 0.018) / txtScale,
                                      color: stopStatus
                                          ? Colors.amber
                                          : netFlag
                                              ? activeColor
                                              : inActiveColor,
                                      fontWeight: FontWeight.w700),
                                ),
                              ),
                            ),
                          ),
                          onTap: () async {
                            stopStatus = true;
                            if (beep) {
                              await advancedPlayer
                                  .play(AssetSource('sound/dongdong.mp3'));
                              popUpStatus = false;
                              popDownStatus = false;
                              server.getReq("POP_STOP", 1); // 팝업 정지
                            }
                          },
                        ),
                        InkWell(
                          onTap: () async {
                            stopStatus = false;
                            popUpStatus = false;
                            popDownStatus = true;
                            server.getReq("POP_DOWN", 1); // 팝업 하강
                            if (beep) {
                              await advancedPlayer
                                  .play(AssetSource('sound/switchOn.mp3'));
                            }
                          },
                          child: SizedBox(
                            width: mobile
                                ? dispWidth * 0.09
                                : dispWidth * 0.05, // 고정...
                            height: dispHeight * 0.15, // 고정...
                            child: Image.asset(
                              'assets/images/PopDown.png',
                              color: popDownStatus
                                  ? Colors.amber
                                  : netFlag
                                      ? activeColor
                                      : inActiveColor,
                            ),
                          ),
                        ),
                      ],
                    ),
                  ),
                ],
              ),
              SizedBox(
                height: dispHeight * 0.1,
              ),
              InkWell(
                onTap: () {
                  V.isPhoneTable[1]
                      ? launch("tel://${V.phoneNumber[1]}")
                      : null;
                },
                onLongPress: () async {
                  V.comFlag = false;
                  await Get.toNamed("/phoneNumber");
                  V.comFlag = true;
                },
                child: Container(
                  alignment: Alignment.center,
                  height: dispHeight * 0.05,
                  child: Column(
                    children: <Widget>[
                      V.isPhoneTable[1]
                          ? Text(
                              "${V.phoneName[1]} :  ${V.phoneNumber[1]}",
                              style: TextStyle(
                                color: Colors.yellow,
                                fontWeight: FontWeight.bold,
                                fontStyle: FontStyle.italic,
                                fontSize: mobile
                                    ? (dispHeight * 0.03) / txtScale
                                    : (dispWidth * 0.03) / txtScale,
                              ),
                            )
                          : Text(
                              '긴급 연락처 등록',
                              style: TextStyle(
                                color: Colors.yellow,
                                fontWeight: FontWeight.bold,
                                fontStyle: FontStyle.italic,
                                fontSize: mobile
                                    ? (dispHeight * 0.03) / txtScale
                                    : (dispWidth * 0.03) / txtScale,
                              ),
                            ),
                    ],
                  ),
                ),
              ),
              V.isPhoneTable[2]
                  ? InkWell(
                      onTap: () {
                        launch("tel://${V.phoneNumber[2]}");
                      },
                      onLongPress: () async {
                        await Get.toNamed("/phoneNumber");
                      },
                      child: Container(
                        alignment: Alignment.center,
                        margin: const EdgeInsets.symmetric(vertical: 2.0),
                        child: Padding(
                          padding: const EdgeInsets.all(5.0),
                          child: Column(
                            children: <Widget>[
                              Text(
                                "${V.phoneName[2]} :  ${V.phoneNumber[2]}",
                                style: TextStyle(
                                  color: Colors.yellow,
                                  fontWeight: FontWeight.bold,
                                  fontStyle: FontStyle.italic,
                                  fontSize: mobile
                                      ? (dispHeight * 0.03) / txtScale
                                      : (dispWidth * 0.03) / txtScale,
                                ),
                              )
                            ],
                          ),
                        ),
                      ),
                    )
                  : const SizedBox.shrink(),
              V.isPhoneTable[3]
                  ? InkWell(
                      onTap: () {
                        launch("tel://${V.phoneNumber[3]}");
                      },
                      onLongPress: () async {
                        await Get.toNamed("/phoneNumber");
                      },
                      child: Container(
                        alignment: Alignment.center,
                        margin: const EdgeInsets.symmetric(vertical: 2.0),
                        child: Padding(
                          padding: const EdgeInsets.all(5.0),
                          child: Column(
                            children: <Widget>[
                              Text(
                                "${V.phoneName[3]} :  ${V.phoneNumber[3]}",
                                style: TextStyle(
                                  color: Colors.yellow,
                                  fontWeight: FontWeight.bold,
                                  fontStyle: FontStyle.italic,
                                  fontSize: mobile
                                      ? (dispHeight * 0.03) / txtScale
                                      : (dispWidth * 0.03) / txtScale,
                                ),
                              )
                            ],
                          ),
                        ),
                      ),
                    )
                  : const SizedBox.shrink(),
              V.isPhoneTable[4]
                  ? InkWell(
                      onTap: () {
                        launch("tel://${V.phoneNumber[4]}");
                      },
                      onLongPress: () async {
                        await Get.toNamed("/phoneNumber");
                      },
                      child: Container(
                        alignment: Alignment.center,
                        margin: const EdgeInsets.symmetric(vertical: 2.0),
                        child: Padding(
                          padding: const EdgeInsets.all(5.0),
                          child: Column(
                            children: <Widget>[
                              Text(
                                "${V.phoneName[4]} :  ${V.phoneNumber[4]}",
                                style: TextStyle(
                                  color: Colors.yellow,
                                  fontWeight: FontWeight.bold,
                                  fontStyle: FontStyle.italic,
                                  fontSize: mobile
                                      ? (dispHeight * 0.03) / txtScale
                                      : (dispWidth * 0.03) / txtScale,
                                ),
                              )
                            ],
                          ),
                        ),
                      ),
                    )
                  : const SizedBox.shrink(),
              V.isPhoneTable[5]
                  ? InkWell(
                      onTap: () {
                        launch("tel://${V.phoneNumber[5]}");
                      },
                      onLongPress: () async {
                        await Get.toNamed("/phoneNumber");
                      },
                      child: Container(
                        alignment: Alignment.center,
                        margin: const EdgeInsets.symmetric(vertical: 2.0),
                        child: Padding(
                          padding: const EdgeInsets.all(5.0),
                          child: Column(
                            children: <Widget>[
                              Text(
                                "${V.phoneName[5]} :  ${V.phoneNumber[5]}",
                                style: TextStyle(
                                  color: Colors.yellow,
                                  fontWeight: FontWeight.bold,
                                  fontStyle: FontStyle.italic,
                                  fontSize: mobile
                                      ? (dispHeight * 0.03) / txtScale
                                      : (dispWidth * 0.03) / txtScale,
                                ),
                              )
                            ],
                          ),
                        ),
                      ),
                    )
                  : const SizedBox.shrink(),
            ],
          ),
        ),
      ),
    );
  }
}

// ignore: must_be_immutable
class LedProcess extends StatefulWidget {
  final int ledCh;
  // ignore: prefer_typing_uninitialized_variables
  const LedProcess(this.ledCh, {Key? key}) : super(key: key);

  @override
  // ignore: no_logic_in_create_state, library_private_types_in_public_api
  _LedProcessState createState() => _LedProcessState(ledCh);
}

class _LedProcessState extends State<LedProcess> {
  final int ledCh;
  _LedProcessState(this.ledCh);

  @override
  Widget build(BuildContext context) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
      children: (ledCh == 0)
          ? <Widget>[
              SizedBox(
                width: mobile ? dispWidth * 0.08 : dispWidth * 0.05,
                child: Text(
                  '${(V.ledValue[ledCh]).round()}',
                  textAlign: TextAlign.right,
                  style: TextStyle(
                      color: V.ledStatus[ledCh] ? Colors.amber : inActiveColor,
                      fontSize: mobile
                          ? (dispWidth * 0.04) / txtScale
                          : (dispWidth * 0.023) / txtScale,
                      fontStyle: FontStyle.italic,
                      fontWeight: FontWeight.w800),
                ),
              ),
            ]
          : <Widget>[
              SizedBox(
                width: mobile ? dispWidth * 0.08 : dispWidth * 0.05,
                child: Text(
                  '${(V.ledValue[ledCh]).round()}',
                  textAlign: TextAlign.right,
                  style: TextStyle(
                      color: V.ledStatus[ledCh]
                          ? Colors.amber
                          : netFlag
                              ? activeColor
                              : inActiveColor,
                      fontSize: mobile
                          ? (dispWidth * 0.04) / txtScale
                          : (dispWidth * 0.023) / txtScale,
                      fontStyle: FontStyle.italic,
                      fontWeight: FontWeight.w800),
                ),
              ),
              SizedBox(
                // Slider 길이
                width: mobile ? dispWidth * 0.7 : dispWidth * 0.45,
                height: mobile ? dispHeight * 0.06 : dispHeight * 0.08,
                child: SliderTheme(
                  data: SliderTheme.of(context).copyWith(
                    trackHeight:
                        mobile ? dispHeight * 0.004 : dispHeight * 0.008,
                    thumbColor: Colors.redAccent,
                    thumbShape: RoundSliderThumbShape(
                        enabledThumbRadius:
                            mobile ? dispWidth * 0.022 : dispWidth * 0.012),
                    overlayColor: Colors.red.withAlpha(32),
                    overlayShape:
                        const RoundSliderOverlayShape(overlayRadius: 28.0),
                  ),
                  child: Slider(
                    min: 1.0,
                    max: 100.0,
                    inactiveColor: netFlag ? activeColor : inActiveColor,
                    activeColor: Colors.amber,
                    value: V.ledValue[ledCh],
                    onChanged: V.ledStatus[ledCh]
                        ? (value) => setState(() {
                              V.ledValue[ledCh] = value;
                              if (mobile) {
                                V.monitor =
                                    '${V.ledName[ledCh]} 밝기 값  ${V.ledValue[ledCh].round()}';
                              }
                              print('${V.ledName[ledCh]} Slider Value : '
                                  '${(V.ledValue[ledCh]).round()}..');
                            })
                        : null,
                    onChangeEnd: (double newValue) {
                      V.ledValue[ledCh] = newValue;
                      server.getReq("LED", ledCh);
                    },
                    label: (V.ledValue[ledCh]).round().toInt().toString(),
                  ),
                ),
              ),
              SizedBox(
                width: mobile ? 0 : dispWidth * 0.008,
              ),
              InkWell(
                child: Container(
                  color: Colors.transparent,
                  child: AnimatedContainer(
                    width: mobile ? dispWidth * 0.16 : dispWidth * 0.09,
                    height: mobile ? dispHeight * 0.045 : dispHeight * 0.08,
                    decoration: BoxDecoration(
                        color: const Color(0xff292d32),
                        borderRadius: BorderRadius.circular(mobile
                            ? (dispWidth * buttonRadius * 3)
                            : dispWidth * buttonRadius),
                        gradient: const LinearGradient(
                          begin: Alignment.topLeft,
                          end: Alignment.bottomRight,
                          colors: [
                            Color(0xff292d32),
                            Color(0xff292d32),
                          ],
                        ),
                        boxShadow: V.ledStatus[ledCh]
                            ? [
                                BoxShadow(
                                  color: Colors.amber,
                                  offset: mobile
                                      ? Offset(-2.0, -2.0)
                                      : Offset(-4.0, -4.0),
                                  blurRadius: 6,
                                  spreadRadius: 0.0,
                                ),
                                BoxShadow(
                                  color: Colors.amber,
                                  offset: mobile
                                      ? Offset(2.0, 2.0)
                                      : Offset(4.0, 4.0),
                                  blurRadius: 6,
                                  spreadRadius: 0.0,
                                ),
                                BoxShadow(
                                  color: Colors.amber,
                                  offset: mobile
                                      ? Offset(2.0, -2.0)
                                      : Offset(4.0, -4.0),
                                  blurRadius: 6,
                                  spreadRadius: 0.0,
                                ),
                                BoxShadow(
                                  color: Colors.amber,
                                  offset: mobile
                                      ? Offset(-2.0, 2.0)
                                      : Offset(-4.0, 4.0),
                                  blurRadius: 6,
                                  spreadRadius: 0.0,
                                ),
                              ]
                            : [
                                BoxShadow(
                                  color: netFlag ? activeColor : inActiveColor,
                                  offset: Offset(-0.1, -0.1),
                                  blurRadius: 1,
                                  spreadRadius: 0.0,
                                ),
                                BoxShadow(
                                  color: netFlag ? activeColor : inActiveColor,
                                  offset: Offset(0.1, 0.1),
                                  blurRadius: 1,
                                  spreadRadius: 0.0,
                                ),
                              ]),
                    duration: const Duration(seconds: 1),
                    curve: Curves.fastOutSlowIn,
                    child: Center(
                      child: Text(
                        V.ledName[ledCh],
                        style: TextStyle(
                            fontSize: mobile
                                ? (dispWidth * 0.04) / txtScale
                                : (dispWidth * 0.018) / txtScale,
                            color: V.ledStatus[ledCh]
                                ? Colors.amber
                                : netFlag
                                    ? activeColor
                                    : inActiveColor,
                            fontWeight: FontWeight.w700),
                      ),
                    ),
                  ),
                ),
                onTap: () async {
                  V.ledStatus[ledCh]
                      ? V.ledStatus[ledCh] = false
                      : V.ledStatus[ledCh] = true;

                  server.getReq("LED", ledCh);

                  if (V.ledStatus[ledCh] == true) {
                    V.monitor = '${V.ledName[ledCh]} On..';
                    if (beep) {
                      await advancedPlayer
                          .play(AssetSource('sound/switchOn.mp3'));
                    }
                  } else {
                    V.monitor = '${V.ledName[ledCh]} Off..';
                    if (beep) {
                      await advancedPlayer
                          .play(AssetSource('sound/switchOff.mp3'));
                    }
                  }
                  print('_isChecked_led[ledCh] : ${V.ledStatus[ledCh]}');
                  setState(() {});
                },
                onLongPress: () {},
              ),
              SizedBox(
                width: mobile ? dispWidth * 0.03 : dispWidth * 0.01,
              )
            ],
    );
  }
}

class TVProcess extends StatefulWidget {
  const TVProcess({Key? key}) : super(key: key);

  @override
  State<TVProcess> createState() => _TVProcessState();
}

class _TVProcessState extends State<TVProcess> {
  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        color: Colors.transparent,
        child: AnimatedContainer(
          width: mobile ? dispWidth * 0.16 : dispWidth * 0.09,
          height: mobile ? dispHeight * 0.045 : dispHeight * 0.08,
          decoration: BoxDecoration(
              color: const Color(0xff292d32),
              borderRadius: BorderRadius.circular(mobile
                  ? (dispWidth * buttonRadius * 3)
                  : dispWidth * buttonRadius),
              gradient: const LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [
                  Color(0xff292d32),
                  Color(0xff292d32),
                ],
              ),
              boxShadow: tvStatus
                  ? [
                      BoxShadow(
                        color: Colors.amber,
                        offset:
                            mobile ? Offset(-2.0, -2.0) : Offset(-4.0, -4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        offset: mobile ? Offset(2.0, 2.0) : Offset(4.0, 4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        offset: mobile ? Offset(2.0, -2.0) : Offset(4.0, -4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        offset: mobile ? Offset(-2.0, 2.0) : Offset(-4.0, 4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                    ]
                  : [
                      BoxShadow(
                        color: netFlag ? activeColor : inActiveColor,
                        offset: Offset(-0.1, -0.1),
                        blurRadius: 1,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: netFlag ? activeColor : inActiveColor,
                        offset: Offset(0.1, 0.1),
                        blurRadius: 1,
                        spreadRadius: 0.0,
                      ),
                    ]),
          duration: const Duration(seconds: 1),
          curve: Curves.fastOutSlowIn,
          child: Center(
            child: Text(
              'TV',
              style: TextStyle(
                  fontSize: mobile
                      ? (dispWidth * 0.04) / txtScale
                      : (dispWidth * 0.018) / txtScale,
                  color: tvStatus
                      ? Colors.amber
                      : netFlag
                          ? activeColor
                          : inActiveColor,
                  fontWeight: FontWeight.w700),
            ),
          ),
        ),
      ),
      onTap: () async {
        if (tvStatus == true) {
          if (beep) {
            await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
          }
          tvStatus = false;
          V.monitor = 'TV Off..';
        } else {
          if (beep) {
            await advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
          }
          tvStatus = true;
          V.monitor = 'TV On..';
        }
        setState(() {});
        server.getReq("TV", 0);
        tvStatus ? print('TV On..') : print('TV Off..');
      },
    );
  }
}

class FRIGEProcess extends StatefulWidget {
  const FRIGEProcess({Key? key}) : super(key: key);

  @override
  State<FRIGEProcess> createState() => _FRIGEProcessState();
}

class _FRIGEProcessState extends State<FRIGEProcess> {
  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        color: Colors.transparent,
        child: AnimatedContainer(
          width: mobile ? dispWidth * 0.16 : dispWidth * 0.09,
          height: mobile ? dispHeight * 0.045 : dispHeight * 0.08,
          decoration: BoxDecoration(
              color: const Color(0xff292d32),
              borderRadius: BorderRadius.circular(mobile
                  ? (dispWidth * buttonRadius * 3)
                  : dispWidth * buttonRadius),
              gradient: const LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [
                  Color(0xff292d32),
                  Color(0xff292d32),
                ],
              ),
              boxShadow: frigeStatus
                  ? [
                      BoxShadow(
                        color: Colors.amber,
                        offset:
                            mobile ? Offset(-2.0, -2.0) : Offset(-4.0, -4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        offset: mobile ? Offset(2.0, 2.0) : Offset(4.0, 4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        offset: mobile ? Offset(2.0, -2.0) : Offset(4.0, -4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        offset: mobile ? Offset(-2.0, 2.0) : Offset(-4.0, 4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                    ]
                  : [
                      BoxShadow(
                        color: netFlag ? activeColor : inActiveColor,
                        offset: Offset(-0.1, -0.1),
                        blurRadius: 1,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: netFlag ? activeColor : inActiveColor,
                        offset: Offset(0.1, 0.1),
                        blurRadius: 1,
                        spreadRadius: 0.0,
                      ),
                    ]),
          duration: const Duration(seconds: 1),
          curve: Curves.fastOutSlowIn,
          child: Center(
            child: Text(
              '냉장고',
              style: TextStyle(
                  fontSize: mobile
                      ? (dispWidth * 0.04) / txtScale
                      : (dispWidth * 0.018) / txtScale,
                  color: frigeStatus
                      ? Colors.amber
                      : netFlag
                          ? activeColor
                          : inActiveColor,
                  fontWeight: FontWeight.w700),
            ),
          ),
        ),
      ),
      onTap: () async {
        if (frigeStatus == true) {
          if (beep) {
            await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
          }
          frigeStatus = false;
          V.monitor = '냉장고 Off..';
        } else {
          if (beep) {
            await advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
          }
          frigeStatus = true;
          V.monitor = '냉장고 On..';
        }
        setState(() {});
        server.getReq("FRIGE", 0);
        frigeStatus ? print('냉장고 On..') : print('냉장고 Off..');
      },
    );
  }
}

class InvertorProcess extends StatefulWidget {
  const InvertorProcess({Key? key}) : super(key: key);

  @override
  State<InvertorProcess> createState() => _InvertorProcessState();
}

class _InvertorProcessState extends State<InvertorProcess> {
  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        color: Colors.transparent,
        child: AnimatedContainer(
          width: mobile ? dispWidth * 0.16 : dispWidth * 0.09,
          height: mobile ? dispHeight * 0.045 : dispHeight * 0.08,
          decoration: BoxDecoration(
              color: const Color(0xff292d32),
              borderRadius: BorderRadius.circular(mobile
                  ? (dispWidth * buttonRadius * 3)
                  : dispWidth * buttonRadius),
              gradient: const LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [
                  Color(0xff292d32),
                  Color(0xff292d32),
                ],
              ),
              boxShadow: invertorStatus
                  ? [
                      BoxShadow(
                        color: Colors.amber,
                        offset:
                            mobile ? Offset(-2.0, -2.0) : Offset(-4.0, -4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        offset: mobile ? Offset(2.0, 2.0) : Offset(4.0, 4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        offset: mobile ? Offset(2.0, -2.0) : Offset(4.0, -4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        offset: mobile ? Offset(-2.0, 2.0) : Offset(-4.0, 4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                    ]
                  : [
                      BoxShadow(
                        color: netFlag ? activeColor : inActiveColor,
                        offset: Offset(-0.1, -0.1),
                        blurRadius: 1,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: netFlag ? activeColor : inActiveColor,
                        offset: Offset(0.1, 0.1),
                        blurRadius: 1,
                        spreadRadius: 0.0,
                      ),
                    ]),
          duration: const Duration(seconds: 1),
          curve: Curves.fastOutSlowIn,
          child: Center(
            child: Text(
              '인버터',
              style: TextStyle(
                  fontSize: mobile
                      ? (dispWidth * 0.04) / txtScale
                      : (dispWidth * 0.018) / txtScale,
                  color: invertorStatus
                      ? Colors.amber
                      : netFlag
                          ? activeColor
                          : inActiveColor,
                  fontWeight: FontWeight.w700),
            ),
          ),
        ),
      ),
      onTap: () async {
        if (invertorStatus == true) {
          if (beep) {
            await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
          }
          invertorStatus = false;
          await server.getReq("INVERTOR", 0);
          V.monitor = '인버터 Off..';
        } else {
          if (beep) {
            await advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
          }
          invertorStatus = true;
          await server.getReq("INVERTOR", 1);
          V.monitor = '인버터 On..';
        }
        setState(() {});
        invertorStatus ? print('인버터 On..') : print('인버터 Off..');
      },
    );
  }
}

class RemoteProcess extends StatefulWidget {
  const RemoteProcess({Key? key}) : super(key: key);

  @override
  State<RemoteProcess> createState() => _RemoteProcessState();
}

class _RemoteProcessState extends State<RemoteProcess> {
  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        color: Colors.transparent,
        child: AnimatedContainer(
          width: mobile ? dispWidth * 0.16 : dispWidth * 0.09,
          height: mobile ? dispHeight * 0.045 : dispHeight * 0.08,
          decoration: BoxDecoration(
              color: const Color(0xff292d32),
              borderRadius: BorderRadius.circular(mobile
                  ? (dispWidth * buttonRadius * 3)
                  : dispWidth * buttonRadius),
              gradient: const LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [
                  Color(0xff292d32),
                  Color(0xff292d32),
                ],
              ),
              boxShadow: remoteStatus
                  ? [
                      BoxShadow(
                        color: Colors.amber,
                        offset:
                            mobile ? Offset(-2.0, -2.0) : Offset(-4.0, -4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        offset: mobile ? Offset(2.0, 2.0) : Offset(4.0, 4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        offset: mobile ? Offset(2.0, -2.0) : Offset(4.0, -4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        offset: mobile ? Offset(-2.0, 2.0) : Offset(-4.0, 4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                    ]
                  : [
                      BoxShadow(
                        color: netFlag ? activeColor : inActiveColor,
                        offset: Offset(-0.1, -0.1),
                        blurRadius: 1,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: netFlag ? activeColor : inActiveColor,
                        offset: Offset(0.1, 0.1),
                        blurRadius: 1,
                        spreadRadius: 0.0,
                      ),
                    ]),
          duration: const Duration(seconds: 1),
          curve: Curves.fastOutSlowIn,
          child: Center(
            child: Text(
              '원 격',
              style: TextStyle(
                  fontSize: mobile
                      ? (dispWidth * 0.04) / txtScale
                      : (dispWidth * 0.018) / txtScale,
                  color: remoteStatus
                      ? Colors.amber
                      : netFlag
                          ? activeColor
                          : inActiveColor,
                  fontWeight: FontWeight.w700),
            ),
          ),
        ),
      ),
      onTap: () async {
        if (remoteStatus == true) {
          server.getReq("REMOTE", 0);
          if (beep) {
            await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
          }
          remoteStatus = false;
        } else {
          server.getReq("REMOTE", 1);
          if (beep) {
            await advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
          }
          remoteStatus = true;
        }

        setState(() {});
        // await server.getReq("REMOTE", remoteStatus ? 1 : 0);
      },
    );
  }
}

class AllLedProcess extends StatefulWidget {
  const AllLedProcess({Key? key}) : super(key: key);

  @override
  State<AllLedProcess> createState() => _AllLedProcessState();
}

class _AllLedProcessState extends State<AllLedProcess> {
  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        color: Colors.transparent,
        child: AnimatedContainer(
          width: mobile ? dispWidth * 0.16 : dispWidth * 0.09,
          height: mobile ? dispHeight * 0.045 : dispHeight * 0.08,
          decoration: BoxDecoration(
              color: const Color(0xff292d32),
              borderRadius: BorderRadius.circular(mobile
                  ? (dispWidth * buttonRadius * 3)
                  : dispWidth * buttonRadius),
              gradient: const LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [
                  Color(0xff292d32),
                  Color(0xff292d32),
                ],
              ),
              boxShadow: allLedStatus
                  ? [
                      BoxShadow(
                        color: Colors.amber,
                        offset:
                            mobile ? Offset(-2.0, -2.0) : Offset(-4.0, -4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        offset: mobile ? Offset(2.0, 2.0) : Offset(4.0, 4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        offset: mobile ? Offset(2.0, -2.0) : Offset(4.0, -4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        offset: mobile ? Offset(-2.0, 2.0) : Offset(-4.0, 4.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                    ]
                  : [
                      BoxShadow(
                        color: netFlag ? activeColor : inActiveColor,
                        offset: Offset(-0.1, -0.1),
                        blurRadius: 1,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: netFlag ? activeColor : inActiveColor,
                        offset: Offset(0.1, 0.1),
                        blurRadius: 1,
                        spreadRadius: 0.0,
                      ),
                    ]),
          duration: const Duration(seconds: 1),
          curve: Curves.fastOutSlowIn,
          child: Center(
            child: Text(
              '전체등',
              style: TextStyle(
                  fontSize: mobile
                      ? (dispWidth * 0.04) / txtScale
                      : (dispWidth * 0.018) / txtScale,
                  color: allLedStatus
                      ? Colors.amber
                      : netFlag
                          ? activeColor
                          : inActiveColor,
                  fontWeight: FontWeight.w700),
            ),
          ),
        ),
      ),
      onTap: () async {
        if (allLedStatus == true) {
          if (beep) {
            await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
          }
          allLedStatus = false;
          V.ledStatus[1] = false;
          V.ledStatus[2] = false;
          V.ledStatus[3] = false;
          V.ledStatus[4] = false;
          V.ledStatus[5] = false;
          V.ledStatus[6] = false;
          V.monitor = '전체등 Off..';
        } else {
          if (beep) {
            await advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
          }
          allLedStatus = true;
          V.ledStatus[1] = true;
          V.ledStatus[2] = true;
          V.ledStatus[3] = true;
          V.ledStatus[4] = true;
          V.ledStatus[5] = true;
          V.ledStatus[6] = true;
          V.monitor = '전체등 On..';
        }

        setState(() {});
        await server.getReq("ALL", allLedStatus ? 1 : 0);
        await server.getReq("request", 0);
        allLedStatus ? print('전체등 On..') : print('전체등 Off..');
      },
    );
  }
}

class LandScreen extends StatefulWidget {
  const LandScreen({Key? key}) : super(key: key);
  @override
  State<LandScreen> createState() => _LandScreenState();
}

class _LandScreenState extends State<LandScreen> {
  @override
  Widget build(BuildContext context) {
    return Container(
      color: const Color(0xff333333),
      child: Scaffold(
        // landscape 디자인...
        backgroundColor: Colors.transparent, //Colors.transparent,
        appBar: PreferredSize(
          preferredSize: Size.fromHeight(
            dispHeight * appBarHeight,
          ),
          child: AppBarScreen(), // 공통으로 적용...
        ),

        body: SingleChildScrollView(
          child: Row(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: <Widget>[
              Container(
                // Display 창...
                width: dispWidth * 0.2,
                height: dispHeight * 0.919,
                margin: EdgeInsets.symmetric(vertical: dispHeight * 0.001),
                decoration: BoxDecoration(
                  borderRadius: BorderRadius.circular(dispWidth * borderRadius),
                  border: Border.all(
                      width: dispWidth * boardLine,
                      color: netFlag
                          ? Colors.white
                          : Colors.white54), //Container 테두리 값, 색상
                ),
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.spaceAround,
                  children: <Widget>[
                    SizedBox(
                      // Battery Icon 및 전압 표시...
                      height: dispHeight * 0.1, //Battery Icon 높이 기존 : 100
                      child: Row(
                        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                        children: <Widget>[
                          InkWell(
                            onDoubleTap: () async {
                              server.getReq("TIME", 1);
                            },
                            child: Container(
                              width: mobile
                                  ? (dispWidth * 0.06) * 1.1
                                  : (dispWidth * 0.06) * 0.9, //Battery Icon 폭
                              height:
                                  dispWidth * 0.045, //Battery Icon 높이 기존 : 100
                              decoration: BoxDecoration(
                                image: DecorationImage(
                                  image: AssetImage(voltPng),
                                  colorFilter: ColorFilter.mode(
                                      netFlag
                                          // ? Colors.white
                                          ? batColor
                                          : const Color(0xff808080),
                                      BlendMode.srcIn),
                                  fit: BoxFit.fill,
                                ),
                              ),
                            ),
                          ),
                          Text(
                            netFlag ? voltText : '--.- V',
                            textAlign: TextAlign.right,
                            style: TextStyle(
                              fontWeight: FontWeight.w700,
                              fontSize: (dispWidth * 0.028) /
                                  txtScale, //Battery 전압 Size 기존 : 38
                              fontStyle: FontStyle.italic,
                              color: netFlag
                                  ? Colors.white
                                  : const Color(0xff808080),
                            ),
                          ),
                        ],
                      ),
                    ),
                    SizedBox(
                      // 온도계 및 온도표시...
                      height: dispHeight * 0.1, //Battery Icon 높이 기존 : 100
                      child: Row(
                        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                        children: <Widget>[
                          SizedBox(
                            width: dispWidth * 0.003,
                          ),
                          Container(
                            width: (dispWidth * 0.06) / 1.2, //Battery Icon 폭
                            height:
                                dispWidth * 0.045, //Battery Icon 높이 기존 : 100
                            decoration: BoxDecoration(
                              image: DecorationImage(
                                image:
                                    const AssetImage('assets/images/temp.png'),
                                colorFilter: ColorFilter.mode(
                                    // mainIconColor, BlendMode.srcIn),
                                    netFlag
                                        ? Colors.white
                                        : const Color(0xff808080),
                                    BlendMode.srcIn),
                                fit: BoxFit.fill,
                              ),
                            ),
                          ),
                          Text(
                            netFlag ? tempText : '--.- ℃',
                            textAlign: TextAlign.right,
                            style: TextStyle(
                              fontWeight: FontWeight.w700,
                              fontSize: (dispWidth * 0.027) / txtScale,
                              fontStyle: FontStyle.italic,
                              color: netFlag
                                  ? Colors.white
                                  : const Color(0xff808080),
                            ),
                          )
                        ],
                      ),
                    ),
                    SizedBox(
                      // 습도계 및 습도표시...
                      height: dispHeight * 0.1, //습도계 SizeBox 높이 기존 : 100
                      child: Row(
                        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                        children: <Widget>[
                          SizedBox(
                            width: dispWidth * 0.001,
                          ),
                          Container(
                            width: (dispWidth * 0.06) / 1.25, //Battery Icon 폭
                            height:
                                dispWidth * 0.045, //Battery Icon 높이 기존 : 100
                            decoration: BoxDecoration(
                              image: DecorationImage(
                                image: const AssetImage(
                                    'assets/images/humidity.png'),
                                colorFilter: ColorFilter.mode(
                                    netFlag
                                        ? Colors.white
                                        : const Color(0xff808080),
                                    BlendMode.srcIn),
                                fit: BoxFit.fill,
                              ),
                            ),
                          ),
                          Text(
                            netFlag ? '${V.humidity.toString()} % ' : '--.- %',
                            textAlign: TextAlign.right,
                            style: TextStyle(
                              fontWeight: FontWeight.w700,
                              fontSize: (dispWidth * 0.028) / txtScale,
                              fontStyle: FontStyle.italic,
                              color: netFlag
                                  ? Colors.white
                                  : const Color(0xff808080),
                            ),
                          )
                        ],
                      ),
                    ),
                    InkWell(
                      onTap: () async {
                        stopStatus = false;
                        popUpStatus = true;
                        server.getReq("POP_UP", 1); // 팝업 상승
                        popDownStatus = false;
                        if (beep) {
                          await advancedPlayer
                              .play(AssetSource('sound/switchOn.mp3'));
                        }
                      },
                      child: SizedBox(
                        width: dispWidth * 0.1, // 고정...
                        height: dispHeight * 0.12, // 고정...
                        child: Image.asset(
                          'assets/images/PopUp.png',
                          color: popUpStatus
                              ? Colors.amber
                              : netFlag
                                  ? activeColor
                                  : inActiveColor,
                        ),
                      ),
                    ),
                    InkWell(
                      child: Container(
                        color: Colors.transparent,
                        child: AnimatedContainer(
                          width: dispWidth * 0.08,
                          height: dispHeight * 0.1,
                          decoration: BoxDecoration(
                              color: const Color(0xff292d32),
                              borderRadius: BorderRadius.circular(mobile
                                  ? (dispWidth * buttonRadius * 3)
                                  : dispWidth * buttonRadius),
                              gradient: const LinearGradient(
                                begin: Alignment.topLeft,
                                end: Alignment.bottomRight,
                                colors: [
                                  Color(0xff292d32),
                                  Color(0xff292d32),
                                ],
                              ),
                              boxShadow: stopStatus
                                  ? [
                                      BoxShadow(
                                        color: Colors.amber,
                                        offset: mobile
                                            ? Offset(-2.0, -2.0)
                                            : Offset(-4.0, -4.0),
                                        blurRadius: 6,
                                        spreadRadius: 0.0,
                                      ),
                                      BoxShadow(
                                        color: Colors.amber,
                                        offset: mobile
                                            ? Offset(2.0, 2.0)
                                            : Offset(4.0, 4.0),
                                        blurRadius: 6,
                                        spreadRadius: 0.0,
                                      ),
                                      BoxShadow(
                                        color: Colors.amber,
                                        offset: mobile
                                            ? Offset(2.0, -2.0)
                                            : Offset(4.0, -4.0),
                                        blurRadius: 6,
                                        spreadRadius: 0.0,
                                      ),
                                      BoxShadow(
                                        color: Colors.amber,
                                        offset: mobile
                                            ? Offset(-2.0, 2.0)
                                            : Offset(-4.0, 4.0),
                                        blurRadius: 6,
                                        spreadRadius: 0.0,
                                      ),
                                    ]
                                  : [
                                      BoxShadow(
                                        color: netFlag
                                            ? activeColor
                                            : inActiveColor,
                                        offset: Offset(-0.1, -0.1),
                                        blurRadius: 1,
                                        spreadRadius: 0.0,
                                      ),
                                      BoxShadow(
                                        color: netFlag
                                            ? activeColor
                                            : inActiveColor,
                                        offset: Offset(0.1, 0.1),
                                        blurRadius: 1,
                                        spreadRadius: 0.0,
                                      ),
                                    ]),
                          duration: const Duration(seconds: 1),
                          curve: Curves.fastOutSlowIn,
                          child: Center(
                            child: Text(
                              '정 지',
                              style: TextStyle(
                                  fontSize: mobile
                                      ? (dispWidth * 0.04) / txtScale
                                      : (dispWidth * 0.018) / txtScale,
                                  color: stopStatus
                                      ? Colors.amber
                                      : netFlag
                                          ? activeColor
                                          : inActiveColor,
                                  fontWeight: FontWeight.w700),
                            ),
                          ),
                        ),
                      ),
                      onTap: () async {
                        stopStatus = true;
                        if (beep) {
                          await advancedPlayer
                              .play(AssetSource('sound/dongdong.mp3'));
                          popUpStatus = false;
                          popDownStatus = false;
                          server.getReq("POP_STOP", 1); // 팝업 정지
                        }
                      },
                    ),
                    InkWell(
                      onTap: () async {
                        stopStatus = false;
                        popUpStatus = false;
                        popDownStatus = true;
                        server.getReq("POP_DOWN", 1); // 팝업 하강
                        if (beep) {
                          await advancedPlayer
                              .play(AssetSource('sound/switchOn.mp3'));
                        }
                      },
                      child: SizedBox(
                        width: dispWidth * 0.1, // 고정...
                        height: dispHeight * 0.12, // 고정...
                        child: Image.asset(
                          'assets/images/PopDown.png',
                          color: popDownStatus
                              ? Colors.amber
                              : netFlag
                                  ? activeColor
                                  : inActiveColor,
                        ),
                      ),
                    ),
                  ],
                ),
              ),
              SizedBox(
                width: dispWidth * 0.003,
              ),
              Row(
                // 전체등, 인버터 등 스위치...
                children: [
                  Container(
                    width: dispWidth * 0.13,
                    height: dispHeight * 0.919,
                    decoration: BoxDecoration(
                      color: Colors.transparent,
                      borderRadius:
                          BorderRadius.circular(dispWidth * borderRadius),
                      border: Border.all(
                          width: dispWidth * boardLine,
                          color: netFlag ? Colors.white : Colors.white54),
                    ),
                    child: Column(
                      mainAxisAlignment: MainAxisAlignment.spaceAround,
                      // ignore: prefer_const_literals_to_create_immutables
                      children: <Widget>[
                        // RemoteProcess(),
                        AllLedProcess(),
                        InvertorProcess(),
                        TVProcess(),
                        FRIGEProcess(),
                      ],
                    ),
                  ),
                  SizedBox(
                    width: dispWidth * 0.003,
                  ),
                  Row(
                    // LED 처리 Routine...
                    children: [
                      Container(
                        width: dispWidth * 0.663,
                        height: dispHeight * 0.919,
                        decoration: BoxDecoration(
                          color: Colors.transparent,
                          borderRadius:
                              BorderRadius.circular(dispWidth * borderRadius),
                          border: Border.all(
                              width: dispWidth * boardLine,
                              color: netFlag ? Colors.white : Colors.white54),
                        ),
                        child: Column(
                          // LED 버튼의 상하 간격 조절...
                          mainAxisAlignment: MainAxisAlignment.spaceAround,
                          // ignore: prefer_const_literals_to_create_immutables
                          children: <Widget>[
                            LedProcess(1),
                            LedProcess(2),
                            LedProcess(3),
                            LedProcess(4),
                            LedProcess(5),
                            LedProcess(6),
                            LedProcess(7),
                            LedProcess(8),
                          ],
                        ),
                      ),
                    ],
                  ),
                ],
              ),
            ],
          ),
        ),
      ),
    );
  }
}

class Server {
  // ignore: duplicate_ignore
  Future<void> getReq(String division, int chanel) async {
    // return;
    // ignore: prefer_typing_uninitialized_variables
    var response;
    // ignore: prefer_typing_uninitialized_variables
    var parsedJson;

    if (networkSelect) {
      s = 'http://';
      s += stationIp;
      s += '/';
    } else {
      s = 'http://192.168.4.1/';
      // s = 'http://192.168.0.40/';
    }

    // print('s : $s');

    Dio dio = Dio();

    if (V.comFlag == false) {
      s += "root";
      print('s : $s');
      response = await dio.get(s);
      print(response.statusCode);
      print(response.statusMessage);
      dio.close();
      return;
    } else if (division == "request") {
      s += "Request";
      response = await dio.get(s);
      parsedJson = jsonDecode(response.data);
      print('s : $s');

      print('parsedJson : $parsedJson');
      print('response.statusCode : ${response.statusCode}');
      print('response.statusMessage : ${response.statusMessage}');

      V.btVolt = parsedJson['aux_v'];
      if (V.btVolt > 13.2) {
        // 전압에 따른 밧데리 색상 표현...
        batColor = Colors.white;
      } else if (V.btVolt < 13.19 && V.btVolt > 12.5) {
        batColor = Colors.yellow;
      } else if (V.btVolt < 12.5) {
        batColor = Colors.red;
      }

      V.temperature = parsedJson['Temperature'];
      V.humidity = parsedJson['Humidity'];

      int pwmParse = 0;
      for (int i = 1; i < 9; i++) {
        pwmParse = parsedJson['ledValue$i'];
        if (pwmParse == 0) {
          V.ledValue[i] = 1.0;
        } else {
          V.ledValue[i] = pwmParse.roundToDouble();
        }
      }

      for (int i = 1; i < 9; i++) {
        if (parsedJson['ledFlag$i'] == 1) {
          statusFlag = true;
        } else {
          statusFlag = false;
        }

        if (V.ledStatus[i] != statusFlag) {
          if (parsedJson['ledFlag$i'] == 1) {
            V.ledStatus[i] = true;
          } else {
            V.ledStatus[i] = false;
          }
        }
      }

      if (parsedJson['invertor'] == 1) {
        statusFlag = true;
      } else {
        statusFlag = false;
      }

      if (invertorStatus != statusFlag) {
        if (parsedJson['invertor'] == 1) {
          invertorStatus = true;
        } else {
          invertorStatus = false;
        }
      }

      if (parsedJson['tv'] == 1) {
        statusFlag = true;
      } else {
        statusFlag = false;
      }
      if (tvStatus != statusFlag) {
        if (parsedJson['tv'] == 1) {
          tvStatus = true;
        } else {
          tvStatus = false;
        }
      }

      if (parsedJson['frige'] == 1) {
        statusFlag = true;
      } else {
        statusFlag = false;
      }
      if (frigeStatus != statusFlag) {
        if (parsedJson['frige'] == 1) {
          frigeStatus = true;
        } else {
          frigeStatus = false;
        }
      }

      if (parsedJson['popUp'] == 1) {
        popUpStatus = true;
      } else {
        popUpStatus = false;
      }

      if (parsedJson['popStop'] == 1) {
        stopStatus = true;
      } else {
        stopStatus = false;
      }

      if (parsedJson['popDown'] == 1) {
        popDownStatus = true;
      } else {
        popDownStatus = false;
      }

      if (parsedJson['remote'] == 1) {
        remoteStatus = true;
      } else {
        remoteStatus = false;
      }

      if (parsedJson['ledAllFlag'] == 1) {
        allLedStatus = true;
      } else {
        allLedStatus = false;
      }

      if (parsedJson['sleepFlag'] == 1) {
        sleepFlag = true;
      } else {
        sleepFlag = false;
      }
    } else if (division == "REMOTE") {
      s += 'json?j={"division":"$division","Value":$chanel}';
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "ALL") {
      s += 'json?j={"division":"$division","Value":$chanel}';
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "LED") {
      // LED 값을 "OFF"와 PWM 값으로 분리...
      if (V.ledStatus[chanel] == true) {
        var pwm = V.ledValue[chanel].round().toInt();
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":$pwm}';
      } else {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "REMOTE") {
      if (remoteStatus == true) {
        s += 'json?j={"division":"$division","Value":1}';
      } else {
        s += 'json?j={"division":"$division","Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "POP_UP") {
      if (popUpStatus == true) {
        s += 'json?j={"division":"$division","Value":1}';
      } else {
        s += 'json?j={"division":"$division","Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "POP_STOP") {
      if (stopStatus == true) {
        s += 'json?j={"division":"$division","Value":1}';
      } else {
        s += 'json?j={"division":"$division","Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "POP_DOWN") {
      if (popDownStatus == true) {
        s += 'json?j={"division":"$division","Value":1}';
      } else {
        s += 'json?j={"division":"$division","Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "INVERTOR") {
      if (invertorStatus == true) {
        s += 'json?j={"division":"$division","Value":1}';
      } else {
        s += 'json?j={"division":"$division","Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "TV") {
      if (tvStatus == true) {
        s += 'json?j={"division":"$division","Value":1}';
      } else {
        s += 'json?j={"division":"$division","Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "FRIGE") {
      if (frigeStatus == true) {
        s += 'json?j={"division":"$division","Value":1}';
      } else {
        s += 'json?j={"division":"$division","Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "PowerOff") {
      // 메인보드 Power OFF 값 전송...
      s += 'json?j={"division":"$division","Chanel":$chanel}';
      if (wiFiCheck) response = await dio.get(s);
      print(response.statusCode); // 송수신이 정상이면 "200"
      print(response.statusMessage); // 송수신이 정상이면 'OK'
    } else if (division == "SLEEP") {
      // 취침모드 상태 값 전송...
      s += 'json?j={"division":"$division","Value":$chanel}';
      if (wiFiCheck) response = await dio.get(s);
      print(response.statusCode); // 송수신이 정상이면 "200"
      print(response.statusMessage); // 송수신이 정상이면 'OK'
    } else if (division == "LED_NAME") {
      // LED 이름 전송...
      if (chanel == 10) {
        s += 'json?j={"division":"$division","Chanel":$chanel}';
      } else if (chanel == 0) {
        s += 'json?j={"division":"$division","Chanel":$chanel}';
      } else {
        s +=
            'json?j={"division":"$division","Chanel":$chanel,"LED_NAME":"${V.ledName[chanel]}"}';
      }
      // s += 'json?j={"division":"$division","Value":$chanel,"LED_NAME":"어닝등"}';
      if (wiFiCheck) response = await dio.get(s);
      print(response.statusCode); // 송수신이 정상이면 "200"
      print(response.statusMessage); // 송수신이 정상이면 'OK'
    } else if (division == "TIME") {
      // 현재 시간 전송...
      var time = DateTime.now();
      String nowTime = DateFormat('y:MM:dd:HH:mm:ss').format(time);
      s += 'json?j={"division":"$division","NOW_TIME":"$nowTime"}';

      if (wiFiCheck) response = await dio.get(s);
      print(response.statusCode); // 송수신이 정상이면 "200"
      print(response.statusMessage); // 송수신이 정상이면 'OK'
    } else if (division == "QR_OK") {
      // 현재 시간 전송...
      s += 'json?j={"division":"$division","Chanel":"$chanel"}';

      if (wiFiCheck) response = await dio.get(s);
      print(response.statusCode); // 송수신이 정상이면 "200"
      print(response.statusMessage); // 송수신이 정상이면 'OK'
    }

    if (response.statusCode == 200) {
      wifiCount = 0;
      print("response.statusCode : ${response.statusCode}");
    }

    dio.close();
  }
}
