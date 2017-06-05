from picamera.array import PiRGBArray
from picamera import PiCamera
import time
import numpy as np
import argparse
import cv2

# initialize camera and grab a reference to raw camera capture
camera = PiCamera()
width, height = 640, 480
camera.resolution = (width, height)
camera.framerate = 4
camera.vflip = True
camera.hflip = True
rawCapture = PiRGBArray(camera, size=(width, height))

## Sky Blue Detection
lowerb = np.array([120,95,40],dtype="uint8")
upperb = np.array([185,150,100], dtype="uint8")
## Yellow Detection
lowery = np.array([0, 80, 80],dtype="uint8")
uppery = np.array([70, 220, 220], dtype="uint8")

inside = 10
cent2cent = 11.7
outside = 13.4
desired = 35.5
aimStr = "Actual Distance: " + str(desired) + " inches"

# capture frames from the camera
for frame in camera.capture_continuous(rawCapture, format="bgr", use_video_port=True):
    raw = frame.array
    image = cv2.GaussianBlur(raw, (5,5), 0) 
    
    maskb = cv2.inRange(image, lowerb, upperb)
    masky = cv2.inRange(image, lowery, uppery)

    imageb, contoursb, hierarchyb = cv2.findContours(maskb,cv2.RETR_LIST,cv2.CHAIN_APPROX_NONE)
    imagey, contoursy, hierarchyy = cv2.findContours(masky,cv2.RETR_LIST,cv2.CHAIN_APPROX_SIMPLE)
    sortedContoursb = sorted(contoursb, key=cv2.contourArea, reverse=True)
    sortedContoursy = sorted(contoursy, key=cv2.contourArea, reverse=True)
    
##  Create Ellipse
    ellipseb, ellipsey = 0, 0
    xb, yb, xy, yy, = 0, 0, 0, 0
    if len(sortedContoursb) > 0:
        if len(sortedContoursb[0]) > 5:
            ellipseb = cv2.fitEllipse(sortedContoursb[0])
            (xb, yb), (MA, ma), angle = ellipseb
            cv2.ellipse(image,ellipseb,(0,255,0),2)

    if len(sortedContoursy) > 0:
        if len(sortedContoursy[0]) > 5:
            ellipsey = cv2.fitEllipse(sortedContoursy[0])
            (xy, yy), (MA, ma), angle = ellipsey
            cv2.ellipse(image,ellipsey,(0,255,0),2)
            
    if (xb != 0) and (xy != 0):
        pixel = xb - xy
        dist = abs(round(510*cent2cent/pixel,1))
        cv2.line(image, (int(xb),int(yb)), (int(xy),int(yy)), (0,0,255),2)
        distStr = "Detected Distance: " + str(dist) + " inches"
        cv2.putText(image, distStr, (0, height-15), cv2.FONT_HERSHEY_SIMPLEX, 1,(0,0,255), 2)
        print(distStr)
##        print(pixel)

##  Box Method (not terribly useful rn)
    if (False):
        if len(contoursb) > 0:
            cntb = contoursb[0]
        if len(contoursy) > 0:
            cnty = contoursy[0]
            
        rectblue = cv2.maxAreaRect(cntb)
        boxblue = cv2.boxPoints(rectblue)
        boxblue = np.int0(boxblue)
        cv2.drawContours(image, [boxblue],0,(255,0,0))
        
        rectyellow = cv2.miAreaRect(cnty)
        boxyellow = cv2.boxPoints(rectyellow)
        boxyellow = np.int0(boxyellow)
        cv2.drawContours(image, [boxyellow],0,(0,255,255))

    # Display Image
    cv2.putText(image, aimStr, (0,30), cv2.FONT_HERSHEY_SIMPLEX, 1,(0,0,255), 2)
    output = cv2.bitwise_and(image, image, mask = maskb)
    output1 = cv2.bitwise_and(image, image, mask = masky)
    outputTot = cv2.addWeighted(output,1,output1,1,0)
    cv2.imshow("images", np.hstack([image, outputTot]))

    # clear the stream in preparation for next frame
    rawCapture.truncate(0)
    
    # if the `q` key was pressed, break from the loop
    key = cv2.waitKey(1) & 0xFF
    if key == ord("q"):
        break
