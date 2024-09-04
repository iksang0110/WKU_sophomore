// #include <Stepper.h>

// const int stepsPerRevolution = 200;  // 모터의 한 바퀴 당 스텝 수
// const int stepDelay = 0.1; // 모터 속도를 조절하기 위한 딜레이 (밀리초)

// Stepper myStepper(stepsPerRevolution, 8, 9);  // 스텝 모터 객체 생성

// String currentCommand = "";  // 현재 명령 저장 변수

// void setup() {
//   // 시리얼 통신 초기화
//   Serial.begin(9600);
//   // 모터 속도 설정 (필요에 따라 조정 가능)
//   myStepper.setSpeed(20);  
// }

// void loop() {
//   // 시리얼 입력이 있을 경우
//   if (Serial.available() > 0) {
//     String command = Serial.readStringUntil('\n');
//     currentCommand = command;  // 새로운 명령을 현재 명령으로 설정
//   }

//   // 현재 명령에 따라 모터를 제어
//   if (currentCommand == "UP") {
//     myStepper.step(1);  // 모터를 위로 한 스텝 이동
//     delay(stepDelay);   // 딜레이를 추가하여 속도 조절
//   } else if (currentCommand == "DOWN") {
//     myStepper.step(-1);  // 모터를 아래로 한 스텝 이동
//     delay(stepDelay);    // 딜레이를 추가하여 속도 조절
//   }
// }
//--------------------------------------------------------------------------------

#include <Stepper.h>

const int stepsPerRevolution = 200;  // 모터의 한 바퀴 당 스텝 수
const int stepDelay = 0.1; // 모터 속도를 조절하기 위한 딜레이 (밀리초)

Stepper myStepper(stepsPerRevolution, 8, 9);  // 스텝 모터 객체 생성

String currentCommand = "";  // 현재 명령 저장 변수

void setup() {
  // 시리얼 통신 초기화
  Serial.begin(9600);
  // 모터 속도 설정 (필요에 따라 조정 가능)
  myStepper.setSpeed(2000);  
}

void loop() {
  // 시리얼 입력이 있을 경우
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    currentCommand = command;  // 새로운 명령을 현재 명령으로 설정
  }

  // 현재 명령에 따라 모터를 제어
  if (currentCommand == "UP") {
    myStepper.step(-1);  // 모터를 위로 한 스텝 이동
    delay(stepDelay);   // 딜레이를 추가하여 속도 조절
  } else if (currentCommand == "DOWN") {
    myStepper.step(1);  // 모터를 아래로 한 스텝 이동
    delay(stepDelay);    // 딜레이를 추가하여 속도 조절
  } else if (currentCommand == "STOP") {
    // 현재 명령이 "STOP"이면 모터를 멈춤
    myStepper.step(0);
  }
}
