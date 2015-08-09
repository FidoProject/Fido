from graphics import *
import matplotlib.colors as colors
import time

framerate = 30

r = g = b = 50
m1 = 100; m2 = -50
vibration = False

win = GraphWin("Fido Simulator", 500, 400)

while True:
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
	mot1Text = Text(Point(200,150),"Motor One")
	mot2Text = Text(Point(300,150),"Motor Two")
	mot1Text.draw(win); mot2Text.draw(win)
	if not m1 == 0:
		if m1>0: mot1Mag = Line(Point(200,130),Point(200,130-(m1)))
		else: mot1Mag = Line(Point(200,170),Point(200,170-(m1)))
		mot1Mag.setArrow("last")
		mot1Mag.draw(win)
	if not m1 == 0:
		if m2>0: mot2Mag = Line(Point(300,130),Point(300,130-(m2)))
		else: mot2Mag = Line(Point(300,170),Point(300,170-(m2)))
		mot2Mag.setArrow("last")
		mot2Mag.draw(win)

	# refresh frame
	time.sleep(1/framerate)
	win.flush()
win.close() 