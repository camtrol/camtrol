bool logoFlag = true;
String logoText = '   풀하우스캠핑카 V5.0';

class V {
  // '' null로 초기화 하면 error 발생...
  static String conSSID = ' '; // AP Mode 일때 IP Address...
  static String conPW = ' '; // AP Mode 일때 IP Address...
  static List<String> outName = [' ', ' ', ' ', ' ', ' ', ' ', ' '];
  static List<String> spName = [' ', ' ', ' ', '난방 1', '난방 2'];
  static List<String> ledName = [' ', ' ', ' ', ' ', ' ', ' ', ' '];
  static List<String> ledSaveName = [
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
  ];
  static List<String> outSaveName = [
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
    ' ',
  ];
  static List<String> ledPushName = [
    '',
    '실내등', // 10A
    '간접등', // 10A
    '주방등', // 10A
    '벙커등', // 10A
    '욕실등', // 10A
    '어닝등' // 10A
  ];
  static List<String> outPushName = [
    '',
    '현관등', // 10A
    '물펌프', // 20A
    '맥스팬', // 10A
    '인버터팬', // 10A
    '보일러', // 1A Poly Switch
    '냉장고' // 20A
  ];
  static List<String> motorName = ['', '히  터', '벙  커', '거  실'];
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
    100.0
  ];
  static List<double> motorValue = [100.0, 100.0, 100.0, 100.0];
  static List<double> spValue = [100.0, 100.0, 100.0, 100.0, 100.0];
  static List<bool> ledStatus = [
    false,
    false,
    false,
    false,
    false,
    false,
    false
  ];
  static List<bool> motorStatus = [false, false, false, false];
  static List<bool> outStatus = [
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

  static bool invertorStatus = false;
  static bool allLedStatus = false;

  static List<int> cleanLevelValue = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];
  static List<int> wasteLevelValue = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0];

  static String btPercentIcon = 'images/Battery_0.png';
  static double btVolt = 0.0;
  // static int btPercent = 0;
  static double temperature = 0.0;
  static double setTempValue = 25.0;

  static double ratio = 0.0;

  static int fuel = 0;
  static bool fuelSensor = false;
  static int cleanWater = 0;
  static bool cleanSensor = false;
  static int wasteWater = 0;
  static bool wasteSensor = false;
  static bool cleanSensorType = true;
  static bool wasteSensorType = true;
  static String monitor = ' ';
  static double textScaleRatio = 0.0;
}
