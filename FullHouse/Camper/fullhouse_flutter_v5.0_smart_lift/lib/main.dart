// ignore_for_file: prefer_const_constructors
import 'dart:convert';
import 'dart:async';
import 'package:audioplayers/audioplayers.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:get/get.dart';
import 'package:dio/dio.dart';
import 'package:shared_preferences/shared_preferences.dart';

String aPIp = 'http://192.168.4.1/'; // AP Mode 일때 IP Address...
String stationIp = 'http://192.168.148.76/'; // Station Mode 일때 IP Address...
String s = ' ';
String logoText = '  리프트   V5.0';

List<int> pwm = [0, 0, 0, 0, 0, 0, 0];

double dispHeight = 0.0;
double dispWidth = 0.0;
double pixelRatio = 0.0;
double txtScale = 0.0;

bool logoFlag = true;
bool serialDebug = false;

bool mobile = true; // true : 휴대폰, false : 태블릿
bool initFlag = false; // 초기 Reset 출력용

bool chFlag = true; // true : 12 Ch, false : 8 Ch
bool dispDir = true;

bool netFlag = false;

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

// bool current1Detect = false;
// bool current2Detect = false;
// bool current3Detect = false;
// bool current4Detect = false;

double current1 = 0.00;
double current2 = 0.00;
double current3 = 0.00;
double current4 = 0.00;
double currentAll = 0.00;

Server server = Server();

const Color appBarColor = Color(0x552C1FFF); //FF39F1
const Color titleColor = Colors.white;

const Color outOnColor = Color(0xFF6B56F5);
const Color outOffColor = Colors.lime; //6F58FF;

bool timerRun = true;
bool beep = true;

int wifiCount = 0;

double boardLine = 0.0026;
double borderRadius = 0.01;
double appBarHeight = 0.078;

bool wiFiCheck = false;

const timeout = Duration(seconds: 3);
const ms = Duration(milliseconds: 1);

class Var {
  static double ratio = 0.0;

  static bool wiFiType = true; // WiFi : True, Hot Spot : false...

  static String ssid = 'CamTrol V3.5-001';
  static String password = '01211001001';

  static String ssidBuffer = ' ';
  static String passwordBuffer = ' ';
  static String ipBuffer = ' ';

  static double textScaleRatio = 0.0;

  static bool comFlag = true;
  static bool comError = false;
}

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
            name: "/",
            page: () => const MyHomePage(),
            transition: Transition.fadeIn),
        GetPage(
            name: '/wifichange',
            page: () => const WiFiChange(),
            transition: Transition.fadeIn),
      ],
      debugShowCheckedModeBanner: false,
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({Key? key}) : super(key: key);

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  // AudioCache audioCache = AudioCache();

  @override
  void initState() {
    SystemChrome.setEnabledSystemUIMode(SystemUiMode.manual, overlays: []);

    if (beep) advancedPlayer.play(AssetSource('sound/intro.mp3'));
    (() async {
      SharedPreferences prefs = await SharedPreferences.getInstance();

      setState(() {
        beep = prefs.getBool('beep') ?? true;
        Var.wiFiType = prefs.getBool('wiFiType') ?? true;
        Var.wiFiType = true;
        stationIp = prefs.getString('stationIp') ?? 'http://192.168.148.76/';
      });
    })();

    super.initState();
  }

  void handleTimeout() async {
    wiFiCheck = true;
    wifiCount++;

    print(
        "WiFi Count : $wifiCount,  Var.comFlag : $Var.comFlag, SerialDebug : $serialDebug");

    if (wifiCount > 3) {
      netFlag = false;
      initFlag = false;
      if (wifiCount > 100) {
        wifiCount = 10;
      }
    } else {
      netFlag = true;
    }

    if (Var.comFlag == true) {
      // await server.getReq("request", 0);
      server.getReq("request", 0);

      // if (!current1Detect && leg1UpFlag) {
      //   if (current1 > 3.0) {
      //     current1Detect = true;
      //     await server.getReq("L1U", 0);
      //   }
      // }

      // 초기 1회 실행 All Off...
      if (initFlag == false && netFlag == true) {
        await server.getReq("LAU", 0);
        await server.getReq("LAD", 0);
        initFlag = true;
      }
    }

    setState(() {});
    if (timerRun == false) startTimeout(1000);
  }

  startTimeout([int? milliseconds]) {
    var duration = milliseconds == null ? timeout : ms * milliseconds;
    return Timer(duration, handleTimeout);
  }

  Future<void> _ackAlert(BuildContext context) {
    return showDialog<void>(
      context: context,
      builder: (BuildContext context) {
        return AlertDialog(
          title: const Text('Not in stock'),
          content: const Text('This item is no longer available'),
          actions: <Widget>[
            // ignore: deprecated_member_use
            ElevatedButton(
              child: const Text('Ok'),
              onPressed: () {
                Get.back();
              },
            ),
          ],
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    // 초기 1회만 실행
    if (timerRun == true) {
      startTimeout(1000);
      timerRun = false;
    }

    if (Var.comError == true) _ackAlert(context);

    dispHeight = MediaQuery.of(context).size.height;
    dispWidth = MediaQuery.of(context).size.width;
    pixelRatio = MediaQuery.of(context).devicePixelRatio;
    txtScale = MediaQuery.of(context).textScaleFactor;

    var rowSize = MediaQuery.of(context).textScaleFactor;

    if (rowSize <= 1.0) {
      Var.textScaleRatio = 1.0;
    } else if (rowSize > 1.0) {
      Var.textScaleRatio = 0.7;
    }

    if (pixelRatio > 2.3) {
      mobile = true;
    } else {
      mobile = false;
    }

    // print('dispHeight : $dispHeight dispWidth : $dispWidth');

    // mobile
    //     ? SystemChrome.setPreferredOrientations([DeviceOrientation.portraitUp])
    //     : dispDir
    //         ? SystemChrome.setPreferredOrientations(
    //             [DeviceOrientation.landscapeLeft])
    //         : SystemChrome.setPreferredOrientations(
    //             [DeviceOrientation.landscapeRight]);
    SystemChrome.setPreferredOrientations([DeviceOrientation.portraitUp]);
    return PortScreen();
  }
}

class WiFiChange extends StatefulWidget {
  const WiFiChange({Key? key}) : super(key: key);

  @override
  WiFiChangeState createState() => WiFiChangeState();
}

class WiFiChangeState extends State<WiFiChange> {
  // AudioCache audioCache = AudioCache();

  final stationIpController = TextEditingController();

  bool wifiSelect = Var.wiFiType; // WiFi : True, Hot Spot : false...
  bool ssidFlag = false;
  bool passwodFlag = false;
  bool ipFlag = false;
  bool isSwitched = false;

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
      backgroundColor: Color(0x8045FFA2),
      appBar: PreferredSize(
        preferredSize: Size.fromHeight(
          mobile
              ? (dispHeight * appBarHeight) * 0.6
              : (dispHeight * appBarHeight),
        ),
        child: AppBar(
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
      body: SingleChildScrollView(
        child: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.start,
            children: <Widget>[
              SizedBox(
                // appbar 와 간격 띄우기...
                height: (mobile ? dispHeight : dispWidth) * 0.01,
              ),
              Container(
                width: wifiSelect
                    ? mobile // WiFi Mode 일 때 가로넓이...
                        ? dispWidth * 0.6
                        : dispHeight * 0.5
                    : mobile // 테더링 Mode 일 때 가로넓이...
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
                  children: <Widget>[
                    wifiSelect
                        ? SizedBox.shrink()
                        : SizedBox(
                            height:
                                mobile ? dispHeight * 0.02 : dispWidth * 0.02,
                          ),
                    wifiSelect
                        ? SizedBox.shrink()
                        : isSwitched
                            ? SizedBox.shrink()
                            : SizedBox(
                                // 테더링 추가 연결이면 표시...
                                width:
                                    mobile ? dispWidth * 0.6 : dispWidth * 0.3,
                                height: mobile
                                    ? dispHeight * 0.05
                                    : dispWidth * 0.045,
                                child: TextField(
                                  cursorColor: Colors.white,
                                  onChanged: (text) {
                                    Var.ssidBuffer = text;
                                    ssidFlag = true;
                                  },
                                  // controller: ssidController,
                                  style: TextStyle(
                                    fontSize: mobile
                                        ? (dispWidth * 0.05) / txtScale
                                        : (dispHeight * 0.035) / txtScale,
                                    fontWeight: FontWeight.w400,
                                    color: Colors.black,
                                  ),
                                  decoration: InputDecoration(
                                    labelText: Var.ssid,
                                    // hintText: '네트워크 이름 입력 (영문 30자 이내)',
                                    // hintStyle: TextStyle(
                                    //   fontSize: 12.0,
                                    //   color: Colors.black,
                                    // ),
                                    labelStyle: TextStyle(
                                      // fontSize: mobile
                                      //     ? (dispWidth * 0.03) / txtScale
                                      //     : (dispHeight * 0.03) / txtScale,
                                      color: Colors.white,
                                    ),
                                    focusedBorder: OutlineInputBorder(
                                      borderRadius: BorderRadius.all(
                                          Radius.circular(5.0)),
                                      borderSide: BorderSide(
                                        width: 2,
                                        color: Color(0xE0D4AA39),
                                      ),
                                    ),
                                    border: OutlineInputBorder(
                                      borderRadius: BorderRadius.all(
                                          Radius.circular(5.0)),
                                    ),
                                  ),
                                  // keyboardType: TextInputType.streetAddress,
                                ),
                              ),
                    wifiSelect
                        ? SizedBox.shrink()
                        : isSwitched
                            ? SizedBox.shrink()
                            : Text(
                                '네트워크 SSID를 입력 합니다.',
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
                        : SizedBox.shrink(),
                    wifiSelect
                        ? SizedBox.shrink()
                        : SizedBox(
                            height: mobile
                                ? (dispHeight * 0.03) / 2
                                : (dispWidth * 0.04) / 2,
                          ),
                    wifiSelect
                        ? SizedBox.shrink()
                        : isSwitched
                            ? SizedBox.shrink()
                            : SizedBox(
                                width:
                                    mobile ? dispWidth * 0.6 : dispWidth * 0.3,
                                height: mobile
                                    ? dispHeight * 0.05
                                    : dispWidth * 0.045,
                                child: TextField(
                                  cursorColor: Colors.white,
                                  onChanged: (text) {
                                    Var.passwordBuffer = text;
                                    passwodFlag = true;
                                  },
                                  // controller: passwordController,
                                  style: TextStyle(
                                    fontSize: mobile
                                        ? (dispWidth * 0.05) / txtScale
                                        : (dispHeight * 0.035) / txtScale,
                                    fontWeight: FontWeight.w400,
                                    color: Colors.black,
                                  ),
                                  decoration: InputDecoration(
                                    labelText: Var.password,
                                    // hintText: '비밀번호 입력 (30자 이내)',
                                    // hintStyle: TextStyle(
                                    //   fontSize: 12.0,
                                    //   color: Colors.black,
                                    // ),
                                    labelStyle: TextStyle(
                                      color: Colors.white,
                                    ),
                                    focusedBorder: OutlineInputBorder(
                                      borderRadius: BorderRadius.all(
                                          Radius.circular(5.0)),
                                      borderSide: BorderSide(
                                        width: 2,
                                        color: Color(0xE0D4AA39),
                                      ),
                                    ),
                                    border: OutlineInputBorder(
                                      borderRadius: BorderRadius.all(
                                          Radius.circular(5.0)),
                                    ),
                                  ),
                                  // keyboardType: TextInputType.streetAddress,
                                ),
                              ),
                    wifiSelect
                        ? SizedBox.shrink()
                        : isSwitched
                            ? SizedBox.shrink()
                            : Text(
                                '네트워크 비밀번호를 입력 합니다.',
                                style: TextStyle(
                                  fontSize: mobile
                                      ? (dispWidth * 0.04) / txtScale
                                      : (dispHeight * 0.035) / txtScale,
                                ),
                              ),
                    wifiSelect
                        ? SizedBox.shrink()
                        : SizedBox(
                            height:
                                mobile ? dispHeight * 0.03 : dispWidth * 0.02,
                          ),
                    wifiSelect
                        ? SizedBox.shrink()
                        : isSwitched
                            ? SizedBox(
                                width:
                                    mobile ? dispWidth * 0.6 : dispWidth * 0.3,
                                height: mobile
                                    ? dispHeight * 0.05
                                    : dispWidth * 0.045,
                                child: TextField(
                                  cursorColor: Colors.white,
                                  onChanged: (text) {
                                    Var.ipBuffer = stationIpController.text;
                                    stationIp = 'http://';
                                    stationIp += Var.ipBuffer;
                                    stationIp += '/';
                                    ipFlag = true;
                                  },
                                  controller: stationIpController,
                                  style: TextStyle(
                                    fontSize: mobile
                                        ? (dispWidth * 0.05) / txtScale
                                        : (dispHeight * 0.035) / txtScale,
                                    fontWeight: FontWeight.w400,
                                    color: Colors.black,
                                  ),
                                  decoration: InputDecoration(
                                    labelText: stationIp,
                                    // hintText: 'IP 주소 입력 (숫자만 입력 합니다.)',
                                    // hintStyle: TextStyle(
                                    //   fontSize: 12.0,
                                    //   color: Colors.black,
                                    // ),
                                    labelStyle: TextStyle(color: Colors.white),
                                    focusedBorder: OutlineInputBorder(
                                      borderRadius: BorderRadius.all(
                                          Radius.circular(5.0)),
                                      borderSide: BorderSide(
                                        width: 2,
                                        color: Color(0xE0D4AA39),
                                      ),
                                    ),
                                    border: OutlineInputBorder(
                                      borderRadius: BorderRadius.all(
                                          Radius.circular(5.0)),
                                    ),
                                  ),
                                  // keyboardType: TextInputType.number,
                                ),
                              )
                            : SizedBox.shrink(),
                    wifiSelect
                        ? SizedBox.shrink()
                        : isSwitched
                            ? Text(
                                '숫자와 점 만 입력 합니다. 예) 192.168.148.76',
                                style: TextStyle(
                                  fontSize: mobile
                                      ? (dispWidth * 0.038) / txtScale
                                      : (dispHeight * 0.03) / txtScale,
                                ),
                              )
                            : SizedBox.shrink(),

                    // mobile ? SizedBox(height: 5.0) : SizedBox.shrink(),
                    wifiSelect ? SizedBox.shrink() : SizedBox(height: 20.0),
                    RadioListTile(
                      activeColor: Colors.black,
                      title: Text(
                        '와이파이 연결',
                        style: TextStyle(
                            color: Colors.black,
                            fontSize: mobile
                                ? (dispWidth * 0.04) / txtScale
                                : (dispHeight * 0.025) / txtScale,
                            fontStyle: FontStyle.italic,
                            fontWeight: FontWeight.w800),
                      ),
                      value: wiFi.A,
                      groupValue: wifiSelect ? wiFi.A : wiFi.B,
                      onChanged: (value) /*async*/ {
                        wifiSelect = true;
                        if (beep) {
                          advancedPlayer
                              .play(AssetSource('sound/iconClick.mp3'));
                        }
                        setState(() {});
                      },
                    ),
                    RadioListTile(
                      activeColor: Colors.black,
                      title: Text(
                        isSwitched ? '테더링 추가 연결' : '테더링 네트워크 저장',
                        style: TextStyle(
                            color: Colors.black,
                            fontSize: mobile
                                ? (dispWidth * 0.04) / txtScale
                                : (dispHeight * 0.025) / txtScale,
                            fontStyle: FontStyle.italic,
                            fontWeight: FontWeight.w800),
                      ),
                      value: wiFi.B,
                      groupValue: wifiSelect ? wiFi.A : wiFi.B,
                      onChanged: (value) async {
                        wifiSelect = false;
                        if (beep) {
                          advancedPlayer
                              .play(AssetSource('sound/iconClick.mp3'));
                        }
                        setState(() {});
                      },
                    ),
                    SizedBox(
                        height: mobile ? dispHeight * 0.02 : dispWidth * 0.02),
                    SizedBox(
                      child: wifiSelect
                          ? null
                          : Row(
                              mainAxisAlignment: MainAxisAlignment.center,
                              // ignore: prefer_const_literals_to_create_immutables
                              children: <Widget>[
                                Text(
                                  "네트워크 저장",
                                  style: TextStyle(
                                    color: Colors.black,
                                    fontWeight: FontWeight.bold,
                                    fontSize: mobile
                                        ? (dispWidth * 0.04) / txtScale
                                        : (dispHeight * 0.025) / txtScale,
                                  ),
                                ),
                                Switch(
                                  value: isSwitched,
                                  onChanged: (value) {
                                    setState(() {
                                      isSwitched = value;
                                      // print(isSwitched);
                                    });
                                  },
                                  activeTrackColor: Colors.lightGreenAccent,
                                  activeColor: Colors.green,
                                ),
                                Text(
                                  "네트워크 접속",
                                  style: TextStyle(
                                    color: Colors.black,
                                    fontWeight: FontWeight.bold,
                                    fontSize: mobile
                                        ? (dispWidth * 0.04) / txtScale
                                        : (dispHeight * 0.025) / txtScale,
                                  ),
                                ),
                              ],
                            ),
                    ),
                    SizedBox(
                      height: mobile ? dispHeight * 0.05 : dispWidth * 0.05,
                    ),
                    SizedBox(
                      child: Row(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: <Widget>[
                          SizedBox(
                            width: mobile ? dispWidth * 0.17 : dispWidth * 0.09,
                            height:
                                mobile ? dispHeight * 0.045 : dispHeight * 0.07,
                            child: ElevatedButton(
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
                                SharedPreferences prefs =
                                    await SharedPreferences.getInstance();
                                prefs.setBool('wiFiType', wifiSelect);
                                if (ssidFlag) {
                                  Var.ssid = Var.ssidBuffer;
                                  prefs.setString('ssid', Var.ssidBuffer);
                                }
                                if (passwodFlag) {
                                  Var.password = Var.passwordBuffer;
                                  prefs.setString(
                                      'password', Var.passwordBuffer);
                                }
                                if (ipFlag) {
                                  prefs.setString('stationIp', stationIp);
                                }

                                await server.getReq(
                                    "wifiSend", 1); // ssid, password 저장하기...

                                setState(() {});
                                if (beep) {
                                  advancedPlayer
                                      .play(AssetSource('sound/intro.mp3'));
                                }
                                showDialog(
                                  context: context,
                                  builder: (context) => AlertDialog(
                                    backgroundColor: Color(0xC0D4AA39),
                                    title: Text(
                                      "저장 되었습니다.",
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
                                            ? dispWidth * 0.17
                                            : dispWidth * 0.09,
                                        height: mobile
                                            ? dispHeight * 0.045
                                            : dispHeight * 0.07,
                                        child: ElevatedButton(
                                            child: Text(
                                              "확인",
                                              style: TextStyle(
                                                color: Colors.black,
                                                fontWeight: FontWeight.bold,
                                                fontSize: mobile
                                                    ? (dispWidth * 0.035) /
                                                        txtScale
                                                    : (dispHeight * 0.03) /
                                                        txtScale,
                                              ),
                                            ),
                                            onPressed: () {
                                              // server.getReq("SensorSend", 0);
                                              Get.back();
                                            }),
                                      ),
                                    ],
                                  ),
                                );
                              },
                            ),
                          ),
                          SizedBox(
                              width: mobile
                                  ? dispHeight * 0.04
                                  : dispWidth * 0.04),
                          SizedBox(
                            width: mobile ? dispWidth * 0.17 : dispWidth * 0.09,
                            height:
                                mobile ? dispHeight * 0.045 : dispHeight * 0.07,
                            child: ElevatedButton(
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
                    SizedBox(
                        height: mobile ? dispHeight * 0.02 : dispWidth * 0.02),
                  ],
                ),
              ),
              SizedBox(
                // 컨테이너 하단부 간격 띄우기 간격 띄우기...
                height: (mobile ? dispHeight : dispWidth) * 0.05,
              ),
            ],
          ),
        ),
      ),
    );
  }
}

enum wiFi { A, B }

class AppBarScreen extends StatefulWidget {
  // ignore: prefer_const_constructors_in_immutables
  AppBarScreen({Key? key}) : super(key: key);

  @override
  _AppBarScreenState createState() => _AppBarScreenState();
}

class _AppBarScreenState extends State<AppBarScreen> {
  // AudioCache audioCache = AudioCache();

  @override
  Widget build(BuildContext context) {
    return AppBar(
      // automaticallyImplyLeading: true,
      backgroundColor: appBarColor,
      title: Row(
        crossAxisAlignment: CrossAxisAlignment.center,
        children: [
          logoFlag // true 면 logo 삽입...
              ? Container(
                  // width: mobile ? dispWidth * 0.08 : dispWidth * 0.04,
                  // height: mobile ? dispHeight * 0.026 : dispHeight * 0.04,
                  width: dispWidth * 0.08,
                  height: dispHeight * 0.026,
                  decoration: BoxDecoration(
                    image: DecorationImage(
                      // image: AssetImage('assets/images/kim_logo.png'),
                      image: AssetImage('assets/images/camperIcon.png'),
                      colorFilter:
                          ColorFilter.mode(Colors.white, BlendMode.srcIn),
                      // fit: BoxFit.fill,
                    ),
                  ),
                )
              : SizedBox.shrink(),
          InkWell(
            onLongPress: () async {
              // SharedPreferences prefs = await SharedPreferences.getInstance();
              // dispDir = !dispDir;
              // prefs.setBool('dispDir', dispDir);
            },
            child: Column(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                mobile
                    ? SizedBox(
                        height: dispHeight * 0.017,
                      )
                    : SizedBox.shrink(),
                SizedBox(
                  // width: mobile ? dispWidth * 0.35 : dispWidth * 0.26,
                  // height: mobile ? dispHeight * 0.04 : dispHeight * 0.045,
                  width: dispWidth * 0.35,
                  height: dispHeight * 0.04,
                  child: Text(
                    logoFlag ? logoText : 'CamTrol V3.5',
                    style: TextStyle(
                      fontWeight: FontWeight.bold,
                      fontStyle: FontStyle.italic,
                      // fontSize: mobile
                      //     ? (dispHeight * 0.023) / txtScale
                      //     : (dispHeight * 0.035) / txtScale,
                      fontSize: (dispHeight * 0.023) / txtScale,
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
        IconButton(
          icon: (Var.wiFiType)
              ? netFlag
                  ? Icon(Icons.wifi,
                      // size: mobile ? dispWidth * 0.06 : dispHeight * 0.05)
                      size: dispWidth * 0.06)
                  : Icon(Icons.wifi_off,
                      // size: mobile ? dispWidth * 0.06 : dispHeight * 0.05)
                      size: dispWidth * 0.06)
              : netFlag
                  ? Icon(Icons.wifi_tethering,
                      // size: mobile ? dispWidth * 0.06 : dispHeight * 0.05)
                      size: dispWidth * 0.06)
                  : Icon(Icons.wifi_tethering_off,
                      // size: mobile ? dispWidth * 0.06 : dispHeight * 0.05),
                      size: dispWidth * 0.06),
          tooltip: 'WiFi',
          onPressed: () async {
            if (beep) advancedPlayer.play(AssetSource('sound/iconClick.mp3'));
            await server.getReq("wifiSend", 0);
            if (serialDebug) {
              await Get.toNamed('/wifichange');
            }
          },
        ),
        SizedBox(
            // width: mobile
            //     ? (pixelRatio > 1.5)
            //         ? dispWidth * 0.0015
            //         : dispWidth * 0.03
            //     : (pixelRatio > 1.5)
            //         ? dispWidth * 0.015
            //         : dispWidth * 0.03),
            width: (pixelRatio > 1.5) ? dispWidth * 0.0015 : dispWidth * 0.03),
        IconButton(
          icon: Icon(Icons.exit_to_app,
              // size: mobile ? dispWidth * 0.06 : dispHeight * 0.05),
              size: dispWidth * 0.06),
          tooltip: '프로그램 종료',
          onPressed: () async {
            if (beep) advancedPlayer.play(AssetSource('sound/intro.mp3'));
            showDialog(
              context: context,
              builder: (context) => AlertDialog(
                  backgroundColor: Color(0xC0D4AA39),
                  title: Text(
                    "프로그램을 종료 합니다.",
                    style: TextStyle(
                      fontSize: mobile
                          ? (dispWidth * 0.04) / txtScale
                          : (dispHeight * 0.04) / txtScale,
                      color: Colors.white,
                    ),
                  ),
                  actions: <Widget>[
                    SizedBox(
                        width: mobile ? dispHeight * 0.005 : dispWidth * 0.066),
                    SizedBox(
                      width: mobile ? dispWidth * 0.22 : dispWidth * 0.22,
                      height: mobile ? dispHeight * 0.055 : dispHeight * 0.075,
                      child: ElevatedButton(
                          child: Text(
                            "종  료",
                            style: TextStyle(
                              color: Colors.black,
                              fontWeight: FontWeight.bold,
                              fontSize: mobile
                                  ? (dispWidth * 0.04) / txtScale
                                  : (dispHeight * 0.03) / txtScale,
                            ),
                          ),
                          onPressed: () async {
                            // await server.getReq("POWER", 0);
                            SystemChannels.platform
                                .invokeMethod('SystemNavigator.pop');
                          }),
                    ),
                    SizedBox(
                      width: mobile ? dispHeight * 0.02 : dispWidth * 0.04,
                    ),
                    SizedBox(
                      width: mobile ? dispWidth * 0.23 : dispWidth * 0.22,
                      height: mobile ? dispHeight * 0.055 : dispHeight * 0.075,
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
                          onPressed: () async {
                            // await server.getReq("sleep", 0);
                            Get.back();
                          }),
                    ),
                    SizedBox(
                      width: mobile ? dispHeight * 0.005 : dispWidth * 0.06,
                    ),
                  ]),
            );
          },
        ),
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
    return Scaffold(
      backgroundColor: Colors.blueGrey,
      appBar: PreferredSize(
        // preferredSize: mobile
        //     // Appbar 높이...
        //     ? Size.fromHeight((dispHeight * appBarHeight) * 0.65)
        //     : Size.fromHeight((dispHeight * appBarHeight)),
        preferredSize:
            // Appbar 높이...
            Size.fromHeight((dispHeight * appBarHeight) * 0.65),
        child: AppBarScreen(), // 공통으로 적용...
      ),
      body: Stack(
        children: <Widget>[
          Container(
            // margin: mobile
            //     ? EdgeInsets.all(dispWidth * 0.02)
            //     : EdgeInsets.all(dispWidth * 0.016),
            margin: EdgeInsets.all(dispWidth * 0.02),
            decoration: BoxDecoration(
              image: DecorationImage(
                image: AssetImage('assets/images/motor_home.png'),
              ),
            ),
          ),
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
                            size: mobile ? dispWidth * 0.25 : dispWidth * 0.18,
                            // size: dispWidth * 0.25,
                            color: leg1UpFlag ? Colors.red : Colors.orange,
                          ),
                          onTap: () {
                            setState(() {
                              if (!allUpFlag && !allDownFlag) {
                                if (leg1DownFlag) {
                                  leg1DownFlag = false;
                                  advancedPlayer
                                      .play(AssetSource('sound/switchOff.mp3'));
                                  server.getReq("L1D", 0);
                                } else {
                                  leg1UpFlag = !leg1UpFlag;
                                  leg1UpFlag
                                      ? server.getReq("L1U", 1)
                                      : server.getReq("L1U", 0);
                                  leg1UpFlag
                                      ? advancedPlayer.play(
                                          AssetSource('sound/switchOn.mp3'))
                                      : advancedPlayer.play(
                                          AssetSource('sound/switchOff.mp3'));
                                  leg1UpFlag
                                      ? print('press LEG 1 UP ON')
                                      : print('press LEG 1 UP OFF');
                                }
                              }
                            });
                          },
                        ),
                      ),
                      Text(
                        "${current1.toStringAsFixed(1)} A",
                        style: TextStyle(
                            // color: Colors.red,
                            fontSize: mobile
                                ? (dispWidth * 0.07) / txtScale
                                : (dispWidth * 0.05) / txtScale,
                            // fontSize: (dispWidth * 0.1) / txtScale,
                            fontWeight: FontWeight.w800),
                      ),
                      GestureDetector(
                        child: InkWell(
                          child: Icon(
                            Icons.arrow_circle_down,
                            size: mobile ? dispWidth * 0.25 : dispWidth * 0.18,
                            // size: dispWidth * 0.25,
                            color: leg1DownFlag ? Colors.red : Colors.orange,
                          ),
                          onTap: () {
                            setState(() {
                              if (!allUpFlag && !allDownFlag) {
                                if (leg1UpFlag) {
                                  leg1UpFlag = false;
                                  advancedPlayer
                                      .play(AssetSource('sound/switchOff.mp3'));
                                  server.getReq("L1U", 0);
                                } else {
                                  leg1DownFlag = !leg1DownFlag;
                                  leg1DownFlag
                                      ? server.getReq("L1D", 1)
                                      : server.getReq("L1D", 0);
                                  leg1DownFlag
                                      ? advancedPlayer.play(
                                          AssetSource('sound/switchOn.mp3'))
                                      : advancedPlayer.play(
                                          AssetSource('sound/switchOff.mp3'));
                                  leg1DownFlag
                                      ? print('press LEG 1 Down ON')
                                      : print('press LEG 1 Down OFF');
                                }
                              }
                            });
                          },
                        ),
                      ),
                    ],
                  ),
                  Column(
                    children: [
                      GestureDetector(
                        child: InkWell(
                          child: Icon(
                            Icons.arrow_circle_up,
                            size: mobile ? dispWidth * 0.25 : dispWidth * 0.18,
                            // size: dispWidth * 0.25,
                            color: leg3UpFlag ? Colors.red : Colors.orange,
                          ),
                          onTap: () {
                            setState(() {
                              if (!allUpFlag && !allDownFlag) {
                                if (leg3DownFlag) {
                                  leg3DownFlag = false;
                                  advancedPlayer
                                      .play(AssetSource('sound/switchOff.mp3'));
                                  server.getReq("L3D", 0);
                                } else {
                                  leg3UpFlag = !leg3UpFlag;
                                  leg3UpFlag
                                      ? server.getReq("L3U", 1)
                                      : server.getReq("L3U", 0);
                                  leg3UpFlag
                                      ? advancedPlayer.play(
                                          AssetSource('sound/switchOn.mp3'))
                                      : advancedPlayer.play(
                                          AssetSource('sound/switchOff.mp3'));
                                  leg3UpFlag
                                      ? print('press LEG 3 UP ON')
                                      : print('press LEG 3 UP OFF');
                                }
                              }
                            });
                          },
                        ),
                      ),
                      Text(
                        "${current3.toStringAsFixed(1)} A",
                        style: TextStyle(
                            // color: Colors.red,
                            fontSize: mobile
                                ? (dispWidth * 0.07) / txtScale
                                : (dispWidth * 0.05) / txtScale,
                            // fontSize: (dispWidth * 0.1) / txtScale,
                            fontWeight: FontWeight.w800),
                      ),
                      GestureDetector(
                        child: InkWell(
                          child: Icon(
                            Icons.arrow_circle_down,
                            size: mobile ? dispWidth * 0.25 : dispWidth * 0.18,
                            // size: dispWidth * 0.25,
                            color: leg3DownFlag ? Colors.red : Colors.orange,
                          ),
                          onTap: () {
                            setState(() {
                              if (!allUpFlag && !allDownFlag) {
                                if (leg3UpFlag) {
                                  leg3UpFlag = false;
                                  advancedPlayer
                                      .play(AssetSource('sound/switchOff.mp3'));
                                  server.getReq("L3U", 0);
                                } else {
                                  leg3DownFlag = !leg3DownFlag;
                                  leg3DownFlag
                                      ? server.getReq("L3D", 1)
                                      : server.getReq("L3D", 0);
                                  leg3DownFlag
                                      ? advancedPlayer.play(
                                          AssetSource('sound/switchOn.mp3'))
                                      : advancedPlayer.play(
                                          AssetSource('sound/switchOff.mp3'));
                                  leg3DownFlag
                                      ? print('press LEG 3 Down ON')
                                      : print('press LEG 3 Down OFF');
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
                            size: mobile ? dispWidth * 0.25 : dispWidth * 0.18,
                            // size: dispWidth * 0.25,
                            color: allUpFlag ? Colors.red : Colors.black,
                          ),
                          onTap: () async {
                            if (allDownFlag) {
                              allDownFlag = false;
                              advancedPlayer
                                  .play(AssetSource('sound/switchOff.mp3'));
                              await server.getReq("LAD", 0);
                            } else {
                              setState(() {
                                allUpFlag = !allUpFlag;
                                allUpFlag
                                    ? advancedPlayer
                                        .play(AssetSource('sound/switchOn.mp3'))
                                    : advancedPlayer.play(
                                        AssetSource('sound/switchOff.mp3'));
                                allUpFlag
                                    ? server.getReq("LAU", 1)
                                    : server.getReq("LAU", 0);
                                allUpFlag
                                    ? print('press LEG ALL UP ON')
                                    : print('press LEG ALL UP OFF');
                              });
                            }
                          },
                        ),
                      ),
                      Text(
                        "${currentAll.toStringAsFixed(1)} A",
                        style: TextStyle(
                            color: Colors.red,
                            // fontSize: mobile
                            //     ? (dispWidth * 0.07) / txtScale
                            //     : (dispWidth * 0.05) / txtScale,
                            fontSize: (dispWidth * 0.1) / txtScale,
                            fontWeight: FontWeight.w800),
                      ),
                      GestureDetector(
                        child: InkWell(
                          child: Icon(
                            Icons.arrow_circle_down,
                            size: mobile ? dispWidth * 0.25 : dispWidth * 0.18,
                            // size: dispWidth * 0.25,
                            color: allDownFlag ? Colors.red : Colors.black,
                          ),
                          onTap: () async {
                            if (allUpFlag) {
                              allUpFlag = false;
                              advancedPlayer
                                  .play(AssetSource('sound/switchOff.mp3'));
                              await server.getReq("LAU", 0);
                            } else {
                              setState(() {
                                allDownFlag = !allDownFlag;
                                allDownFlag
                                    ? advancedPlayer
                                        .play(AssetSource('sound/switchOn.mp3'))
                                    : advancedPlayer.play(
                                        AssetSource('sound/switchOff.mp3'));
                                allDownFlag
                                    ? server.getReq("LAD", 1)
                                    : server.getReq("LAD", 0);
                                allDownFlag
                                    ? print('press LEG ALL DOWN ON')
                                    : print('press LEG ALL DOWN OFF');
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
                            size: mobile ? dispWidth * 0.25 : dispWidth * 0.18,
                            // size: dispWidth * 0.25,
                            color: leg2UpFlag ? Colors.red : Colors.orange,
                          ),
                          onTap: () async {
                            setState(() {
                              if (!allUpFlag && !allDownFlag) {
                                if (leg2DownFlag) {
                                  leg2DownFlag = false;
                                  advancedPlayer
                                      .play(AssetSource('sound/switchOff.mp3'));
                                  server.getReq("L2D", 0);
                                } else {
                                  leg2UpFlag = !leg2UpFlag;
                                  leg2UpFlag
                                      ? server.getReq("L2U", 1)
                                      : server.getReq("L2U", 0);
                                  leg2UpFlag
                                      ? advancedPlayer.play(
                                          AssetSource('sound/switchOn.mp3'))
                                      : advancedPlayer.play(
                                          AssetSource('sound/switchOff.mp3'));
                                  leg2UpFlag
                                      ? print('press LEG 2 UP ON')
                                      : print('press LEG 2 UP OFF');
                                }
                              }
                            });
                          },
                        ),
                      ),
                      Text(
                        "${current2.toStringAsFixed(1)} A",
                        style: TextStyle(
                            // color: Colors.red,
                            fontSize: mobile
                                ? (dispWidth * 0.07) / txtScale
                                : (dispWidth * 0.05) / txtScale,
                            // fontSize: (dispWidth * 0.1) / txtScale,
                            fontWeight: FontWeight.w800),
                      ),
                      GestureDetector(
                        child: InkWell(
                          child: Icon(
                            Icons.arrow_circle_down,
                            size: mobile ? dispWidth * 0.25 : dispWidth * 0.18,
                            // size: dispWidth * 0.25,
                            color: leg2DownFlag ? Colors.red : Colors.orange,
                          ),
                          onTap: () async {
                            setState(() {
                              if (!allUpFlag && !allDownFlag) {
                                if (leg2UpFlag) {
                                  leg2UpFlag = false;
                                  advancedPlayer
                                      .play(AssetSource('sound/switchOff.mp3'));
                                  server.getReq("L2U", 0);
                                } else {
                                  leg2DownFlag = !leg2DownFlag;
                                  leg2DownFlag
                                      ? server.getReq("L2D", 1)
                                      : server.getReq("L2D", 0);
                                  leg2DownFlag
                                      ? advancedPlayer.play(
                                          AssetSource('sound/switchOn.mp3'))
                                      : advancedPlayer.play(
                                          AssetSource('sound/switchOff.mp3'));
                                  leg2DownFlag
                                      ? print('press LEG 2 Down ON')
                                      : print('press LEG 2 Down OFF');
                                }
                              }
                            });
                          },
                        ),
                      ),
                    ],
                  ),
                  Column(
                    children: [
                      GestureDetector(
                        child: InkWell(
                          child: Icon(
                            Icons.arrow_circle_up,
                            size: mobile ? dispWidth * 0.25 : dispWidth * 0.18,
                            // size: dispWidth * 0.25,
                            color: leg4UpFlag ? Colors.red : Colors.orange,
                          ),
                          onTap: () async {
                            setState(() {
                              if (!allUpFlag && !allDownFlag) {
                                if (leg4DownFlag) {
                                  leg4DownFlag = false;
                                  advancedPlayer
                                      .play(AssetSource('sound/switchOff.mp3'));
                                  server.getReq("L4D", 0);
                                } else {
                                  leg4UpFlag = !leg4UpFlag;
                                  leg4UpFlag
                                      ? server.getReq("L4U", 1)
                                      : server.getReq("L4U", 0);
                                  leg4UpFlag
                                      ? advancedPlayer.play(
                                          AssetSource('sound/switchOn.mp3'))
                                      : advancedPlayer.play(
                                          AssetSource('sound/switchOff.mp3'));
                                  leg4UpFlag
                                      ? print('press LEG 4 UP ON')
                                      : print('press LEG 4 UP OFF');
                                }
                              }
                            });
                          },
                        ),
                      ),
                      Text(
                        "${current4.toStringAsFixed(1)} A",
                        style: TextStyle(
                            // color: Colors.red,
                            fontSize: mobile
                                ? (dispWidth * 0.07) / txtScale
                                : (dispWidth * 0.05) / txtScale,
                            // fontSize: (dispWidth * 0.1) / txtScale,
                            fontWeight: FontWeight.w800),
                      ),
                      GestureDetector(
                        child: InkWell(
                          child: Icon(
                            Icons.arrow_circle_down,
                            size: mobile ? dispWidth * 0.25 : dispWidth * 0.18,
                            // size: dispWidth * 0.25,
                            color: leg4DownFlag ? Colors.red : Colors.orange,
                          ),
                          onTap: () async {
                            setState(() {
                              if (!allUpFlag && !allDownFlag) {
                                if (leg4UpFlag) {
                                  leg4UpFlag = false;
                                  advancedPlayer
                                      .play(AssetSource('sound/switchOff.mp3'));
                                  server.getReq("L4U", 0);
                                } else {
                                  leg4DownFlag = !leg4DownFlag;
                                  leg4DownFlag
                                      ? server.getReq("L4D", 1)
                                      : server.getReq("L4D", 0);
                                  leg4DownFlag
                                      ? advancedPlayer.play(
                                          AssetSource('sound/switchOn.mp3'))
                                      : advancedPlayer.play(
                                          AssetSource('sound/switchOff.mp3'));
                                  leg4DownFlag
                                      ? print('press LEG 4 Down ON')
                                      : print('press LEG 4 Down OFF');
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
    );
  }
}

class Server {
  // ignore: duplicate_ignore
  Future<void> getReq(String division, int chanel) async {
    // ignore: prefer_typing_uninitialized_variables
    var response;
    // ignore: prefer_typing_uninitialized_variables
    s = Var.wiFiType ? aPIp : stationIp;

    // ignore: prefer_typing_uninitialized_variables
    var parsedJson;

    Dio dio = Dio();

    if (Var.comFlag == false) {
      s += "root";
      response = await dio.get(s);
      print(response.statusCode);
      print(response.statusMessage);
      dio.close();
      return;
    } else if (division == "request") {
      s += "Request";
      response = await dio.get(s);
      parsedJson = jsonDecode(response.data);

      print(response.statusCode);
      print(response.statusMessage);
      print(response);

      (parsedJson['serialDebug'] == 1)
          ? serialDebug = true
          : serialDebug = false;
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
      (parsedJson['LAD_Flag'] == 1) ? allDownFlag = true : allDownFlag = false;

      current1 = parsedJson['CURRENT1'];
      current2 = parsedJson['CURRENT2'];
      current3 = parsedJson['CURRENT3'];
      current4 = parsedJson['CURRENT4'];
      currentAll = current1 + current2 + current3 + current4;

      // print(parsedJson);
    } else if (division == "L1U") {
      if (chanel == 1) {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":1}';
      } else {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "L1D") {
      if (chanel == 1) {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":1}';
      } else {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "L2U") {
      if (chanel == 1) {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":1}';
      } else {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "L2D") {
      if (chanel == 1) {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":1}';
      } else {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "L3U") {
      if (chanel == 1) {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":1}';
      } else {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "L3D") {
      if (chanel == 1) {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":1}';
      } else {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "L4U") {
      if (chanel == 1) {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":1}';
      } else {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "L4D") {
      if (chanel == 1) {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":1}';
      } else {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "LAU") {
      if (chanel == 1) {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":1}';
      } else {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "LAD") {
      if (chanel == 1) {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":1}';
      } else {
        s += 'json?j={"division":"$division","Chanel":$chanel,"Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    } else if (division == "wifiSend") {
      // Hot Spot Mode를 위한 ssid, password 전송...
      if (chanel == 0) {
        // esp32에서 해당 값 읽어오기...
        s += 'json?j={"division":"$division","Chanel":$chanel}';
      } else if (chanel == 1) {
        // esp32로 해당 값 전송하기...
        s +=
            'json?j={"division":"$division","ssid":"${Var.ssid}","password":"${Var.password}","Chanel":$chanel}';
        // if (wiFiCheck) response = await dio.get(s);
      }

      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      Var.ssid = parsedJson['ssid'];
      Var.password = parsedJson['password'];
      print(response.statusCode); // 송수신이 정상이면 "200"
      print(response.statusMessage); // 송수신이 정상이면 'OK'
      print(parsedJson); // 송수신이 정상이면 'OK'
    } else if (division == "POWER") {
      if (chanel == 1) {
        s += 'json?j={"division":"$division","POWER":$chanel,"Value":1}';
      } else {
        s += 'json?j={"division":"$division","POWER":$chanel,"Value":0}';
      }
      if (wiFiCheck) response = await dio.get(s);
      var parsedJson = jsonDecode(response.data);
      var result = parsedJson['result'];
      print("parsedJson result: $result");
    }

    if (response.statusCode == 200) {
      wifiCount = 0;
    }

    dio.close();
  }
}
