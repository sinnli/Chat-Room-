all: TCP_sender TCP_receiver UDP_sender UDP_receiver select_server chat_server chat_client
TCP_sender: TCP_sender.c
	gcc TCP_sender.c -o TCP_sender

TCP_receiver: TCP_receiver.c
	gcc TCP_receiver.c -o TCP_receiver


UDP_sender: UDP_sender.c
	gcc UDP_sender.c -o UDP_sender


UDP_receiver: UDP_receiver.c
	gcc UDP_receiver.c -o UDP_receiver


select_server: select_server.c
	gcc select_server.c -o select_server


chat_server: chat_server.c
	gcc chat_server.c -o chat_server


chat_client: chat_client.c
	gcc chat_client.c -o chat_client

