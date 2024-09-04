import cv2
import serial
import time

# Arduino와 시리얼 통신 설정
arduino = serial.Serial('/dev/tty.usbserial-14220', 9600)
time.sleep(2)  # 시리얼 통신 초기화 대기

# Haar Cascade 파일 로드 (OpenCV에서 제공)
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# 웹캡 캡처 설정
cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break
    
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))
    
    for (x, y, w, h) in faces:
        # 얼굴의 중심 좌표 계산
        face_center_y = y + h // 2
        
        # 화면의 높이 계산
        frame_center_y = frame.shape[0] // 2
        
        # 얼굴의 위치가 화면 중앙보다 위에 있는 경우
        if face_center_y < frame_center_y:
            arduino.write(b'UP')  # 'U'는 Up을 의미
            print("Sending command: U")  # 디버깅 출력
        # 얼굴의 위치가 화면 중앙보다 아래에 있는 경우
        else:
            arduino.write(b'DOWN')  # 'D'는 Down을 의미
            print("Sending command: D")  # 디버깅 출력

    # 프레임에 얼굴 인식 결과 표시
    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)
    
    # 화면에 결과 출력
    cv2.imshow('Frame', frame)
    
    # 'q' 키를 누르면 종료
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# # 종료 후 정리
# cap.release()
# cv2.destroyAllWindows()
# arduino.close()

# # # # # # import cv2
# # # # # # import serial
# # # # # # import time

# # # # # # # 시리얼 포트 설정 (아두이노와 연결된 포트)
# # # # # # arduino = serial.Serial('/dev/tty.usbserial-14220', 9600)  # 'COM3'을 사용 중인 포트로 변경하세요.
# # # # # # time.sleep(2)  # 시리얼 포트가 열리는 시간을 대기

# # # # # # # 얼굴 인식 모델 로드 (OpenCV 제공)
# # # # # # face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

# # # # # # # 비디오 캡처 객체 생성
# # # # # # cap = cv2.VideoCapture(0)

# # # # # # while True:
# # # # # #     ret, frame = cap.read()
# # # # # #     gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
# # # # # #     faces = face_cascade.detectMultiScale(gray, 1.1, 4)

# # # # # #     if len(faces) > 0:
# # # # # #         x, y, w, h = faces[0]  # 첫 번째 얼굴만 사용
# # # # # #         face_center_y = y + h // 2
# # # # # #         frame_center_y = frame.shape[0] // 2

# # # # # #         if face_center_y < frame_center_y - 50:
# # # # # #             arduino.write(b'U')  # 얼굴이 위에 있으면 'U' 전송 (Up)
# # # # # #         elif face_center_y > frame_center_y + 50:
# # # # # #             arduino.write(b'D')  # 얼굴이 아래에 있으면 'D' 전송 (Down)
# # # # # #         else:
# # # # # #             arduino.write(b'S')  # 얼굴이 중앙에 있으면 'S' 전송 (Stop)

# # # # # #     cv2.imshow('frame', frame)

# # # # # #     if cv2.waitKey(1) & 0xFF == ord('q'):
# # # # # #         break

# # # # # #     time.sleep(1)  # 1초마다 얼굴 위치 확인

# # # # # # cap.release()
# # # # # # cv2.destroyAllWindows()
# # # # # # arduino.close()

# # # # # # # import cv2
# # # # # # # import serial
# # # # # # # import time

# # # # # # # # 시리얼 포트 설정
# # # # # # # arduino = serial.Serial('/dev/tty.usbserial-14220', 9600)  # 시리얼 포트와 속도 설정 (포트 번호는 환경에 맞게 변경)

# # # # # # # # 얼굴 인식을 위한 OpenCV 설정
# # # # # # # face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
# # # # # # # cap = cv2.VideoCapture(0)

# # # # # # # while True:
# # # # # # #     ret, frame = cap.read()
# # # # # # #     if not ret:
# # # # # # #         break

# # # # # # #     gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
# # # # # # #     faces = face_cascade.detectMultiScale(gray, 1.1, 4)

# # # # # # #     for (x, y, w, h) in faces:
# # # # # # #         # 얼굴의 중심 좌표 계산
# # # # # # #         face_center_y = y + h // 2

# # # # # # #         # 화면을 상하로 나눔
# # # # # # #         frame_height = frame.shape[0]
# # # # # # #         if face_center_y > frame_height / 2:
# # # # # # #             # 아래쪽에 있으면 모터를 아래로 돌림
# # # # # # #             arduino.write(b'DOWN\n')
# # # # # # #         else:
# # # # # # #             # 위쪽에 있으면 모터를 위로 올림
# # # # # # #             arduino.write(b'UP\n')

# # # # # # #         # 얼굴 영역에 사각형 그리기
# # # # # # #         cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)

# # # # # # #     # 화면에 출력
# # # # # # #     cv2.imshow('frame', frame)

# # # # # # #     if cv2.waitKey(1) & 0xFF == ord('q'):
# # # # # # #         break

# # # # # # # # 자원 해제
# # # # # # # cap.release()
# # # # # # # cv2.destroyAllWindows()
# # # # # # # arduino.close()

# # # # # # # import serial
# # # # # # # import sys
# # # # # # # import termios
# # # # # # # import atexit
# # # # # # # import tty
# # # # # # # from select import select

# # # # # # # # 시리얼 포트 설정
# # # # # # # arduino = serial.Serial('/dev/tty.usbserial-14220', 9600)  # 시리얼 포트와 속도 설정 (포트 번호는 환경에 맞게 변경)

# # # # # # # # 기존 터미널 설정 저장
# # # # # # # old_settings = termios.tcgetattr(sys.stdin)

# # # # # # # def restore_terminal_settings():
# # # # # # #     termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)

# # # # # # # atexit.register(restore_terminal_settings)

# # # # # # # # 터미널을 raw 모드로 설정
# # # # # # # tty.setcbreak(sys.stdin.fileno())

# # # # # # # print("Press 'U' to move UP and 'D' to move DOWN. Press 'Q' to quit.")

# # # # # # # try:
# # # # # # #     while True:
# # # # # # #         if select([sys.stdin], [], [], 0.1)[0]:
# # # # # # #             key = sys.stdin.read(1)
# # # # # # #             if key.lower() == 'q':
# # # # # # #                 break
# # # # # # #             elif key.lower() == 'u':
# # # # # # #                 # 'U' 키를 누르면 위로 이동
# # # # # # #                 arduino.write(b'UP\n')
# # # # # # #             elif key.lower() == 'd':
# # # # # # #                 # 'D' 키를 누르면 아래로 이동
# # # # # # #                 arduino.write(b'DOWN\n')
# # # # # # # finally:
# # # # # # #     restore_terminal_settings()
# # # # # # #     arduino.close()

# # # # import cv2
# # # # import serial
# # # # import time

# # # # # 시리얼 포트 설정
# # # # arduino = serial.Serial('/dev/tty.usbserial-14220', 9600)  # 시리얼 포트와 속도 설정 (포트 번호는 환경에 맞게 변경)

# # # # # 얼굴 인식을 위한 OpenCV 설정
# # # # face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
# # # # cap = cv2.VideoCapture(0)

# # # # face_detected = False

# # # # while not face_detected:
# # # #     ret, frame = cap.read()
# # # #     if not ret:
# # # #         break

# # # #     gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
# # # #     faces = face_cascade.detectMultiScale(gray, scaleFactor=1.3, minNeighbors=5, minSize=(30, 30))

# # # #     for (x, y, w, h) in faces:
# # # #         # 얼굴의 중심 좌표 계산
# # # #         face_center_y = y + h // 2

# # # #         # 화면을 상하로 나눔
# # # #         frame_height = frame.shape[0]
# # # #         if face_center_y > frame_height / 2:
# # # #             # 아래쪽에 있으면 모터를 아래로 돌림
# # # #             arduino.write(b'DOWN\n')
# # # #             print("Sending command: D")
# # # #         else:
# # # #             # 위쪽에 있으면 모터를 위로 올림
# # # #             arduino.write(b'UP\n')
# # # #             print("Sending command: U")
# # # #         face_detected = True
# # # #         break

# # # #     # 얼굴 영역에 사각형 그리기
# # # #     for (x, y, w, h) in faces:
# # # #         cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)

# # # #     # 화면에 출력
# # # #     cv2.imshow('frame', frame)

# # # #     if cv2.waitKey(1) & 0xFF == ord('q'):
# # # #         break

# # # # # 자원 해제
# # # # cap.release()
# # # # cv2.destroyAllWindows()
# # # # arduino.close()
# # # import cv2
# # # import serial
# # # import time

# # # # 시리얼 포트 설정
# # # arduino = serial.Serial('/dev/tty.usbserial-14220', 9600)  # 시리얼 포트와 속도 설정

# # # # 얼굴 인식을 위한 OpenCV 설정
# # # face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
# # # cap = cv2.VideoCapture(0)

# # # face_detected = False

# # # while not face_detected:
# # #     ret, frame = cap.read()
# # #     if not ret:
# # #         break

# # #     gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
# # #     faces = face_cascade.detectMultiScale(gray, scaleFactor=1.3, minNeighbors=5, minSize=(30, 30))

# # #     for (x, y, w, h) in faces:
# # #         # 얼굴의 중심 좌표 계산
# # #         face_center_y = y + h // 2

# # #         # 가상의 선을 화면의 중앙에 설정
# # #         frame_height = frame.shape[0]
# # #         virtual_line = frame_height // 2

# # #         # 얼굴이 가상의 선보다 위에 있는지 아래에 있는지 확인
# # #         if face_center_y > virtual_line:
# # #             # 아래쪽에 있으면 모터를 아래로 돌림
# # #             arduino.write(b'DOWN\n')
# # #         else:
# # #             # 위쪽에 있으면 모터를 위로 올림
# # #             arduino.write(b'UP\n')

# # #         face_detected = True
# # #         break

# # #     # 얼굴 영역에 사각형 그리기 및 가상의 선 그리기
# # #     for (x, y, w, h) in faces:
# # #         cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)
# # #     cv2.line(frame, (0, virtual_line), (frame.shape[1], virtual_line), (0, 255, 0), 2)

# # #     # 화면에 출력
# # #     cv2.imshow('frame', frame)

# # #     if cv2.waitKey(1) & 0xFF == ord('q'):
# # #         break

# # # # 자원 해제
# # # cap.release()
# # # cv2.destroyAllWindows()
# # # arduino.close()
# # import cv2
# # import serial
# # import time

# # # 시리얼 포트 설정
# # arduino = serial.Serial('/dev/tty.usbserial-14220', 9600)  # 시리얼 포트와 속도 설정

# # # 얼굴 인식을 위한 OpenCV 설정
# # face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
# # cap = cv2.VideoCapture(0)

# # # 프레임 크기 설정 및 가상의 선 설정
# # ret, frame = cap.read()
# # frame_height = frame.shape[0]
# # virtual_line = frame_height // 2

# # face_detected = False

# # while not face_detected:
# #     ret, frame = cap.read()
# #     if not ret:
# #         break

# #     gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
# #     faces = face_cascade.detectMultiScale(gray, scaleFactor=1.3, minNeighbors=5, minSize=(30, 30))

# #     for (x, y, w, h) in faces:
# #         # 얼굴의 중심 좌표 계산
# #         face_center_y = y + h // 2

# #         # 얼굴이 가상의 선보다 위에 있는지 아래에 있는지 확인
# #         if face_center_y > virtual_line:
# #             # 아래쪽에 있으면 모터를 아래로 돌림
# #             arduino.write(b'DOWN\n')
# #             print("Sending command: D")
# #         else:
# #             # 위쪽에 있으면 모터를 위로 올림
# #             arduino.write(b'UP\n')
# #             print("Sending command: U")

# #         face_detected = True
# #         break

# #     # 얼굴 영역에 사각형 그리기 및 가상의 선 그리기
# #     for (x, y, w, h) in faces:
# #         cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)
# #     cv2.line(frame, (0, virtual_line), (frame.shape[1], virtual_line), (0, 255, 0), 2)

# #     # 화면에 출력
# #     cv2.imshow('frame', frame)

# #     if cv2.waitKey(1) & 0xFF == ord('q'):
# #         break

# # # 자원 해제
# # cap.release()
# # cv2.destroyAllWindows()
# # arduino.close()
# import cv2
# import serial
# import time

# # 시리얼 포트 설정
# arduino = serial.Serial('/dev/tty.usbserial-14220', 9600)  # 시리얼 포트와 속도 설정

# # 얼굴 인식을 위한 OpenCV 설정
# face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
# cap = cv2.VideoCapture(0)

# # 프레임 크기 설정 및 가상의 선 설정
# ret, frame = cap.read()
# frame_height = frame.shape[0]
# virtual_line = frame_height // 2

# face_detected = False

# while not face_detected:
#     ret, frame = cap.read()
#     if not ret:
#         break

#     gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
#     faces = face_cascade.detectMultiScale(gray, scaleFactor=1.3, minNeighbors=5, minSize=(30, 30))

#     for (x, y, w, h) in faces:
#         # 얼굴의 중심 좌표 계산
#         face_center_y = y + h // 2

#         # 얼굴이 가상의 선보다 위에 있는지 아래에 있는지 확인
#         if face_center_y > virtual_line:
#               # 위쪽에 있으면 모터를 위로 올림
#             arduino.write(b'UP\n')
#             print("위")
            
#         else:
#            # 아래쪽에 있으면 모터를 아래로 돌림
#             arduino.write(b'DOWN\n')
#             print("아래")
#         face_detected = True
#         break

#     # 얼굴 영역에 사각형 그리기 및 가상의 선 그리기
#     for (x, y, w, h) in faces:
#         cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)
#     cv2.line(frame, (0, virtual_line), (frame.shape[1], virtual_line), (0, 255, 0), 2)

#     # 화면에 출력
#     cv2.imshow('frame', frame)

#     if cv2.waitKey(1) & 0xFF == ord('q'):
#         break

# # 자원 해제
# cap.release()
# cv2.destroyAllWindows()
# arduino.close()
# 

import cv2
import mediapipe as mp
import serial

# 시리얼 포트 설정
arduino = serial.Serial('/dev/tty.usbserial-14220', 9600)  # 시리얼 포트와 속도 설정

# MediaPipe 얼굴 검출 모델 설정
mp_face_detection = mp.solutions.face_detection
mp_drawing = mp.solutions.drawing_utils

# 얼굴 검출 모델 초기화
face_detection = mp_face_detection.FaceDetection(model_selection=0, min_detection_confidence=0.7)

# 비디오 캡처 초기화
cap = cv2.VideoCapture(0)

# 프레임 크기 설정 및 가상의 선 설정
ret, frame = cap.read()
frame_height, frame_width = frame.shape[:2]
virtual_line = frame_height // 2

face_detected = False

while not face_detected:
    ret, frame = cap.read()
    if not ret:
        break

    # BGR 이미지를 RGB로 변환
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    results = face_detection.process(rgb_frame)

    if results.detections:
        for detection in results.detections:
            # 얼굴의 바운딩 박스 가져오기
            bboxC = detection.location_data.relative_bounding_box
            ih, iw, _ = frame.shape
            (x, y, w, h) = (int(bboxC.xmin * iw), int(bboxC.ymin * ih),
                            int(bboxC.width * iw), int(bboxC.height * ih))
            
            # 얼굴의 중심 좌표 계산
            face_center_y = y + h // 2

            # 얼굴이 가상의 선보다 위에 있는지 아래에 있는지 확인
            if face_center_y > virtual_line:
                # 아래쪽에 있으면 모터를 아래로 돌림
                arduino.write(b'DOWN\n')
                print("아래")
            else:
                # 위쪽에 있으면 모터를 위로 올림
                arduino.write(b'UP\n')
                print("위")

            face_detected = True

            # 얼굴 영역에 사각형 그리기
            cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)
            break

    # 가상의 선 그리기
    cv2.line(frame, (0, virtual_line), (frame_width, virtual_line), (0, 255, 0), 2)

    # 화면에 출력
    cv2.imshow('frame', frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# 자원 해제
cap.release()
cv2.destroyAllWindows()
arduino.close()
