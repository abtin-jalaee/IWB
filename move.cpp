#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <X11/extensions/XTest.h>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;


void SendClick(int button, Bool down) 
{
    Display *display = XOpenDisplay(NULL);
    XTestFakeButtonEvent(display, button, down, CurrentTime);
    XFlush(display);
    XCloseDisplay(display);
    usleep (1);
}
 
// Get coordinates
void coords (int *x, int *y)
{
Display *display = XOpenDisplay(NULL);
    XEvent event;
    XQueryPointer (display, DefaultRootWindow (display),
                 &event.xbutton.root, &event.xbutton.window,
                 &event.xbutton.x_root, &event.xbutton.y_root,
                 &event.xbutton.x, &event.xbutton.y,
                 &event.xbutton.state);
    *x = event.xbutton.x;
    *y = event.xbutton.y;
    XCloseDisplay(display);
}

// move cursor (absolute)
void moveTo(int x, int y) {
    Display *display = XOpenDisplay(NULL);
    int xTmp = 0, yTmp = 0;
    coords(&xTmp, &yTmp);
    XWarpPointer(display, None, None, 0, 0, 0, 0, -xTmp, -yTmp);
    XWarpPointer(display, None, None, 0, 0, 0, 0, x, y);
    XFlush(display);
    XCloseDisplay(display);
    usleep(1);
}


struct point
{
    int x;
    int y;
};

int main()
{

    std::vector<point> points;
    std::ifstream fin("data.txt");

{
    std::string first_line; 
    getline(fin, first_line); // read first line ("X Y")
}

while( fin.good())
{
    point temp;
    fin >> temp.x >> temp.y;
    points.push_back(temp);
}

    for (unsigned i = 0; i < points.size(); i++){
	cout << points.at(i).x << " " << points.at(i).y << " " << endl;
	if(points.at(i).x == -1 && points.at(i).y == -1){
		SendClick(1,false);
		sleep(0.5);
	}else{
		moveTo(points.at(i).x, points.at(i).y);
		sleep(0.75);
		SendClick (1, true);    
	}
}
    return 0;
}
