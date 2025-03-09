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
// import 'package:url_launcher/url_launcher.dart';
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

bool companyFlag = true;
bool trunkFlag = true;
bool wifiSelect = true;
bool statusFlag = false;
bool mobile = true; // true : 휴대폰, false : 태블릿
bool lteFlag = false;
bool sleepModeFlag = true;
bool sleepFlag = false;
bool sleepFlagReset = true;
bool wifiConFlag = false;
bool networkSelect = false; // true : Hot Spot, False : WiFi
bool qrOkFlag = true; // QR 코드로 정상 접속 되면 true...

// bool carFlag = false;

bool displayDir = true;
// bool sensorChgFlag = false;

bool netFlag = false;
// bool factoryMode = false;

// bool isSwitched = false;

bool remoteStatus = false;
bool allLedStatus = false;

bool frigeStatus = false;
bool pumpStatus = false;
bool cigarStatus = false;
bool tvStatus = false;
bool invertorStatus = false;

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

List<String> ledPushName = [
  // 스타리아 Namae...
  '전체등', // 0
  '무드등', // 1
  '실내등', // 3
  '트렁크', // 2
  '어닝등', // 4
  '물펌프', // 5
  '시거잭', // 6
  '냉장고', // 7
  'TV', // 8
  '인버터' // 9
];

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
        allLedStatus = false;
        pumpStatus = false;
        cigarStatus = false;
        frigeStatus = false;
        tvStatus = false;
        invertorStatus = false;
        V.ledStatus[1] = false;
        V.ledStatus[2] = false;
        V.ledStatus[3] = false;
        V.ledStatus[4] = false;
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
      print("QR_OK : $qrOkFlag");

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

    trunkFlag = prefs.getBool('trunkFlag') ?? true;

    // if (trunkFlag) {
    //   // 카니발 Name...
    //   ledPushName[1] = '무드등';
    //   ledPushName[2] = '실내등';
    //   ledPushName[3] = '예  비';
    //   ledPushName[4] = '트렁크등';
    //   ledPushName[5] = '트렁크';
    //   ledPushName[6] = '시거잭1';
    //   ledPushName[7] = '시거잭2';
    //   ledPushName[8] = '상부등';
    //   ledPushName[9] = '인버터';
    // }

    // if (!trunkFlag) {
    V.ledName[1] = prefs.getString('led1SaveName') ?? ledPushName[1];
    V.ledName[2] = prefs.getString('led2SaveName') ?? ledPushName[2];
    V.ledName[3] = prefs.getString('led3SaveName') ?? ledPushName[3];
    V.ledName[4] = prefs.getString('led4SaveName') ?? ledPushName[4];
    V.ledName[5] = prefs.getString('led5SaveName') ?? ledPushName[5];
    V.ledName[6] = prefs.getString('led6SaveName') ?? ledPushName[6];
    V.ledName[7] = prefs.getString('led7SaveName') ?? ledPushName[7];
    V.ledName[8] = prefs.getString('led8SaveName') ?? ledPushName[8];
    V.ledName[9] = prefs.getString('led9SaveName') ?? ledPushName[9];
    // } else {
    //   // 카니발 Name...
    //   V.ledName[1] = ledPushName[1] = '무드등';
    //   V.ledName[2] = ledPushName[2] = '실내등';
    //   V.ledName[3] = ledPushName[3] = '예  비';
    //   V.ledName[4] = ledPushName[4] = '트렁크등';
    //   V.ledName[5] = ledPushName[5] = '트렁크';
    //   V.ledName[6] = ledPushName[6] = '시거잭1';
    //   V.ledName[7] = ledPushName[7] = '시거잭2';
    //   V.ledName[8] = ledPushName[8] = '상부등';
    //   V.ledName[9] = ledPushName[9] = '인버터';
    // }

    print('ledName[1] : ${V.ledName[1]}');
    print('ledName[2] : ${V.ledName[2]}');
    print('ledName[3] : ${V.ledName[3]}');
    print('ledName[4] : ${V.ledName[4]}');
    print('ledName[5] : ${V.ledName[5]}');
    print('ledName[6] : ${V.ledName[6]}');
    print('ledName[7] : ${V.ledName[7]}');
    print('ledName[8] : ${V.ledName[8]}');
    print('ledName[9] : ${V.ledName[9]}');

    networkSelect = prefs.getBool('networkSelect') ?? false;

    // if (networkSelect) {
    V.hotSpotSSID = (prefs.getString('ssidPush') ?? "CamTrol V6.0-A001");
    V.hotSpotPW = (prefs.getString('pwPush') ?? "01234567");
    // } else {
    V.conSSID = (prefs.getString('ssidPush') ?? "CamTrol V6.0-A001");
    V.conPW = (prefs.getString('pwPush') ?? "01234567");
    // }

    companyFlag = prefs.getBool('companyFlag') ?? true;

    beep = prefs.getBool('beep') ?? true;
    mobile = prefs.getBool('mobile') ?? true;
    displayDir = prefs.getBool('displayDir') ?? true;
    sleepModeFlag = prefs.getBool('sleepModeFlag') ?? true;
    stationIp = prefs.getString('stationIp') ?? '192.168.0.100';

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

    return PortScreen();
    // return mobile ? PortScreen() : LandScreen();
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
  final _led9NameController = TextEditingController(text: V.ledName[9]);

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
        print("led9SaveName : ${prefs.getString('led9SaveName')}");
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
                                        : ch == 9
                                            ? _led9NameController
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

                              for (int i = 1; i < 10; i++) {
                                V.ledName[i] = ledPushName[i];
                                V.ledSaveName[i] = ledPushName[i];
                                prefs.setString(
                                    'led${i}SaveName', ledPushName[i]);
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
                                for (int i = 1; i < 10; i++) {
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
                                print('led9 SaveName : ${V.ledSaveName[9]}');
                                print("");

                                print('led1 name : ${V.ledName[1]}');
                                print('led2 name : ${V.ledName[2]}');
                                print('led3 name : ${V.ledName[3]}');
                                print('led4 name : ${V.ledName[4]}');
                                print('led5 name : ${V.ledName[5]}');
                                print('led6 name : ${V.ledName[6]}');
                                print('led7 name : ${V.ledName[7]}');
                                print('led8 name : ${V.ledName[8]}');
                                print('led9 name : ${V.ledName[9]}');
                              });
                              if (beep) {
                                advancedPlayer
                                    .play(AssetSource('sound/iconClick.mp3'));
                              }
                              await server.getReq("LED_NAME", 1);
                              await server.getReq("LED_NAME", 2);
                              await server.getReq("LED_NAME", 3);
                              await server.getReq("LED_NAME", 4);
                              await server.getReq("LED_NAME", 5);
                              await server.getReq("LED_NAME", 6);
                              await server.getReq("LED_NAME", 7);
                              await server.getReq("LED_NAME", 8);
                              await server.getReq("LED_NAME", 9);
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
                SizedBox(height: dispHeight * 0.05),
                Container(
                  // 조명 이름 설정 Container...
                  width: mobile ? dispWidth * 0.46 : dispWidth * 0.2,
                  // height: mobile ? dispHeight * 0.5 : dispHeight * 0.88,
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
                      _ledNameChange(9),
                      SizedBox(height: dispHeight * 0.02),
                    ],
                  ),
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
              companyFlag
                  ? 'assets/images/camperVan.png'
                  : 'assets/images/logo.png',
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
                InkWell(
                  // onDoubleTap: () async {
                  onLongPress: () async {
                    server.getReq("TIME", 1);
                  },
                  child: SizedBox(
                    width: mobile ? dispWidth * 0.3 : dispWidth * 0.26,
                    height: mobile ? dispHeight * 0.04 : dispHeight * 0.045,
                    child: Text(
                      companyFlag ? '    MS V3.0' : logoText,
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
                height: dispHeight * 0.02,
              ),

              // LED 출력용 Container..
              Container(
                // Button 및 Slide Bar 표시...
                height: dispHeight * 0.5,
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
                    // LedProcess(0),
                    LedProcess(1),
                    LedProcess(2),
                    LedProcess(3),
                    LedProcess(4),
                  ],
                ),
              ), // display 창과의 간격 늘림...
              SizedBox(
                height: dispHeight * 0.02,
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
                    PumpProcess(),
                    CigarProcess(),
                    // FRIGEProcess(),
                  ],
                ),
              ),
              SizedBox(
                height: dispHeight * 0.02,
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
                    // AllLedProcess(),
                    FrigeProcess(),
                    TVProcess(),
                    InvertorProcess(),
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

class PumpProcess extends StatefulWidget {
  const PumpProcess({Key? key}) : super(key: key);

  @override
  State<PumpProcess> createState() => _PumpProcessState();
}

class _PumpProcessState extends State<PumpProcess> {
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
              boxShadow: pumpStatus
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
              //  '물펌프',
              V.ledName[5],
              style: TextStyle(
                  fontSize: mobile
                      ? (dispWidth * 0.04) / txtScale
                      : (dispWidth * 0.018) / txtScale,
                  color: pumpStatus
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
        if (pumpStatus == true) {
          if (beep) {
            await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
          }
          pumpStatus = false;
          V.monitor = '펌프 Off..';
        } else {
          if (beep) {
            await advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
          }
          pumpStatus = true;
          V.monitor = '펌프 On..';
        }
        setState(() {});
        server.getReq("PUMP", 0);
        pumpStatus ? print('펌프 On..') : print('펌프 Off..');
      },
    );
  }
}

class CigarProcess extends StatefulWidget {
  const CigarProcess({Key? key}) : super(key: key);

  @override
  State<CigarProcess> createState() => _CigarProcessState();
}

class _CigarProcessState extends State<CigarProcess> {
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
              boxShadow: cigarStatus
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
              // '시거잭',
              V.ledName[6],
              style: TextStyle(
                  fontSize: mobile
                      ? (dispWidth * 0.04) / txtScale
                      : (dispWidth * 0.018) / txtScale,
                  color: cigarStatus
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
        if (cigarStatus == true) {
          if (beep) {
            await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
          }
          cigarStatus = false;
          V.monitor = '시거잭 Off..';
        } else {
          if (beep) {
            await advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
          }
          cigarStatus = true;
          V.monitor = '시거잭 On..';
        }
        setState(() {});
        server.getReq("CIGAR", 0);
        cigarStatus ? print('시거잭 On..') : print('시거잭 Off..');
      },
    );
  }
}

class FrigeProcess extends StatefulWidget {
  const FrigeProcess({Key? key}) : super(key: key);

  @override
  State<FrigeProcess> createState() => _FrigeProcessState();
}

class _FrigeProcessState extends State<FrigeProcess> {
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
              // '냉장고',
              V.ledName[7],
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
              // 'TV',
              V.ledName[8],
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
              // '인버터',
              V.ledName[9],
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
        await server.getReq("LED_ALL", allLedStatus ? 1 : 0);
        await server.getReq("request", 0);
        allLedStatus ? print('전체등 On..') : print('전체등 Off..');
      },
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

      int pwmParse = 0;
      for (int i = 0; i < 5; i++) {
        pwmParse = parsedJson['ledValue$i'];
        if (pwmParse == 0) {
          V.ledValue[i] = 1.0;
        } else {
          V.ledValue[i] = pwmParse.roundToDouble();
        }
      }

      for (int i = 0; i < 5; i++) {
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

      if (parsedJson['PUMP'] == 1) {
        pumpStatus = true;
      } else {
        pumpStatus = false;
      }

      if (parsedJson['CIGAR'] == 1) {
        cigarStatus = true;
      } else {
        cigarStatus = false;
      }

      if (parsedJson['FRIGE'] == 1) {
        frigeStatus = true;
      } else {
        frigeStatus = false;
      }

      if (parsedJson['TV'] == 1) {
        tvStatus = true;
      } else {
        tvStatus = false;
      }

      if (parsedJson['INVERTOR'] == 1) {
        invertorStatus = true;
      } else {
        invertorStatus = false;
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

      SharedPreferences prefs = await SharedPreferences.getInstance();
      if (parsedJson['companyFlag'] == 1) {
        companyFlag = true; // MS Mode...
        // if (companyFlag != prefs.getBool('companyFlag')) {
        //   prefs.setBool('companyFlag', companyFlag);
        // }
      } else {
        companyFlag = false; // CARIND Mode...
      }
      if (companyFlag != prefs.getBool('companyFlag')) {
        prefs.setBool('companyFlag', companyFlag);
      }
      if (parsedJson['trunkFlag'] == 1) {
        trunkFlag = true; // 카니발 Mode...
      } else {
        trunkFlag = false; // 스타리아 Mode...
      }
      if (trunkFlag != prefs.getBool('trunkFlag')) {
        prefs.setBool('trunkFlag', trunkFlag);
        if (trunkFlag) {
          V.ledName[1] = ledPushName[1] = '무드등';
          V.ledName[2] = ledPushName[2] = '실내등';
          V.ledName[3] = ledPushName[3] = '예  비';
          V.ledName[4] = ledPushName[4] = '트렁크등';
          V.ledName[5] = ledPushName[5] = '트렁크';
          V.ledName[6] = ledPushName[6] = '시거잭1';
          V.ledName[7] = ledPushName[7] = '시거잭2';
          V.ledName[8] = ledPushName[8] = '상부등';
          V.ledName[9] = ledPushName[9] = '인버터';
        } else {
          V.ledName[1] = ledPushName[1] = '무드등';
          V.ledName[2] = ledPushName[2] = '실내등';
          V.ledName[3] = ledPushName[3] = '트렁크';
          V.ledName[4] = ledPushName[4] = '어닝등';
          V.ledName[5] = ledPushName[5] = '물펌프';
          V.ledName[6] = ledPushName[6] = '시거잭';
          V.ledName[7] = ledPushName[7] = '냉장고';
          V.ledName[8] = ledPushName[8] = 'TV';
          V.ledName[9] = ledPushName[9] = '인버터';
        }
        prefs.setString('led1SaveName', V.ledName[1]);
        prefs.setString('led2SaveName', V.ledName[2]);
        prefs.setString('led3SaveName', V.ledName[3]);
        prefs.setString('led4SaveName', V.ledName[4]);
        prefs.setString('led5SaveName', V.ledName[5]);
        prefs.setString('led6SaveName', V.ledName[6]);
        prefs.setString('led7SaveName', V.ledName[7]);
        prefs.setString('led8SaveName', V.ledName[8]);
        prefs.setString('led9SaveName', V.ledName[9]);
      }
      trunkFlag ? print("카니발 모드...") : print("스타리아 모드...");
    } else if (division == "LED_ALL") {
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
    } else if (division == "PUMP") {
      if (pumpStatus == true) {
        s += 'json?j={"division":"$division","Value":1}';
      } else {
        s += 'json?j={"division":"$division","Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "CIGAR") {
      if (cigarStatus == true) {
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
      String nowTime = DateFormat('y:MM:dd:HH:mm:ss:E').format(time);
      s += 'json?j={"division":"$division","NOW_TIME":"$nowTime"}';

      if (wiFiCheck) response = await dio.get(s);
      print(response.statusCode); // 송수신이 정상이면 "200"
      print(response.statusMessage); // 송수신이 정상이면 'OK'
    } else if (division == "QR_OK") {
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
