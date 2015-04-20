#! /usr/local/bin/python

# compile with python2.6 ./test.py 666 66 0
import objc
import sys
try:
        x=int(sys.argv[1])
        y=int(sys.argv[2])
        try:
                delay=int(sys.argv[3])
        except:
                delay=0
except:
        print "USAGE mouseclick [int x] [int y] [optional delay in seconds]"
        exit()
print "mouse click at ", x, ",", y," in ", delay, "seconds"
import time
from Quartz.CoreGraphics import CGEventCreateMouseEvent
from Quartz.CoreGraphics import CGEventPost
from Quartz.CoreGraphics import kCGEventMouseMoved
from Quartz.CoreGraphics import kCGEventLeftMouseDown
from Quartz.CoreGraphics import kCGEventLeftMouseDown
from Quartz.CoreGraphics import kCGEventLeftMouseDragged
from Quartz.CoreGraphics import kCGEventLeftMouseUp
from Quartz.CoreGraphics import kCGMouseButtonLeft
from Quartz.CoreGraphics import kCGHIDEventTap
from Quartz.CoreGraphics import kCGEventKeyDown
from Quartz.CoreGraphics import kCGEventTabletPointer
from Quartz.CoreGraphics import kCGEventTabletProximity
from Quartz.CoreGraphics import CGEventSetIntegerValueField
from Quartz.CoreGraphics import kCGMouseEventClickState
from Quartz.CoreGraphics import CGEventSetType
def mouseEvent(type, posx, posy):
        theEvent = CGEventCreateMouseEvent(None, type, (posx,posy), kCGMouseButtonLeft)
        CGEventPost(kCGHIDEventTap, theEvent)
def mousemove(posx,posy):
		mouseEvent(kCGEventMouseMoved, posx,posy);
		#mouseEvent(kCGEventTabletProximity, posx,posy);
		#mouseEvent(kCGEventLeftMouseDown, posx,posy);
		#mouseEvent(kCGEventLeftMouseUp, posx,posy);
def mouseclick(posx,posy):
        mouseEvent(kCGEventLeftMouseDown, posx,posy);
        mouseEvent(kCGEventLeftMouseUp, posx,posy);


def doubleClick(posx, posy, clickCount):
		theEvent = CGEventCreateMouseEvent(None, kCGEventLeftMouseDown, (posx,posy), kCGMouseButtonLeft); 
		CGEventSetIntegerValueField(theEvent, kCGMouseEventClickState, clickCount);  
		CGEventPost(kCGHIDEventTap, theEvent);
		CGEventSetType(theEvent, kCGEventLeftMouseUp);  
		CGEventPost(kCGHIDEventTap, theEvent);  
		CGEventSetType(theEvent, kCGEventLeftMouseDown);  
		CGEventPost(kCGHIDEventTap, theEvent);  
		CGEventSetType(theEvent, kCGEventLeftMouseUp); 
		CGEventPost(kCGHIDEventTap, theEvent); 
		


        
time.sleep(delay);
mousemove(x,y);
doubleClick(x,y,1);
doubleClick(x,y,1);
#mouseclick(x,y);
#mousemove(730,267);
#doubleClick(730,267,2);
#time.sleep(0.01);
#mouseclick(268,332);
print "done."
