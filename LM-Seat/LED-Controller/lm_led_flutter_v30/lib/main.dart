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

int rs485Count = 0;
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

      if (wifiCount > 6) {
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
                  // width: mobile // WiFi Mode 일 때 가로넓이...
                  //     ? dispWidth * 0.85
                  //     : dispHeight * 0.7,
                  width: dispWidth * 0.85,
                  decoration: BoxDecoration(
                    // borderRadius: BorderRadius.circular(mobile
                    //     ? dispHeight * borderRadius
                    //     : dispWidth * borderRadius),
                    borderRadius:
                        BorderRadius.circular(dispHeight * borderRadius),
                    border: Border.all(
                        // width: mobile
                        //     ? dispWidth * boardLine
                        //     : dispHeight * boardLine,
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
                      // SizedBox(
                      //   // height: mobile ? dispHeight * 0.02 : dispWidth * 0.03,
                      //   height: dispHeight * 0.02,
                      // ),
                      // SizedBox(
                      //   child: Row(
                      //     mainAxisAlignment: MainAxisAlignment.center,
                      //     children: [
                      //       Text(
                      //         '와이파이 연결',
                      //         style: TextStyle(
                      //           // fontSize: mobile
                      //           //     ? (dispWidth * 0.04) / txtScale
                      //           //     : (dispHeight * 0.035) / txtScale,
                      //           fontSize: (dispWidth * 0.04) / txtScale,
                      //         ),
                      //       ),
                      //       Switch(
                      //         // value: networkSelect,
                      //         value: networkSelect,
                      //         onChanged: (value) {
                      //           setState(() {
                      //             networkSelect = value;
                      //             // SharedPreferences prefs =
                      //             //     await SharedPreferences.getInstance();
                      //             // networkSelect = value;
                      //             // prefs.setBool('networkSelect', value);
                      //           });
                      //         },
                      //       ),
                      //       Text(
                      //         '핫스팟 연결',
                      //         style: TextStyle(
                      //           // fontSize: mobile
                      //           //     ? (dispWidth * 0.04) / txtScale
                      //           //     : (dispHeight * 0.035) / txtScale,
                      //           fontSize: (dispWidth * 0.04) / txtScale,
                      //         ),
                      //       ),
                      //     ],
                      //   ),
                      // ),
                      networkSelect
                          ? SizedBox(
                              // width: mobile ? dispWidth * 0.6 : dispWidth * 0.3,
                              width: dispWidth * 0.6,
                              // height: mobile
                              //     ? dispHeight * 0.07
                              //     : dispWidth * 0.045,
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
                                  // fontSize: mobile
                                  //     ? (dispWidth * 0.05) / txtScale
                                  //     : (dispHeight * 0.035) / txtScale,
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
            width: dispWidth * 0.08, // 고정...
            // height: mobile ? 30 : dispWidth * 0.035, // 고정...
            height: 30, // 고정...
            child: Image.asset(
              'assets/images/logo.png',
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
                  // height: mobile ? dispHeight * 0.015 : dispHeight * 0.0,
                  height: dispHeight * 0.015,
                ),
                SizedBox(
                  // width: mobile ? dispWidth * 0.4 : dispWidth * 0.26,
                  // height: mobile ? dispHeight * 0.04 : dispHeight * 0.045,
                  width: dispWidth * 0.4,
                  height: dispHeight * 0.04,
                  child: Text(
                    logoFlag ? logoText : 'CamTrol V3.5',
                    style: TextStyle(
                      fontWeight: FontWeight.bold,
                      fontStyle: FontStyle.italic,
                      // fontSize: mobile
                      //     ? (dispHeight * 0.02) / txtScale
                      //     : (dispHeight * 0.04) / txtScale,
                      fontSize: (dispHeight * 0.02) / txtScale,
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
        const SizedBox.shrink(),
        // (rs485Flag && netFlag)
        (rs485Flag)
            ? SizedBox(
                width: dispWidth * 0.06,
                height: dispHeight * 0.05,
                child: Image.asset(
                  'assets/images/network.png',
                  color: activeColor,
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
      // decoration: BoxDecoration(
      //   image: DecorationImage(
      //     fit: BoxFit.cover,
      //     image: AssetImage('assets/images/sunrise.jpg'), // 배경 이미지
      //   ),
      // ),
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
              // SizedBox(
              //   height: dispHeight * 0.003,
              // ),
              // // display 창과의 간격 늘림...
              SizedBox(
                height: dispHeight * 0.01,
              ),
              Container(
                // 상태 알림 창...
                height: dispHeight * 0.05,
                decoration: BoxDecoration(
                  borderRadius:
                      BorderRadius.circular(dispHeight * borderRadius),
                ),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                  // ignore: prefer_const_literals_to_create_immutables
                  children: <Widget>[
                    Container(
                      width: (dispWidth * 0.1) / 1.2,
                      height: dispWidth * 0.06,
                      decoration: BoxDecoration(
                        image: switchState
                            ? DecorationImage(
                                image: const AssetImage(
                                    'assets/images/switch.png'),
                                colorFilter: ColorFilter.mode(
                                    netFlag
                                        ? Colors.white
                                        : const Color(0xff808080),
                                    BlendMode.srcIn),
                                fit: BoxFit.fill,
                              )
                            : DecorationImage(
                                image: const AssetImage(
                                    'assets/images/switch.png'),
                                colorFilter: ColorFilter.mode(
                                    Colors.transparent, BlendMode.srcIn),
                                fit: BoxFit.fill,
                              ),
                      ),
                    ),
                    Container(
                      width: (dispWidth * 0.13) / 1.2,
                      height: dispWidth * 0.06,
                      decoration: BoxDecoration(
                        image: doorState
                            ? DecorationImage(
                                image:
                                    const AssetImage('assets/images/door.png'),
                                colorFilter: ColorFilter.mode(
                                    Colors.transparent, BlendMode.srcIn),
                              )
                            : netFlag
                                ? DecorationImage(
                                    image: const AssetImage(
                                        'assets/images/door.png'),
                                  )
                                : DecorationImage(
                                    image: const AssetImage(
                                        'assets/images/door.png'),
                                    colorFilter: ColorFilter.mode(
                                        Colors.transparent, BlendMode.srcIn),
                                  ),
                      ),
                    ),
                    Container(
                      width: (dispWidth * 0.1) / 1.2,
                      height: dispWidth * 0.06,
                      decoration: BoxDecoration(
                        image: batState
                            ? DecorationImage(
                                image: const AssetImage(
                                    'assets/images/battery.png'),
                                colorFilter: ColorFilter.mode(
                                    Colors.transparent, BlendMode.srcIn),
                                fit: BoxFit.fill,
                              )
                            : DecorationImage(
                                image: const AssetImage(
                                    'assets/images/battery.png'),
                                colorFilter: ColorFilter.mode(
                                    netFlag ? Colors.white : Colors.transparent,
                                    BlendMode.srcIn),
                                fit: BoxFit.fill,
                              ),
                      ),
                    ),
                  ],
                ),
              ),
              SizedBox(
                height: dispHeight * 0.01,
              ),
              Container(
                // 플로어등, TV Button...
                height: dispHeight * 0.12,
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
                    // InvertorProcess(),
                    TVProcess(),
                  ],
                ),
              ),
              SizedBox(
                height: dispHeight * 0.01,
              ),
              // LED 출력용 Container..
              Container(
                // Button 및 Slide Bar 표시...
                height: dispHeight * 0.7,
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
                // width: mobile ? dispWidth * 0.08 : dispWidth * 0.05,
                width: dispWidth * 0.08,
                child: Text(
                  '${(V.ledValue[ledCh]).round()}',
                  textAlign: TextAlign.right,
                  style: TextStyle(
                      color: V.ledStatus[ledCh] ? Colors.amber : inActiveColor,
                      // fontSize: mobile
                      //     ? (dispWidth * 0.04) / txtScale
                      //     : (dispWidth * 0.023) / txtScale,
                      fontSize: (dispWidth * 0.04) / txtScale,
                      fontStyle: FontStyle.italic,
                      fontWeight: FontWeight.w800),
                ),
              ),
            ]
          : <Widget>[
              SizedBox(
                // width: mobile ? dispWidth * 0.08 : dispWidth * 0.05,
                width: dispWidth * 0.08,
                child: Text(
                  '${(V.ledValue[ledCh]).round()}',
                  textAlign: TextAlign.right,
                  style: TextStyle(
                      color: V.ledStatus[ledCh]
                          ? Colors.amber
                          : netFlag
                              ? activeColor
                              : inActiveColor,
                      // fontSize: mobile
                      //     ? (dispWidth * 0.04) / txtScale
                      //     : (dispWidth * 0.023) / txtScale,
                      fontSize: (dispWidth * 0.04) / txtScale,
                      fontStyle: FontStyle.italic,
                      fontWeight: FontWeight.w800),
                ),
              ),
              SizedBox(
                // Slider 길이
                // width: mobile ? dispWidth * 0.6 : dispWidth * 0.45,
                // height: mobile ? dispHeight * 0.06 : dispHeight * 0.08,
                width: dispWidth * 0.6,
                height: dispHeight * 0.06,
                child: SliderTheme(
                  data: SliderTheme.of(context).copyWith(
                    trackHeight:
                        // mobile ? dispHeight * 0.004 : dispHeight * 0.008,
                        dispHeight * 0.004,
                    thumbColor: Colors.redAccent,
                    thumbShape: RoundSliderThumbShape(
                        enabledThumbRadius:
                            // mobile ? dispWid/th * 0.022 : dispWidth * 0.012),
                            dispWidth * 0.022),
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
                              V.monitor =
                                  '${V.ledName[ledCh]} 밝기 값  ${V.ledValue[ledCh].round()}';

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
                // width: mobile ? 0 : dispWidth * 0.008,
                width: 0,
              ),
              InkWell(
                child: Container(
                  color: Colors.transparent,
                  child: AnimatedContainer(
                    // width: mobile ? dispWidth * 0.25 : dispWidth * 0.09,
                    // height: mobile ? dispHeight * 0.06 : dispHeight * 0.08,
                    width: dispWidth * 0.25,
                    height: dispHeight * 0.06,
                    decoration: BoxDecoration(
                        color: const Color(0xff292d32),
                        // borderRadius: BorderRadius.circular(mobile
                        //     ? (dispWidth * buttonRadius * 3)
                        //     : dispWidth * buttonRadius),
                        borderRadius: BorderRadius.circular(
                            (dispWidth * buttonRadius * 3)),
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
                                  // offset: mobile
                                  //     ? Offset(-2.0, -2.0)
                                  //     : Offset(-4.0, -4.0),
                                  offset: Offset(-2.0, -2.0),
                                  blurRadius: 6,
                                  spreadRadius: 0.0,
                                ),
                                BoxShadow(
                                  color: Colors.amber,
                                  // offset: mobile
                                  //     ? Offset(2.0, 2.0)
                                  //     : Offset(4.0, 4.0),
                                  offset: Offset(2.0, 2.0),
                                  blurRadius: 6,
                                  spreadRadius: 0.0,
                                ),
                                BoxShadow(
                                  color: Colors.amber,
                                  // offset: mobile
                                  //     ? Offset(2.0, -2.0)
                                  //     : Offset(4.0, -4.0),
                                  offset: Offset(2.0, -2.0),
                                  blurRadius: 6,
                                  spreadRadius: 0.0,
                                ),
                                BoxShadow(
                                  color: Colors.amber,
                                  // offset: mobile
                                  //     ? Offset(-2.0, 2.0)
                                  //     : Offset(-4.0, 4.0),
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
                    child: Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: [
                        Container(
                          width: (dispWidth * 0.08) / 1.2, //Battery Icon 폭
                          height: dispWidth * 0.045, //Battery Icon 높이 기존 : 100
                          decoration: BoxDecoration(
                            image: DecorationImage(
                              image:
                                  const AssetImage('assets/images/Light.png'),
                              colorFilter: ColorFilter.mode(
                                  // mainIconColor, BlendMode.srcIn),
                                  netFlag
                                      ? V.ledStatus[ledCh]
                                          ? Colors.amber
                                          : Colors.white
                                      : const Color(0xff808080),
                                  BlendMode.srcIn),
                              fit: BoxFit.fill,
                            ),
                          ),
                        ),
                        Text(
                          V.ledName[ledCh],
                          style: TextStyle(
                              // fontSize: mobile
                              //     ? (dispWidth * 0.04) / txtScale
                              //     : (dispWidth * 0.018) / txtScale,
                              fontSize: (dispWidth * 0.04) / txtScale,
                              color: V.ledStatus[ledCh]
                                  ? Colors.amber
                                  : netFlag
                                      ? activeColor
                                      : inActiveColor,
                              fontWeight: FontWeight.w700),
                        ),
                      ],
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
                // width: mobile ? dispWidth * 0.03 : dispWidth * 0.01,
                width: dispWidth * 0.03,
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
          // width: mobile ? dispWidth * 0.25 : dispWidth * 0.09,
          // height: mobile ? dispHeight * 0.06 : dispHeight * 0.08,
          width: dispWidth * 0.25,
          height: dispHeight * 0.06,
          decoration: BoxDecoration(
              color: const Color(0xff292d32),
              // borderRadius: BorderRadius.circular(mobile
              //     ? (dispWidth * buttonRadius * 3)
              //     : dispWidth * buttonRadius),
              borderRadius:
                  BorderRadius.circular((dispWidth * buttonRadius * 3)),
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
          child: Row(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: [
              Container(
                width: (dispWidth * 0.07) / 1.2, //Battery Icon 폭
                height: dispWidth * 0.045, //Battery Icon 높이 기존 : 100
                decoration: BoxDecoration(
                  image: DecorationImage(
                    image: const AssetImage('assets/images/tv.png'),
                    colorFilter: ColorFilter.mode(
                        // mainIconColor, BlendMode.srcIn),
                        netFlag
                            ? tvStatus
                                ? Colors.amber
                                : Colors.white
                            : const Color(0xff808080),
                        BlendMode.srcIn),
                    fit: BoxFit.fill,
                  ),
                ),
              ),
              Text(
                'TV',
                style: TextStyle(
                    // fontSize: mobile
                    //     ? (dispWidth * 0.04) / txtScale
                    //     : (dispWidth * 0.018) / txtScale,
                    fontSize: (dispWidth * 0.04) / txtScale,
                    color: tvStatus
                        ? Colors.amber
                        : netFlag
                            ? activeColor
                            : inActiveColor,
                    fontWeight: FontWeight.w700),
              ),
            ],
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
          // width: mobile ? dispWidth * 0.25 : dispWidth * 0.09,
          // height: mobile ? dispHeight * 0.06 : dispHeight * 0.08,
          width: dispWidth * 0.25,
          height: dispHeight * 0.06,
          decoration: BoxDecoration(
              color: const Color(0xff292d32),
              // borderRadius: BorderRadius.circular(mobile
              //     ? (dispWidth * buttonRadius * 3)
              //     : dispWidth * buttonRadius),
              borderRadius:
                  BorderRadius.circular((dispWidth * buttonRadius * 3)),
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
          child: Row(
            mainAxisAlignment: MainAxisAlignment.spaceEvenly,
            children: [
              Transform.rotate(
                angle: (180 * 3.1415927) / 180,
                child: Container(
                  width: (dispWidth * 0.08) / 1.2, //Battery Icon 폭
                  height: dispWidth * 0.045, //Battery Icon 높이 기존 : 100
                  decoration: BoxDecoration(
                    image: DecorationImage(
                      image: const AssetImage('assets/images/Light.png'),
                      colorFilter: ColorFilter.mode(
                          // mainIconColor, BlendMode.srcIn),
                          netFlag
                              ? invertorStatus
                                  ? Colors.amber
                                  : Colors.white
                              : const Color(0xff808080),
                          BlendMode.srcIn),
                      fit: BoxFit.fill,
                    ),
                  ),
                ),
              ),
              Text(
                '플로어',
                style: TextStyle(
                    // fontSize: mobile
                    //     ? (dispWidth * 0.04) / txtScale
                    //     : (dispWidth * 0.018) / txtScale,
                    fontSize: (dispWidth * 0.04) / txtScale,
                    color: invertorStatus
                        ? Colors.amber
                        : netFlag
                            ? activeColor
                            : inActiveColor,
                    fontWeight: FontWeight.w700),
              ),
            ],
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

      // V.btVolt = parsedJson['aux_v'];
      // if (V.btVolt > 13.2) {
      //   // 전압에 따른 밧데리 색상 표현...
      //   batColor = Colors.white;
      // } else if (V.btVolt < 13.19 && V.btVolt > 12.5) {
      //   batColor = Colors.yellow;
      // } else if (V.btVolt < 12.5) {
      //   batColor = Colors.red;
      // }

      // V.temperature = parsedJson['Temperature'];
      // V.humidity = parsedJson['Humidity'];

      dynamic pwmParse = 0;
      for (int i = 1; i < 6; i++) {
        pwmParse = parsedJson['ledValue$i'];
        if (pwmParse == 0) {
          V.ledValue[i] = 1.0;
        } else {
          V.ledValue[i] = pwmParse.roundToDouble();
        }
      }

      for (int i = 1; i < 6; i++) {
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

      if (parsedJson['SWITCH'] == 1) {
        switchState = true;
      } else {
        switchState = false;
      }

      if (parsedJson['DOOR'] == 1) {
        doorState = false;
      } else {
        doorState = true;
      }

      if (parsedJson['BAT'] == 1) {
        batState = false;
      } else {
        batState = true;
      }

      if (parsedJson['COM_FLAG'] == 1) {
        // rs485Flag = true;
        rs485Count = 0;
      } else {
        // rs485Flag = false;
      }

      if (parsedJson['invertor'] == 1) {
        invertorStatus = true;
      } else {
        invertorStatus = false;
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
    rs485Count++;
    (rs485Count > 10) ? rs485Flag = false : rs485Flag = true;
    print("rs485Flag : $rs485Flag");
    print("rs485Count : $rs485Count");
  }
}
