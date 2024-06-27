# import cv2
# import serial

# # 시리얼 포트 설정 (로컬 환경에서 실행)
# arduino = serial.Serial('/dev/tty.usbserial-14220', 9600)  # 시리얼 포트와 속도 설정

# # 얼굴 및 눈 검출기 초기화 (OpenCV Haar Cascade 사용)
# face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
# eye_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_eye.xml')

# # 비디오 캡처 초기화
# cap = cv2.VideoCapture(0)

# # 프레임 크기 설정 및 가상의 선 설정
# ret, frame = cap.read()
# frame_height, frame_width = frame.shape[:2]
# virtual_line = frame_height // 2

# while True:
#     ret, frame = cap.read()
#     if not ret:
#         break

#     gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
#     faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

#     for (x, y, w, h) in faces:
#         # 얼굴 영역에서 눈 검출
#         roi_gray = gray[y:y+h, x:x+w]
#         roi_color = frame[y:y+h, x:x+w]
#         eyes = eye_cascade.detectMultiScale(roi_gray)
        
#         eye_centers = []
#         for (ex, ey, ew, eh) in eyes:
#             # 눈의 중심 좌표 계산
#             eye_center = (x + ex + ew//2, y + ey + eh//2)
#             eye_centers.append(eye_center)
#             # 눈 영역에 점 그리기
#             cv2.circle(frame, eye_center, 5, (0, 255, 0), 2)
#             # 한번에 두개의 눈만 처리합니다.
#             if len(eye_centers) == 2:
#                 break

#         if len(eye_centers) == 2:
#             # 두 눈의 중심점 중간 좌표 계산
#             eye_center_y = (eye_centers[0][1] + eye_centers[1][1]) // 2

#             # 눈의 중심이 가상의 선보다 위에 있는지 아래에 있는지 확인
#             if eye_center_y > virtual_line:
#                 # 아래쪽에 있으면 모터를 아래로 돌림
#                 arduino.write(b'DOWN\n')
#                 print("아래")
#                 position_text = "Below Line"
#             else:
#                 # 위쪽에 있으면 모터를 위로 올림
#                 arduino.write(b'UP\n')
#                 print("위")
#                 position_text = "Above Line"

#             # 위치 텍스트 그리기
#             cv2.putText(frame, position_text, (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (36, 255, 12), 2)
        
#         # 얼굴 영역에 사각형 그리기
#         cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)

#     # 가상의 선 그리기
#     cv2.line(frame, (0, virtual_line), (frame_width, virtual_line), (0, 255, 0), 2)

#     # 화면에 출력
#     cv2.imshow('frame', frame)

#     if cv2.waitKey(1) & 0xFF == ord('q'):
#         break

# # 자원 해제
# cap.release()
# cv2.destroyAllWindows()
# arduino.close()
# ---------------------------------------------------------

import cv2
import serial

# 시리얼 포트 설정 (로컬 환경에서 실행)
arduino = serial.Serial('/dev/tty.usbserial-14220', 9600)  # 시리얼 포트와 속도 설정

# 얼굴 및 눈 검출기 초기화 (OpenCV Haar Cascade 사용)
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
eye_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_eye.xml')

# 비디오 캡처 초기화
cap = cv2.VideoCapture(0)

# 프레임 크기 설정 및 가상의 선 설정
ret, frame = cap.read()
frame_height, frame_width = frame.shape[:2]
virtual_line = frame_height // 2

while True:
    ret, frame = cap.read()
    if not ret:
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

    if len(faces) == 0:
        # 얼굴이 감지되지 않으면 "STOP" 명령 전송
        arduino.write(b'STOP\n')
        print("멈춤")
    else:
        for (x, y, w, h) in faces:
            # 얼굴 영역에서 눈 검출
            roi_gray = gray[y:y+h, x:x+w]
            roi_color = frame[y:y+h, x:x+w]
            eyes = eye_cascade.detectMultiScale(roi_gray)
            
            eye_centers = []
            for (ex, ey, ew, eh) in eyes:
                # 눈의 중심 좌표 계산
                eye_center = (x + ex + ew//2, y + ey + eh//2)
                eye_centers.append(eye_center)
                # 눈 영역에 점 그리기
                cv2.circle(frame, eye_center, 5, (0, 255, 0), 2)
                # 한번에 두개의 눈만 처리합니다.
                if len(eye_centers) == 2:
                    break

            if len(eye_centers) == 2:
                # 두 눈의 중심점 중간 좌표 계산
                eye_center_y = (eye_centers[0][1] + eye_centers[1][1]) // 2

                # 눈의 중심이 가상의 선보다 위에 있는지 아래에 있는지 확인
                if eye_center_y > virtual_line:
                    # 아래쪽에 있으면 모터를 아래로 돌림
                    arduino.write(b'DOWN\n')
                    print("아래")
                    position_text = "Below Line"
                else:
                    # 위쪽에 있으면 모터를 위로 올림
                    arduino.write(b'UP\n')
                    print("위")
                    position_text = "Above Line"

                # 위치 텍스트 그리기
                cv2.putText(frame, position_text, (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (36, 255, 12), 2)
            
            # 얼굴 영역에 사각형 그리기
            cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)

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


# 화면에 jpg 파일을 띄워서 키오스크 ui처럼 뜨게 할거야 jpg 파일 넣을 수 있게 코드 어디에 넣어야 하는지 표시
# 순서 
# 1. 사용자 얼굴 인식 후 모터 제어
# 2. 모터가 멈추면 (어서오세요 원광 카페입니다 음성으로 말하기)<- 여기서 jpg 파일 띄우기 
# 3. 쉬운 UI로 바꾸시겠습니까? 음성으로 물어보고 사용자가 키보드로 Y/N 입력
# 3 Y -> 쉬운 UI로 바꾸기<- 여기서 jpg 파일 띄우기
# 4. Y/N 입력되면 Y일때 UI 바꾸고 음성으로 주문 할건지 물어보기
# 5. Y/N 으로 입력받아서 Y일때 음성인식으로 주문받기
# 6. 주문 받으면 주문 내용을 말 해주고 주문 UI 띄우기 <- 여기서 jpg 파일 띄우기