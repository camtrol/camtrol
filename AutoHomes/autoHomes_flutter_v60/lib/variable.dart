bool logoFlag = true;
String logoText = '  오토홈스  V6.0';

class V {
  // '' null로 초기화 하면 error 발생...
  static String conSSID = ' '; // AP Mode 일때 Network Name...
  static String conPW = ' '; // AP Mode 일때 Password...
  static String hotSpotSSID = ' '; // Hop Spot Mode 일때 Network Name...
  static String hotSpotPW = ' '; // Hop Spot Mode 일때 Password...
  static List<String> ledName = [' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '];
  static List<String> ledSaveName = [
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' '
  ];
  // static List<String> outName = [' ', ' ', ' ', ' ', ' ', ' ', ' '];
  // static List<String> spName = [' ', ' ', ' ', '난방 1', '난방 2'];
  static List<String> ledPushName = [
    '전체등', // 0
    '전체등', // 1
    '실내등', // 2
    '사이드', // 3
    '팝업등', // 4
    '시트열선', // 5
    '무드등', // 6
    '후면등', // 7
    '어닝등' // 8
  ];
  static List<String> phoneName = [
    '연락처 0',
    '연락처 1',
    '연락처 2',
    '연락처 3',
    '연락처 4',
    '연락처 5'
  ];
  static List<String> phoneNumber = [
    '전화번호',
    '전화번호',
    '전화번호',
    '전화번호',
    '전화번호',
    '전화번호'
  ];
  static List<double> ledValue = [
    100.0,
    100.0,
    100.0,
    100.0,
    100.0,
    100.0,
    100.0,
    100.0,
    100.0
  ];
  static List<bool> ledStatus = [
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

  static List<bool> isPhoneTable = [false, false, false, false, false, false];

  static double btVolt = 0.0;
  static double temperature = 0.0;
  static double humidity = 0.0;

  static double ratio = 0.0;

  static String monitor = ' ';
  static double textScaleRatio = 0.0;

  static bool comFlag = true;
}
