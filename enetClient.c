#include "/home/abtin/enet-1.3.12/include/enet/enet.h"
#include <stdio.h>
#include <string.h>
int main (int argc, char ** argv)
{
unsigned long * array;

ENetAddress address;
ENetEvent event;
ENetPeer * peer;
ENetHost * client;
char message[1024];
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
    


//enet_address_set_host (&address, "10.1.107.69");
enet_address_set_host (&address, "10.1.107.69");
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
    	array = (unsigned long *)event.packet -> data;
        printf ("A packet of length %u containing x is %d and y is %d was received from %s on channel %u.\n",
                event.packet -> dataLength,
                // event.packet -> data[0],
//                 event.packet -> data[4],
//                 event.packet -> data[8],
				array[0],
				array[4],
                event.peer -> data,
                event.channelID);
        /* Clean up the packet now that we're done using it. */
        enet_packet_destroy (event.packet);
        
        break;
       
    case ENET_EVENT_TYPE_DISCONNECT:
        printf ("%s disconnected.\n", event.peer -> data);
        /* Reset the peer's client information. */
        event.peer -> data = NULL;
    }
}
    
    // printf("Say > ");
//         gets(message);
// 
//         if (strlen(message) > 0) {
//             ENetPacket *packet = enet_packet_create(message, strlen(message) + 1, ENET_PACKET_FLAG_RELIABLE);
//             enet_peer_send(peer, 0, packet);
//         }
    }
    enet_host_destroy(client);
    return 0;
}
