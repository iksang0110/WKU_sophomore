// 가변저항을 이용한 리니어 이송레일 정역제어

/* 라이브러리 include */
#include <HCMotor.h>

/* 모터드라이버 연결핀 */
#define DIR_PIN 8 //스텝모터드라이버 DIR 연결핀
#define CLK_PIN 9 //스텝모터드라이버 CLK 연결핀

/* 가변저항 연결핀 */
#define POT_PIN A0

/* 스텝모터 구동 중간지점(데드존 영역) */
#define DEADZONE 20 

/* 아날로그 입력값 0~1024 값으로 정역영역지정 */
#define POT_REV_MIN 0
#define POT_REV_MAX (512 - DEADZONE)
#define POT_FWD_MIN (512 + DEADZONE)
#define POT_FWD_MAX 1024

/* HCMotor 라이브러리 인스턴스 생성 */
HCMotor HCMotor;

void setup() 
{

  /* 라이브러리 초기화 */
  HCMotor.Init();

  /* 모터0을 스텝모터로 설정하고 연결된 핀을 지정 */
  HCMotor.attach(0, STEPPER, CLK_PIN, DIR_PIN);

  /* 모터를 연속동작모드로 설정*/
  HCMotor.Steps(0,CONTINUOUS);
}


void loop() 
{
  int Speed, Pot;

  /* 가변저항으로 부터 아날로그 값을 입력받음 */ 
  Pot = analogRead(POT_PIN);

  /* 역방향? */
  if (Pot >= POT_REV_MIN && Pot <= POT_REV_MAX)
  {
    HCMotor.Direction(0, REVERSE);
    Speed = map(Pot, POT_REV_MIN, POT_REV_MAX, 10, 1024);

  /* 정방향? */
  }else if (Pot >= POT_FWD_MIN && Pot <= POT_FWD_MAX)
  {
    HCMotor.Direction(0, FORWARD);
    Speed = map(Pot, POT_FWD_MIN, POT_FWD_MAX, 1024, 10);

  /* 데드존(중간지점)? */
  }else
  {
    Speed = 0;
  }

  /* 속도설정 */
  HCMotor.DutyCycle(0, Speed);

}