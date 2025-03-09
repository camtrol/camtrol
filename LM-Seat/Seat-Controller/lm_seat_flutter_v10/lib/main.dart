// ignore_for_file: prefer_const_constructors

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

late Timer timer;

String stationIp = '192.168.0.100'; // Hot Spot Mode 일때 IP Address...
String aPIp = 'http://192.168.4.1/'; // AP Mode 일때 IP Address...
String s = ' ';

List<int> pwm = [0, 0, 0, 0, 0];

late double dispHeight;
late double dispWidth;
late double pixelRatio;
late double txtScale;

bool btnMemoryFlag = false;

bool btn1LeftFlag = false;
bool btn1RightFlag = false;
bool btn2UpFlag = false;
bool btn2DownFlag = false;
bool btn3LeftFlag = false;
bool btn3RightFlag = false;
bool seatHeat1Flag = false;
bool seatHeat2Flag = false;

bool statusFlag = false;
bool lteFlag = false;
bool wifiConFlag = false;
bool networkSelect = false; // true : Hot Spot, False : WiFi
bool qrOkFlag = false; // QR 코드로 정상 접속 되면 true...

bool displayDir = true;

bool netFlag = false;

bool rs485Flag = false;
bool switchState = false;
bool doorState = false;
bool batState = false;

bool invertorStatus = false;
bool tvStatus = false;

Server server = Server();

Color portraitBackColor = const Color(0xff333333);

Color activeColor = Colors.white;
Color inActiveColor = Color.fromARGB(255, 171, 171, 171);

const Color appBarColor = Color(0x552C1FFF); //FF39F1
const Color titleColor = Colors.white;

bool beep = true;

int wifiCount = 10;
int wifiConCount = 0;

int h1LED = 0;
int h2LED = 0;
int h3LED = 0;

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
            name: '/wifichange',
            page: () => WiFiChange(),
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
    print("NET Initial...");
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

      if (wifiCount > 10) {
        netFlag = false;
        invertorStatus = false;
        tvStatus = false;
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

      // print("netFlag : $netFlag");
      // print("V.btVolt : ${(V.btVolt).toString()} V");

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
    print("NET _loadPrefs Page in");
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
    // V.ledName[6] = prefs.getString('led6SaveName') ?? V.ledPushName[6];
    // V.ledName[7] = prefs.getString('led7SaveName') ?? V.ledPushName[7];
    // V.ledName[8] = prefs.getString('led8SaveName') ?? V.ledPushName[8];

    print('ledName[1] : ${V.ledName[1]}');
    print('ledName[2] : ${V.ledName[2]}');
    print('ledName[3] : ${V.ledName[3]}');
    print('ledName[4] : ${V.ledName[4]}');
    print('ledName[5] : ${V.ledName[5]}');
    // print('ledName[6] : ${V.ledName[6]}');
    // print('ledName[7] : ${V.ledName[7]}');
    // print('ledName[8] : ${V.ledName[8]}');

    // networkSelect = prefs.getBool('networkSelect') ?? false;

    // if (networkSelect) {
    // V.hotSpotSSID = (prefs.getString('ssidPush') ?? "CamTrol V6.0-A001");
    // V.hotSpotPW = (prefs.getString('pwPush') ?? "01234567");
    // } else {
    V.conSSID = (prefs.getString('ssidPush') ?? "CamTrol V6.0-A001");
    V.conPW = (prefs.getString('pwPush') ?? "01234567");
    // }
    // print("NET networkSelect : $networkSelect");
    // print("NET SSID : ${V.conSSID}");
    // print("NET PSWD : ${V.conPW}");

    beep = prefs.getBool('beep') ?? true;
    // mobile = prefs.getBool('mobile') ?? true;
    displayDir = prefs.getBool('displayDir') ?? true;

    // isSwitched = prefs.getBool('isSwitched') ?? isSwitched;

    // V.isPhoneTable[1] = prefs.getBool('isPhoneTableFlag1') ?? true;
    // V.isPhoneTable[2] = prefs.getBool('isPhoneTableFlag2') ?? false;
    // V.isPhoneTable[3] = prefs.getBool('isPhoneTableFlag3') ?? false;
    // V.isPhoneTable[4] = prefs.getBool('isPhoneTableFlag4') ?? false;
    // V.isPhoneTable[5] = prefs.getBool('isPhoneTableFlag5') ?? false;

    // V.phoneName[1] = prefs.getString('phoneName1') ?? '연락처 1 입력';
    // V.phoneName[2] = prefs.getString('phoneName2') ?? '연락처 2 입력';
    // V.phoneName[3] = prefs.getString('phoneName3') ?? '연락처 3 입력';
    // V.phoneName[4] = prefs.getString('phoneName4') ?? '연락처 4 입력';
    // V.phoneName[5] = prefs.getString('phoneName5') ?? '연락처 5 입력';

    // V.phoneNumber[1] = prefs.getString('phoneNumber1') ?? '전화번호 입력';
    // V.phoneNumber[2] = prefs.getString('phoneNumber2') ?? '전화번호 입력';
    // V.phoneNumber[3] = prefs.getString('phoneNumber3') ?? '전화번호 입력';
    // V.phoneNumber[4] = prefs.getString('phoneNumber4') ?? '전화번호 입력';
    // V.phoneNumber[5] = prefs.getString('phoneNumber5') ?? '전화번호 입력';
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

    // if (pixelRatio > 2.3) {
    //   mobile = true;
    // } else {
    //   mobile = false;
    // }

    SystemChrome.setPreferredOrientations([DeviceOrientation.portraitUp]);

    // mobile
    //     ? SystemChrome.setPreferredOrientations([DeviceOrientation.portraitUp])
    //     : displayDir
    //         ? SystemChrome.setPreferredOrientations(
    //             [DeviceOrientation.landscapeRight])
    //         : SystemChrome.setPreferredOrientations(
    //             [DeviceOrientation.landscapeLeft]);

    return PortScreen();
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
          // mobile
          //     ? (dispHeight * appBarHeight) * 0.6
          //     : (dispHeight * appBarHeight),
          (dispHeight * appBarHeight) * 0.6,
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
              // fontSize: mobile
              //     ? ((dispWidth * 0.03) / txtScale)
              //     : ((dispHeight * 0.04) / txtScale),
              fontSize: ((dispWidth * 0.03) / txtScale),
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
                    // fontSize: mobile
                    //     ? (dispHeight * 0.03) / txtScale
                    //     : (dispHeight * 0.03) / txtScale,
                    fontSize: (dispHeight * 0.03) / txtScale,
                  ))
              : Text('QR 코드를 스캔 해 주세요.',
                  style: TextStyle(
                    color: Colors.black,
                    fontWeight: FontWeight.bold,
                    // fontSize: mobile
                    //     ? (dispHeight * 0.03) / txtScale
                    //     : (dispHeight * 0.04) / txtScale,
                    fontSize: (dispHeight * 0.03) / txtScale,
                  )),
          SizedBox(
            // width: mobile ? dispWidth * 0.35 : dispWidth * 0.15,
            // height: mobile ? dispHeight * 0.05 : dispHeight * 0.06,
            width: dispWidth * 0.35,
            height: dispHeight * 0.05,
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
                      // fontSize: mobile
                      //     ? (dispWidth * 0.04) / txtScale
                      //     : (dispWidth * 0.02) / txtScale,
                      fontSize: (dispWidth * 0.04) / txtScale,
                    ),
                  );
                },
              ),
            ),
          ),
          SizedBox(
            // width: mobile ? dispWidth * 0.35 : dispWidth * 0.1,
            // height: mobile ? dispHeight * 0.05 : dispHeight * 0.06,
            width: dispWidth * 0.35,
            height: dispHeight * 0.05,
            child: ElevatedButton(
                child: Text(
                  "저장",
                  style: TextStyle(
                    color: Colors.black,
                    fontWeight: FontWeight.bold,
                    // fontSize: mobile
                    //     ? (dispWidth * 0.04) / txtScale
                    //     : (dispWidth * 0.02) / txtScale,
                    fontSize: (dispWidth * 0.04) / txtScale,
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
                          // fontSize: mobile
                          //     ? (dispWidth * 0.045) / txtScale
                          //     : (dispHeight * 0.035) / txtScale,
                          fontSize: (dispWidth * 0.045) / txtScale,
                        ),
                      ),
                      actions: <Widget>[
                        // ignore: deprecated_member_use
                        SizedBox(
                          // width: mobile ? dispWidth * 0.20 : dispWidth * 0.09,
                          // height:
                          //     mobile ? dispHeight * 0.045 : dispHeight * 0.07,
                          width: dispWidth * 0.20,
                          height: dispHeight * 0.045,
                          child: ElevatedButton(
                              child: Text(
                                "예",
                                style: TextStyle(
                                  color: Colors.black,
                                  fontWeight: FontWeight.bold,
                                  // fontSize: mobile
                                  //     ? (dispWidth * 0.04) / txtScale
                                  //     : (dispHeight * 0.03) / txtScale,
                                  fontSize: (dispWidth * 0.04) / txtScale,
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
                                    // print("NET networkSelect : $networkSelect");
                                    // print("NET SSID : ${convertString[3]}");
                                    // print("NET PSWD : ${convertString[4]}");
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
                                // mobile ? dispHeight * 0.04 : dispWidth * 0.04),
                                dispHeight * 0.04),
                        // ignore: deprecated_member_use
                        SizedBox(
                          // width: mobile ? dispWidth * 0.25 : dispWidth * 0.13,
                          width: dispWidth * 0.25,
                          height:
                              // mobile ? dispHeight * 0.045 : dispHeight * 0.07,
                              dispHeight * 0.045,
                          child: ElevatedButton(
                              child: Text(
                                "아니요",
                                style: TextStyle(
                                  color: Colors.black,
                                  fontWeight: FontWeight.bold,
                                  // fontSize: mobile
                                  //     ? (dispWidth * 0.04) / txtScale
                                  //     : (dispHeight * 0.03) / txtScale,
                                  fontSize: (dispWidth * 0.04) / txtScale,
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
          // mobile
          //     ? (dispHeight * appBarHeight) * 0.6
          //     : (dispHeight * appBarHeight),
          (dispHeight * appBarHeight) * 0.6,
        ),
        child: AppBar(
          backgroundColor: appBarColor,
          title: Text(
            'WiFi 네트워크 변경...',
            style: TextStyle(
              color: titleColor,
              fontWeight: FontWeight.bold,
              // fontSize: mobile
              //     ? (dispHeight * 0.02) / txtScale
              //     : (dispHeight * 0.04) / txtScale,
              fontSize: (dispHeight * 0.02) / txtScale,
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
                  // height: (mobile ? dispHeight : dispWidth) * 0.01,
                  height: dispHeight * 0.01,
                ),
                Container(
                  width: dispWidth * 0.85,
                  decoration: BoxDecoration(
                    borderRadius:
                        BorderRadius.circular(dispHeight * borderRadius),
                    border: Border.all(
                        width: dispWidth * boardLine,
                        color: Colors.black), //Container 테두리 값, 색상
                  ),
                  child: Column(
                    children: <Widget>[
                      SizedBox(
                        // height: mobile ? dispHeight * 0.03 : dispWidth * 0.03,
                        height: dispHeight * 0.03,
                      ),
                      SizedBox(
                        // width: mobile ? dispWidth * 0.6 : dispWidth * 0.3,
                        // height: mobile ? dispHeight * 0.07 : dispWidth * 0.045,
                        width: dispWidth * 0.6,
                        height: dispHeight * 0.07,
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
                            // fontSize: mobile
                            //     ? (dispWidth * 0.05) / txtScale
                            //     : (dispHeight * 0.035) / txtScale,
                            fontSize: (dispWidth * 0.05) / txtScale,
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
                          // fontSize: mobile
                          //     ? (dispWidth * 0.04) / txtScale
                          //     : (dispHeight * 0.035) / txtScale,
                          fontSize: (dispWidth * 0.04) / txtScale,
                        ),
                      ),
                      SizedBox(
                          // height: mobile
                          //     ? (dispHeight * 0.03) / 2
                          //     : (dispWidth * 0.04) / 2)
                          height: (dispHeight * 0.03) / 2),
                      SizedBox(
                        // height: mobile
                        //     ? (dispHeight * 0.03) / 2
                        //     : (dispWidth * 0.04) / 2,
                        height: (dispHeight * 0.03) / 2,
                      ),
                      SizedBox(
                        // width: mobile ? dispWidth * 0.6 : dispWidth * 0.3,
                        // height: mobile ? dispHeight * 0.07 : dispWidth * 0.045,
                        width: dispWidth * 0.6,
                        height: dispHeight * 0.07,
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
                            // fontSize: mobile
                            //     ? (dispWidth * 0.05) / txtScale
                            //     : (dispHeight * 0.035) / txtScale,
                            fontSize: (dispWidth * 0.05) / txtScale,
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
                          // fontSize: mobile
                          //     ? (dispWidth * 0.04) / txtScale
                          //     : (dispHeight * 0.035) / txtScale,
                          fontSize: (dispWidth * 0.04) / txtScale,
                        ),
                      ),
                      SizedBox(
                        // height: mobile ? dispHeight * 0.05 : dispWidth * 0.03,
                        height: dispHeight * 0.05,
                      ),
                      IconButton(
                        icon: Icon(Icons.qr_code_scanner,
                            // size: mobile ? dispWidth * 0.3 : dispHeight * 0.2,
                            size: dispWidth * 0.3,
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
                      networkSelect
                          ? SizedBox(
                              width: dispWidth * 0.6,
                              height: dispHeight * 0.07,
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
                                  fontSize: (dispWidth * 0.05) / txtScale,
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
                        // height: mobile ? dispHeight * 0.05 : dispWidth * 0.03,
                        height: dispHeight * 0.05,
                      ),
                      SizedBox(
                        child: Row(
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: <Widget>[
                            SizedBox(
                              width:
                                  // mobile ? dispWidth * 0.21 : dispWidth * 0.09,
                                  dispWidth * 0.21,
                              // height: mobile
                              //     ? dispHeight * 0.045
                              //     : dispHeight * 0.07,
                              height: dispHeight * 0.045,
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
                                    // fontSize: mobile
                                    //     ? (dispWidth * 0.035) / txtScale
                                    //     : (dispHeight * 0.03) / txtScale,
                                    fontSize: (dispWidth * 0.035) / txtScale,
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
                                          // fontSize: mobile
                                          //     ? (dispWidth * 0.045) / txtScale
                                          //     : (dispHeight * 0.035) / txtScale,
                                          fontSize:
                                              (dispWidth * 0.045) / txtScale,
                                        ),
                                      ),
                                      actions: <Widget>[
                                        // ignore: deprecated_member_use
                                        SizedBox(
                                          // width: mobile
                                          //     ? dispWidth * 0.20
                                          //     : dispWidth * 0.09,
                                          width: dispWidth * 0.20,
                                          // height: mobile
                                          //     ? dispHeight * 0.045
                                          //     : dispHeight * 0.07,
                                          height: dispHeight * 0.045,
                                          child: ElevatedButton(
                                              child: Text(
                                                "예",
                                                style: TextStyle(
                                                  color: Colors.black,
                                                  fontWeight: FontWeight.bold,
                                                  // fontSize: mobile
                                                  //     ? (dispWidth * 0.04) /
                                                  //         txtScale
                                                  //     : (dispHeight * 0.03) /
                                                  //         txtScale,
                                                  fontSize: (dispWidth * 0.04) /
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
                                            // width: mobile
                                            //     ? dispHeight * 0.04
                                            //     : dispWidth * 0.04),
                                            width: dispHeight * 0.04),
                                        // ignore: deprecated_member_use
                                        SizedBox(
                                          // width: mobile
                                          //     ? dispWidth * 0.25
                                          //     : dispWidth * 0.13,
                                          width: dispWidth * 0.25,
                                          // height: mobile
                                          //     ? dispHeight * 0.045
                                          //     : dispHeight * 0.07,
                                          height: dispHeight * 0.045,
                                          child: ElevatedButton(
                                              child: Text(
                                                "아니요",
                                                style: TextStyle(
                                                  color: Colors.black,
                                                  fontWeight: FontWeight.bold,
                                                  // fontSize: mobile
                                                  //     ? (dispWidth * 0.04) /
                                                  //         txtScale
                                                  //     : (dispHeight * 0.03) /
                                                  //         txtScale,
                                                  fontSize: (dispWidth * 0.04) /
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
                                // width: mobile
                                //     ? dispHeight * 0.04
                                //     : dispWidth * 0.04),
                                width: dispHeight * 0.04),
                            SizedBox(
                              width:
                                  // mobile ? dispWidth * 0.21 : dispWidth * 0.09,
                                  dispWidth * 0.21,
                              // height: mobile
                              //     ? dispHeight * 0.045
                              //     : dispHeight * 0.07,
                              height: dispHeight * 0.045,
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
                                    // fontSize: mobile
                                    //     ? (dispWidth * 0.035) / txtScale
                                    //     : (dispHeight * 0.03) / txtScale,
                                    fontSize: (dispWidth * 0.035) / txtScale,
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
                              // mobile ? dispHeight * 0.05 : dispWidth * 0.03),
                              dispHeight * 0.05),
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
            // width: mobile ? dispWidth * 0.08 : dispWidth * 0.04, // 고정...
            // width: dispWidth * 0.08, // 고정...
            // height: mobile ? 30 : dispWidth * 0.035, // 고정...
            height: 30, // 고정...
            child: Image.asset(
              'assets/images/LM-Logo.png',
              // color: activeColor,
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
                  // height: mobile ? dispHeight * 0.015 : dispHeight * 0.0,
                  height: dispHeight * 0.015,
                ),
              ],
            ),
          ),
        ],
      ),
      actions: <Widget>[
        const SizedBox.shrink(),
        // mobile ? const SizedBox.shrink() : SizedBox(width: dispWidth * 0.1),
        // const SizedBox.shrink(),
        (rs485Flag && netFlag)
            ? SizedBox(
                width: dispWidth * 0.06,
                height: dispHeight * 0.05,
                child: Image.asset(
                  'assets/images/network.png',
                  // color: activeColor,
                ),
              )
            : SizedBox.shrink(),
        SizedBox(width: dispWidth * 0.04),
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
                  // size: mobile ? dispWidth * 0.06 : dispHeight * 0.05,
                  size: dispWidth * 0.06,
                  color: netFlag ? activeColor : inActiveColor)
              : Icon(Icons.wifi_off,
                  // size: mobile ? dispWidth * 0.06 : dispHeight * 0.05,
                  size: dispWidth * 0.06,
                  color: netFlag ? activeColor : Colors.amber),
        ),
        // SizedBox(width: mobile ? dispWidth * 0.04 : dispWidth * 0.03),
        SizedBox(width: dispWidth * 0.04),
        // IconButton(
        //   icon: Icon(Icons.change_circle_outlined,
        //       size: mobile ? dispWidth * 0.06 : dispHeight * 0.05,
        //       color: netFlag ? activeColor : inActiveColor),
        //   tooltip: '설정',
        //   onPressed: () async {
        //     if (beep) {
        //       await advancedPlayer.play(AssetSource('sound/iconClick.mp3'));
        //     }
        //     await Get.toNamed("/setting");
        //   },
        // ),
        SizedBox(
            // width: mobile
            //     ? (pixelRatio > 1.5)
            //         ? dispWidth * 0.015
            //         : dispWidth * 0.03
            //     : (pixelRatio > 1.5)
            //         ? dispWidth * 0.001
            //         : dispWidth * 0.03),
            width: (pixelRatio > 1.5) ? dispWidth * 0.015 : dispWidth * 0.03),
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
        backgroundColor: Colors.transparent,
        appBar: PreferredSize(
          preferredSize: Size.fromHeight((dispHeight * appBarHeight) * 0.65),
          child: AppBarScreen(), // 공통으로 적용...
        ),
        body: SingleChildScrollView(
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: <Widget>[
              SizedBox(
                height: dispHeight * 0.01,
              ),
              Container(
                height: dispHeight * 0.18,
                decoration: BoxDecoration(
                    borderRadius:
                        BorderRadius.circular(dispHeight * borderRadius),
                    border: Border.all(
                        width: dispHeight * 0.0015,
                        color: netFlag ? Colors.white : Colors.white54)),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                  // ignore: prefer_const_literals_to_create_immutables
                  children: <Widget>[
                    Btn1Left(),
                    Btn1Right(),
                  ],
                ),
              ),
              SizedBox(
                height: dispHeight * 0.015,
              ),
              Container(
                // 플로어등, TV Button...
                height: dispHeight * 0.18,
                decoration: BoxDecoration(
                    borderRadius:
                        BorderRadius.circular(dispHeight * borderRadius),
                    border: Border.all(
                        width: dispHeight * 0.0015,
                        color: netFlag ? Colors.white : Colors.white54)),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                  // ignore: prefer_const_literals_to_create_immutables
                  children: <Widget>[
                    Btn2Up(),
                    Btn2Down(),
                  ],
                ),
              ),
              SizedBox(
                height: dispHeight * 0.015,
              ),
              Container(
                // 플로어등, TV Button...
                height: dispHeight * 0.18,
                decoration: BoxDecoration(
                    borderRadius:
                        BorderRadius.circular(dispHeight * borderRadius),
                    border: Border.all(
                        width: dispHeight * 0.0015,
                        color: netFlag ? Colors.white : Colors.white54)),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                  // ignore: prefer_const_literals_to_create_immutables
                  children: <Widget>[
                    Btn3Up(),
                    Btn3Down(),
                  ],
                ),
              ),
              SizedBox(
                height: dispHeight * 0.015,
              ),
              Container(
                // 메모리 Button...
                height: dispHeight * 0.1,
                decoration: BoxDecoration(
                    borderRadius:
                        BorderRadius.circular(dispHeight * borderRadius),
                    border: Border.all(
                        width: dispHeight * 0.0015,
                        color: netFlag ? Colors.white : Colors.white54)),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                  // ignore: prefer_const_literals_to_create_immutables
                  children: <Widget>[
                    ElevatedButton(
                      style: ElevatedButton.styleFrom(
                        backgroundColor: btnMemoryFlag
                            ? Color.fromARGB(255, 222, 60, 15)
                            : Colors.blue,
                        surfaceTintColor: Colors.blue,
                        foregroundColor: Colors.white,
                        shape: RoundedRectangleBorder(
                          borderRadius: BorderRadius.circular(12),
                        ),
                      ),
                      onPressed: () {
                        btnMemoryFlag = btnMemoryFlag ? false : true;
                      },
                      child:
                          // btnMemoryFlag ? const Text("저장") : const Text("호출"),
                          const Text("SAVE"),
                    ),
                    ElevatedButton(
                      style: ElevatedButton.styleFrom(
                        backgroundColor: Colors.blue,
                        surfaceTintColor: Colors.blue,
                        foregroundColor: Colors.white,
                        shape: RoundedRectangleBorder(
                          borderRadius: BorderRadius.circular(12),
                        ),
                      ),
                      onPressed: () {
                        if (btnMemoryFlag) {
                          server.getReq("MEMORY1", 1);
                        } else {
                          server.getReq("CALL1", 1);
                        }
                        btnMemoryFlag = false;
                      },
                      child: const Text("1"),
                    ),
                    ElevatedButton(
                      style: ElevatedButton.styleFrom(
                        backgroundColor: Colors.blue,
                        surfaceTintColor: Colors.blue,
                        foregroundColor: Colors.white,
                        shape: RoundedRectangleBorder(
                          borderRadius: BorderRadius.circular(12),
                        ),
                      ),
                      onPressed: () {
                        if (btnMemoryFlag) {
                          server.getReq("MEMORY2", 1);
                        } else {
                          server.getReq("CALL2", 1);
                        }
                        btnMemoryFlag = false;
                      },
                      child: const Text("2"),
                    ),
                  ],
                ),
              ),
              SizedBox(
                height: dispHeight * 0.015,
              ),
              Container(
                // 플로어등, TV Button...
                height: dispHeight * 0.2,
                decoration: BoxDecoration(
                    borderRadius:
                        BorderRadius.circular(dispHeight * borderRadius),
                    border: Border.all(
                        width: dispHeight * 0.0015,
                        color: netFlag ? Colors.white : Colors.white54)),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceAround,
                  // ignore: prefer_const_literals_to_create_immutables
                  children: <Widget>[
                    SeatHeater1(),
                    SeatHeater2(),
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

class Btn1Left extends StatefulWidget {
  const Btn1Left({Key? key}) : super(key: key);

  @override
  State<Btn1Left> createState() => _Btn1LeftState();
}

class _Btn1LeftState extends State<Btn1Left> {
  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        color: Colors.transparent,
        child: AnimatedContainer(
          width: dispWidth * 0.25,
          height: dispHeight * 0.13,
          decoration: BoxDecoration(
              color: const Color(0xff292d32),
              borderRadius:
                  BorderRadius.circular((dispWidth * buttonRadius * 3)),
              gradient: const LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [
                  Color.fromARGB(255, 74, 78, 83),
                  Color(0xff292d32),
                ],
              ),
              boxShadow: btn1LeftFlag
                  ? [
                      BoxShadow(
                        color: Colors.amber,
                        offset:
                            // mobile ? Offset(-2.0, -2.0) : Offset(-4.0, -4.0),
                            Offset(-2.0, -2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(2.0, 2.0) : Offset(4.0, 4.0),
                        offset: Offset(2.0, 2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(2.0, -2.0) : Offset(4.0, -4.0),
                        offset: Offset(2.0, -2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(-2.0, 2.0) : Offset(-4.0, 4.0),
                        offset: Offset(-2.0, 2.0),
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
          child: Container(
              margin: const EdgeInsets.symmetric(horizontal: 10),
              child: Image.asset('assets/images/seat-77.png')),
        ),
      ),
      // onTapUp: (onTapUp) async {
      onTapCancel: () async {
        print("onTapUp...");
        btn1LeftFlag = false;
        await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
        server.getReq("BTN_1_LEFT", 0);
        setState(() {});
      },
      onTapUp: (onTapUp) async {
        print("onTapUp...");
        btn1LeftFlag = false;
        await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
        server.getReq("BTN_1_LEFT", 0);
        setState(() {});
      },
      onTapDown: (onTapDown) async {
        print("onTapDown...");
        btn1LeftFlag = true;
        await advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
        server.getReq("BTN_1_LEFT", 1);
        setState(() {});
      },
    );
  }
}

class Btn1Right extends StatefulWidget {
  const Btn1Right({Key? key}) : super(key: key);

  @override
  State<Btn1Right> createState() => _Btn1RightState();
}

class _Btn1RightState extends State<Btn1Right> {
  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        color: Colors.transparent,
        child: AnimatedContainer(
          width: dispWidth * 0.25,
          height: dispHeight * 0.13,
          decoration: BoxDecoration(
              color: const Color(0xff292d32),
              borderRadius:
                  BorderRadius.circular((dispWidth * buttonRadius * 3)),
              gradient: const LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [
                  Color(0xff292d32),
                  Color.fromARGB(255, 74, 78, 83),
                ],
              ),
              boxShadow: btn1RightFlag
                  ? [
                      BoxShadow(
                        color: Colors.amber,
                        offset:
                            // mobile ? Offset(-2.0, -2.0) : Offset(-4.0, -4.0),
                            Offset(-2.0, -2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(2.0, 2.0) : Offset(4.0, 4.0),
                        offset: Offset(2.0, 2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(2.0, -2.0) : Offset(4.0, -4.0),
                        offset: Offset(2.0, -2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(-2.0, 2.0) : Offset(-4.0, 4.0),
                        offset: Offset(-2.0, 2.0),
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
          child: Container(
              margin: const EdgeInsets.symmetric(horizontal: 10),
              child: Image.asset('assets/images/seat-74.png')),
        ),
      ),
      onTapCancel: () async {
        print("onTapUp...");
        btn1RightFlag = false;
        await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
        server.getReq("BTN_1_RIGHT", 0);
        setState(() {});
      },
      onTapUp: (onTapUp) async {
        print("onTapUp...");
        btn1RightFlag = false;
        await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
        server.getReq("BTN_1_RIGHT", 0);
        setState(() {});
      },
      onTapDown: (onTapDown) async {
        print("onTapDown...");
        btn1RightFlag = true;
        await advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
        server.getReq("BTN_1_RIGHT", 1);
        setState(() {});
      },
    );
  }
}

class Btn2Up extends StatefulWidget {
  const Btn2Up({Key? key}) : super(key: key);

  @override
  State<Btn2Up> createState() => _Btn2UpState();
}

class _Btn2UpState extends State<Btn2Up> {
  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        color: Colors.transparent,
        child: AnimatedContainer(
          width: dispWidth * 0.25,
          height: dispHeight * 0.13,
          decoration: BoxDecoration(
              color: const Color(0xff292d32),
              borderRadius:
                  BorderRadius.circular((dispWidth * buttonRadius * 3)),
              gradient: const LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [
                  Color.fromARGB(255, 74, 78, 83),
                  Color(0xff292d32),
                ],
              ),
              boxShadow: btn2UpFlag
                  ? [
                      BoxShadow(
                        color: Colors.amber,
                        offset:
                            // mobile ? Offset(-2.0, -2.0) : Offset(-4.0, -4.0),
                            Offset(-2.0, -2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(2.0, 2.0) : Offset(4.0, 4.0),
                        offset: Offset(2.0, 2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(2.0, -2.0) : Offset(4.0, -4.0),
                        offset: Offset(2.0, -2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(-2.0, 2.0) : Offset(-4.0, 4.0),
                        offset: Offset(-2.0, 2.0),
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
          child: Container(
              margin: const EdgeInsets.symmetric(horizontal: 10),
              child: Image.asset('assets/images/seat-75.png')),
        ),
      ),
      onTapCancel: () async {
        print("onTap Cancle...");
        btn2UpFlag = false;
        await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
        server.getReq("BTN_3_LEFT", 0);
        setState(() {});
      },
      onTapUp: (onTapUp) async {
        print("onTap Up...");
        btn2UpFlag = false;
        await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
        server.getReq("BTN_3_LEFT", 0);
        setState(() {});
      },
      onTapDown: (onTapDown) async {
        print("onTap Down...");
        btn2UpFlag = true;
        await advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
        server.getReq("BTN_3_LEFT", 1);
        setState(() {});
      },
    );
  }
}

class Btn2Down extends StatefulWidget {
  const Btn2Down({Key? key}) : super(key: key);

  @override
  State<Btn2Down> createState() => _Btn2DownState();
}

class _Btn2DownState extends State<Btn2Down> {
  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        color: Colors.transparent,
        child: AnimatedContainer(
          width: dispWidth * 0.25,
          height: dispHeight * 0.13,
          decoration: BoxDecoration(
              color: const Color(0xff292d32),
              borderRadius:
                  BorderRadius.circular((dispWidth * buttonRadius * 3)),
              gradient: const LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [
                  Color(0xff292d32),
                  Color.fromARGB(255, 74, 78, 83),
                ],
              ),
              boxShadow: btn2DownFlag
                  ? [
                      BoxShadow(
                        color: Colors.amber,
                        offset:
                            // mobile ? Offset(-2.0, -2.0) : Offset(-4.0, -4.0),
                            Offset(-2.0, -2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(2.0, 2.0) : Offset(4.0, 4.0),
                        offset: Offset(2.0, 2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(2.0, -2.0) : Offset(4.0, -4.0),
                        offset: Offset(2.0, -2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(-2.0, 2.0) : Offset(-4.0, 4.0),
                        offset: Offset(-2.0, 2.0),
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
          child: Container(
              margin: const EdgeInsets.symmetric(horizontal: 10),
              child: Image.asset('assets/images/seat-78.png')),
        ),
      ),
      onTapCancel: () async {
        print("onTapUp...");
        btn2DownFlag = false;
        await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
        server.getReq("BTN_3_RIGHT", 0);
        setState(() {});
      },
      onTapUp: (onTapUp) async {
        print("onTapUp...");
        btn2DownFlag = false;
        await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
        server.getReq("BTN_3_RIGHT", 0);
        setState(() {});
      },
      onTapDown: (onTapDown) async {
        print("onTapDown...");
        btn2DownFlag = true;
        await advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
        server.getReq("BTN_3_RIGHT", 1);
        setState(() {});
      },
    );
  }
}

class Btn3Up extends StatefulWidget {
  const Btn3Up({Key? key}) : super(key: key);

  @override
  State<Btn3Up> createState() => _Btn3UpState();
}

class _Btn3UpState extends State<Btn3Up> {
  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        color: Colors.transparent,
        child: AnimatedContainer(
          width: dispWidth * 0.25,
          height: dispHeight * 0.13,
          decoration: BoxDecoration(
              color: const Color(0xff292d32),
              borderRadius:
                  BorderRadius.circular((dispWidth * buttonRadius * 3)),
              gradient: const LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [
                  Color.fromARGB(255, 74, 78, 83),
                  Color(0xff292d32),
                ],
              ),
              boxShadow: btn3LeftFlag
                  ? [
                      BoxShadow(
                        color: Colors.amber,
                        offset:
                            // mobile ? Offset(-2.0, -2.0) : Offset(-4.0, -4.0),
                            Offset(-2.0, -2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(2.0, 2.0) : Offset(4.0, 4.0),
                        offset: Offset(2.0, 2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(2.0, -2.0) : Offset(4.0, -4.0),
                        offset: Offset(2.0, -2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(-2.0, 2.0) : Offset(-4.0, 4.0),
                        offset: Offset(-2.0, 2.0),
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
          child: Container(
              margin: const EdgeInsets.symmetric(horizontal: 10),
              child: Image.asset('assets/images/seat-79.png')),
        ),
      ),
      onTapCancel: () async {
        print("onTapUp...");
        btn3LeftFlag = false;
        await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
        server.getReq("BTN_2_DOWN", 0);
        setState(() {});
      },
      onTapUp: (onTapUp) async {
        print("onTapUp...");
        btn3LeftFlag = false;
        await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
        server.getReq("BTN_2_DOWN", 0);
        setState(() {});
      },
      onTapDown: (onTapDown) async {
        print("onTapDown...");
        btn3LeftFlag = true;
        await advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
        server.getReq("BTN_2_DOWN", 1);
        setState(() {});
      },
    );
  }
}

class Btn3Down extends StatefulWidget {
  const Btn3Down({Key? key}) : super(key: key);

  @override
  State<Btn3Down> createState() => _Btn3DownState();
}

class _Btn3DownState extends State<Btn3Down> {
  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        color: Colors.transparent,
        child: AnimatedContainer(
          width: dispWidth * 0.25,
          height: dispHeight * 0.13,
          decoration: BoxDecoration(
              color: const Color(0xff292d32),
              borderRadius:
                  BorderRadius.circular((dispWidth * buttonRadius * 3)),
              gradient: const LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [
                  Color(0xff292d32),
                  Color.fromARGB(255, 74, 78, 83),
                ],
              ),
              boxShadow: btn3RightFlag
                  ? [
                      BoxShadow(
                        color: Colors.amber,
                        offset:
                            // mobile ? Offset(-2.0, -2.0) : Offset(-4.0, -4.0),
                            Offset(-2.0, -2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(2.0, 2.0) : Offset(4.0, 4.0),
                        offset: Offset(2.0, 2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(2.0, -2.0) : Offset(4.0, -4.0),
                        offset: Offset(2.0, -2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(-2.0, 2.0) : Offset(-4.0, 4.0),
                        offset: Offset(-2.0, 2.0),
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
          child: Container(
              margin: const EdgeInsets.symmetric(horizontal: 10),
              child: Image.asset('assets/images/seat-76.png')),
        ),
      ),
      onTapCancel: () async {
        print("onTapUp...");
        btn3RightFlag = false;
        await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
        server.getReq("BTN_2_UP", 0);
        setState(() {});
      },
      onTapUp: (onTapUp) async {
        print("onTapUp...");
        btn3RightFlag = false;
        await advancedPlayer.play(AssetSource('sound/switchOff.mp3'));
        server.getReq("BTN_2_UP", 0);
        setState(() {});
      },
      onTapDown: (onTapDown) async {
        print("onTapDown...");
        btn3RightFlag = true;
        await advancedPlayer.play(AssetSource('sound/switchOn.mp3'));
        server.getReq("BTN_2_UP", 1);
        setState(() {});
      },
    );
  }
}

class SeatHeater1 extends StatefulWidget {
  const SeatHeater1({Key? key}) : super(key: key);

  @override
  State<SeatHeater1> createState() => _SeatHeater1State();
}

class _SeatHeater1State extends State<SeatHeater1> {
  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        color: Colors.transparent,
        child: AnimatedContainer(
          width: dispWidth * 0.25,
          height: dispHeight * 0.16,
          decoration: BoxDecoration(
              color: const Color(0xff292d32),
              borderRadius:
                  BorderRadius.circular((dispWidth * buttonRadius * 3)),
              gradient: const LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [
                  Color.fromARGB(255, 74, 78, 83),
                  Color(0xff292d32),
                ],
              ),
              boxShadow: seatHeat1Flag
                  ? [
                      BoxShadow(
                        color: Colors.amber,
                        offset:
                            // mobile ? Offset(-2.0, -2.0) : Offset(-4.0, -4.0),
                            Offset(-2.0, -2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(2.0, 2.0) : Offset(4.0, 4.0),
                        offset: Offset(2.0, 2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(2.0, -2.0) : Offset(4.0, -4.0),
                        offset: Offset(2.0, -2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(-2.0, 2.0) : Offset(-4.0, 4.0),
                        offset: Offset(-2.0, 2.0),
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
          child: Column(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: [
              Container(
                margin: const EdgeInsets.symmetric(horizontal: 15),
                child: Image.asset('assets/images/seat-82.png'),
              ),
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                children: [
                  (h1LED == 7)
                      ? Container(
                          width: dispWidth * 0.025,
                          height: dispHeight * 0.023,
                          color: Color.fromARGB(255, 234, 128, 36),
                        )
                      : Container(
                          width: dispWidth * 0.025,
                          height: dispHeight * 0.023,
                          decoration: BoxDecoration(
                            border: Border.all(
                                width: 1,
                                color: Color.fromARGB(
                                    255, 234, 128, 36)), //Container 테두리 값, 색상
                          ),
                        ),
                  (h1LED == 7) || (h1LED == 5)
                      ? Container(
                          width: dispWidth * 0.025,
                          height: dispHeight * 0.023,
                          color: Color.fromARGB(255, 234, 128, 36),
                        )
                      : Container(
                          width: dispWidth * 0.025,
                          height: dispHeight * 0.023,
                          decoration: BoxDecoration(
                            border: Border.all(
                                width: 1,
                                color: Color.fromARGB(
                                    255, 234, 128, 36)), //Container 테두리 값, 색상
                          ),
                        ),
                  (h1LED == 7) || (h1LED == 5) || (h1LED == 1)
                      ? Container(
                          width: dispWidth * 0.025,
                          height: dispHeight * 0.023,
                          color: Color.fromARGB(255, 234, 128, 36),
                        )
                      : Container(
                          width: dispWidth * 0.025,
                          height: dispHeight * 0.023,
                          decoration: BoxDecoration(
                            border: Border.all(
                                width: 1,
                                color: Color.fromARGB(
                                    255, 234, 128, 36)), //Container 테두리 값, 색상
                          ),
                        ),
                ],
              ),
            ],
          ),
        ),
      ),
      onTapCancel: () {
        seatHeat1Flag = false;
      },
      onTapUp: (onTapUp) {
        seatHeat1Flag = false;
      },
      onTapDown: (onTapDown) {
        seatHeat1Flag = true;
        server.getReq("SEAT_HEATER1", 1);
      },
    );
  }
}

class SeatHeater2 extends StatefulWidget {
  const SeatHeater2({Key? key}) : super(key: key);

  @override
  State<SeatHeater2> createState() => _SeatHeater2State();
}

class _SeatHeater2State extends State<SeatHeater2> {
  @override
  Widget build(BuildContext context) {
    return InkWell(
      child: Container(
        color: Colors.transparent,
        child: AnimatedContainer(
          width: dispWidth * 0.25,
          height: dispHeight * 0.16,
          decoration: BoxDecoration(
              color: const Color(0xff292d32),
              borderRadius:
                  BorderRadius.circular((dispWidth * buttonRadius * 3)),
              gradient: const LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [
                  Color.fromARGB(255, 74, 78, 83),
                  Color(0xff292d32),
                ],
              ),
              boxShadow: seatHeat2Flag
                  ? [
                      BoxShadow(
                        color: Colors.amber,
                        offset:
                            // mobile ? Offset(-2.0, -2.0) : Offset(-4.0, -4.0),
                            Offset(-2.0, -2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(2.0, 2.0) : Offset(4.0, 4.0),
                        offset: Offset(2.0, 2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(2.0, -2.0) : Offset(4.0, -4.0),
                        offset: Offset(2.0, -2.0),
                        blurRadius: 6,
                        spreadRadius: 0.0,
                      ),
                      BoxShadow(
                        color: Colors.amber,
                        // offset: mobile ? Offset(-2.0, 2.0) : Offset(-4.0, 4.0),
                        offset: Offset(-2.0, 2.0),
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
          child: Column(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: [
              Container(
                margin: const EdgeInsets.symmetric(horizontal: 15),
                child: Image.asset('assets/images/seat-82.png'),
              ),
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                children: [
                  (h2LED == 7)
                      ? Container(
                          width: dispWidth * 0.025,
                          height: dispHeight * 0.023,
                          color: Color.fromARGB(255, 234, 128, 36),
                        )
                      : Container(
                          width: dispWidth * 0.025,
                          height: dispHeight * 0.023,
                          decoration: BoxDecoration(
                            border: Border.all(
                                width: 1,
                                color: Color.fromARGB(
                                    255, 234, 128, 36)), //Container 테두리 값, 색상
                          ),
                        ),
                  (h2LED == 7) || (h2LED == 5)
                      ? Container(
                          width: dispWidth * 0.025,
                          height: dispHeight * 0.023,
                          color: Color.fromARGB(255, 234, 128, 36),
                        )
                      : Container(
                          width: dispWidth * 0.025,
                          height: dispHeight * 0.023,
                          decoration: BoxDecoration(
                            border: Border.all(
                                width: 1,
                                color: Color.fromARGB(
                                    255, 234, 128, 36)), //Container 테두리 값, 색상
                          ),
                        ),
                  (h2LED == 7) || (h2LED == 5) || (h2LED == 1)
                      ? Container(
                          width: dispWidth * 0.025,
                          height: dispHeight * 0.023,
                          color: Color.fromARGB(255, 234, 128, 36),
                        )
                      : Container(
                          width: dispWidth * 0.025,
                          height: dispHeight * 0.023,
                          decoration: BoxDecoration(
                            border: Border.all(
                                width: 1,
                                color: Color.fromARGB(
                                    255, 234, 128, 36)), //Container 테두리 값, 색상
                          ),
                        ),
                ],
              ),
            ],
          ),
        ),
      ),
      onTapCancel: () {
        seatHeat2Flag = false;
      },
      onTapUp: (onTapUp) {
        seatHeat2Flag = false;
      },
      onTapDown: (onTapDown) {
        seatHeat2Flag = true;
        server.getReq("SEAT_HEATER2", 1);
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

      if (response.statusCode == 200) {
        wifiCount = 0;
        print("response.statusCode : ${response.statusCode}");
      }

      h1LED = parsedJson['H1_LED'];
      h2LED = parsedJson['H2_LED'];
      h3LED = parsedJson['H3_LED'];

      print("Heater LED : $h1LED, $h2LED, $h3LED");
    } else if (division == "BTN_1_LEFT") {
      if (btn1LeftFlag == true) {
        s += 'json?j={"division":"$division","Chanel":1}';
      } else {
        s += 'json?j={"division":"$division","Chanel":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "BTN_1_RIGHT") {
      if (btn1RightFlag == true) {
        s += 'json?j={"division":"$division","Chanel":1}';
      } else {
        s += 'json?j={"division":"$division","Chanel":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "BTN_3_LEFT") {
      if (btn2UpFlag == true) {
        s += 'json?j={"division":"$division","Chanel":1}';
      } else {
        s += 'json?j={"division":"$division","Chanel":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "BTN_3_RIGHT") {
      if (btn2DownFlag == true) {
        s += 'json?j={"division":"$division","Chanel":1}';
      } else {
        s += 'json?j={"division":"$division","Chanel":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "BTN_2_DOWN") {
      if (btn3LeftFlag == true) {
        s += 'json?j={"division":"$division","Chanel":1}';
      } else {
        s += 'json?j={"division":"$division","Chanel":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "BTN_2_UP") {
      if (btn3RightFlag == true) {
        s += 'json?j={"division":"$division","Chanel":1}';
      } else {
        s += 'json?j={"division":"$division","Chanel":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "CALL1") {
      s += 'json?j={"division":"$division","Chanel":1}';

      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "CALL2") {
      s += 'json?j={"division":"$division","Chanel":1}';

      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "CALL2") {
      s += 'json?j={"division":"$division","Chanel":1}';

      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "MEMORY1") {
      s += 'json?j={"division":"$division","Chanel":1}';

      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "MEMORY2") {
      s += 'json?j={"division":"$division","Chanel":1}';

      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "SEAT_HEATER1") {
      if (seatHeat1Flag == true) {
        s += 'json?j={"division":"$division","Value":1}';
      } else {
        s += 'json?j={"division":"$division","Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "SEAT_HEATER2") {
      if (seatHeat2Flag == true) {
        s += 'json?j={"division":"$division","Value":1}';
      } else {
        s += 'json?j={"division":"$division","Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
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
