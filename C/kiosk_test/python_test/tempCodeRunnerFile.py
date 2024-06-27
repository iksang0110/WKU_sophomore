# import cv2
# import speech_recognition as sr
# from gtts import gTTS
# import playsound
# import os
# import serial
# import time

# # 메뉴 목록
# menu = {
#     "아이스 아메리카노": 4000,
#     "에스프레소": 3000,
#     "아이스티": 5000,
#     "아메리카노" : 4000,
# }

# # 시리얼 포트 설정 (로컬 환경에서 실행)
# arduino = serial.Serial('/dev/tty.usbserial-14220', 9600)  # 시리얼 포트와 속도 설정

# # TTS 함수 (gtts 및 playsound 사용)
# def speak(text):
#     tts = gTTS(text=text, lang='ko')
#     filename = "voice.mp3"
#     tts.save(filename)
#     playsound.playsound(filename)
#     os.remove(filename)

# # 얼굴 인식 및 모터 제어 함수
# def detect_and_move():
#     face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
#     eye_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_eye.xml')
#     cap = cv2.VideoCapture(0)
#     face_detected = False
#     start_time = 0
#     stable_position = False

#     while True:
#         ret, frame = cap.read()
#         gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
#         faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

#         if len(faces) == 0:
#             # 얼굴이 감지되지 않으면 "STOP" 명령 전송
#             arduino.write(b'STOP\n')
#             print("멈춤")
#             stable_position = False
#         else:
#             for (x, y, w, h) in faces:
#                 # 얼굴 영역에서 눈 검출
#                 roi_gray = gray[y:y+h, x:x+w]
#                 roi_color = frame[y:y+h, x:x+w]
#                 eyes = eye_cascade.detectMultiScale(roi_gray)
                
#                 eye_centers = []
#                 for (ex, ey, ew, eh) in eyes:
#                     # 눈의 중심 좌표 계산
#                     eye_center = (x + ex + ew//2, y + ey + eh//2)
#                     eye_centers.append(eye_center)
#                     # 눈 영역에 점 그리기
#                     cv2.circle(frame, eye_center, 5, (0, 255, 0), 2)
#                     # 한번에 두개의 눈만 처리합니다.
#                     if len(eye_centers) == 2:
#                         break

#                 if len(eye_centers) == 2:
#                     # 두 눈의 중심점 중간 좌표 계산
#                     eye_center_y = (eye_centers[0][1] + eye_centers[1][1]) // 2
#                     frame_height = frame.shape[0]
#                     virtual_line = frame_height // 2

#                     # 눈의 중심이 가상의 선보다 위에 있는지 아래에 있는지 확인
#                     if eye_center_y > virtual_line:
#                         # 아래쪽에 있으면 모터를 아래로 돌림
#                         arduino.write(b'DOWN\n')
#                         print("아래")
#                         position_text = "Below Line"
#                     else:
#                         # 위쪽에 있으면 모터를 위로 올림
#                         arduino.write(b'UP\n')
#                         print("위")
#                         position_text = "Above Line"

#                     # 얼굴이 동일 위치에 2초 동안 있는지 확인
#                     if stable_position and (time.time() - start_time >= 5):
#                         # 얼굴 인식 완료, 모터 제어 멈춤
#                         arduino.write(b'STOP\n')
#                         cap.release()
#                         cv2.destroyAllWindows()
#                         return True

#                     if not stable_position:
#                         start_time = time.time()
#                         stable_position = True

#                     # 얼굴 영역에 사각형 그리기 및 위치 텍스트 그리기
#                     cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)
#                     cv2.putText(frame, position_text, (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (36, 255, 12), 2)

#         # 화면에 출력
#         cv2.imshow('Face Detection', frame)
#         if cv2.waitKey(1) & 0xFF == ord('q'):
#             break

#     cap.release()
#     cv2.destroyAllWindows()
#     return False

# # 음성 인식 함수
# def recognize_speech():
#     recognizer = sr.Recognizer()
#     mic = sr.Microphone()

#     with mic as source:
#         print("음성을 인식하는 중입니다. 말씀해주세요...")
#         recognizer.adjust_for_ambient_noise(source)
#         audio = recognizer.listen(source)

#     try:
#         speech = recognizer.recognize_google(audio, language="ko-KR")
#         print(f"사용자: {speech}")
#         return speech
#     except sr.UnknownValueError:
#         print("음성을 인식할 수 없습니다.")
#         return None
#     except sr.RequestError as e:
#         print(f"음성 인식 서비스 오류: {e}")
#         return None

# # 메인 함수
# def main():
#     while True:
#         if detect_and_move():
#             speak("얼굴을 인식했습니다. 주문을 말씀해주세요.")
#             order = recognize_speech()

#             if order and order in menu:
#                 price = menu[order]
#                 speak(f"{order}를 주문하셨습니다. 가격은 {price}원 입니다.")
#                 print(f"{order}: {price}원 주문 완료")
#             else:
#                 speak("죄송합니다, 메뉴에 없는 항목입니다.")
#                 print("메뉴에 없는 항목")

# if __name__ == "__main__":
#     main()

import cv2
import speech_recognition as sr
from gtts import gTTS
import playsound
import os
import serial
import time

# 메뉴 목록
menu = {
    "아이스 아메리카노": 4000,
    "에스프레소": 3000,
    "아이스티": 5000,
    "아메리카노" : 4000,
}

# 시리얼 포트 설정 (로컬 환경에서 실행)
arduino = serial.Serial('/dev/tty.usbserial-14220', 9600)  # 시리얼 포트와 속도 설정

# TTS 함수 (gtts 및 playsound 사용)
def speak(text):
    tts = gTTS(text=text, lang='ko')
    filename = "voice.mp3"
    tts.save(filename)
    playsound.playsound(filename)
    os.remove(filename)

# 얼굴 인식 및 모터 제어 함수
def detect_and_move():
    face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
    eye_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_eye.xml')
    cap = cv2.VideoCapture(0)
    face_detected = False
    start_time = 0
    stable_position = False

    while True:
        ret, frame = cap.read()
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

        if len(faces) == 0:
            # 얼굴이 감지되지 않으면 "STOP" 명령 전송
            arduino.write(b'STOP\n')
            print("멈춤")
            stable_position = False
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
                    frame_height = frame.shape[0]
                    virtual_line = frame_height // 2

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

                    # 얼굴이 동일 위치에 2초 동안 있는지 확인
                    if stable_position and (time.time() - start_time >= 5):
                        # 얼굴 인식 완료, 모터 제어 멈춤
                        arduino.write(b'STOP\n')
                        cap.release()
                        cv2.destroyAllWindows()
                        return True

                    if not stable_position:
                        start_time = time.time()
                        stable_position = True

                    # 얼굴 영역에 사각형 그리기 및 위치 텍스트 그리기
                    cv2.rectangle(frame, (x, y), (x+w, y+h), (255, 0, 0), 2)
                    cv2.putText(frame, position_text, (x, y-10), cv2.FONT_HERSHEY_SIMPLEX, 0.9, (36, 255, 12), 2)

        # 화면에 출력
        cv2.imshow('Face Detection', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()
    return False

# 음성 인식 함수
def recognize_speech():
    recognizer = sr.Recognizer()
    mic = sr.Microphone()

    with mic as source:
        print("음성을 인식하는 중입니다. 말씀해주세요...")
        recognizer.adjust_for_ambient_noise(source)
        audio = recognizer.listen(source)

    try:
        speech = recognizer.recognize_google(audio, language="ko-KR")
        print(f"사용자: {speech}")
        return speech
    except sr.UnknownValueError:
        print("음성을 인식할 수 없습니다.")
        return None
    except sr.RequestError as e:
        print(f"음성 인식 서비스 오류: {e}")
        return None

# JPG 파일을 표시하는 함수
def display_image(image_path, window_name="Image", delay=1000):
    image = cv2.imread(image_path)
    cv2.imshow(window_name, image)
    cv2.waitKey(delay)
    cv2.destroyAllWindows()

# 메인 함수
def main():
    while True:
        if detect_and_move():
            # Display initial welcome image
            display_image('/Users/02.011x/Documents/GitHub/WKU_sophomore/C/kiosk_test/python_test/welcome.jpg')  # Add the path to your JPG file here
            speak("어서오세요 원광 카페입니다. ")
            
            speak("큰 글자 간편 주문으로 바꾸시겠습니까? ")
            easy_ui = input("쉬운 주문으로  바꾸시겠습니까? (Y/N): ").strip().upper()

            if easy_ui == 'Y':
                # Display easy UI image
                display_image('/Users/02.011x/Documents/GitHub/WKU_sophomore/C/kiosk_test/python_test/easy_ui.jpg')  # Add the path to your JPG file here
                speak("큰 글자 간편 주문으로 변경되었습니다. 음성으로 주문하시겠습니까? ")
                voice_order = input("음성으로 주문하시겠습니까? (Y/N): ").strip().upper()

                if voice_order == 'Y':
                    speak("주문을 말씀해주세요.")
                    order = recognize_speech()

                    if order and order in menu:
                        price = menu[order]
                        speak(f"{order}를 주문하셨습니다. 가격은 {price}원 입니다.")
                        # Display order confirmation image
                        display_image('/Users/02.011x/Documents/GitHub/WKU_sophomore/C/kiosk_test/python_test/order_confirmation.jpg')  # Add the path to your JPG file here
                        print(f"{order}: {price}원 주문 완료")
                    else:
                        speak("죄송합니다, 메뉴에 없는 항목입니다.")
                        print("메뉴에 없는 항목")
            else:
                speak("주문을 말씀해주세요.")
                order = recognize_speech()

                if order and order in menu:
                    price = menu[order]
                    speak(f"{order}를 주문하셨습니다. 가격은 {price}원 입니다.")
                    # Display order confirmation image
                    display_image('order_confirmation.jpg')  # Add the path to your JPG file here
                    print(f"{order}: {price}원 주문 완료")
                else:
                    speak("죄송합니다, 메뉴에 없는 항목입니다.")
                    print("메뉴에 없는 항목")

if __name__ == "__main__":
    main()
