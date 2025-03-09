// ignore_for_file: prefer_const_constructors

import 'package:flutter_analog_clock/flutter_analog_clock.dart';
import 'package:flutter_iot_wifi/flutter_iot_wifi.dart';
import 'package:permission_handler/permission_handler.dart';
import 'dart:developer';
import 'dart:io';
import 'package:qr_code_scanner/qr_code_scanner.dart';
import 'package:syncfusion_flutter_gauges/gauges.dart';
import 'variable.dart';
import 'dart:convert';
import 'dart:async';
import 'package:audioplayers/audioplayers.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:percent_indicator/circular_percent_indicator.dart';
import 'package:get/get.dart';
import 'package:dio/dio.dart';
import 'package:shared_preferences/shared_preferences.dart';
import 'package:url_launcher/url_launcher.dart';
import 'package:slide_digital_clock/slide_digital_clock.dart';

late Timer timer;

String s = ' ';

String voltPng = 'assets/images/car-battery.png';
String tempPng = 'assets/images/tempIn.png';

String voltText = ' ';
String tempText = ' ';

List<int> pwm = [0, 0, 0, 0, 0, 0, 0];

double dispHeight = 0.0;
double dispWidth = 0.0;
double pixelRatio = 0.0;
double txtScale = 0.0;

bool statusFlag = false;
bool mobile = true; // true : 휴대폰, false : 태블릿...
bool lteFlag = false;
bool sleepModeFlag = true;
bool sleepFlag = false;
bool sleepFlagReset = true;
bool wifiConFlag = false;
bool clnValveFlag = false;
bool wstValveFlag = false;

bool leg1UpFlag = false;
bool leg1DownFlag = false;

bool leg2UpFlag = false;
bool leg2DownFlag = false;

bool leg3UpFlag = false;
bool leg3DownFlag = false;

bool leg4UpFlag = false;
bool leg4DownFlag = false;

bool allUpFlag = false;
bool allDownFlag = false;

bool dispDir = true;

bool netFlag = false;
bool netMaskFlag = false;

Server server = Server();

Color activeColor = Colors.white;
Color inActiveColor = const Color.fromARGB(255, 171, 171, 171);

const Color portraitBackColor = Colors.black;
const Color landscapeBackColor = Colors.black;

const Color appBarColor = Color(0x552C1FFF); //FF39F1
const Color titleColor = Colors.white;

const Color mainBackColor = Colors.grey;
const Color mainBorderColor = Colors.red;

const Color mainIconColor = Colors.black;
const Color mainFontColor = Colors.black;

const Color ledBackColor = Color(0xff3b3b3b);
const Color ledBoarColor = Colors.lightGreen;
const Color outBackColor = Color(0xff3b3b3b);
const Color outBoardColor = Colors.blue;
const Color messageFontColor = Colors.white;
const Color messageBackColor = Colors.blueGrey;
const Color messageBoardColor = Colors.blue;

const Color displayFontColor = Colors.grey;

Color sleepColor = Colors.white;

const Color cleanBarColor = Colors.blue;
const Color cleanBackColor = Color(0x80F8FFFE);
const Color wasteBarColor = Colors.redAccent;
const Color wasteBackColor = Color(0x80F8FFFE);

const Color slideFontColor = Colors.white;
const Color slideValeColor = Colors.grey;
const Color slideSumColor = Colors.grey;
const Color landscapeSlideColor = Colors.white;
const Color portraitSlideColor = Colors.white;

const Color ledFontColor = Colors.grey;
const Color ledOffColor = Colors.lime;
const Color ledOnColor = Color(0xFF6B56F5);
const Color ledWhiteShadowColor = Colors.grey;
const Color ledBlackShadowColor = Colors.grey;

const Color outFontColor = Colors.grey;
const Color outOffColor = Colors.lime; //6F58FF;
const Color outOnColor = Color(0xFF6B56F5);
const Color outWhiteShadowColor = Colors.grey;
const Color outBlackShadowColor = Colors.grey;

// bool timerRun = true;
bool beep = true;

int wifiCount = 10;
int wifiConCount = 0;

// 밧데리 Icon 더블 클릭시 전환...
bool carType = true; // true: 봉고, false: 칸 용 버튼명 변경...

bool myLiftFlag = false;

// bool liftFlag = false;
bool liftFlag = true;
bool alertFlag = true;

int cleanAdc = 0;
int wasteAdc = 0;

int aLowValue = 0;
int aHighValue = 0;
int bLowValue = 0;
int bHighValue = 0;

double hConstant = 21.79;
double boardLine = 0.0026;
double borderRadius = 0.01;
double buttonRadius = 0.006;
double appBarHeight = 0.078;

// double disp1tWidth = 0.2;
// double disp1tHeight = 0.915;
// double disp1SizetHeight = 0.12;
// double disp1WidthWidth = 0.06;
// double disp1WidthHeight = 0.045;

// Upper Container 변수
// const double upperMainContainerHeight = 100.0;
// final Color displayBackGroundColor = Colors.grey.shade500;
// const double lcdWindowHeight = 90.0;
// const double iconSize = 20.0;
// const double iconFontSize = 16.0;

// Middle Container 변수
const double middleMainContainerHeight = 300.0;
final Color middleMainContainerBackGroundColor = Colors.grey.shade600;
// const Color iconColor = Colors.white;
// const Color textColor = Colors.white;

// Color batColor = Colors.white;
Color batColor = const Color(0xFF6B56F5);

const double ledSwitchWidth = 80.0;
const double ledSwitchHeight = 40.0;
const Color ledSwitchOutLineColor = Colors.black54;
const double ledSwitchFontSize = 17.0;
const double sliderWidth = 220.0;
const double landscapeSliderWidth = 250.0;
const double sliderValueTextWidth = 40.0;
const double sliderValueFontSize = 20.0;

// Lower Container 변수
const double lowerMainContainerHeight = 70.0;

// bool wiFiCheck = false;

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
    final theme = ThemeData(primarySwatch: Colors.blue);
    final sliderTheme = theme.sliderTheme
        .copyWith(showValueIndicator: ShowValueIndicator.always);
    return GetMaterialApp(
      theme: theme.copyWith(sliderTheme: sliderTheme),
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
        // GetPage(
        //     name: '/myLift',
        //     page: () => const MyLift(),
        //     transition: Transition.fadeIn),
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
      FlutterIotWifi.connect(V.conSSID, V.conPW, prefix: true)
          .then((value) => print("connect initiated: $value"));
    } else {
      print("don't have permission");
    }
    print("conSSID: ${V.conSSID}, conPW: ${V.conPW}\n");
  }

  @override
  void initState() {
    _loadPrefs();
    SystemChrome.setEnabledSystemUIMode(SystemUiMode.manual, overlays: []);

    timer = Timer.periodic(const Duration(milliseconds: 1000), (timer) {
      // wiFiCheck = true;
      wifiCount++;

      if (!netFlag) {
        wifiConCount++;
      }

      if (wifiConCount > 50) {
        wifiConCount = 0;
        wifiConFlag = false;
      }

      // if (!wifiConFlag && !netFlag && V.conSSID != " " && V.conPW != " ") {
      if (!wifiConFlag && !netFlag && !netMaskFlag) {
        if (wifiConCount == 6) {
          _connect();
          wifiConFlag = true;
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
        netMaskFlag = false;
        V.motorStatus[1] = false;
        V.motorStatus[2] = false;
        V.motorStatus[3] = false;
        V.allLedStatus = false;
        V.invertorStatus = false;
        clnValveFlag = false;
        wstValveFlag = false;
        for (int i = 1; i < 7; i++) {
          V.ledStatus[i] = false;
          V.outStatus[i] = false;
        }
        // V.ledStatus[1] = false;
        // V.ledStatus[2] = false;
        // V.ledStatus[3] = false;
        // V.ledStatus[4] = false;
        // V.ledStatus[5] = false;
        // V.ledStatus[6] = false;
        // V.outStatus[1] = false;
        // V.outStatus[2] = false;
        // V.outStatus[3] = false;
        // V.outStatus[4] = false;
        // V.outStatus[5] = false;
        // V.outStatus[6] = false;
        setState(() {});
        if (wifiCount > 100) {
          wifiCount = 10;
        }
      } else {
        netFlag = true;
      }

      if (V.wasteWater > 95 && alertFlag == true) {
        alertFlag = false;
        if (beep) advancedPlayer.play(AssetSource('sound/dongdong.mp3'));
        showDialog(
          context: context,
          builder: (context) => AlertDialog(
            backgroundColor: const Color(0xC0D4AA39),
            title: Text(
              "오수가 가득 찼습니다...",
              style: TextStyle(
                color: Colors.white,
                fontSize: mobile
                    ? (dispWidth * 0.05) / txtScale
                    : (dispHeight * 0.04) / txtScale,
              ),
            ),
            actions: <Widget>[
              SizedBox(
                width: mobile ? dispWidth * 0.18 : dispWidth * 0.09,
                height: mobile ? dispHeight * 0.045 : dispHeight * 0.07,
                child: ElevatedButton(
                    child: Text(
                      "확인",
                      style: TextStyle(
                        color: Colors.black,
                        fontWeight: FontWeight.bold,
                        fontSize: mobile
                            ? (dispWidth * 0.04) / txtScale
                            : (dispHeight * 0.03) / txtScale,
                      ),
                    ),
                    onPressed: () {
                      Get.back();
                      setState(() {});
                    }),
              ),
            ],
          ),
        );
      } else if (V.wasteWater != 100) {
        alertFlag = true;
      }

      // if (V.comFlag == true) {
      //   server.getReq("request", " ", 0);
      // }

      server.getReq("request", " ", 0);
      print("wifiCount : $wifiCount");
      print("netFlag : $netFlag");

      if (netFlag) {
        print("V.btVolt : ${(V.btVolt).toString()} V");
        setState(() {});
      }
    });
    super.initState();
    //  permission(); //프로그램 최초 mic 권한 얻기..
  }

  void _loadPrefs() async {
    if (beep) advancedPlayer.play(AssetSource('sound/intro.mp3'));
    SharedPreferences prefs = await SharedPreferences.getInstance();
    // File에 최초 저장시 null 대응...
    print('');
    print('ledName[1] : ${V.ledName[1]}');
    print('ledName[2] : ${V.ledName[2]}');
    print('ledName[3] : ${V.ledName[3]}');
    print('ledName[4] : ${V.ledName[4]}');
    print('ledName[5] : ${V.ledName[5]}');
    print('ledName[6] : ${V.ledName[6]}');

    print('');
    print('outName[1] : ${V.outName[1]}');
    print('outName[2] : ${V.outName[2]}');
    print('outName[3] : ${V.outName[3]}');
    print('outName[4] : ${V.outName[4]}');
    print('outName[5] : ${V.outName[5]}');
    print('outName[6] : ${V.outName[6]}');

    print('');
    V.ledName[1] = (prefs.getString('led1SaveName') ?? V.ledPushName[1]);
    V.ledName[2] = (prefs.getString('led2SaveName') ?? V.ledPushName[2]);
    V.ledName[3] = (prefs.getString('led3SaveName') ?? V.ledPushName[3]);
    V.ledName[4] = (prefs.getString('led4SaveName') ?? V.ledPushName[4]);
    V.ledName[5] = (prefs.getString('led5SaveName') ?? V.ledPushName[5]);
    V.ledName[6] = (prefs.getString('led6SaveName') ?? V.ledPushName[6]);

    V.outName[1] = (prefs.getString('out1SaveName') ?? V.outPushName[1]);
    V.outName[2] = (prefs.getString('out2SaveName') ?? V.outPushName[2]);
    V.outName[3] = (prefs.getString('out3SaveName') ?? V.outPushName[3]);
    V.outName[4] = (prefs.getString('out4SaveName') ?? V.outPushName[4]);
    V.outName[5] = (prefs.getString('out5SaveName') ?? V.outPushName[5]);
    V.outName[6] = (prefs.getString('out6SaveName') ?? V.outPushName[6]);

    print('ledName[1] : ${V.ledName[1]}');
    print('ledName[2] : ${V.ledName[2]}');
    print('ledName[3] : ${V.ledName[3]}');
    print('ledName[4] : ${V.ledName[4]}');
    print('ledName[5] : ${V.ledName[5]}');
    print('ledName[6] : ${V.ledName[6]}');

    print('');
    print('outName[1] : ${V.outName[1]}');
    print('outName[2] : ${V.outName[2]}');
    print('outName[3] : ${V.outName[3]}');
    print('outName[4] : ${V.outName[4]}');
    print('outName[5] : ${V.outName[5]}');
    print('outName[6] : ${V.outName[6]}');

    V.conSSID = (prefs.getString('ssidPush') ?? " ");
    V.conPW = (prefs.getString('pwPush') ?? " ");

    beep = prefs.getBool('beep') ?? true;
    mobile = prefs.getBool('mobile') ?? true;
    dispDir = prefs.getBool('dispDir') ?? true;
    sleepModeFlag = prefs.getBool('sleepModeFlag') ?? true;

    V.setTempValue = prefs.getDouble('setTempValue') ?? 25.0;

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
        : myLiftFlag
            ? SystemChrome.setPreferredOrientations(
                [DeviceOrientation.portraitUp])
            : dispDir
                ? SystemChrome.setPreferredOrientations(
                    [DeviceOrientation.landscapeLeft])
                : SystemChrome.setPreferredOrientations(
                    [DeviceOrientation.landscapeRight]);
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
                                // setState(() {
                                if (result != null) {
                                  V.conSSID = convertString[3];
                                  prefs.setString('ssidPush', convertString[3]);
                                  V.conPW = convertString[4];
                                  prefs.setString('pwPush', convertString[4]);
                                }
                                // });
                                if (beep) {
                                  advancedPlayer
                                      .play(AssetSource('sound/iconClick.mp3'));
                                }
                                setState(() {});
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
        advancedPlayer.play(AssetSource('sound/dongdong.mp3'));
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
  _SleepModeState createState() => _SleepModeState();
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
              // color: Color(0xD0574B4C),
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
          if (beep) advancedPlayer.play(AssetSource('sound/dongdong.mp3'));
          await server.getReq("sleep", "취침", 0);
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
                  dialPlateColor: const Color(0x40574B4C),
                  hourHandColor: const Color(0x803B5C4F), // 시침 색상...
                  minuteHandColor: const Color(0x803B5C4F), // 분침 색상...
                  secondHandColor: const Color(0x80D11F22), // 초침 색상
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
  _PhoneNumberState createState() => _PhoneNumberState();
}

class _PhoneNumberState extends State<PhoneNumber> {
  // AudioCache audioCache = AudioCache();

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
      // backgroundColor: Color(0x80574B4C),
      backgroundColor: const Color(0x8045FFA2),
      appBar: PreferredSize(
        preferredSize: Size.fromHeight(
          mobile
              ? (dispHeight * appBarHeight) * 0.6
              : (dispHeight * appBarHeight),
        ),
        child: AppBar(
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
      body: SingleChildScrollView(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.start,
          children: <Widget>[
            SizedBox(
              height: (mobile ? dispHeight : dispWidth) * 0.01,
            ),
            Container(
              // width: 300.0,
              // margin:
              //     const EdgeInsets.symmetric(vertical: 5.0), // Appbar와의 간격...
              width: dispWidth * 0.95,
              height: dispHeight * 0.4,
              decoration: BoxDecoration(
                borderRadius: BorderRadius.circular(mobile
                    ? dispHeight * borderRadius
                    : dispWidth * borderRadius),
                border: Border.all(
                    width:
                        mobile ? dispWidth * boardLine : dispHeight * boardLine,
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
                          // shape: CircleBorder(),
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
                      // SizedBox(height: dispHeight * 0.023),
                      SizedBox(
                        width: dispWidth * 0.45,
                        height: dispHeight * 0.05,
                        child: TextField(
                          cursorColor: Colors.white,
                          style: TextStyle(
                              fontSize: dispWidth * 0.04,
                              // height: 1.0,
                              color: Colors.white),
                          decoration: InputDecoration(
                            // hintText: '이름 입력',
                            enabled: V.isPhoneTable[1] ? true : false,
                            focusedBorder: const OutlineInputBorder(
                              borderSide:
                                  BorderSide(color: Colors.black, width: 1.5),
                            ),
                            border: const OutlineInputBorder(),
                            labelText: V.phoneName[1],
                            labelStyle: const TextStyle(
                              color: Colors.white,
                            ),
                          ),
                          onChanged: (text) {
                            _nameFlag[1] = true;
                            _phoneName[1] = text;
                          },
                        ),
                      ),
                      SizedBox(
                        width: dispWidth * 0.3,
                        height: dispHeight * 0.05,
                        child: TextField(
                          cursorColor: Colors.white,
                          style: TextStyle(
                              fontSize: dispWidth * 0.04,
                              // height: 1.0,
                              color: Colors.white),
                          decoration: InputDecoration(
                            // hintText: '전화번호 입력',
                            enabled: V.isPhoneTable[1] ? true : false,
                            focusedBorder: const OutlineInputBorder(
                              borderSide:
                                  BorderSide(color: Colors.black, width: 1.5),
                            ),
                            border: const OutlineInputBorder(),
                            labelText: V.phoneNumber[1],
                            labelStyle: const TextStyle(
                              color: Colors.white,
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
                        width: dispWidth * 0.45,
                        height: dispHeight * 0.05,
                        child: TextField(
                          cursorColor: Colors.white,
                          style: TextStyle(
                              fontSize: dispWidth * 0.04,
                              // height: 1.0,
                              color: Colors.white),
                          decoration: InputDecoration(
                            // hintText: '이름 입력',
                            enabled: V.isPhoneTable[2] ? true : false,
                            focusedBorder: const OutlineInputBorder(
                              borderSide:
                                  BorderSide(color: Colors.black, width: 1.5),
                            ),
                            border: const OutlineInputBorder(),
                            labelText: V.phoneName[2],
                            labelStyle: const TextStyle(
                              color: Colors.white,
                            ),
                          ),
                          onChanged: (text) {
                            _nameFlag[2] = true;
                            _phoneName[2] = text;
                          },
                        ),
                      ),
                      SizedBox(
                        width: dispWidth * 0.3,
                        height: dispHeight * 0.05,
                        child: TextField(
                          cursorColor: Colors.white,
                          style: TextStyle(
                              fontSize: dispWidth * 0.04,
                              // height: 1.0,
                              color: Colors.white),
                          decoration: InputDecoration(
                            // hintText: '전화번호 입력',
                            enabled: V.isPhoneTable[2] ? true : false,
                            focusedBorder: const OutlineInputBorder(
                              borderSide:
                                  BorderSide(color: Colors.black, width: 1.5),
                            ),
                            border: const OutlineInputBorder(),
                            labelText: V.phoneNumber[2],
                            labelStyle: const TextStyle(
                              color: Colors.white,
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
                          // shape: CircleBorder(),
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
                        width: dispWidth * 0.45,
                        height: dispHeight * 0.05,
                        child: TextField(
                          cursorColor: Colors.white,
                          style: TextStyle(
                              fontSize: dispWidth * 0.04,
                              // height: 1.0,
                              color: Colors.white),
                          decoration: InputDecoration(
                            // hintText: '이름 입력',
                            enabled: V.isPhoneTable[3] ? true : false,
                            focusedBorder: const OutlineInputBorder(
                              borderSide:
                                  BorderSide(color: Colors.black, width: 1.5),
                            ),
                            border: const OutlineInputBorder(),
                            labelText: V.phoneName[3],
                            labelStyle: const TextStyle(
                              color: Colors.white,
                            ),
                          ),
                          onChanged: (text) {
                            _nameFlag[3] = true;
                            _phoneName[3] = text;
                          },
                        ),
                      ),
                      SizedBox(
                        width: dispWidth * 0.3,
                        height: dispHeight * 0.05,
                        child: TextField(
                          cursorColor: Colors.white,
                          style: TextStyle(
                              fontSize: dispWidth * 0.04,
                              // height: 1.0,
                              color: Colors.white),
                          decoration: InputDecoration(
                            // hintText: '전화번호 입력',
                            enabled: V.isPhoneTable[3] ? true : false,
                            focusedBorder: const OutlineInputBorder(
                              borderSide:
                                  BorderSide(color: Colors.black, width: 1.5),
                            ),
                            border: const OutlineInputBorder(),
                            labelText: V.phoneNumber[3],
                            labelStyle: const TextStyle(
                              color: Colors.white,
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
                          // shape: CircleBorder(),
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
                        width: dispWidth * 0.45,
                        height: dispHeight * 0.05,
                        child: TextField(
                          cursorColor: Colors.white,
                          style: TextStyle(
                              fontSize: dispWidth * 0.04,
                              // height: 1.0,
                              color: Colors.white),
                          decoration: InputDecoration(
                            // hintText: '이름 입력',
                            enabled: V.isPhoneTable[4] ? true : false,
                            focusedBorder: const OutlineInputBorder(
                              borderSide:
                                  BorderSide(color: Colors.black, width: 1.5),
                            ),
                            border: const OutlineInputBorder(),
                            labelText: V.phoneName[4],
                            labelStyle: const TextStyle(
                              color: Colors.white,
                            ),
                          ),
                          onChanged: (text) {
                            _nameFlag[4] = true;
                            _phoneName[4] = text;
                          },
                        ),
                      ),
                      SizedBox(
                        width: dispWidth * 0.3,
                        height: dispHeight * 0.05,
                        child: TextField(
                          cursorColor: Colors.white,
                          style: TextStyle(
                              fontSize: dispWidth * 0.04,
                              // height: 1.0,
                              color: Colors.white),
                          decoration: InputDecoration(
                            // hintText: '전화번호 입력',
                            enabled: V.isPhoneTable[4] ? true : false,
                            focusedBorder: const OutlineInputBorder(
                              borderSide:
                                  BorderSide(color: Colors.black, width: 1.5),
                            ),
                            border: const OutlineInputBorder(),
                            labelText: V.phoneNumber[4],
                            labelStyle: const TextStyle(
                              color: Colors.white,
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
                          // shape: CircleBorder(),
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
                        width: dispWidth * 0.45,
                        height: dispHeight * 0.05,
                        child: TextField(
                          cursorColor: Colors.white,
                          style: TextStyle(
                              fontSize: dispWidth * 0.04,
                              // height: 1.0,
                              color: Colors.white),
                          decoration: InputDecoration(
                            // hintText: '이름 입력',
                            enabled: V.isPhoneTable[5] ? true : false,
                            focusedBorder: const OutlineInputBorder(
                              borderSide:
                                  BorderSide(color: Colors.black, width: 1.5),
                            ),
                            border: const OutlineInputBorder(),
                            labelText: V.phoneName[5],
                            labelStyle: const TextStyle(
                              color: Colors.white,
                            ),
                          ),
                          onChanged: (text) {
                            _nameFlag[5] = true;
                            _phoneName[5] = text;
                          },
                        ),
                      ),
                      SizedBox(
                        width: dispWidth * 0.3,
                        height: dispHeight * 0.05,
                        child: TextField(
                          cursorColor: Colors.white,
                          style: TextStyle(
                              fontSize: dispWidth * 0.04,
                              // height: 1.0,
                              color: Colors.white),
                          decoration: InputDecoration(
                            // hintText: '전화번호 입력',
                            enabled: V.isPhoneTable[5] ? true : false,
                            focusedBorder: const OutlineInputBorder(
                              borderSide:
                                  BorderSide(color: Colors.black, width: 1.5),
                            ),
                            border: const OutlineInputBorder(),
                            labelText: V.phoneNumber[5],
                            labelStyle: const TextStyle(
                              color: Colors.white,
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
                    width: dispWidth * 0.17,
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
                                width: dispWidth * 0.17,
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
                    width: dispWidth * 0.17,
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
    );
  }
}

class WiFiChange extends StatefulWidget {
  // ignore: prefer_const_constructors_in_immutables
  WiFiChange({Key? key}) : super(key: key);

  @override
  WiFiChangeState createState() => WiFiChangeState();
}

class WiFiChangeState extends State<WiFiChange> {
  String ssidPush = " ";
  String pwPush = " ";

  bool ssidFlag = false;
  bool pwFlag = false;
  // bool ipFlag = false;

  final _ssidTextController = TextEditingController(text: V.conSSID);
  final _pwTextController = TextEditingController(text: V.conPW);

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
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
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
                    // mainAxisAlignment: MainAxisAlignment.center,
                    crossAxisAlignment: CrossAxisAlignment.center,
                    children: <Widget>[
                      SizedBox(
                        height: mobile ? dispHeight * 0.03 : dispWidth * 0.03,
                      ),
                      SizedBox(
                        width: mobile ? dispWidth * 0.6 : dispWidth * 0.3,
                        height: mobile ? dispHeight * 0.07 : dispWidth * 0.045,
                        child: TextField(
                          // SSID 입력창...
                          controller: _ssidTextController,
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
                            labelText: V.conSSID,
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
                          controller: _pwTextController,
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
                            labelText: V.conPW,
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
                      InkWell(
                        child: Icon(Icons.qr_code_scanner,
                            size: mobile ? dispWidth * 0.3 : dispHeight * 0.2,
                            color: Colors.black),
                        onTap: () async {
                          if (beep) {
                            await advancedPlayer
                                .play(AssetSource('sound/iconClick.mp3'));
                          }
                          await Get.toNamed('/qrCodeRead');
                        },
                      ),
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
                                                    V.conSSID = ssidPush;
                                                    prefs.setString(
                                                        'ssidPush', ssidPush);
                                                  }
                                                  if (pwFlag) {
                                                    V.conPW = pwPush;
                                                    prefs.setString(
                                                        'pwPush', pwPush);
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
  List<bool> outNameFlag = [
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

  final _out1NameController = TextEditingController(text: V.outName[1]);
  final _out2NameController = TextEditingController(text: V.outName[2]);
  final _out3NameController = TextEditingController(text: V.outName[3]);
  final _out4NameController = TextEditingController(text: V.outName[4]);
  final _out5NameController = TextEditingController(text: V.outName[5]);
  final _out6NameController = TextEditingController(text: V.outName[6]);
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

        print("out1SaveName : ${prefs.getString('out1SaveName')}");
        print("out2SaveName : ${prefs.getString('out2SaveName')}");
        print("out3SaveName : ${prefs.getString('out3SaveName')}");
        print("out4SaveName : ${prefs.getString('out4SaveName')}");
        print("out5SaveName : ${prefs.getString('out5SaveName')}");
        print("out6SaveName : ${prefs.getString('out6SaveName')}");
      });
    })();
    // initSpeechRecognizer();
    //  permission(); //프로그램 최초 mic 권한 얻기..
  }

  Widget _ledNameChange(int ch) {
    return SizedBox(
      width: mobile ? dispWidth * 0.3 : dispWidth * 0.16,
      height: mobile ? dispHeight * 0.06 : dispHeight * 0.08,
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

  Widget _outNameChange(int ch) {
    return SizedBox(
      width: mobile ? dispWidth * 0.3 : dispWidth * 0.16,
      height: mobile ? dispHeight * 0.06 : dispHeight * 0.08,
      child: TextField(
        controller: ch == 1
            ? _out1NameController
            : ch == 2
                ? _out2NameController
                : ch == 3
                    ? _out3NameController
                    : ch == 4
                        ? _out4NameController
                        : ch == 5
                            ? _out5NameController
                            : ch == 6
                                ? _out6NameController
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
          labelText: V.outName[ch],
          labelStyle: const TextStyle(
            color: Colors.white,
          ),
        ),
        onChanged: (text) {
          setState(() {
            V.outSaveName[ch] = text;
            outNameFlag[ch] = true;
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
            '출력 이름 변경...',
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
                  color: Colors.white),
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

                              for (int i = 1; i < 7; i++) {
                                V.ledName[i] = V.ledPushName[i];
                                V.ledSaveName[i] = V.ledPushName[i];
                                prefs.setString(
                                    'led${i}SaveName', V.ledPushName[i]);
                              }

                              for (int i = 1; i < 7; i++) {
                                V.outName[i] = V.outPushName[i];
                                V.outSaveName[i] = V.outPushName[i];
                                prefs.setString(
                                    'out${i}SaveName', V.outPushName[i]);
                              }
                              setState(() {});

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
                color: Colors.white,
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
                                for (int i = 1; i < 7; i++) {
                                  if (ledNameFlag[i]) {
                                    V.ledName[i] = V.ledSaveName[i];
                                    prefs.setString(
                                        'led${i}SaveName', V.ledSaveName[i]);
                                  }
                                }

                                for (int i = 1; i < 7; i++) {
                                  if (outNameFlag[i]) {
                                    V.outName[i] = V.outSaveName[i];
                                    prefs.setString(
                                        'out${i}SaveName', V.outSaveName[i]);
                                  }
                                }

                                print('led1 SaveName : ${V.ledSaveName[1]}');
                                print('led2 SaveName : ${V.ledSaveName[2]}');
                                print('led3 SaveName : ${V.ledSaveName[3]}');
                                print('led4 SaveName : ${V.ledSaveName[4]}');
                                print('led5 SaveName : ${V.ledSaveName[5]}');
                                print('led6 SaveName : ${V.ledSaveName[6]}');
                                print("");

                                print('led1 name : ${V.ledName[1]}');
                                print('led2 name : ${V.ledName[2]}');
                                print('led3 name : ${V.ledName[3]}');
                                print('led4 name : ${V.ledName[4]}');
                                print('led5 name : ${V.ledName[5]}');
                                print('led6 name : ${V.ledName[6]}');

                                print('out1 SaveName : ${V.outSaveName[1]}');
                                print('out2 SaveName : ${V.outSaveName[2]}');
                                print('out3 SaveName : ${V.outSaveName[3]}');
                                print('out4 SaveName : ${V.outSaveName[4]}');
                                print('out5 SaveName : ${V.outSaveName[5]}');
                                print('out6 SaveName : ${V.outSaveName[6]}');
                                print("");

                                print('out1 name : ${V.outName[1]}');
                                print('out2 name : ${V.outName[2]}');
                                print('out3 name : ${V.outName[3]}');
                                print('out4 name : ${V.outName[4]}');
                                print('out5 name : ${V.outName[5]}');
                                print('out6 name : ${V.outName[6]}');
                              });
                              if (beep) {
                                advancedPlayer
                                    .play(AssetSource('sound/iconClick.mp3'));
                              }
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
                                mobile ? dispHeight * 0.01 : dispHeight * 0.01),
                        _ledNameChange(1),
                        _ledNameChange(2),
                        _ledNameChange(3),
                        _ledNameChange(4),
                        _ledNameChange(5),
                        _ledNameChange(6),
                      ]),
                ),
                mobile
                    ? SizedBox.shrink()
                    : SizedBox(
                        width: dispWidth * 0.04,
                      ),
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
                                mobile ? dispHeight * 0.01 : dispHeight * 0.01),
                        _outNameChange(1),
                        _outNameChange(2),
                        _outNameChange(3),
                        _outNameChange(4),
                        _outNameChange(5),
                        _outNameChange(6),
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
  // ignore: prefer_const_constructors_in_immutables
  AppBarScreen({Key? key}) : super(key: key);

  @override
  _AppBarScreenState createState() => _AppBarScreenState();
}

class _AppBarScreenState extends State<AppBarScreen> {
  // late Timer _timer;

  Future<bool> _checkPermissions() async {
    if (await Permission.location.request().isGranted) {
      return true;
    }
    return false;
  }

  void _connect() async {
    if (await _checkPermissions()) {
      FlutterIotWifi.connect(V.conSSID, V.conPW, prefix: true)
          .then((value) => print("connect initiated: $value"));
    } else {
      print("don't have permission");
    }
    print("_connect 함수 들어감..............................");
  }

  @override
  void dispose() {
    // _timer.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return AppBar(
      // automaticallyImplyLeading: true,
      backgroundColor: appBarColor,
      title: Row(
        crossAxisAlignment: CrossAxisAlignment.center,
        children: [
          InkWell(
            child: InkWell(
              onLongPress: () async {
                await advancedPlayer.play(AssetSource('sound/dongdong.mp3'));
                showDialog(
                  context: context,
                  builder: (context) => AlertDialog(
                    backgroundColor: const Color(0xC0D4AA39),
                    title: Text(
                      liftFlag ? "리프트 전원을 끕니다." : "리프트 전원을 켭니다.",
                      style: TextStyle(
                        color: Colors.white,
                        fontSize: mobile
                            ? (dispWidth * 0.05) / txtScale
                            : (dispWidth * 0.03) / txtScale,
                      ),
                    ),
                    actions: <Widget>[
                      // ignore: deprecated_member_use
                      SizedBox(
                        width: mobile ? dispWidth * 0.17 : dispWidth * 0.1,
                        height: mobile ? dispHeight * 0.045 : dispHeight * 0.1,
                        child: ElevatedButton(
                            child: Text(
                              "확인",
                              style: TextStyle(
                                color: Colors.black,
                                fontWeight: FontWeight.bold,
                                fontSize: mobile
                                    ? (dispWidth * 0.035) / txtScale
                                    : (dispWidth * 0.025) / txtScale,
                              ),
                            ),
                            onPressed: () {
                              liftFlag
                                  ? server.getReq("LIFT", "리프트 전원:", 0)
                                  : server.getReq("LIFT", "리프트 전원:", 1);
                              Get.back();
                            }),
                      ),
                    ],
                  ),
                );
              },
              child: Container(
                // 풀하우스 logo...
                width: mobile ? dispWidth * 0.08 : dispWidth * 0.04,
                height: mobile ? dispHeight * 0.026 : dispHeight * 0.04,
                decoration: BoxDecoration(
                  image: DecorationImage(
                    image: const AssetImage('assets/images/camper.png'),
                    colorFilter: ColorFilter.mode(
                        (liftFlag && netFlag)
                            ? Colors.white
                            : const Color.fromARGB(140, 255, 86, 34),
                        BlendMode.srcIn),
                    fit: BoxFit.fill,
                  ),
                ),
              ),
            ),
          ),
          InkWell(
            onLongPress: () async {
              SharedPreferences prefs = await SharedPreferences.getInstance();
              dispDir = !dispDir;
              prefs.setBool('dispDir', dispDir);
            },
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                SizedBox(
                  width: mobile ? dispWidth * 0.36 : dispWidth * 0.26,
                  child: Text(
                    logoText,
                    style: TextStyle(
                      fontWeight: FontWeight.bold,
                      fontStyle: FontStyle.italic,
                      fontSize: mobile
                          ? (dispHeight * 0.0182) / txtScale
                          : (dispHeight * 0.04) / txtScale,
                      color: titleColor,
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
                areaDecoration: const BoxDecoration(color: Colors.transparent),
                is24HourTimeFormat: false,
                digitAnimationStyle: Curves.fastLinearToSlowEaseIn,
                areaAligment: AlignmentDirectional.center,
                minuteDigitDecoration:
                    const BoxDecoration(color: Colors.transparent),
                hourMinuteDigitTextStyle:
                    TextStyle(fontSize: dispHeight * 0.045),
                secondDigitDecoration:
                    const BoxDecoration(color: Colors.transparent),
                secondDigitTextStyle: TextStyle(fontSize: dispHeight * 0.025),
                amPmDigitTextStyle: TextStyle(
                    fontSize: dispHeight * 0.025,
                    color: Colors.white,
                    fontWeight: FontWeight.bold),
              ),
        mobile
            ? const SizedBox.shrink()
            : SizedBox(
                width: dispWidth * 0.15,
              ),
        IconButton(
          icon: Icon(Icons.hotel,
              size: mobile ? dispWidth * 0.06 : dispHeight * 0.05,
              color: sleepColor),
          tooltip: '취침모드',
          onPressed: () async {
            if (beep) advancedPlayer.play(AssetSource('sound/iconClick.mp3'));
            if (mobile == true) {
              if (sleepFlag == false) {
                await server.getReq("sleep", "취침모드", 1);
              } else {
                await server.getReq("sleep", "취침모드", 0);
              }
            } else {
              sleepFlagReset = true;
              await server.getReq("sleep", "취침모드", 1);
            }
          },
        ),
        SizedBox(
            width: mobile
                ? (pixelRatio > 1.5)
                    ? dispWidth * 0.0015
                    : dispWidth * 0.03
                : (pixelRatio > 2.0)
                    ? dispWidth * 0.015
                    : dispWidth * 0.03),
        IconButton(
          icon: Icon(Icons.change_circle_outlined,
              size: mobile ? dispWidth * 0.06 : dispHeight * 0.05),
          tooltip: '설정',
          onPressed: () async {
            if (beep) advancedPlayer.play(AssetSource('sound/iconClick.mp3'));
            await Get.toNamed("/setting");
          },
        ),
        SizedBox(
            width: mobile
                ? dispWidth * 0.033
                : (pixelRatio > 1.5)
                    ? dispWidth * 0.015
                    : dispWidth * 0.03),
        InkWell(
          onTap: () async {
            if (!netFlag && beep) {
              wifiConFlag = true;
              if (V.conSSID == " " || V.conPW == " ") {
                ScaffoldMessenger.of(context).showSnackBar(
                  const SnackBar(content: Text('네트워크 정보를 확인 해 주세요.')),
                );
                await advancedPlayer.play(AssetSource('sound/dongdong.mp3'));
              } else {
                netMaskFlag = true;
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
        SizedBox(
            width: mobile
                ? dispWidth * 0.03
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
  _PortScreenState createState() => _PortScreenState();
}

class _PortScreenState extends State<PortScreen> {
  // AudioCache audioCache = AudioCache();
  // AudioPlayer advancedPlayer = AudioPlayer();

  @override
  Widget build(BuildContext context) {
    return Container(
      decoration: const BoxDecoration(
        image: DecorationImage(
            // image: AssetImage('assets/images/wooden-textured-background.jpg'),
            // image: AssetImage('assets/images/wood1.png'),
            image: AssetImage('assets/images/sunrise.jpg'),
            fit: BoxFit.cover),
      ),
      child: Scaffold(
        // Portrait 디자인...
        backgroundColor: Colors.transparent,
        // backgroundColor: portraitBackColor,
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
                // height: dispHeight * 0.18, //Container 높이 기존 : 150
                height: dispHeight * 0.17, //Container 높이 기존 : 150
                decoration: BoxDecoration(
                  borderRadius:
                      BorderRadius.circular(dispHeight * borderRadius),
                  border: Border.all(
                      width: dispHeight * boardLine,
                      color: Colors.white), //Container 테두리 값, 색상
                ),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceAround,
                  children: <Widget>[
                    SizedBox(
                      // Battery Icon 및 전압 표시...
                      width: dispWidth * 0.17, //Battery Icon 높이 기존 : 100
                      height: dispHeight * 0.1,

                      // width: dispWidth * 0.12,
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.center,
                        mainAxisAlignment: MainAxisAlignment.spaceAround,
                        children: <Widget>[
                          InkWell(
                            onDoubleTap: () async {
                              SharedPreferences prefs =
                                  await SharedPreferences.getInstance();
                              // carType = !carType;
                              carType ? carType = false : carType = true;
                              setState(() {
                                if (!carType) {
                                  V.ledName[1] = "라인등";
                                  V.ledName[2] = "중앙등";
                                  V.ledName[3] = "간접등";
                                  V.ledName[4] = "욕실등";
                                  V.ledName[5] = "현관등";
                                  V.outName[1] = "예비";

                                  prefs.setString('led1SaveName', V.ledName[1]);
                                  prefs.setString('led2SaveName', V.ledName[2]);
                                  prefs.setString('led3SaveName', V.ledName[3]);
                                  prefs.setString('led4SaveName', V.ledName[4]);
                                  prefs.setString('led5SaveName', V.ledName[5]);
                                  prefs.setString('out1SaveName', V.outName[1]);
                                } else {
                                  V.ledName[1] = "실내등";
                                  V.ledName[2] = "간접등";
                                  V.ledName[3] = "주방등";
                                  V.ledName[4] = "벙커등";
                                  V.ledName[5] = "욕실등";
                                  V.outName[1] = "현관등";

                                  prefs.setString('led1SaveName', V.ledName[1]);
                                  prefs.setString('led2SaveName', V.ledName[2]);
                                  prefs.setString('led3SaveName', V.ledName[3]);
                                  prefs.setString('led4SaveName', V.ledName[4]);
                                  prefs.setString('led5SaveName', V.ledName[5]);
                                  prefs.setString('out1SaveName', V.outName[1]);
                                }
                                if (beep) {
                                  advancedPlayer
                                      .play(AssetSource('sound/dongdong.mp3'));
                                }
                              });
                            },
                            child: Container(
                              width: dispWidth * 0.13, //Battery Icon 폭...
                              height: dispHeight * 0.05, //Battery Icon 높이...
                              decoration: BoxDecoration(
                                image: DecorationImage(
                                  image: AssetImage(voltPng),
                                  colorFilter: ColorFilter.mode(
                                      batColor, BlendMode.srcIn),
                                  fit: BoxFit.fill,
                                ),
                              ),
                            ),
                            onLongPress: () async {},
                          ),
                          Text(
                            // '${(V.btVolt).toString()} V',
                            netFlag ? voltText : '--.- V',
                            textAlign: TextAlign.right,
                            style: TextStyle(
                                fontWeight: FontWeight.w900,
                                fontSize: (dispWidth * 0.045) /
                                    txtScale, //Battery 전압 Size 기존 : 38
                                fontStyle: FontStyle.italic,
                                color: netFlag ? batColor : Colors.black),
                          ),
                        ],
                      ),
                    ),
                    SizedBox(
                      // 온도계, 온도표시, 습도계, 습도표시를 담고 있음...
                      width: dispWidth * 0.3,
                      height: dispHeight * 0.14,

                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.center,
                        mainAxisAlignment: MainAxisAlignment.spaceAround,
                        children: <Widget>[
                          InkWell(
                            onTap: () {},
                            child: Row(
                              // 온도계와 온도 표시 Row...
                              mainAxisAlignment: MainAxisAlignment.spaceAround,
                              children: [
                                Container(
                                  width: dispWidth * 0.1, // Temp Icon 폭
                                  height: dispHeight *
                                      0.045, // Temp Icon 높이 기존 : 100
                                  decoration: BoxDecoration(
                                    image: DecorationImage(
                                      // image: AssetImage('assets/images/temp.png'),
                                      image: AssetImage(tempPng),
                                      // colorFilter: ColorFilter.mode(
                                      //     mainIconColor, BlendMode.srcIn),
                                      fit: BoxFit.fill,
                                    ),
                                  ),
                                ),
                                Column(
                                  children: [
                                    Text(
                                      // 실내온도 표시...
                                      netFlag ? tempText : '--.- ℃',
                                      textAlign: TextAlign.right,
                                      style: TextStyle(
                                          fontWeight: FontWeight.w900,
                                          // fontSize: 18.0, //Battery 전압 Size 기존 : 38
                                          fontSize: (dispWidth * 0.045) /
                                              txtScale, //Battery 전압 Size 기존 : 38
                                          fontStyle: FontStyle.italic,
                                          color: mainFontColor),
                                    ),
                                    Text(
                                      // 설정온도 표시...
                                      netFlag
                                          ? ' ${(V.setTempValue).toStringAsFixed(1)}'
                                              ' ℃'
                                          : '--.- ℃',
                                      textAlign: TextAlign.right,
                                      style: TextStyle(
                                          //                              fontFamily: 'Karma',
                                          fontWeight: FontWeight.w900,
                                          fontSize: (dispWidth * 0.045) /
                                              txtScale, //Battery 전압 Size 기존 : 38
                                          fontStyle: FontStyle.italic,
                                          color: Colors.purpleAccent),
                                    ),
                                  ],
                                ),
                              ],
                            ),
                          ),
                          Row(
                            mainAxisAlignment: MainAxisAlignment.spaceAround,
                            children: [
                              SizedBox(
                                width: dispWidth * 0.12, // Temp Icon 폭
                                height:
                                    dispHeight * 0.045, // Temp Icon 높이 기존 : 100
                                child: ElevatedButton(
                                  style: ElevatedButton.styleFrom(
                                    backgroundColor: Colors.purpleAccent,
                                  ),
                                  child: Text(
                                    "-",
                                    style: TextStyle(
                                      color: Colors.black,
                                      fontWeight: FontWeight.bold,
                                      fontSize: dispWidth * 0.04,
                                    ),
                                  ),
                                  onPressed: () {
                                    if (V.setTempValue > 18.0) {
                                      // 하한 온도 설정...
                                      V.setTempValue -= 0.5;
                                      server.getReq("SET_TEMP", "설정온도",
                                          (V.setTempValue * 10).toInt());
                                    }
                                    if (beep) {
                                      advancedPlayer.play(
                                          AssetSource('sound/switchOff.mp3'));
                                    }
                                    setState(() {});
                                  },
                                ),
                              ),
                              SizedBox(
                                width: dispWidth * 0.12, // Temp Icon 폭
                                height:
                                    dispHeight * 0.045, // Temp Icon 높이 기존 : 100
                                child: ElevatedButton(
                                  style: ElevatedButton.styleFrom(
                                    backgroundColor: Colors.purpleAccent,
                                  ),
                                  child: Text(
                                    "+",
                                    style: TextStyle(
                                      color: Colors.black,
                                      fontWeight: FontWeight.bold,
                                      fontSize: dispWidth * 0.04,
                                    ),
                                  ),
                                  onPressed: () {
                                    if (V.setTempValue < 50.0) {
                                      // 상한 온도 설정...
                                      V.setTempValue += 0.5;
                                      server.getReq("SET_TEMP", "설정온도",
                                          (V.setTempValue * 10).toInt());
                                    }
                                    if (beep) {
                                      advancedPlayer.play(
                                          AssetSource('sound/switchOn.mp3'));
                                    }
                                    setState(() {});
                                  },
                                ),
                              ),
                            ],
                          ),
                        ],
                      ),
                    ),
                    Column(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: <Widget>[
                        InkWell(
                          child: CircularPercentIndicator(
                            radius: dispWidth * 0.2,
                            lineWidth: dispWidth * 0.035,
                            percent: netFlag ? V.cleanWater / 100.0 : 0.0,
                            arcBackgroundColor: cleanBackColor,
                            arcType: ArcType.FULL,
                            center: Text(
                              V.cleanSensor
                                  ? netFlag
                                      ? "${V.cleanWater.toString()}%"
                                      : "---"
                                  : "---",
                              style: TextStyle(
                                  fontWeight: FontWeight.bold,
                                  // fontSize: 12.0 * V.textScaleRatio,
                                  fontSize: (dispWidth * 0.035) / txtScale,
                                  color: mainFontColor),
                            ),
                            circularStrokeCap: CircularStrokeCap.round,
                            backgroundColor: Colors.transparent,
                            progressColor: cleanBarColor,
                          ),
                        ),
                        Text(
                          '청수',
                          style: TextStyle(
                              fontWeight: FontWeight.w900,
                              fontSize: (dispWidth * 0.04) / txtScale,
                              color: mainFontColor),
                        ),
                      ],
                    ),
                    Column(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: <Widget>[
                        CircularPercentIndicator(
                          radius: dispWidth * 0.2,
                          lineWidth: dispWidth * 0.035,
                          percent: netFlag ? V.wasteWater / 100.0 : 0.0,
                          arcBackgroundColor: wasteBackColor,
                          arcType: ArcType.FULL,
                          center: Text(
                            V.wasteSensor
                                ? netFlag
                                    ? "${V.wasteWater.toString()}%"
                                    : "---"
                                : "---",
                            style: TextStyle(
                                fontWeight: FontWeight.bold,
                                // fontSize: 12.0 * V.textScaleRatio,
                                fontSize: (dispWidth * 0.035) / txtScale,
                                color: mainFontColor),
                          ),
                          circularStrokeCap: CircularStrokeCap.round,
                          backgroundColor: Colors.transparent,
                          progressColor: wasteBarColor,
                        ),
//
                        Text(
                          '오수',
                          style: TextStyle(
                              fontWeight: FontWeight.w900,
                              fontSize: (dispWidth * 0.04) / txtScale,
                              color: mainFontColor),
                        )
                      ],
                    ),
                    Column(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: <Widget>[
                        // Container(
                        SizedBox(
                          height: dispHeight * 0.1,
                          child: SfLinearGauge(
                              minimum: 0.0,
                              maximum: 100.0,
                              orientation: LinearGaugeOrientation.vertical,
                              majorTickStyle: LinearTickStyle(length: 5),
                              axisLabelStyle: TextStyle(
                                  fontSize: (dispWidth * 0.02) / txtScale,
                                  color: Colors.black),
                              barPointers: [
                                LinearBarPointer(
                                    value: V.fuel.toDouble(),
                                    color: Colors.deepOrange),
                              ],
                              axisTrackStyle: LinearAxisTrackStyle(
                                  color: Colors.cyan,
                                  edgeStyle: LinearEdgeStyle.bothFlat,
                                  thickness: (dispWidth * 0.01) / txtScale,
                                  borderColor: Colors.grey)),
                        ),
                        Text(
                          '연료',
                          style: TextStyle(
                              fontWeight: FontWeight.w900,
                              fontSize: (dispWidth * 0.04) / txtScale,
                              color: mainFontColor),
                        )
                      ],
                    ),
                  ],
                ),
              ),
              // display 창과의 간격 늘림...
              SizedBox(
                height: dispHeight * 0.003,
              ),
              Container(
                // Button 및 Slide Bar 표시...
                // margin: const EdgeInsets.symmetric(vertical: 2.0),
                height: dispHeight * 0.2,
                decoration: BoxDecoration(
                    borderRadius:
                        BorderRadius.circular(dispHeight * borderRadius),
                    border: Border.all(
                        width: dispHeight * boardLine, color: Colors.white)),
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.spaceAround,
                  children: <Widget>[
                    MotorProcess(1),
                    MotorProcess(2),
                    MotorProcess(3),
                  ],
                ),
              ),
              SizedBox(
                height: dispHeight * 0.003,
              ),
              Container(
                // Button 및 Slide Bar 표시...
                // margin: EdgeInsets.symmetric(
                //   vertical: dispHeight * 0.01,
                // ),
                height: dispHeight * 0.07,
                decoration: BoxDecoration(
                    borderRadius:
                        BorderRadius.circular(dispHeight * borderRadius),
                    border: Border.all(
                        width: dispHeight * boardLine, color: Colors.white)),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceAround,
                  children: <Widget>[
                    AllLedProcess(),
                    InvertorProcess(),
                    CleanValveProcess(),
                    WasteValveProcess(),
                  ],
                ),
              ),
              SizedBox(
                height: dispHeight * 0.003,
              ),
              // LED 출력용 Container..
              Container(
                // Button 및 Slide Bar 표시...
                // margin: const EdgeInsets.symmetric(vertical: 2.0),
                height: dispHeight * 0.4,
                decoration: BoxDecoration(
                    // color: ledBackColor,
                    borderRadius:
                        BorderRadius.circular(dispHeight * borderRadius),
                    border: Border.all(
                        width: dispHeight * boardLine, color: Colors.white)),
                // border: Border.all(width: 2, color: ledBoarColor)),
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.spaceAround,
                  children: <Widget>[
                    LedProcess(1),
                    LedProcess(2),
                    LedProcess(3),
                    LedProcess(4),
                    LedProcess(5),
                    LedProcess(6),
                  ],
                ),
              ),
              SizedBox(
                height: dispHeight * 0.003,
              ),
//            LowerDisplay(),
              Column(
                children: <Widget>[
                  Container(
                    height: dispHeight * 0.08,
                    decoration: BoxDecoration(
                        borderRadius:
                            BorderRadius.circular(dispHeight * borderRadius),
                        border: Border.all(
                            width: dispHeight * boardLine,
                            color: Colors.white)),
                    child: Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: <Widget>[
                        OutProcess(1),
                        OutProcess(2),
                        OutProcess(3),
                        OutProcess(4),
                        OutProcess(5),
                        OutProcess(6),
                      ],
                    ),
                  ),
                ],
              ),
              if (liftFlag && netFlag)
                SizedBox(
                  height: dispHeight * 0.2,
                ),
              if (liftFlag && netFlag)
                Container(
                  margin: EdgeInsets.all(dispWidth * 0.02),
                  decoration: const BoxDecoration(
                    image: DecorationImage(
                      image: AssetImage('assets/images/motor_home.png'),
                    ),
                  ),
                  child: Stack(
                    children: <Widget>[
                      Row(
                        mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                        // mainAxisAlignment: MainAxisAlignment.center,
                        children: [
                          Column(
                            mainAxisAlignment: MainAxisAlignment.spaceAround,
                            crossAxisAlignment: CrossAxisAlignment.center,
                            children: [
                              Column(
                                children: [
                                  SizedBox(
                                    // height: mobile ? dispHeight * 0.25 : dispHeight * 0.15,
                                    height: dispHeight * 0.25,
                                  ),
                                  GestureDetector(
                                    child: InkWell(
                                      child: Icon(
                                        Icons.arrow_circle_up,
                                        size: mobile
                                            ? dispWidth * 0.25
                                            : dispWidth * 0.18,
                                        // size: dispWidth * 0.25,
                                        color: leg1UpFlag
                                            ? Colors.red
                                            : Colors.black54,
                                      ),
                                      onTap: () {
                                        setState(() {
                                          if (!allUpFlag && !allDownFlag) {
                                            if (leg1DownFlag) {
                                              leg1DownFlag = false;
                                              advancedPlayer.play(AssetSource(
                                                  'sound/switchOff.mp3'));
                                              server.getReq(
                                                  "L1D", "리프트 1 하강정지", 0);
                                            } else {
                                              leg1UpFlag = !leg1UpFlag;
                                              leg1UpFlag
                                                  ? server.getReq(
                                                      "L1U", "리프트 1 상승시작", 1)
                                                  : server.getReq(
                                                      "L1U", "리프트 1 상승정지", 0);
                                              leg1UpFlag
                                                  ? advancedPlayer.play(
                                                      AssetSource(
                                                          'sound/switchOn.mp3'))
                                                  : advancedPlayer.play(
                                                      AssetSource(
                                                          'sound/switchOff.mp3'));
                                              leg1UpFlag
                                                  ? print('press LEG 1 UP ON')
                                                  : print('press LEG 1 UP OFF');
                                            }
                                          }
                                        });
                                      },
                                    ),
                                  ),
                                  SizedBox(
                                    height: dispHeight * 0.03,
                                  ),
                                  GestureDetector(
                                    child: InkWell(
                                      child: Icon(
                                        Icons.arrow_circle_down,
                                        size: mobile
                                            ? dispWidth * 0.25
                                            : dispWidth * 0.18,
                                        // size: dispWidth * 0.25,
                                        color: leg1DownFlag
                                            ? Colors.red
                                            : Colors.black54,
                                      ),
                                      onTap: () {
                                        setState(() {
                                          if (!allUpFlag && !allDownFlag) {
                                            if (leg1UpFlag) {
                                              leg1UpFlag = false;
                                              advancedPlayer.play(AssetSource(
                                                  'sound/switchOff.mp3'));
                                              server.getReq(
                                                  "L1U", "리프트 1 상승정지", 0);
                                            } else {
                                              leg1DownFlag = !leg1DownFlag;
                                              leg1DownFlag
                                                  ? server.getReq(
                                                      "L1D", "리프트 1 하강시작", 1)
                                                  : server.getReq(
                                                      "L1D", "리프트 1 하강정지", 0);
                                              leg1DownFlag
                                                  ? advancedPlayer.play(
                                                      AssetSource(
                                                          'sound/switchOff.mp3'))
                                                  : advancedPlayer.play(
                                                      AssetSource(
                                                          'sound/switchOn.mp3'));
                                              leg1DownFlag
                                                  ? print('press LEG 1 Down ON')
                                                  : print(
                                                      'press LEG 1 Down OFF');
                                            }
                                          }
                                        });
                                      },
                                    ),
                                  ),
                                ],
                              ),
                              SizedBox(
                                height: dispHeight * 0.05,
                              ),
                              Column(
                                children: [
                                  GestureDetector(
                                    child: InkWell(
                                      child: Icon(
                                        Icons.arrow_circle_up,
                                        size: mobile
                                            ? dispWidth * 0.25
                                            : dispWidth * 0.18,
                                        // size: dispWidth * 0.25,
                                        color: leg3UpFlag
                                            ? Colors.red
                                            : Colors.black54,
                                      ),
                                      onTap: () {
                                        setState(() {
                                          if (!allUpFlag && !allDownFlag) {
                                            if (leg3DownFlag) {
                                              leg3DownFlag = false;
                                              advancedPlayer.play(AssetSource(
                                                  'sound/switchOff.mp3'));
                                              server.getReq(
                                                  "L3D", "리프트 3 하강정지", 0);
                                            } else {
                                              leg3UpFlag = !leg3UpFlag;
                                              leg3UpFlag
                                                  ? server.getReq(
                                                      "L3U", "리프트 3 상승시작", 1)
                                                  : server.getReq(
                                                      "L3U", "리프트 3 상승정지", 0);
                                              leg3UpFlag
                                                  ? advancedPlayer.play(
                                                      AssetSource(
                                                          'sound/switchOn.mp3'))
                                                  : advancedPlayer.play(
                                                      AssetSource(
                                                          'sound/switchOff.mp3'));
                                              leg3UpFlag
                                                  ? print('press LEG 3 UP ON')
                                                  : print('press LEG 3 UP OFF');
                                            }
                                          }
                                        });
                                      },
                                    ),
                                  ),
                                  SizedBox(
                                    height: dispHeight * 0.03,
                                  ),
                                  GestureDetector(
                                    child: InkWell(
                                      child: Icon(
                                        Icons.arrow_circle_down,
                                        size: mobile
                                            ? dispWidth * 0.25
                                            : dispWidth * 0.18,
                                        color: leg3DownFlag
                                            ? Colors.red
                                            : Colors.black54,
                                      ),
                                      onTap: () {
                                        setState(() {
                                          if (!allUpFlag && !allDownFlag) {
                                            if (leg3UpFlag) {
                                              leg3UpFlag = false;
                                              advancedPlayer.play(AssetSource(
                                                  'sound/switchOff.mp3'));
                                              server.getReq(
                                                  "L3U", "리프트 3 상승정지", 0);
                                            } else {
                                              leg3DownFlag = !leg3DownFlag;
                                              leg3DownFlag
                                                  ? server.getReq(
                                                      "L3D", "리프트 3 하강시작", 1)
                                                  : server.getReq(
                                                      "L3D", "리프트 3 하강정지", 0);
                                              leg3DownFlag
                                                  ? advancedPlayer.play(
                                                      AssetSource(
                                                          'sound/switchOn.mp3'))
                                                  : advancedPlayer.play(
                                                      AssetSource(
                                                          'sound/switchOff.mp3'));
                                              leg3DownFlag
                                                  ? print('press LEG 3 Down ON')
                                                  : print(
                                                      'press LEG 3 Down OFF');
                                            }
                                          }
                                        });
                                      },
                                    ),
                                  ),
                                ],
                              ),
                            ],
                          ),
                          Column(
                            mainAxisAlignment: MainAxisAlignment.spaceAround,
                            crossAxisAlignment: CrossAxisAlignment.center,
                            children: [
                              Column(
                                children: [
                                  SizedBox(
                                    height: dispHeight * 0.2,
                                  ),
                                  Text(
                                    "UP",
                                    // "$wifiCount",
                                    style: TextStyle(
                                        color: Colors.red,
                                        fontSize: mobile
                                            ? (dispWidth * 0.1) / txtScale
                                            : (dispWidth * 0.07) / txtScale,
                                        // fontSize: (dispWidth * 0.1) / txtScale,
                                        fontWeight: FontWeight.w800),
                                  ),
                                  GestureDetector(
                                    child: InkWell(
                                      child: Icon(
                                        Icons.arrow_circle_up,
                                        size: mobile
                                            ? dispWidth * 0.25
                                            : dispWidth * 0.18,
                                        // size: dispWidth * 0.25,
                                        color: allUpFlag
                                            ? Colors.red
                                            : Colors.black54,
                                      ),
                                      onTap: () async {
                                        if (allDownFlag) {
                                          allDownFlag = false;
                                          advancedPlayer.play(AssetSource(
                                              'sound/switchOff.mp3'));
                                          await server.getReq(
                                              "LAD", "리프트 전체 하강정지", 0);
                                        } else {
                                          setState(() {
                                            allUpFlag = !allUpFlag;
                                            allUpFlag
                                                ? advancedPlayer.play(
                                                    AssetSource(
                                                        'sound/switchOn.mp3'))
                                                : advancedPlayer.play(
                                                    AssetSource(
                                                        'sound/switchOff.mp3'));
                                            allUpFlag
                                                ? server.getReq(
                                                    "LAU", "리프트 전체 상승시작", 1)
                                                : server.getReq(
                                                    "LAU", "리프트 전체 상승정지", 0);
                                            allUpFlag
                                                ? print('press LEG ALL UP ON')
                                                : print('press LEG ALL UP OFF');
                                          });
                                        }
                                      },
                                    ),
                                  ),
                                  SizedBox(
                                    height: dispHeight * 0.03,
                                  ),
                                  GestureDetector(
                                    child: InkWell(
                                      child: Icon(
                                        Icons.arrow_circle_down,
                                        size: mobile
                                            ? dispWidth * 0.25
                                            : dispWidth * 0.18,
                                        // size: dispWidth * 0.25,
                                        color: allDownFlag
                                            ? Colors.red
                                            : Colors.black54,
                                      ),
                                      onTap: () async {
                                        if (allUpFlag) {
                                          allUpFlag = false;
                                          advancedPlayer.play(AssetSource(
                                              'sound/switchOff.mp3'));
                                          await server.getReq(
                                              "LAU", "리프트 전체 상승정지", 0);
                                        } else {
                                          setState(() {
                                            allDownFlag = !allDownFlag;
                                            allDownFlag
                                                ? advancedPlayer.play(
                                                    AssetSource(
                                                        'sound/switchOn.mp3'))
                                                : advancedPlayer.play(
                                                    AssetSource(
                                                        'sound/switchOff.mp3'));
                                            allDownFlag
                                                ? server.getReq(
                                                    "LAD", "리프트 전체 하강시작", 1)
                                                : server.getReq(
                                                    "LAD", "리프트 전체 하강정지", 0);
                                            allDownFlag
                                                ? print('press LEG ALL DOWN ON')
                                                : print(
                                                    'press LEG ALL DOWN OFF');
                                          });
                                        }
                                      },
                                    ),
                                  ),
                                  Text(
                                    "DOWN",
                                    style: TextStyle(
                                        color: Colors.red,
                                        fontSize: mobile
                                            ? (dispWidth * 0.1) / txtScale
                                            : (dispWidth * 0.07) / txtScale,
                                        // fontSize: (dispWidth * 0.1) / txtScale,
                                        fontWeight: FontWeight.w800),
                                  ),
                                ],
                              ),
                            ],
                          ),
                          Column(
                            mainAxisAlignment: MainAxisAlignment.spaceAround,
                            crossAxisAlignment: CrossAxisAlignment.center,
                            children: [
                              Column(
                                children: [
                                  SizedBox(
                                    // height: mobile ? dispHeight * 0.25 : dispHeight * 0.15,
                                    height: dispHeight * 0.25,
                                  ),
                                  GestureDetector(
                                    child: InkWell(
                                      child: Icon(
                                        Icons.arrow_circle_up,
                                        size: mobile
                                            ? dispWidth * 0.25
                                            : dispWidth * 0.18,
                                        // size: dispWidth * 0.25,
                                        color: leg2UpFlag
                                            ? Colors.red
                                            : Colors.black54,
                                      ),
                                      onTap: () async {
                                        setState(() {
                                          if (!allUpFlag && !allDownFlag) {
                                            if (leg2DownFlag) {
                                              leg2DownFlag = false;
                                              advancedPlayer.play(AssetSource(
                                                  'sound/switchOff.mp3'));
                                              server.getReq(
                                                  "L2D", "리프트 2 하강정지", 0);
                                            } else {
                                              leg2UpFlag = !leg2UpFlag;
                                              leg2UpFlag
                                                  ? server.getReq(
                                                      "L2U", "리프트 2 상승시작", 1)
                                                  : server.getReq(
                                                      "L2U", "리프트 2 하강정지", 0);
                                              leg2UpFlag
                                                  ? advancedPlayer.play(
                                                      AssetSource(
                                                          'sound/switchOn.mp3'))
                                                  : advancedPlayer.play(
                                                      AssetSource(
                                                          'sound/switchOff.mp3'));
                                              leg2UpFlag
                                                  ? print('press LEG 2 UP ON')
                                                  : print('press LEG 2 UP OFF');
                                            }
                                          }
                                        });
                                      },
                                    ),
                                  ),
                                  SizedBox(
                                    height: dispHeight * 0.03,
                                  ),
                                  GestureDetector(
                                    child: InkWell(
                                      child: Icon(
                                        Icons.arrow_circle_down,
                                        size: mobile
                                            ? dispWidth * 0.25
                                            : dispWidth * 0.18,
                                        color: leg2DownFlag
                                            ? Colors.red
                                            : Colors.black54,
                                      ),
                                      onTap: () async {
                                        setState(() {
                                          if (!allUpFlag && !allDownFlag) {
                                            if (leg2UpFlag) {
                                              leg2UpFlag = false;
                                              advancedPlayer.play(AssetSource(
                                                  'sound/switchOff.mp3'));
                                              server.getReq(
                                                  "L2U", "리프트 2 상승정지", 0);
                                            } else {
                                              leg2DownFlag = !leg2DownFlag;
                                              leg2DownFlag
                                                  ? server.getReq(
                                                      "L2D", "리프트 2 하강시작", 1)
                                                  : server.getReq(
                                                      "L2D", "리프트 2 하강정지", 0);
                                              leg2DownFlag
                                                  ? advancedPlayer.play(
                                                      AssetSource(
                                                          'sound/switchOn.mp3'))
                                                  : advancedPlayer.play(
                                                      AssetSource(
                                                          'sound/switchOff.mp3'));
                                              leg2DownFlag
                                                  ? print('press LEG 2 Down ON')
                                                  : print(
                                                      'press LEG 2 Down OFF');
                                            }
                                          }
                                        });
                                      },
                                    ),
                                  ),
                                ],
                              ),
                              SizedBox(
                                height: dispHeight * 0.05,
                              ),
                              Column(
                                children: [
                                  GestureDetector(
                                    child: InkWell(
                                      child: Icon(
                                        Icons.arrow_circle_up,
                                        size: mobile
                                            ? dispWidth * 0.25
                                            : dispWidth * 0.18,
                                        // size: dispWidth * 0.25,
                                        color: leg4UpFlag
                                            ? Colors.red
                                            : Colors.black54,
                                      ),
                                      onTap: () async {
                                        setState(() {
                                          if (!allUpFlag && !allDownFlag) {
                                            if (leg4DownFlag) {
                                              leg4DownFlag = false;
                                              advancedPlayer.play(AssetSource(
                                                  'sound/switchOff.mp3'));
                                              server.getReq(
                                                  "L4D", "리프트 4 상승정지", 0);
                                            } else {
                                              leg4UpFlag = !leg4UpFlag;
                                              leg4UpFlag
                                                  ? server.getReq(
                                                      "L4U", "리프트 4 상승시작", 1)
                                                  : server.getReq(
                                                      "L4U", "리프트 4 상승정지", 0);
                                              leg4UpFlag
                                                  ? advancedPlayer.play(
                                                      AssetSource(
                                                          'sound/switchOn.mp3'))
                                                  : advancedPlayer.play(
                                                      AssetSource(
                                                          'sound/switchOn.mp3'));
                                              leg4UpFlag
                                                  ? print('press LEG 4 UP ON')
                                                  : print('press LEG 4 UP OFF');
                                            }
                                          }
                                        });
                                      },
                                    ),
                                  ),
                                  SizedBox(
                                    height: dispHeight * 0.03,
                                  ),
                                  GestureDetector(
                                    child: InkWell(
                                      child: Icon(
                                        Icons.arrow_circle_down,
                                        size: mobile
                                            ? dispWidth * 0.25
                                            : dispWidth * 0.18,
                                        // size: dispWidth * 0.25,
                                        color: leg4DownFlag
                                            ? Colors.red
                                            : Colors.black54,
                                      ),
                                      onTap: () async {
                                        setState(() {
                                          if (!allUpFlag && !allDownFlag) {
                                            if (leg4UpFlag) {
                                              leg4UpFlag = false;
                                              advancedPlayer.play(AssetSource(
                                                  'sound/switchOff.mp3'));
                                              server.getReq(
                                                  "L4U", "리프트 4 상승정지", 0);
                                            } else {
                                              leg4DownFlag = !leg4DownFlag;
                                              leg4DownFlag
                                                  ? server.getReq(
                                                      "L4D", "리프트 4 하강시작", 1)
                                                  : server.getReq(
                                                      "L4D", "리프트 4 하강정지", 0);
                                              leg4DownFlag
                                                  ? advancedPlayer.play(
                                                      AssetSource(
                                                          'sound/switchOn.mp3'))
                                                  : advancedPlayer.play(
                                                      AssetSource(
                                                          'sound/switchOn.mp3'));
                                              leg4DownFlag
                                                  ? print('press LEG 4 Down ON')
                                                  : print(
                                                      'press LEG 4 Down OFF');
                                            }
                                          }
                                        });
                                      },
                                    ),
                                  ),
                                ],
                              ),
                            ],
                          ),
                        ],
                      ),
                    ],
                  ),
                ),
              SizedBox(
                height: liftFlag ? dispHeight * 0.2 : dispHeight * 0.1,
              ),
              InkWell(
                onTap: () {
                  V.isPhoneTable[1]
                      ? launch("tel://${V.phoneNumber[1]}")
                      : null;
                },
                onLongPress: () async {
                  // V.comFlag = false;
                  await Get.toNamed("/phoneNumber");
                  // V.comFlag = true;
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
                                // color: messageFontColor,
                                color: Colors.yellow,
                                // color: messageFontColor,
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
                                // color: messageFontColor,
                                color: Colors.yellow,
                                // color: messageFontColor,
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
                        // launch("tel://01044885977");
                        launch("tel://${V.phoneNumber[2]}");
                      },
                      onLongPress: () async {
                        await Get.toNamed("/phoneNumber");
                      },
                      child: Container(
                        alignment: Alignment.center,
                        margin: const EdgeInsets.symmetric(vertical: 2.0),
                        // width: 250,
                        // height: 40,
                        // decoration: BoxDecoration(
                        //   // color: messageBackColor, //Container 배경 색
                        //   borderRadius: BorderRadius.circular(7),
                        //   border: Border.all(
                        //       // width: 2, color: Color(0xff22FFEE)), //Container 테두리 값, 색상
                        //       width: 2,
                        //       color: Colors.white), //Container 테두리 값, 색상
                        //   // width: 2, color: messageBoardColor), //Container 테두리 값, 색상
                        // ),
                        child: Padding(
                          padding: const EdgeInsets.all(5.0),
                          child: Column(
                            children: <Widget>[
                              Text(
                                "${V.phoneName[2]} :  ${V.phoneNumber[2]}",
                                style: TextStyle(
                                  // color: messageFontColor,
                                  color: Colors.yellow,
                                  // color: messageFontColor,
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
                        // launch("tel://01044885977");
                        launch("tel://${V.phoneNumber[3]}");
                      },
                      onLongPress: () async {
                        await Get.toNamed("/phoneNumber");
                      },
                      child: Container(
                        alignment: Alignment.center,
                        margin: const EdgeInsets.symmetric(vertical: 2.0),
                        // width: 250,
                        // height: 40,
                        // decoration: BoxDecoration(
                        //   // color: messageBackColor, //Container 배경 색
                        //   borderRadius: BorderRadius.circular(7),
                        //   border: Border.all(
                        //       // width: 2, color: Color(0xff22FFEE)), //Container 테두리 값, 색상
                        //       width: 2,
                        //       color: Colors.white), //Container 테두리 값, 색상
                        //   // width: 2, color: messageBoardColor), //Container 테두리 값, 색상
                        // ),
                        child: Padding(
                          padding: const EdgeInsets.all(5.0),
                          child: Column(
                            children: <Widget>[
                              Text(
                                "${V.phoneName[3]} :  ${V.phoneNumber[3]}",
                                style: TextStyle(
                                  // color: messageFontColor,
                                  color: Colors.yellow,
                                  // color: messageFontColor,
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
                        // launch("tel://01044885977");
                        launch("tel://${V.phoneNumber[4]}");
                      },
                      onLongPress: () async {
                        await Get.toNamed("/phoneNumber");
                      },
                      child: Container(
                        alignment: Alignment.center,
                        margin: const EdgeInsets.symmetric(vertical: 2.0),
                        // width: 250,
                        // height: 40,
                        // decoration: BoxDecoration(
                        //   // color: messageBackColor, //Container 배경 색
                        //   borderRadius: BorderRadius.circular(7),
                        //   border: Border.all(
                        //       // width: 2, color: Color(0xff22FFEE)), //Container 테두리 값, 색상
                        //       width: 2,
                        //       color: Colors.white), //Container 테두리 값, 색상
                        //   // width: 2, color: messageBoardColor), //Container 테두리 값, 색상
                        // ),
                        child: Padding(
                          padding: const EdgeInsets.all(5.0),
                          child: Column(
                            children: <Widget>[
                              Text(
                                "${V.phoneName[4]} :  ${V.phoneNumber[4]}",
                                style: TextStyle(
                                  // color: messageFontColor,
                                  color: Colors.yellow,
                                  // color: messageFontColor,
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
                        // launch("tel://01044885977");
                        launch("tel://${V.phoneNumber[5]}");
                      },
                      onLongPress: () async {
                        await Get.toNamed("/phoneNumber");
                      },
                      child: Container(
                        alignment: Alignment.center,
                        margin: const EdgeInsets.symmetric(vertical: 2.0),
                        // width: 250,
                        // height: 40,
                        // decoration: BoxDecoration(
                        //   // color: messageBackColor, //Container 배경 색
                        //   borderRadius: BorderRadius.circular(7),
                        //   border: Border.all(
                        //       // width: 2, color: Color(0xff22FFEE)), //Container 테두리 값, 색상
                        //       width: 2,
                        //       color: Colors.white), //Container 테두리 값, 색상
                        //   // width: 2, color: messageBoardColor), //Container 테두리 값, 색상
                        // ),
                        child: Padding(
                          padding: const EdgeInsets.all(5.0),
                          child: Column(
                            children: <Widget>[
                              Text(
                                "${V.phoneName[5]} :  ${V.phoneNumber[5]}",
                                style: TextStyle(
                                  // color: messageFontColor,
                                  color: Colors.yellow,
                                  // color: messageFontColor,
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
  // ignore: prefer_typing_uninitialized_variables
  var ledCh;
  LedProcess(this.ledCh, {Key? key}) : super(key: key);

  @override
  // ignore: no_logic_in_create_state
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
                      color:
                          V.ledStatus[ledCh] ? slideFontColor : Colors.black54,
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
                // width: sliderValueTextWidth,
                width: mobile ? dispWidth * 0.08 : dispWidth * 0.05,
                child: Text(
                  '${(V.ledValue[ledCh]).round()}',
                  textAlign: TextAlign.right,
                  style: TextStyle(
                      color:
                          V.ledStatus[ledCh] ? slideFontColor : Colors.black54,
                      // fontSize: sliderValueFontSize * V.textScaleRatio,
                      fontSize: mobile
                          ? (dispWidth * 0.04) / txtScale
                          : (dispWidth * 0.023) / txtScale,
                      fontStyle: FontStyle.italic,
                      fontWeight: FontWeight.w800),
                ),
              ),
              SizedBox(
                // width: mobile ? sliderWidth : landscapeSliderWidth,
                width: mobile ? dispWidth * 0.7 : dispWidth * 0.45,
                height: mobile ? dispHeight * 0.06 : dispHeight * 0.08,
                child: SliderTheme(
                  data: SliderTheme.of(context).copyWith(
                    // activeTrackColor: Colors.red[700],
                    // inactiveTrackColor: Colors.red[100],
                    // trackShape: RectangularSliderTrackShape(),
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
                    inactiveColor: const Color(0xff00ABA8),
                    // activeColor: mobile ? portraitSlideColor : landscapeSlideColor,
                    activeColor: portraitSlideColor,
                    value: V.ledValue[ledCh],
                    onChanged: V.ledStatus[ledCh]
                        ? (value) => setState(() {
                              // V.comFlag = false;
                              V.ledValue[ledCh] = value;
                              if (mobile) {
                                V.monitor =
                                    '${V.ledName[ledCh]} 밝기 값  ${V.ledValue[1].round()}';
                              }
                              print('${V.ledName[ledCh]} Slider Value : '
                                  '${(V.ledValue[ledCh]).round()}..');
                            })
                        : null,
                    onChangeEnd: (double newValue) {
                      // V.comFlag = true;
                      V.ledValue[ledCh] = newValue;
                      server.getReq("LED", V.ledName[ledCh], ledCh);
                      // V.comFlag = true;
                    },
                    label: (V.ledValue[ledCh]).round().toInt().toString(),
                  ),
                ),
              ),
              // PortraitLedProcess(1),
              InkWell(
                child: Container(
                  decoration: BoxDecoration(
                    color: V.ledStatus[ledCh] ? ledOnColor : ledOffColor,
                    borderRadius: BorderRadius.circular(mobile
                        ? (dispWidth * buttonRadius * 1.5)
                        : dispWidth * buttonRadius),
                    boxShadow: [
                      BoxShadow(
                          // color: Colors.white,
                          color: V.ledStatus[ledCh]
                              ? Colors.white
                              : Colors.black38,
                          offset: const Offset(-3.0, -3.0),
                          // offset: mobile
                          //     ? const Offset(-3.0, -3.0)
                          //     : const Offset(-3.0, -3.0),
                          blurRadius: 5.0,
                          spreadRadius: 2.0),
                      BoxShadow(
                          // color: Colors.black,
                          color:
                              V.ledStatus[ledCh] ? Colors.white : Colors.black,
                          offset: const Offset(2.0, 2.0),
                          // offset: mobile
                          //     ? const Offset(3.0, 3.0)
                          //     : const Offset(2.0, 2.0),
                          blurRadius: 5.0,
                          spreadRadius: 1.0),
                    ],
                    // border: Border.all(width: 2, color: ledSwitchOutLineColor),
                  ),
                  width: mobile ? dispWidth * 0.16 : dispWidth * 0.09,
                  height: mobile ? dispHeight * 0.045 : dispHeight * 0.08,
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.spaceAround,
                    children: <Widget>[
                      Text(
                        V.ledName[ledCh],
                        style: TextStyle(
                            color: V.ledStatus[ledCh]
                                ? Colors.white
                                : Colors.black,
                            fontSize: mobile
                                ? (dispWidth * 0.036) / txtScale
                                : (dispWidth * 0.018) / txtScale,
                            // letterSpacing: 1.0,
                            fontWeight: FontWeight.w800),
                      ),
                    ],
                  ),
                ),
                onTap: () {
                  setState(() {
                    V.ledStatus[ledCh]
                        ? V.ledStatus[ledCh] = false
                        : V.ledStatus[ledCh] = true;
                    server.getReq("LED", V.ledName[ledCh], ledCh);
                    if (V.ledStatus[ledCh] == true) {
                      V.monitor = '${V.ledName[ledCh]} On..';
                      if (beep) {
                        advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
                      }
                    } else {
                      V.monitor = '${V.ledName[ledCh]} Off..';
                      if (beep) {
                        advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
                      }
                    }
                    print('_isChecked_led[ledCh] : ${V.ledStatus[ledCh]}');
                  });
                },
                onLongPress: () {},
              ),
              SizedBox(
                width: dispWidth * 0.03,
              )
            ],
    );
  }
}

// ignore: must_be_immutable
class MotorProcess extends StatefulWidget {
  // ignore: prefer_typing_uninitialized_variables
  var motorCh;
  MotorProcess(this.motorCh, {Key? key}) : super(key: key);

  @override
  // ignore: no_logic_in_create_state
  _MotorProcessState createState() => _MotorProcessState(motorCh);
}

class _MotorProcessState extends State<MotorProcess> {
  final int motorCh;
  _MotorProcessState(this.motorCh);

  @override
  Widget build(BuildContext context) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
      children: (motorCh == 0)
          ? <Widget>[
              SizedBox(
                width: mobile ? dispWidth * 0.08 : dispWidth * 0.05,
                child: Text(
                  '${(V.motorValue[motorCh]).round()}',
                  textAlign: TextAlign.right,
                  style: TextStyle(
                      color: V.motorStatus[motorCh]
                          ? slideFontColor
                          : Colors.black54,
                      // fontSize: sliderValueFontSize * V.textScaleRatio,
                      fontSize: mobile
                          ? (dispWidth * 0.04) / txtScale
                          : (dispWidth * 0.023) / txtScale,
                      fontStyle: FontStyle.italic,
                      fontWeight: FontWeight.w800),
                ),
              ),
            ]
          : <Widget>[
              mobile
                  ? SizedBox(
                      // width: sliderValueTextWidth,
                      width: mobile ? dispWidth * 0.08 : dispWidth * 0.05,
                      child: Text(
                        '${(V.motorValue[motorCh]).round()}',
                        textAlign: TextAlign.right,
                        style: TextStyle(
                            color: V.motorStatus[motorCh]
                                ? slideFontColor
                                : Colors.black54,
                            // fontSize: sliderValueFontSize * V.textScaleRatio,
                            fontSize: mobile
                                ? (dispWidth * 0.04) / txtScale
                                : (dispWidth * 0.023) / txtScale,
                            fontStyle: FontStyle.italic,
                            fontWeight: FontWeight.w800),
                      ),
                    )
                  : const SizedBox.shrink(),
              SizedBox(
                // width: mobile ? sliderWidth : landscapeSliderWidth,
                width: mobile ? dispWidth * 0.7 : dispWidth * 0.16,
                height: mobile ? dispHeight * 0.06 : dispHeight * 0.08,
                child: SliderTheme(
                  data: SliderTheme.of(context).copyWith(
                    // activeTrackColor: Colors.red[700],
                    // inactiveTrackColor: Colors.red[100],
                    // trackShape: RectangularSliderTrackShape(),
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
                    min: 20.0,
                    max: 100.0,
                    inactiveColor: const Color(0xff00ABA8),
                    // activeColor: mobile ? portraitSlideColor : landscapeSlideColor,
                    // activeColor: portraitSlideColor,
                    activeColor: Colors.purpleAccent,
                    value: V.motorValue[motorCh],
                    onChanged: V.motorStatus[motorCh]
                        ? (value) => setState(() {
                              // V.comFlag = false;
                              V.motorValue[motorCh] = value;
                              if (mobile) {
                                V.monitor =
                                    '${V.motorName[motorCh]} 속도 값  ${V.motorValue[1].round()}';
                              }
                              print('${V.motorName[motorCh]} Slider Value : '
                                  '${(V.motorValue[motorCh]).round()}..');
                            })
                        : null,
                    onChangeEnd: (double newValue) {
                      // V.comFlag = true;
                      V.motorValue[motorCh] = newValue;
                      server.getReq("MOTOR", V.motorName[motorCh], motorCh);
                      // V.comFlag = true;
                    },
                    label: (V.motorValue[motorCh]).round().toInt().toString(),
                  ),
                ),
              ),
              // PortraitLedProcess(1),
              InkWell(
                child: Container(
                  decoration: BoxDecoration(
                    color: V.motorStatus[motorCh]
                        ? Colors.purpleAccent
                        : ledOffColor,
                    borderRadius: BorderRadius.circular(mobile
                        ? (dispWidth * buttonRadius * 1.5)
                        : dispWidth * buttonRadius),
                    boxShadow: [
                      BoxShadow(
                          // color: Colors.white,
                          color: V.motorStatus[motorCh]
                              ? Colors.white
                              : Colors.black38,
                          offset: const Offset(-3.0, -3.0),
                          // offset: mobile
                          //     ? const Offset(-3.0, -3.0)
                          //     : const Offset(-3.0, -3.0),
                          blurRadius: 5.0,
                          spreadRadius: 2.0),
                      BoxShadow(
                          // color: Colors.black,
                          color: V.motorStatus[motorCh]
                              ? Colors.white
                              : Colors.black,
                          offset: const Offset(2.0, 2.0),
                          // offset: mobile
                          //     ? const Offset(3.0, 3.0)
                          //     : const Offset(2.0, 2.0),
                          blurRadius: 5.0,
                          spreadRadius: 1.0),
                    ],
                    // border: Border.all(width: 2, color: ledSwitchOutLineColor),
                  ),
                  width: mobile ? dispWidth * 0.16 : dispWidth * 0.09,
                  height: mobile ? dispHeight * 0.045 : dispHeight * 0.08,
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.spaceAround,
                    children: <Widget>[
                      Text(
                        V.motorName[motorCh],
                        style: TextStyle(
                            color: V.motorStatus[motorCh]
                                ? Colors.white
                                : Colors.black,
                            fontSize: mobile
                                ? (dispWidth * 0.036) / txtScale
                                : (dispWidth * 0.018) / txtScale,
                            // letterSpacing: 1.0,
                            fontWeight: FontWeight.w800),
                      ),
                    ],
                  ),
                ),
                onTap: () {
                  // V.comFlag = false;
                  setState(() {
                    // V.comFlag = true;

                    V.motorStatus[motorCh]
                        ? V.motorStatus[motorCh] = false
                        : V.motorStatus[motorCh] = true;
                    server.getReq("MOTOR", V.motorName[motorCh], motorCh);
                    // V.comFlag = false;
                    if (V.motorStatus[motorCh] == true) {
                      V.monitor = '${V.motorName[motorCh]} On..';
                      if (beep) {
                        advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
                      }
                    } else {
                      V.monitor = '${V.motorName[motorCh]} Off..';
                      if (beep) {
                        advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
                      }
                    }
                    print('_isChecked_led[ledCh] : ${V.motorStatus[motorCh]}');
                    // V.comFlag = true;
                    //  server.getReq("LED", ledCh);
                  });
                },
                onLongPress: () {},
              ),
              mobile
                  ? SizedBox(
                      width: dispWidth * 0.03,
                    )
                  : const SizedBox.shrink(),
            ],
    );
  }
}

// ignore: must_be_immutable
class OutProcess extends StatefulWidget {
  // ignore: prefer_typing_uninitialized_variables
  var outCh;
  OutProcess(this.outCh, {Key? key}) : super(key: key);

  @override
  // ignore: no_logic_in_create_state
  _OutProcessState createState() => _OutProcessState(outCh);
}

class _OutProcessState extends State<OutProcess> {
  final int outCh;
  _OutProcessState(this.outCh);

  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        decoration: BoxDecoration(
          color: V.outStatus[outCh] ? outOnColor : outOffColor,
          borderRadius: BorderRadius.circular(mobile
              ? (dispWidth * buttonRadius * 1.5)
              : dispWidth * buttonRadius),
          boxShadow: [
            BoxShadow(
                color: V.outStatus[outCh] ? Colors.white : Colors.black38,
                offset: const Offset(-3.0, -3.0),
                blurRadius: 5.0,
                spreadRadius: 1.0),
            BoxShadow(
                color: V.outStatus[outCh] ? Colors.white : Colors.black,
                offset: const Offset(2.0, 2.0),
                blurRadius: 5.0,
                spreadRadius: 1.0),
          ],
        ),
        width: mobile ? dispWidth * 0.14 : dispWidth * 0.09,
        height: mobile ? dispHeight * 0.045 : dispHeight * 0.08,
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Text(
              V.outName[outCh],
              style: TextStyle(
                  color: V.outStatus[outCh] ? Colors.white : Colors.black,
                  fontSize: mobile
                      ? (dispWidth * 0.036) / txtScale
                      : (dispWidth * 0.018) / txtScale,
                  fontWeight: FontWeight.w800),
            ),
          ],
        ),
      ),
      onTap: () async {
        // V.comFlag = false;
        if (V.outName[outCh] == '물펌프' && V.cleanWater == 0) {
          if (beep) advancedPlayer.play(AssetSource('sound/alert.mp3'));
          await showDialog(
            context: context,
            builder: (context) => AlertDialog(
              backgroundColor: const Color(0xC0D4AA39),
              title: Text(
                "청수가 0 % 입니다....",
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
                  width: mobile ? dispWidth * 0.18 : dispWidth * 0.09,
                  height: mobile ? dispHeight * 0.045 : dispHeight * 0.07,
                  child: ElevatedButton(
                      child: Text(
                        V.outStatus[outCh] ? "끄기" : "켜기",
                        style: TextStyle(
                          color: Colors.black,
                          fontWeight: FontWeight.bold,
                          fontSize: mobile
                              ? (dispWidth * 0.04) / txtScale
                              : (dispHeight * 0.03) / txtScale,
                        ),
                      ),
                      onPressed: () {
                        if (V.outStatus[outCh] == true) {
                          if (beep) {
                            advancedPlayer
                                .play(AssetSource('sound/switchOff.mp3'));
                          }
                          V.outStatus[outCh] = false;
                          V.monitor = '${V.outName[outCh]} Off..';
                        } else {
                          if (beep) {
                            advancedPlayer
                                .play(AssetSource('sound/switchOn.mp3'));
                          }
                          V.outStatus[outCh] = true;
                          V.monitor = '${V.outName[outCh]} On..';
                        }
                        server.getReq("OUT", V.outName[outCh], outCh);
                        Get.back();
                      }),
                ),
                SizedBox(width: mobile ? dispHeight * 0.04 : dispWidth * 0.04),
                SizedBox(
                  width: mobile ? dispWidth * 0.22 : dispWidth * 0.1,
                  height: mobile ? dispHeight * 0.045 : dispHeight * 0.07,
                  child: ElevatedButton(
                      child: Text(
                        "나가기",
                        style: TextStyle(
                          color: Colors.black,
                          fontWeight: FontWeight.bold,
                          fontSize: mobile
                              ? (dispWidth * 0.04) / txtScale
                              : (dispHeight * 0.03) / txtScale,
                        ),
                      ),
                      onPressed: () {
                        Get.back();
                      }),
                ),
              ],
            ),
          );
        } else {
          if (V.outStatus[outCh] == true) {
            if (beep) advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
            V.outStatus[outCh] = false;
            V.monitor = '${V.outName[outCh]} Off..';
          } else {
            if (beep) advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
            V.outStatus[outCh] = true;
            V.monitor = '${V.outName[outCh]} On..';
          }
          server.getReq("OUT", V.outName[outCh], outCh);
        }

        // setState(() async {
        //   // V.comFlag = true;
        //   V.outStatus[outCh]
        //       ? print('${V.outName[outCh]} On..')
        //       : print('${V.outName[outCh]} Off..');
        // });
      },
    );
  }
}

class InvertorProcess extends StatefulWidget {
  // ignore: prefer_typing_uninitialized_variables, prefer_const_constructors_in_immutables
  InvertorProcess({Key? key}) : super(key: key);

  @override
  _InvertorProcessState createState() => _InvertorProcessState();
}

class _InvertorProcessState extends State<InvertorProcess> {
  // AudioCache audioCache = AudioCache();

  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        decoration: BoxDecoration(
          color: V.invertorStatus ? outOnColor : outOffColor,
          borderRadius: BorderRadius.circular(mobile
              ? (dispWidth * buttonRadius * 1.5)
              : dispWidth * buttonRadius),
          boxShadow: [
            BoxShadow(
                color: V.invertorStatus ? Colors.white : Colors.black38,
                offset: const Offset(-3.0, -3.0),
                // offset: mobile
                //     ? const Offset(-1.0, -1.0)
                //     : const Offset(-2.0, -2.0),
                // blurRadius: mobile ? 3.0 : 5.0,
                blurRadius: 5.0,
                spreadRadius: 1.0),
            BoxShadow(
                // color: Colors.black,
                color: V.invertorStatus ? Colors.white : Colors.black,
                offset: const Offset(2.0, 2.0),
                // mobile ? const Offset(1.0, 1.0) : const Offset(2.0, 2.0),
                // blurRadius: mobile ? 3.0 : 5.0,
                blurRadius: 5.0,
                spreadRadius: 1.0),
          ],
        ),
        width: mobile ? dispWidth * 0.16 : dispWidth * 0.09,
        height: mobile ? dispHeight * 0.045 : dispHeight * 0.08,
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Text(
              '인버터',
              style: TextStyle(
                  fontSize: mobile
                      ? (dispWidth * 0.036) / txtScale
                      : (dispWidth * 0.018) / txtScale,
                  color: V.invertorStatus ? Colors.white : Colors.black,
                  fontWeight: FontWeight.w800),
            )
          ],
        ),
      ),
      onTap: () async {
        // V.comFlag = false;
        if (V.invertorStatus == true) {
          if (beep) advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
          V.invertorStatus = false;
          V.monitor = '인버터 Off..';
        } else {
          if (beep) advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
          V.invertorStatus = true;
          V.monitor = '인버터 On..';
        }
        setState(() {});
        // V.comFlag = true;
        server.getReq("INVERTOR", "인버터", 0);
        V.invertorStatus ? print('인버터 On..') : print('인버터 Off..');
      },
    );
  }
}

class AllLedProcess extends StatefulWidget {
  // ignore: prefer_typing_uninitialized_variables, prefer_const_constructors_in_immutables
  AllLedProcess({Key? key}) : super(key: key);

  @override
  _AllLedProcessState createState() => _AllLedProcessState();
}

class _AllLedProcessState extends State<AllLedProcess> {
  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        decoration: BoxDecoration(
          color: V.allLedStatus ? outOnColor : outOffColor,
          borderRadius: BorderRadius.circular(mobile
              ? (dispWidth * buttonRadius * 1.5)
              : dispWidth * buttonRadius),
          boxShadow: [
            BoxShadow(
                color: V.allLedStatus ? Colors.white : Colors.black38,
                offset: const Offset(-3.0, -3.0),
                blurRadius: 5.0,
                spreadRadius: 1.0),
            BoxShadow(
                color: V.allLedStatus ? Colors.white : Colors.black,
                offset: const Offset(2.0, 2.0),
                blurRadius: 5.0,
                spreadRadius: 1.0),
          ],
        ),
        // Button Size...
        width: mobile ? dispWidth * 0.16 : dispWidth * 0.09,
        height: mobile ? dispHeight * 0.045 : dispHeight * 0.08,
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Text(
              '전체등',
              style: TextStyle(
                  color: V.allLedStatus ? Colors.white : Colors.black,
                  fontSize: mobile
                      ? (dispWidth * 0.036) / txtScale
                      : (dispWidth * 0.018) / txtScale,
                  fontWeight: FontWeight.w800),
            ),
          ],
        ),
      ),
      onTap: () async {
        // V.comFlag = false;
        if (V.allLedStatus == true) {
          if (beep) advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
          V.allLedStatus = false;
          V.monitor = '전체등 Off..';
        } else {
          if (beep) advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
          V.allLedStatus = true;
          V.monitor = '전체등 On..';
        }
        setState(() {});
        // V.comFlag = true;
        await server.getReq("ALL", "전체등", V.allLedStatus ? 1 : 0);
        V.allLedStatus ? print('전체등 On..') : print('전체등 Off..');
      },
    );
  }
}

class CleanValveProcess extends StatefulWidget {
  // ignore: prefer_typing_uninitialized_variables, prefer_const_constructors_in_immutables
  CleanValveProcess({Key? key}) : super(key: key);

  @override
  _CleanValveProcessState createState() => _CleanValveProcessState();
}

class _CleanValveProcessState extends State<CleanValveProcess> {
  // AudioCache audioCache = AudioCache();

  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        decoration: BoxDecoration(
          color: clnValveFlag ? outOnColor : outOffColor,
          borderRadius: BorderRadius.circular(mobile
              ? (dispWidth * buttonRadius * 1.5)
              : dispWidth * buttonRadius),
          boxShadow: [
            BoxShadow(
                color: clnValveFlag ? Colors.white : Colors.black38,
                offset: const Offset(-3.0, -3.0),
                blurRadius: 5.0,
                spreadRadius: 1.0),
            BoxShadow(
                color: clnValveFlag ? Colors.white : Colors.black,
                offset: const Offset(2.0, 2.0),
                blurRadius: 5.0,
                spreadRadius: 1.0),
          ],
        ),
        width: mobile ? dispWidth * 0.16 : dispWidth * 0.09,
        height: mobile ? dispHeight * 0.045 : dispHeight * 0.08,
        child: Padding(
          padding: const EdgeInsets.all(3.0),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.spaceAround,
            children: <Widget>[
              Text(
                '청수퇴수',
                style: TextStyle(
                    fontSize: mobile
                        ? (dispWidth * 0.036) / txtScale
                        : (dispWidth * 0.018) / txtScale,
                    color: clnValveFlag ? Colors.white : Colors.black,
                    fontWeight: FontWeight.w800),
              )
            ],
          ),
        ),
      ),
      onTap: () {
        // V.comFlag = false;
        // V.comFlag = false;
        clnValveFlag ? clnValveFlag = false : clnValveFlag = true;
        setState(() {
          if (clnValveFlag == true) {
            V.monitor =
                // '${V.ledName[0]} On..';
                '청수밸브 열림..';
            if (beep) advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
          } else {
            V.monitor = '청수밸브 닫힘..';
            // '${V.ledName[0]} Off..';
            if (beep) {
              advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
            }
          }
          print('clnValveFlag : $clnValveFlag');
          server.getReq("clnValve", "청수 퇴수", clnValveFlag ? 1 : 0);
        });
      },
      onLongPress: () {},
    );
  }
}

class WasteValveProcess extends StatefulWidget {
  // ignore: prefer_typing_uninitialized_variables, prefer_const_constructors_in_immutables
  WasteValveProcess({Key? key}) : super(key: key);

  @override
  _WasteValveProcessState createState() => _WasteValveProcessState();
}

class _WasteValveProcessState extends State<WasteValveProcess> {
  // AudioCache audioCache = AudioCache();

  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        decoration: BoxDecoration(
          color: wstValveFlag ? outOnColor : outOffColor,
          borderRadius: BorderRadius.circular(mobile
              ? (dispWidth * buttonRadius * 1.5)
              : dispWidth * buttonRadius),
          boxShadow: [
            BoxShadow(
                // color: Colors.white,
                color: wstValveFlag ? Colors.white : Colors.black38,
                offset: const Offset(-3.0, -3.0),
                // offset: mobile
                //     ? const Offset(-3.0, -3.0)
                //     : const Offset(-3.0, -3.0),
                blurRadius: 5.0,
                spreadRadius: 1.0),
            BoxShadow(
                // color: Colors.black,
                color: wstValveFlag ? Colors.white : Colors.black,
                offset: const Offset(2.0, 2.0),
                // offset: mobile
                //     ? const Offset(3.0, 3.0)
                //     : const Offset(2.0, 2.0),
                blurRadius: 5.0,
                spreadRadius: 1.0),
          ],
          // border: Border.all(width: 2, color: ledSwitchOutLineColor),
        ),
        width: mobile ? dispWidth * 0.16 : dispWidth * 0.09,
        height: mobile ? dispHeight * 0.045 : dispHeight * 0.08,
        child: Padding(
          padding: const EdgeInsets.all(3.0),
          child: Column(
            mainAxisAlignment: MainAxisAlignment.spaceAround,
            children: <Widget>[
              Text(
                '오수퇴수',
                style: TextStyle(
                    fontSize: mobile
                        ? (dispWidth * 0.036) / txtScale
                        : (dispWidth * 0.018) / txtScale,
                    color: wstValveFlag ? Colors.white : Colors.black,
                    fontWeight: FontWeight.w800),
              )
            ],
          ),
        ),
      ),
      onTap: () {
        // V.comFlag = false;
        wstValveFlag ? wstValveFlag = false : wstValveFlag = true;
        setState(() {
          if (wstValveFlag == true) {
            V.monitor = '오수밸브 열림..';
            if (beep) advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
          } else {
            V.monitor = '오수밸브 닫힘..';
            if (beep) {
              advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
            }
          }
          print('wstValveFlag : $wstValveFlag');
          // V.comFlag = true;
          server.getReq("wstValve", "오수 퇴수", wstValveFlag ? 1 : 0);
        });
      },
      onLongPress: () {},
    );
  }
}

class LandScreen extends StatefulWidget {
  const LandScreen({Key? key}) : super(key: key);

  @override
  _LandScreenState createState() => _LandScreenState();
}

class _LandScreenState extends State<LandScreen> {
  // AudioCache audioCache = AudioCache();
  // AudioPlayer advancedPlayer = AudioPlayer();

  @override
  Widget build(BuildContext context) {
    return Container(
      decoration: const BoxDecoration(
        //     gradient: LinearGradient(
        //   begin: Alignment.centerLeft,
        //   end: Alignment.centerRight,
        //   colors: const [Color(0xff4ca1af), Color(0xff4ca1af)],
        // )
        image: DecorationImage(
            // image: AssetImage('assets/images/wooden-textured-background.jpg'),
            // image: AssetImage('assets/images/wood3.png'),
            image: AssetImage('assets/images/sunrise.jpg'),
            fit: BoxFit.cover),
      ),
      child: Scaffold(
        // landscape 디자인...
        backgroundColor: const Color(0x30FFC745), //Colors.transparent,

        appBar: PreferredSize(
          preferredSize: Size.fromHeight(
            dispHeight * appBarHeight,
          ),
          // preferredSize: Size(100.0, dispHeight * appBarHeight),
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
                      color: Colors.white), //Container 테두리 값, 색상
                ),
                child: Column(
                  mainAxisAlignment: MainAxisAlignment.spaceAround,
                  children: <Widget>[
                    SizedBox(
                      // Battery Icon 및 전압 표시...
                      height: dispHeight * 0.1, //Battery Icon 높이 기존 : 100
                      child: InkWell(
                        onDoubleTap: () async {
                          // 외부 앱 실행
                          SharedPreferences prefs =
                              await SharedPreferences.getInstance();
                          // carType = !carType;
                          carType ? carType = false : carType = true;
                          setState(() {
                            if (!carType) {
                              V.ledName[1] = "라인등";
                              V.ledName[2] = "중앙등";
                              V.ledName[3] = "간접등";
                              V.ledName[4] = "욕실등";
                              V.ledName[5] = "현관등";
                              V.outName[1] = "예비";

                              prefs.setString('led1SaveName', V.ledName[1]);
                              prefs.setString('led2SaveName', V.ledName[2]);
                              prefs.setString('led3SaveName', V.ledName[3]);
                              prefs.setString('led4SaveName', V.ledName[4]);
                              prefs.setString('led5SaveName', V.ledName[5]);
                              prefs.setString('out1SaveName', V.outName[1]);
                            } else {
                              V.ledName[1] = "실내등";
                              V.ledName[2] = "간접등";
                              V.ledName[3] = "주방등";
                              V.ledName[4] = "벙커등";
                              V.ledName[5] = "욕실등";
                              V.outName[1] = "현관등";

                              prefs.setString('led1SaveName', V.ledName[1]);
                              prefs.setString('led2SaveName', V.ledName[2]);
                              prefs.setString('led3SaveName', V.ledName[3]);
                              prefs.setString('led4SaveName', V.ledName[4]);
                              prefs.setString('led5SaveName', V.ledName[5]);
                              prefs.setString('out1SaveName', V.outName[1]);
                            }
                            if (beep) {
                              advancedPlayer
                                  .play(AssetSource('sound/dongdong.mp3'));
                            }
                          });
                        },
                        child: Row(
                          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                          children: <Widget>[
                            Container(
                              width: (dispWidth * 0.06) * 1.0, //Battery Icon 폭
                              height:
                                  dispWidth * 0.045, //Battery Icon 높이 기존 : 100
                              decoration: BoxDecoration(
                                image: DecorationImage(
                                  image: AssetImage(voltPng),
                                  colorFilter: ColorFilter.mode(
                                      batColor, BlendMode.srcIn),
                                  fit: BoxFit.fill,
                                ),
                              ),
                            ),
                            Text(
                              netFlag ? voltText : '--.- V',
                              textAlign: TextAlign.right,
                              style: TextStyle(
                                  fontWeight: FontWeight.w900,
                                  fontSize: (dispWidth * 0.03) /
                                      txtScale, //Battery 전압 Size 기존 : 38
                                  fontStyle: FontStyle.italic,
                                  color: netFlag ? batColor : Colors.black),
                            )
                          ],
                        ),
                      ),
                    ),
                    SizedBox(
                      // 온도계 및 온도표시...
                      height: dispHeight * 0.12, //temp Icon 높이 기존 : 100
                      child: InkWell(
                        onTap: () {},
                        child: Row(
                          mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                          children: <Widget>[
                            SizedBox(
                              width: dispWidth * 0.003,
                            ),
                            Container(
                              width: (dispWidth * 0.06), //temp Icon 폭
                              height: dispWidth * 0.055, //temp Icon 높이 기존 : 100
                              decoration: BoxDecoration(
                                image: DecorationImage(
                                  image: AssetImage(tempPng),
                                  fit: BoxFit.fill,
                                ),
                              ),
                            ),
                            Column(
                              children: [
                                Text(
                                  netFlag ? tempText : '--.- ℃',
                                  textAlign: TextAlign.right,
                                  style: TextStyle(
                                      fontWeight: FontWeight.w900,
                                      fontSize: (dispWidth * 0.028) / txtScale,
                                      fontStyle: FontStyle.italic,
                                      color: mainFontColor),
                                ),
                                Text(
                                  // 설정온도 표시...
                                  netFlag
                                      ? ' ${(V.setTempValue).toStringAsFixed(1)}'
                                          ' ℃'
                                      : '--.- ℃',
                                  textAlign: TextAlign.right,
                                  style: TextStyle(
                                      //                              fontFamily: 'Karma',
                                      fontWeight: FontWeight.w900,
                                      // fontSize: 18.0, //Battery 전압 Size 기존 : 38
                                      fontSize: (dispWidth * 0.028) /
                                          txtScale, //Battery 전압 Size 기존 : 38
                                      fontStyle: FontStyle.italic,
                                      color: Colors.purpleAccent),
                                ),
                              ],
                            ),
                          ],
                        ),
                      ),
                    ),
                    Row(
                      mainAxisAlignment: MainAxisAlignment.spaceAround,
                      children: [
                        SizedBox(
                          width: dispWidth * 0.07, // Temp Icon 폭
                          height: dispHeight * 0.07, // Temp Icon 높이 기존 : 100
                          child: ElevatedButton(
                            style: ElevatedButton.styleFrom(
                              backgroundColor: Colors.purpleAccent,
                            ),
                            child: Text(
                              "-",
                              style: TextStyle(
                                color: Colors.black,
                                fontWeight: FontWeight.bold,
                                fontSize: dispWidth * 0.03,
                              ),
                            ),
                            onPressed: () async {
                              SharedPreferences prefs =
                                  await SharedPreferences.getInstance();
                              if (V.setTempValue > 18.0) {
                                // 하한 온도 설정...
                                V.setTempValue -= 0.5;
                                server.getReq("SET_TEMP", "설정온도",
                                    (V.setTempValue * 10).toInt());
                                prefs.setDouble('setTempValue', V.setTempValue);
                              }
                              if (beep) {
                                advancedPlayer
                                    .play(AssetSource('sound/switchOff.mp3'));
                              }
                              setState(() {});
                            },
                          ),
                        ),
                        SizedBox(
                          width: dispWidth * 0.07, // Temp Icon 폭
                          height: dispHeight * 0.07, // Temp Icon 높이 기존 : 100
                          child: ElevatedButton(
                            style: ElevatedButton.styleFrom(
                              backgroundColor: Colors.purpleAccent,
                            ),
                            child: Text(
                              "+",
                              style: TextStyle(
                                color: Colors.black,
                                fontWeight: FontWeight.bold,
                                fontSize: dispWidth * 0.03,
                              ),
                            ),
                            onPressed: () async {
                              SharedPreferences prefs =
                                  await SharedPreferences.getInstance();
                              if (V.setTempValue < 50.0) {
                                // 상한 온도 설정...
                                V.setTempValue += 0.5;
                                server.getReq("SET_TEMP", "설정온도",
                                    (V.setTempValue * 10).toInt());
                                prefs.setDouble('setTempValue', V.setTempValue);
                              }
                              if (beep) {
                                advancedPlayer
                                    .play(AssetSource('sound/switchOn.mp3'));
                              }
                              setState(() {});
                            },
                          ),
                        ),
                      ],
                    ),
                    Column(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: <Widget>[
                        InkWell(
                          child: CircularPercentIndicator(
                            radius: dispWidth * 0.115,
                            lineWidth: dispWidth * 0.02,
                            percent: netFlag ? V.cleanWater / 100.0 : 0.0,
                            arcBackgroundColor: cleanBackColor,
                            arcType: ArcType.FULL,
                            center: Text(
                              V.cleanSensor
                                  ? netFlag
                                      ? "${V.cleanWater.toString()}%"
                                      : "---"
                                  : "---",
                              style: TextStyle(
                                fontWeight: FontWeight.bold,
                                fontSize: (dispWidth * 0.025) / txtScale,
                                color: mainFontColor,
                              ),
                            ),
                            circularStrokeCap: CircularStrokeCap.round,
                            backgroundColor: Colors.transparent,
                            progressColor: Colors.blueAccent,
                          ),
                        ),
                        Text(
                          '청수',
                          style: TextStyle(
                              fontWeight: FontWeight.w900,
                              fontSize: (dispWidth * 0.02) / txtScale,
                              color: mainFontColor),
                        ),
                        SizedBox(
                          height: dispWidth * 0.01,
                        ),
                      ],
                    ),
                    Column(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: <Widget>[
                        CircularPercentIndicator(
                          radius: dispWidth * 0.115,
                          lineWidth: dispWidth * 0.02,
                          percent: netFlag ? V.wasteWater / 100.0 : 0.0,
                          // arcBackgroundColor: const Color(0xff5a5a5a),
                          arcBackgroundColor: wasteBackColor,
                          arcType: ArcType.FULL,
                          center: Text(
                            V.wasteSensor
                                ? netFlag
                                    ? "${V.wasteWater.toString()}%"
                                    : "---"
                                : "---",
                            style: TextStyle(
                                fontWeight: FontWeight.bold,
                                fontSize: (dispWidth * 0.025) / txtScale,
                                color: mainFontColor),
                          ),
                          circularStrokeCap: CircularStrokeCap.round,
                          backgroundColor: Colors.transparent,
                          progressColor: Colors.red[700],
                        ),
                        Text(
                          '오수',
                          style: TextStyle(
                              fontWeight: FontWeight.w900,
                              fontSize: (dispWidth * 0.02) / txtScale,
                              color: mainFontColor),
                        )
                      ],
                    ),
                    SizedBox(
                      height: dispHeight * 0.1,
                      child: SfLinearGauge(
                          minimum: 0.0,
                          maximum: 100.0,
                          orientation: LinearGaugeOrientation.horizontal,
                          majorTickStyle: LinearTickStyle(length: 5),
                          axisLabelStyle: TextStyle(
                              fontSize: (dispWidth * 0.01) / txtScale,
                              color: Colors.black),
                          barPointers: [
                            LinearBarPointer(
                                value: V.fuel.toDouble(),
                                color: Colors.deepOrange),
                          ],
                          axisTrackStyle: LinearAxisTrackStyle(
                              color: Colors.cyan,
                              edgeStyle: LinearEdgeStyle.bothFlat,
                              thickness: (dispWidth * 0.01) / txtScale,
                              borderColor: Colors.grey)),
                    ),
                  ],
                ),
              ),
              SizedBox(
                width: dispWidth * 0.003,
              ),
              Column(
                // 전체등, 인버터 등 스위치...
                children: [
                  Container(
                    // 히터, 벙커, 거실 Motor...
                    width: dispWidth * 0.796,
                    height: dispHeight * 0.12,
                    decoration: BoxDecoration(
                      // color: const Color(0xff3b3b3b),
                      color: Colors.transparent,
                      borderRadius:
                          BorderRadius.circular(dispWidth * borderRadius),
                      // border: Border.all(width: 2, color: Colors.lightGreen),
                      border: Border.all(
                          width: dispWidth * boardLine, color: Colors.white),
                    ),
                    child: Row(
                      mainAxisAlignment: MainAxisAlignment.spaceAround,
                      // ignore: prefer_const_literals_to_create_immutables
                      children: <Widget>[
                        MotorProcess(1),
                        MotorProcess(2),
                        MotorProcess(3),
                      ],
                    ),
                  ),
                  SizedBox(
                    height: dispHeight * 0.005,
                  ),
                  Container(
                    // 전체등, 인버터, 청수퇴수, 오수퇴수...
                    width: dispWidth * 0.796,
                    height: dispHeight * 0.12,
                    decoration: BoxDecoration(
                      color: Colors.transparent,
                      borderRadius:
                          BorderRadius.circular(dispWidth * borderRadius),
                      border: Border.all(
                          width: dispWidth * boardLine, color: Colors.white),
                    ),
                    child: Row(
                      mainAxisAlignment: MainAxisAlignment.spaceAround,
                      children: <Widget>[
                        AllLedProcess(),
                        InvertorProcess(),
                        CleanValveProcess(),
                        WasteValveProcess(),
                      ],
                    ),
                  ),
                  SizedBox(
                    height: dispHeight * 0.005,
                  ),
                  Row(
                    // LED 처리 Routine...
                    children: [
                      Container(
                        width: dispWidth * 0.6615,
                        height: dispHeight * 0.67,
                        decoration: BoxDecoration(
                          color: Colors.transparent,
                          borderRadius:
                              BorderRadius.circular(dispWidth * borderRadius),
                          border: Border.all(
                              width: dispWidth * boardLine,
                              color: Colors.white),
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
                          ],
                        ),
                      ),
                      SizedBox(
                        width: dispWidth * 0.003,
                      ),
                      Container(
                        // OUT Channel 처리 창...
                        width: dispWidth * 0.132,
                        height: dispHeight * 0.67,
                        decoration: BoxDecoration(
                            color: Colors.transparent,
                            borderRadius:
                                BorderRadius.circular(dispWidth * borderRadius),
                            border: Border.all(
                                width: dispWidth * boardLine,
                                color: Colors.white)),
                        child: Column(
                          mainAxisAlignment: MainAxisAlignment.spaceAround,
                          children: <Widget>[
                            OutProcess(1),
                            OutProcess(2),
                            OutProcess(3),
                            OutProcess(4),
                            OutProcess(5),
                            OutProcess(6),
                          ],
                        ),
                      ),
                    ],
                  ),
                  SizedBox(
                    height: dispHeight * 0.2,
                  ),
                  if (liftFlag && netFlag)
                    Container(
                      decoration: const BoxDecoration(
                        image: DecorationImage(
                          image: AssetImage('assets/images/motor_home.png'),
                        ),
                      ),
                      child: Stack(
                        children: <Widget>[
                          Row(
                            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                            children: [
                              Column(
                                mainAxisAlignment:
                                    MainAxisAlignment.spaceAround,
                                crossAxisAlignment: CrossAxisAlignment.center,
                                children: [
                                  Column(
                                    children: [
                                      SizedBox(
                                        height: dispHeight * 0.25,
                                      ),
                                      GestureDetector(
                                        child: InkWell(
                                          child: Icon(
                                            Icons.arrow_circle_up,
                                            size: mobile
                                                ? dispWidth * 0.25
                                                : dispWidth * 0.1,
                                            color: leg1UpFlag
                                                ? Colors.red
                                                : Colors.black54,
                                          ),
                                          onTap: () {
                                            setState(() {
                                              if (!allUpFlag && !allDownFlag) {
                                                if (leg1DownFlag) {
                                                  leg1DownFlag = false;
                                                  advancedPlayer.play(AssetSource(
                                                      'sound/switchOff.mp3'));
                                                  server.getReq(
                                                      "L1D", "리프트 1 하강정지", 0);
                                                } else {
                                                  leg1UpFlag = !leg1UpFlag;
                                                  leg1UpFlag
                                                      ? server.getReq("L1U",
                                                          "리프트 1 상승시작", 1)
                                                      : server.getReq("L1U",
                                                          "리프트 1 상승정지", 0);
                                                  leg1UpFlag
                                                      ? advancedPlayer.play(
                                                          AssetSource(
                                                              'sound/switchOn.mp3'))
                                                      : advancedPlayer.play(
                                                          AssetSource(
                                                              'sound/switchOff.mp3'));
                                                  leg1UpFlag
                                                      ? print(
                                                          'press LEG 1 UP ON')
                                                      : print(
                                                          'press LEG 1 UP OFF');
                                                }
                                              }
                                            });
                                          },
                                        ),
                                      ),
                                      SizedBox(
                                        height: dispHeight * 0.03,
                                      ),
                                      GestureDetector(
                                        child: InkWell(
                                          child: Icon(
                                            Icons.arrow_circle_down,
                                            size: mobile
                                                ? dispWidth * 0.25
                                                : dispWidth * 0.1,
                                            color: leg1DownFlag
                                                ? Colors.red
                                                : Colors.black54,
                                          ),
                                          onTap: () {
                                            setState(() {
                                              if (!allUpFlag && !allDownFlag) {
                                                if (leg1UpFlag) {
                                                  leg1UpFlag = false;
                                                  advancedPlayer.play(AssetSource(
                                                      'sound/switchOff.mp3'));
                                                  server.getReq(
                                                      "L1U", "리프트 1 상승정지", 0);
                                                } else {
                                                  leg1DownFlag = !leg1DownFlag;
                                                  leg1DownFlag
                                                      ? server.getReq("L1D",
                                                          "리프트 1 하강시작", 1)
                                                      : server.getReq("L1D",
                                                          "리프트 1 하강정지", 0);
                                                  leg1DownFlag
                                                      ? advancedPlayer.play(
                                                          AssetSource(
                                                              'sound/switchOn.mp3'))
                                                      : advancedPlayer.play(
                                                          AssetSource(
                                                              'sound/switchOff.mp3'));
                                                  leg1DownFlag
                                                      ? print(
                                                          'press LEG 1 Down ON')
                                                      : print(
                                                          'press LEG 1 Down OFF');
                                                }
                                              }
                                            });
                                          },
                                        ),
                                      ),
                                    ],
                                  ),
                                  SizedBox(
                                    height: dispHeight * 0.05,
                                  ),
                                  Column(
                                    children: [
                                      GestureDetector(
                                        child: InkWell(
                                          child: Icon(
                                            Icons.arrow_circle_up,
                                            size: mobile
                                                ? dispWidth * 0.25
                                                : dispWidth * 0.1,
                                            color: leg3UpFlag
                                                ? Colors.red
                                                : Colors.black54,
                                          ),
                                          onTap: () {
                                            setState(() {
                                              if (!allUpFlag && !allDownFlag) {
                                                if (leg3DownFlag) {
                                                  leg3DownFlag = false;
                                                  advancedPlayer.play(AssetSource(
                                                      'sound/switchOff.mp3'));
                                                  server.getReq(
                                                      "L3D", "리프트 3 하강정지", 0);
                                                } else {
                                                  leg3UpFlag = !leg3UpFlag;
                                                  leg3UpFlag
                                                      ? server.getReq("L3U",
                                                          "리프트 3 상승시작", 1)
                                                      : server.getReq("L3U",
                                                          "리프트 3 상승정지", 0);
                                                  leg3UpFlag
                                                      ? advancedPlayer.play(
                                                          AssetSource(
                                                              'sound/switchOnff.mp3'))
                                                      : advancedPlayer.play(
                                                          AssetSource(
                                                              'sound/switchOff.mp3'));
                                                  leg3UpFlag
                                                      ? print(
                                                          'press LEG 3 UP ON')
                                                      : print(
                                                          'press LEG 3 UP OFF');
                                                }
                                              }
                                            });
                                          },
                                        ),
                                      ),
                                      SizedBox(
                                        height: dispHeight * 0.03,
                                      ),
                                      GestureDetector(
                                        child: InkWell(
                                          child: Icon(
                                            Icons.arrow_circle_down,
                                            size: mobile
                                                ? dispWidth * 0.25
                                                : dispWidth * 0.1,
                                            // size: dispWidth * 0.25,
                                            color: leg3DownFlag
                                                ? Colors.red
                                                : Colors.black54,
                                          ),
                                          onTap: () {
                                            setState(() {
                                              if (!allUpFlag && !allDownFlag) {
                                                if (leg3UpFlag) {
                                                  leg3UpFlag = false;
                                                  advancedPlayer.play(AssetSource(
                                                      'sound/switchOff.mp3'));
                                                  server.getReq(
                                                      "L3U", "리프트 3 상승정지", 0);
                                                } else {
                                                  leg3DownFlag = !leg3DownFlag;
                                                  leg3DownFlag
                                                      ? server.getReq("L3D",
                                                          "리프트 3 하강시작", 1)
                                                      : server.getReq("L3D",
                                                          "리프트 3 하강정지", 0);
                                                  leg3DownFlag
                                                      ? advancedPlayer.play(
                                                          AssetSource(
                                                              'sound/switchOn.mp3'))
                                                      : advancedPlayer.play(
                                                          AssetSource(
                                                              'sound/switchOff.mp3'));
                                                  leg3DownFlag
                                                      ? print(
                                                          'press LEG 3 Down ON')
                                                      : print(
                                                          'press LEG 3 Down OFF');
                                                }
                                              }
                                            });
                                          },
                                        ),
                                      ),
                                    ],
                                  ),
                                ],
                              ),
                              Column(
                                mainAxisAlignment:
                                    MainAxisAlignment.spaceAround,
                                crossAxisAlignment: CrossAxisAlignment.center,
                                children: [
                                  Column(
                                    children: [
                                      SizedBox(
                                        height: dispHeight * 0.2,
                                      ),
                                      Text(
                                        "UP",
                                        // "$wifiCount",
                                        style: TextStyle(
                                            color: Colors.red,
                                            fontSize: mobile
                                                ? (dispWidth * 0.1) / txtScale
                                                : (dispWidth * 0.03) / txtScale,
                                            // fontSize: (dispWidth * 0.1) / txtScale,
                                            fontWeight: FontWeight.w800),
                                      ),
                                      GestureDetector(
                                        child: InkWell(
                                          child: Icon(
                                            Icons.arrow_circle_up,
                                            size: mobile
                                                ? dispWidth * 0.25
                                                : dispWidth * 0.1,
                                            color: allUpFlag
                                                ? Colors.red
                                                : Colors.black54,
                                          ),
                                          onTap: () async {
                                            if (allDownFlag) {
                                              allDownFlag = false;
                                              advancedPlayer.play(AssetSource(
                                                  'sound/switchOff.mp3'));
                                              await server.getReq(
                                                  "LAD", "리프트 전체 하강정지", 0);
                                            } else {
                                              setState(() {
                                                allUpFlag = !allUpFlag;
                                                allUpFlag
                                                    ? advancedPlayer.play(
                                                        AssetSource(
                                                            'sound/switchOn.mp3'))
                                                    : advancedPlayer.play(
                                                        AssetSource(
                                                            'sound/switchOff.mp3'));
                                                allUpFlag
                                                    ? server.getReq(
                                                        "LAU", "리프트 전체 상승시작", 1)
                                                    : server.getReq("LAU",
                                                        "리프트 전체 상승정지", 0);
                                                allUpFlag
                                                    ? print(
                                                        'press LEG ALL UP ON')
                                                    : print(
                                                        'press LEG ALL UP OFF');
                                              });
                                            }
                                          },
                                        ),
                                      ),
                                      SizedBox(
                                        height: dispHeight * 0.03,
                                      ),
                                      GestureDetector(
                                        child: InkWell(
                                          child: Icon(
                                            Icons.arrow_circle_down,
                                            size: mobile
                                                ? dispWidth * 0.25
                                                : dispWidth * 0.1,
                                            color: allDownFlag
                                                ? Colors.red
                                                : Colors.black54,
                                          ),
                                          onTap: () async {
                                            if (allUpFlag) {
                                              allUpFlag = false;
                                              advancedPlayer.play(AssetSource(
                                                  'sound/switchOff.mp3'));
                                              await server.getReq(
                                                  "LAU", "리프트 전체 상승정지", 0);
                                            } else {
                                              setState(() {
                                                allDownFlag = !allDownFlag;
                                                allDownFlag
                                                    ? advancedPlayer.play(
                                                        AssetSource(
                                                            'sound/switchOnff.mp3'))
                                                    : advancedPlayer.play(
                                                        AssetSource(
                                                            'sound/switchOff.mp3'));
                                                allDownFlag
                                                    ? server.getReq(
                                                        "LAD", "리프트 전체 하강시작", 1)
                                                    : server.getReq("LAD",
                                                        "리프트 전체 하강정지", 0);
                                                allDownFlag
                                                    ? print(
                                                        'press LEG ALL DOWN ON')
                                                    : print(
                                                        'press LEG ALL DOWN OFF');
                                              });
                                            }
                                          },
                                        ),
                                      ),
                                      Text(
                                        "DOWN",
                                        style: TextStyle(
                                            color: Colors.red,
                                            fontSize: mobile
                                                ? (dispWidth * 0.1) / txtScale
                                                : (dispWidth * 0.03) / txtScale,
                                            fontWeight: FontWeight.w800),
                                      ),
                                    ],
                                  ),
                                ],
                              ),
                              Column(
                                mainAxisAlignment:
                                    MainAxisAlignment.spaceAround,
                                crossAxisAlignment: CrossAxisAlignment.center,
                                children: [
                                  Column(
                                    children: [
                                      SizedBox(
                                        height: dispHeight * 0.25,
                                      ),
                                      GestureDetector(
                                        child: InkWell(
                                          child: Icon(
                                            Icons.arrow_circle_up,
                                            size: mobile
                                                ? dispWidth * 0.25
                                                : dispWidth * 0.1,
                                            color: leg2UpFlag
                                                ? Colors.red
                                                : Colors.black54,
                                          ),
                                          onTap: () async {
                                            setState(() {
                                              if (!allUpFlag && !allDownFlag) {
                                                if (leg2DownFlag) {
                                                  leg2DownFlag = false;
                                                  advancedPlayer.play(AssetSource(
                                                      'sound/switchOff.mp3'));
                                                  server.getReq(
                                                      "L2D", "리프트 2 하강정지", 0);
                                                } else {
                                                  leg2UpFlag = !leg2UpFlag;
                                                  leg2UpFlag
                                                      ? server.getReq("L2U",
                                                          "리프트 2 상승시작", 1)
                                                      : server.getReq("L2U",
                                                          "리프트 2 하강정지", 0);
                                                  leg2UpFlag
                                                      ? advancedPlayer.play(
                                                          AssetSource(
                                                              'sound/switchOn.mp3'))
                                                      : advancedPlayer.play(
                                                          AssetSource(
                                                              'sound/switchOff.mp3'));
                                                  leg2UpFlag
                                                      ? print(
                                                          'press LEG 2 UP ON')
                                                      : print(
                                                          'press LEG 2 UP OFF');
                                                }
                                              }
                                            });
                                          },
                                        ),
                                      ),
                                      SizedBox(
                                        height: dispHeight * 0.03,
                                      ),
                                      GestureDetector(
                                        child: InkWell(
                                          child: Icon(
                                            Icons.arrow_circle_down,
                                            size: mobile
                                                ? dispWidth * 0.25
                                                : dispWidth * 0.1,
                                            color: leg2DownFlag
                                                ? Colors.red
                                                : Colors.black54,
                                          ),
                                          onTap: () async {
                                            setState(() {
                                              if (!allUpFlag && !allDownFlag) {
                                                if (leg2UpFlag) {
                                                  leg2UpFlag = false;
                                                  advancedPlayer.play(AssetSource(
                                                      'sound/switchOff.mp3'));
                                                  server.getReq(
                                                      "L2U", "리프트 2 상승정지", 0);
                                                } else {
                                                  leg2DownFlag = !leg2DownFlag;
                                                  leg2DownFlag
                                                      ? server.getReq("L2D",
                                                          "리프트 2 하강시작", 1)
                                                      : server.getReq("L2D",
                                                          "리프트 2 하강정지", 0);
                                                  leg2DownFlag
                                                      ? advancedPlayer.play(
                                                          AssetSource(
                                                              'sound/switchOn.mp3'))
                                                      : advancedPlayer.play(
                                                          AssetSource(
                                                              'sound/switchOff.mp3'));
                                                  leg2DownFlag
                                                      ? print(
                                                          'press LEG 2 Down ON')
                                                      : print(
                                                          'press LEG 2 Down OFF');
                                                }
                                              }
                                            });
                                          },
                                        ),
                                      ),
                                    ],
                                  ),
                                  SizedBox(
                                    height: dispHeight * 0.05,
                                  ),
                                  Column(
                                    children: [
                                      GestureDetector(
                                        child: InkWell(
                                          child: Icon(
                                            Icons.arrow_circle_up,
                                            size: mobile
                                                ? dispWidth * 0.25
                                                : dispWidth * 0.1,
                                            color: leg4UpFlag
                                                ? Colors.red
                                                : Colors.black54,
                                          ),
                                          onTap: () async {
                                            setState(() {
                                              if (!allUpFlag && !allDownFlag) {
                                                if (leg4DownFlag) {
                                                  leg4DownFlag = false;
                                                  advancedPlayer.play(AssetSource(
                                                      'sound/switchOff.mp3'));
                                                  server.getReq(
                                                      "L4D", "리프트 4 상승정지", 0);
                                                } else {
                                                  leg4UpFlag = !leg4UpFlag;
                                                  leg4UpFlag
                                                      ? server.getReq("L4U",
                                                          "리프트 4 상승시작", 1)
                                                      : server.getReq("L4U",
                                                          "리프트 4 상승정지", 0);
                                                  leg4UpFlag
                                                      ? advancedPlayer.play(
                                                          AssetSource(
                                                              'sound/switchOn.mp3'))
                                                      : advancedPlayer.play(
                                                          AssetSource(
                                                              'sound/switchOff.mp3'));
                                                  leg4UpFlag
                                                      ? print(
                                                          'press LEG 4 UP ON')
                                                      : print(
                                                          'press LEG 4 UP OFF');
                                                }
                                              }
                                            });
                                          },
                                        ),
                                      ),
                                      SizedBox(
                                        height: dispHeight * 0.03,
                                      ),
                                      GestureDetector(
                                        child: InkWell(
                                          child: Icon(
                                            Icons.arrow_circle_down,
                                            size: mobile
                                                ? dispWidth * 0.25
                                                : dispWidth * 0.1,
                                            color: leg4DownFlag
                                                ? Colors.red
                                                : Colors.black54,
                                          ),
                                          onTap: () async {
                                            setState(() {
                                              if (!allUpFlag && !allDownFlag) {
                                                if (leg4UpFlag) {
                                                  leg4UpFlag = false;
                                                  advancedPlayer.play(AssetSource(
                                                      'sound/switchOff.mp3'));
                                                  server.getReq(
                                                      "L4U", "리프트 4 상승정지", 0);
                                                } else {
                                                  leg4DownFlag = !leg4DownFlag;
                                                  leg4DownFlag
                                                      ? server.getReq("L4D",
                                                          "리프트 4 하강시작", 1)
                                                      : server.getReq("L4D",
                                                          "리프트 4 하강정지", 0);
                                                  leg4DownFlag
                                                      ? advancedPlayer.play(
                                                          AssetSource(
                                                              'sound/switchOn.mp3'))
                                                      : advancedPlayer.play(
                                                          AssetSource(
                                                              'sound/switchOff.mp3'));
                                                  leg4DownFlag
                                                      ? print(
                                                          'press LEG 4 Down ON')
                                                      : print(
                                                          'press LEG 4 Down OFF');
                                                }
                                              }
                                            });
                                          },
                                        ),
                                      ),
                                    ],
                                  ),
                                ],
                              ),
                            ],
                          ),
                        ],
                      ),
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
  Future<void> getReq(String division, String name, int chanel) async {
    // return;
    // ignore: prefer_typing_uninitialized_variables
    var response;
    // ignore: prefer_typing_uninitialized_variables
    var parsedJson;

    // s = aPIp;
    s = 'http://192.168.4.1/json?j={"division":';

    Dio dio = Dio();

    if (division == "request") {
      s = "http://192.168.4.1/Request";
      response = await dio.get(s);
      parsedJson = jsonDecode(response.data);

      print("ParsedJson:$parsedJson");

      (parsedJson['MT1Flag'] == 1)
          ? V.motorStatus[1] = true
          : V.motorStatus[1] = false;
      (parsedJson['MT2Flag'] == 1)
          ? V.motorStatus[2] = true
          : V.motorStatus[2] = false;
      (parsedJson['MT3Flag'] == 1)
          ? V.motorStatus[3] = true
          : V.motorStatus[3] = false;

      V.btVolt = parsedJson['aux_v'];
      if (V.btVolt > 13.2) {
        // 전압에 따른 밧데리 색상 표현...
        batColor = const Color(0xFF6B56F5);
      } else if (V.btVolt < 13.19 && V.btVolt > 12.5) {
        batColor = Colors.yellow;
      } else if (V.btVolt < 12.5) {
        batColor = Colors.red;
      }
      V.temperature = parsedJson['Temperature'];
      V.fuel = parsedJson['Fuel'];
      if (V.fuel == 255) {
        V.fuel = 0;
      }

      voltText = '${(V.btVolt).toString()} V';

      if (V.temperature > 90.0 || V.temperature < -60.0) {
        tempText = "--.-";
      } else {
        tempText = '${V.temperature.toString()} ℃ ';
      }

      for (int i = 1; i < 7; i++) {
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

      for (int i = 1; i < 7; i++) {
        if (parsedJson['outOnOff$i'] == 1) {
          statusFlag = true;
        } else {
          statusFlag = false;
        }
        if (V.outStatus[i] != statusFlag) {
          if (parsedJson['outOnOff$i'] == 1) {
            V.outStatus[i] = true;
          } else {
            V.outStatus[i] = false;
          }
        }
      }
      if (parsedJson['invertor'] == 1) {
        statusFlag = true;
      } else {
        statusFlag = false;
      }
      if (V.invertorStatus != statusFlag) {
        if (parsedJson['invertor'] == 1) {
          V.invertorStatus = true;
        } else {
          V.invertorStatus = false;
        }
      }

      for (int i = 1; i < 7; i++) {
        pwm[i] = parsedJson['ledValue$i'];
        if (pwm[i] != 0) V.ledValue[i] = (pwm[i]).roundToDouble();
      }

      V.motorValue[1] = parsedJson['MT1Value'].roundToDouble();
      V.motorValue[2] = parsedJson['MT2Value'].roundToDouble();
      V.motorValue[3] = parsedJson['MT3Value'].roundToDouble();
      if (V.motorValue[1] < 10) V.motorValue[1] = 10;
      if (V.motorValue[2] < 10) V.motorValue[2] = 10;
      if (V.motorValue[3] < 10) V.motorValue[3] = 10;

      // 청수 Sensor 가 없을 때...
      if (parsedJson['Clean'] == 255) {
        // 청수 Sensor 처리..
        V.cleanSensor = false;
        V.cleanWater = 0;
      } else {
        V.cleanSensor = true;
        V.cleanWater = parsedJson['Clean'];
      }

      // 오수 Sensor 가 없을 때...
      if (parsedJson['Waste'] == 255) {
        // 오수 Sensor 처리..
        V.wasteSensor = false;
        V.wasteWater = 0;
      } else {
        V.wasteSensor = true;
        V.wasteWater = parsedJson['Waste'];
      }

      if (parsedJson['clnValve'] == 1) {
        clnValveFlag = true;
      } else {
        clnValveFlag = false;
      }
      if (parsedJson['wstValve'] == 1) {
        wstValveFlag = true;
      } else {
        wstValveFlag = false;
      }

      if (parsedJson['ledAllFlag'] == 1) {
        V.allLedStatus = true;
      } else {
        V.allLedStatus = false;
      }

      if (parsedJson['sleep'] == 1) {
        sleepFlag = true;
        sleepColor = Colors.black;
      } else {
        sleepFlag = false;
        sleepColor = Colors.white;
      }

      (parsedJson['LiftFlag'] == 1) ? liftFlag = true : liftFlag = false;

      if (liftFlag) {
        (parsedJson['L1U_Flag'] == 1) ? leg1UpFlag = true : leg1UpFlag = false;
        (parsedJson['L1D_Flag'] == 1)
            ? leg1DownFlag = true
            : leg1DownFlag = false;
        (parsedJson['L2U_Flag'] == 1) ? leg2UpFlag = true : leg2UpFlag = false;
        (parsedJson['L2D_Flag'] == 1)
            ? leg2DownFlag = true
            : leg2DownFlag = false;
        (parsedJson['L3U_Flag'] == 1) ? leg3UpFlag = true : leg3UpFlag = false;
        (parsedJson['L3D_Flag'] == 1)
            ? leg3DownFlag = true
            : leg3DownFlag = false;
        (parsedJson['L4U_Flag'] == 1) ? leg4UpFlag = true : leg4UpFlag = false;
        (parsedJson['L4D_Flag'] == 1)
            ? leg4DownFlag = true
            : leg4DownFlag = false;
        (parsedJson['LAU_Flag'] == 1) ? allUpFlag = true : allUpFlag = false;
        (parsedJson['LAD_Flag'] == 1)
            ? allDownFlag = true
            : allDownFlag = false;
      }
    } else if (division == "SET_TEMP") {
      s += '"$division","name":"$name","Chanel":$chanel}';
    } else if (division == "ALL") {
      s += '"$division","name":"$name","Value":$chanel}';
    } else if (division == "LED") {
      // LED 값을 "OFF"와 PWM 값으로 분리...
      if (V.ledStatus[chanel] == true) {
        var pwm = V.ledValue[chanel].round().toInt();
        s += '"$division","name":"$name","Chanel":$chanel,"Value":$pwm}';
      } else {
        s += '"$division","name":"$name","Chanel":$chanel,"Value":0}';
      }
    } else if (division == "OUT") {
      (V.outStatus[chanel] == true)
          ? s += '"$division","name":"$name","Chanel":$chanel,"Value":1}'
          : s += '"$division","name":"$name","Chanel":$chanel,"Value":0}';
    } else if (division == "MOTOR") {
      // Heater, Motor 1, Motor 2 처리...
      if (V.motorStatus[chanel] == true) {
        var pwm = V.motorValue[chanel].round().toInt();
        s += '"$division","name":"$name","Chanel":$chanel,"Value":$pwm}';
      } else {
        s += '"$division","name":"$name","Chanel":$chanel,"Value":0}';
      }
    } else if (division == "INVERTOR") {
      (V.invertorStatus == true)
          ? s += '"$division","name":"$name","Value":1}'
          : s += '"$division","name":"$name","Value":0}';
    } else if (division == "sleep") {
      // 취침모드 상태 값 전송...
      s += '"$division","name":"$name","Chanel":$chanel}';
    } else if (division == "clnValve") {
      // 청수밸브 상태 값 전송...
      s += '"$division","name":"$name","Value":$chanel}';
    } else if (division == "wstValve") {
      // 오수밸브 상태 값 전송...
      s += '"$division","name":"$name","Value":$chanel}';
    } else if (division == "L1U") {
      (chanel == 1)
          ? s += '"$division","name":"$name","Chanel":$chanel,"Value":1}'
          : s += '"$division","name":"$name","Chanel":$chanel,"Value":0}';
    } else if (division == "L1D") {
      (chanel == 1)
          ? s += '"$division","name":"$name","Chanel":$chanel,"Value":1}'
          : s += '"$division","name":"$name","Chanel":$chanel,"Value":0}';
    } else if (division == "L2U") {
      (chanel == 1)
          ? s += '"$division","name":"$name","Chanel":$chanel,"Value":1}'
          : s += '"$division","name":"$name","Chanel":$chanel,"Value":0}';
    } else if (division == "L2D") {
      (chanel == 1)
          ? s += '"$division","name":"$name","Chanel":$chanel,"Value":1}'
          : s += '"$division","name":"$name","Chanel":$chanel,"Value":0}';
    } else if (division == "L3U") {
      (chanel == 1)
          ? s += '"$division","name":"$name","Chanel":$chanel,"Value":1}'
          : s += '"$division","name":"$name","Chanel":$chanel,"Value":0}';
    } else if (division == "L3D") {
      (chanel == 1)
          ? s += '"$division","name":"$name","Chanel":$chanel,"Value":1}'
          : s += '"$division","name":"$name","Chanel":$chanel,"Value":0}';
    } else if (division == "L4U") {
      (chanel == 1)
          ? s += '"$division","name":"$name","Chanel":$chanel,"Value":1}'
          : s += '"$division","name":"$name","Chanel":$chanel,"Value":0}';
    } else if (division == "L4D") {
      (chanel == 1)
          ? s += '"$division","name":"$name","Chanel":$chanel,"Value":1}'
          : s += '"$division","name":"$name","Chanel":$chanel,"Value":0}';
    } else if (division == "LAU") {
      (chanel == 1)
          ? s += '"$division","name":"$name","Chanel":$chanel,"Value":1}'
          : s += '"$division","name":"$name","Chanel":$chanel,"Value":0}';
    } else if (division == "LAD") {
      (chanel == 1)
          ? s += '"$division","name":"$name","Chanel":$chanel,"Value":1}'
          : s += '"$division","name":"$name","Chanel":$chanel,"Value":0}';
    } else if (division == "LIFT") {
      (chanel == 1)
          ? s += '"$division","name":"$name","Chanel":$chanel,"Value":1}'
          : s += '"$division","name":"$name","Chanel":$chanel,"Value":0}';
    }
    response = await dio.get(s);
    parsedJson = jsonDecode(response.data);
    var result = parsedJson['result'];
    print("parsedJson result: $result");

    if (response.statusCode == 200) {
      wifiCount = 0;
      print("캠트롤 수신 OK.....................");
      // print("response.statusCode : ${response.statusCode}");
    }

    dio.close();
  }
}
