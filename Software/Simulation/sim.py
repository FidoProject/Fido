from graphics import *
import matplotlib.colors as colors
import time

framerate = 30

r = g = b = 50
m1 = 100; m2 = -50
vibration = False

accelX = accelY = accelZ = 0
gyroX = gyroY = gyroZ = 0
compX = compY = compZ = 0

win = GraphWin("Fido Simulator", 500, 300)

while True:
	# Mouse inputs
	mousePoint = win.checkMouse()
	if mousePoint:
		if mousePoint.getX() > 350: #imu
			if mousePoint.getY() < 100:
				if mousePoint.getX() > 450:
					accelZ = 50-mousePoint.getY()
				elif mousePoint.getX() > 400:
					accelY = 50-mousePoint.getY()
				elif mousePoint.getX() > 350:
					accelX = 50-mousePoint.getY()
			elif mousePoint.getY() < 200:
				if mousePoint.getX() > 450:
					gyroZ = 150-mousePoint.getY()
				elif mousePoint.getX() > 400:
					gyroY = 150-mousePoint.getY()
				elif mousePoint.getX() > 350:
					gyroX = 150-mousePoint.getY()
			else:
				if mousePoint.getX() > 450:
					compZ = 250-mousePoint.getY()
				elif mousePoint.getX() > 400:
					compY = 250-mousePoint.getY()
				elif mousePoint.getX() > 350:
					compX = 250-mousePoint.getY()
	
	# imu graphics
	imuLine = Line(Point(350,0),Point(350,300))
	imuLine.draw(win)
	accelLine = Line(Point(400,0),Point(400,300))
	accelLine.draw(win)
	gyroLine = Line(Point(400,0),Point(400,300))
	gyroLine.draw(win)
	compLine = Line(Point(450,0),Point(450,300))
	compLine.draw(win)
	accelCross = Line(Point(350,100),Point(500,100))
	accelCross.draw(win)
	gyroCross = Line(Point(350,200),Point(500,200))
	gyroCross.draw(win)
	compCross = Line(Point(350,200),Point(500,200))
	compCross.draw(win)
	accelText = Text(Point(300,50),"Accel XYZ")
	accelText.draw(win)
	gyroText = Text(Point(300,150),"Gyro XYZ")
	gyroText.draw(win)
	compText = Text(Point(300,250),"Compass XYZ")
	compText.draw(win)
	

	# rgb led
	ledText = Text(Point(40,30),"RGB Led:")
	ledText.draw(win)
	led = Circle(Point(90,30),15)
	led.setFill(colors.rgb2hex((r,g,b)))
	led.draw(win)

	# vibration
	vibText = Text(Point(39,80),"Vibration:")
	vibText.draw(win)
	vib = Circle(Point(90,80),15)
	if vibration: vib.setFill("red")
	vib.draw(win)

	# motors
	mot1Text = Text(Point(150,150),"Motor One")
	mot2Text = Text(Point(220,150),"Motor Two")
	mot1Text.draw(win); mot2Text.draw(win)
	if not m1 == 0:
		if m1>0: mot1Mag = Line(Point(150,130),Point(150,130-(m1)))
		else: mot1Mag = Line(Point(150,170),Point(150,170-(m1)))
		mot1Mag.setArrow("last")
		mot1Mag.draw(win)
	if not m1 == 0:
		if m2>0: mot2Mag = Line(Point(220,130),Point(220,130-(m2)))
		else: mot2Mag = Line(Point(220,170),Point(220,170-(m2)))
		mot2Mag.setArrow("last")
		mot2Mag.draw(win)

	# refresh frame
	time.sleep(1/framerate)
	win.flush()
win.close() 