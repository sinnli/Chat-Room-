# Chat-Room
The project has one server which is the "chat room" and several clients which can quit anytime.
Two main protocols are used in the transport layer in the OSI model which are the UDP and the TCP 
protocol.

## UDP Sockets
UDP involves the stages of creating a socket, binding of the address and port at the server and
sending data (by the client) and recving (by the server).
Adding the select function to a UDP socket stream is done only on ther server side, to monitor multiple
UDP sockets for incoming data or events. Since UDP is connectionless and there is no ongoing connection
between the server and the clients, the server doesn't need to monitor individual client sockets 
specifically.Instead, it listens for data from any client on a specific port.

## TCP Sockets
In TCP the stages are bit different; first the creation of the socket, binding at the server, listening
at the server, connection establishment and only then the data transferring is enabled.
The select functionality is used on both the server and the client side. TCP is connection-oriented,
and there is an ongoing connection between the client and the server. Therefore, both the server 
and the client may need to handle read and write events on their respective sockets.
