bool logoFlag = true;
String logoText = '  CARIND  V3.0';

class V {
  // '' null로 초기화 하면 error 발생...
  static String conSSID = ' '; // AP Mode 일때 Network Name...
  static String conPW = ' '; // AP Mode 일때 Password...
  static String hotSpotSSID = ' '; // Hop Spot Mode 일때 Network Name...
  static String hotSpotPW = ' '; // Hop Spot Mode 일때 Password...
  static List<String> ledName = [
    ' ',
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
  static List<String> ledSaveName = [
    ' ',
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

// 카니발 Name List...
  // static List<String> ledPushName = [
  //   '전체등', // 0
  //   '무드등', // 1
  //   '실내등', // 3
  //   '예  비', // 2
  //   '어닝등', // 4
  //   '트렁크', // 5
  //   '시거잭 1', // 6
  //   '시거잭 2', // 7
  //   'TV', // 8
  //   '인버터' // 9
  // ];

// 스타리아 Name List...
  // static List<String> ledPushName = [
  //   '전체등', // 0
  //   '무드등', // 1
  //   '실내등', // 3
  //   '트렁크', // 2
  //   '어닝등', // 4
  //   '물펌프', // 5
  //   '시거잭', // 6
  //   '냉장고', // 7
  //   'TV', // 8
  //   '인버터' // 9
  // ];

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

  static double ratio = 0.0;

  static String monitor = ' ';
  static double textScaleRatio = 0.0;

  static bool comFlag = true;
}
