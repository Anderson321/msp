import RPi.GPIO as GPIO
from time import sleep
import time

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)

ECHO_TRIG_H = 20
ECHO_H = 21

GPIO.setup(ECHO_TRIG_H, GPIO.OUT, initial=GPIO.LOW)
GPIO.setup(ECHO_H, GPIO.IN)
pulse_start = long(0)
pulse_end = long(0)

for x in range(0,30):
    GPIO.output(ECHO_TRIG_H, 1)
    sleep(.00001)
    GPIO.output(ECHO_TRIG_H, 0)

    while GPIO.input(ECHO_H)==0:
        pulse_start = time.time()
    while GPIO.input(ECHO_H)==1:
        pulse_end = time.time()

    pulse_duration = pulse_end - pulse_start 
    height = round(pulse_duration * 17150, 3)
    print('Echo (cm):', height)

	with open('pipeH', 'w') as pipeH:
        print("writing to pipeH now...")
        pipeH.write(height)
    sleep(.4)    

print("Done_trial")
pipeH.close()