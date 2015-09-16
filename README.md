# bgm_selficam_stabilizer
control system for selficam(phone with monopod) stabilizer

2015 시스템 프로그램 설계 최종 과제

셀카봉에 서보모터 2개를 달아 카메라(폰)이 바라보는 방향을 조절하도록 제어. 각 모터는 pitch, yaw를 담당.
카메라가 장착되는 곳엔 3축 가속도 센서를 설치.
가속도 센서를 통해 카메라가 기울어졌음을 인식하면 서보모터를 제어하여 카메라가 정면을 바라보도록 제어한다.
이 제어시스템은 uCOS-III를 기반으로 제작되었고 STM32F107 보드에서 동작한다.

The selficam stabilizer I made is constructed with a monopod, two servo mortor and a 3 axis accelerometer.
Servo motors control a cam's directon. One for pitch, the other is yaw.
The accelerometer sense cam's slanting degree.
This system monitors accelerometer's value.
If accelerometer tells the camera is slanting, this system make the camera to look forward by controlling servo motors.
This system based on uCOS-III, STM32F107.
