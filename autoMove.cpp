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
#include "/home/abtin/enet-1.3.12/include/enet/enet.h"
#include <stdio.h>
#include <string.h>
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





int main (int argc, char ** argv)
{
long * array;

ENetAddress address;
ENetEvent event;
ENetPeer * peer;
ENetHost * client;
    if (enet_initialize () != 0)
    {
        fprintf (stderr, "An error occurred while initializing ENet.\n");
        return EXIT_FAILURE;
    }
    atexit (enet_deinitialize);
    
client = enet_host_create (NULL /* create a client host */,
            1 /* only allow 1 outgoing connection */,
            2 /* allow up 2 channels to be used, 0 and 1 */,
            57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
            14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);
if (client == NULL)
{
    fprintf (stderr, 
             "An error occurred while trying to create an ENet client host.\n");
    exit (EXIT_FAILURE);
}
enet_address_set_host (&address, "10.1.107.69");
//enet_address_set_host (&address, "192.168.0.1");
address.port = 7000;

peer = enet_host_connect (client, &address, 2, 0);    
if (peer == NULL)
{
   fprintf (stderr, 
            "No available peers for initiating an ENet connection.\n");
   exit (EXIT_FAILURE);
}
if (enet_host_service (client, & event, 5000) > 0 &&
    event.type == ENET_EVENT_TYPE_CONNECT)
{
    puts ("Connection to some.server.net:1234 succeeded.");

}
else
{ 
    enet_peer_reset (peer);
    puts ("Connection to some.server.net:1234 failed.");
}
    
    while (1) {
    while (enet_host_service (client, & event, 1000) > 0)
{
    switch (event.type)
    {
    case ENET_EVENT_TYPE_CONNECT:
        printf ("A new client connected from %x:%u.\n", 
                event.peer -> address.host,
                event.peer -> address.port);
        /* Store any relevant client information here. */
        event.peer -> data = (void*)"Client information";
        break;
    case ENET_EVENT_TYPE_RECEIVE:
    	array = (long *)event.packet -> data;
        printf ("A packet of length %u containing x is %d and y is %d was received from %s on channel %u.\n",
                event.packet -> dataLength,
		array[0],
		array[1],
                event.peer -> data,
                event.channelID);
        /* Clean up the packet now that we're done using it. */


	if(array[0] == -1 || array[1] == -1){
		SendClick(1,false);
	}else{
		moveTo(array[0], array[1]);
		SendClick (1, true);    
	}
        enet_packet_destroy (event.packet);
        break;
       
    case ENET_EVENT_TYPE_DISCONNECT:
        printf ("%s disconnected.\n", event.peer -> data);
        /* Reset the peer's client information. */
        event.peer -> data = NULL;
    }
   }
 }
    enet_host_destroy(client);
    return 0;
}
















