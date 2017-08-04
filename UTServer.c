#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "packet.h"
#include "extern.h"
//sys/stat.h
//arpa/inet.h

#define BUF_LEN 128
#define MAX_SOCKET 4
#define BUFF_SIZE 1024

//shared memory
struct sockaddr_in server_addrs[5] = {0,};
struct sockaddr_in empty_addr = {0,};
int sendTerm = 10;
int udpSocket = 0;

//UDP sender
void udpSend(TYPE_PACKET* tempPacket)
{
	int i = 0;
	struct sockaddr_in server_addr;

	memset(&server_addr,0,sizeof(server_addr));
	//printf("start UDP once\n");
	for(i = 0 ; i < MAX_SOCKET ; i++)
	{
		//printf("start send  data with UDP\n");
		if(0!=memcmp(&server_addrs[i],&empty_addr,sizeof(empty_addr)))
		{
			printf("do send data with udp this is not loop\n");
			server_addr = server_addrs[i];
			server_addr.sin_family = AF_INET;
			server_addr.sin_port = htons(4000);
			sendto(udpSocket,(char *)tempPacket , sizeof(*(tempPacket)) , 0, (struct sockaddr*)&server_addr,sizeof(server_addr));
			printf("send data success is not loop\n");
		}
	}
}


void *udpThread(void *data)
{
	int sock;
	int i = 0;
	struct sockaddr_in server_addr;
	for(i = 0 ; i < MAX_SOCKET ; i++)
	{
		memset(&server_addrs[i],0x00,sizeof(server_addrs[i]));
		if(0==memcmp(&server_addrs[i],&empty_addr,sizeof(empty_addr)))
		{
			printf("initialize success\n");
		}
	}

	TYPE_PACKET sendPacket;
	makeCmdInfo(&sendPacket,TOUCH_SENSOR,"NO");

	sock = socket(PF_INET, SOCK_DGRAM,0);

	if(sock == -1)
	{
		printf("socket creation fail\n");
		exit(0);
	}

	memset(&server_addr,0,sizeof(server_addr));
	printf("start UDP\n");
	while(1)
	{
		for(i = 0 ; i < MAX_SOCKET ; i++)
		{
			printf("start send data with UDP is loop\n");
			if(0!=memcmp(&server_addrs[i],&empty_addr,sizeof(empty_addr)))
			{
				server_addr = server_addrs[i];
				server_addr.sin_family = AF_INET;
				server_addr.sin_port = htons(4000);
				printCmd(&sendPacket);
				if(sendto(sock,(char *)&sendPacket,sizeof(sendPacket),0,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
				{
					printf("send fail");
				}
//				sendto(sock,(char *)&sendPacket, sizeof(sendPacket) , 0, (struct sockaddr*)&server_addr,sizeof(server_addr));
				printf("send data success\n");
			}
		}
		sleep(sendTerm);
	}
}



//add sockaddr to shared memory
void add_sockaddr(struct sockaddr_in temp_addr)
{
	int i = 0;
	for(i = 0 ; i < MAX_SOCKET ; i++)
	{
		if(memcmp(&server_addrs[i],&empty_addr,sizeof(empty_addr)))
		{
			continue;
		}
		server_addrs[i] = temp_addr;
		break;
	}
}

//main thread       +tcp reciver and sensor observator
int main(int argc, char *argv[])
{
	//initalize<<
	char buffer[BUF_LEN], temp[20];
	struct sockaddr_in server_addr, client_addr;
	int i = 0, msg_size = 128, len = 0, socketFds[5] ={0};
	int server_fd, client_fd ;
	fd_set* socket_fds;
	fd_set tmp;
	socket_fds = &tmp;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	pthread_t p_thread;
	int thr_id;
	int status;
	int a = 100;
	TYPE_PACKET detect_Packet,touch_Packet;
	initSensor();

	//create UDP Socket;
	udpSocket = socket(PF_INET, SOCK_DGRAM,0);
	if(udpSocket == -1)
	{
		printf("Socket create fail\n");
		exit(0);
	}

	memset(&empty_addr,0x00,sizeof(empty_addr));

	//>initalize






	//udp thread create and start <
	thr_id = pthread_create(&p_thread,NULL,udpThread,(void *)&a);
	if(thr_id < 0)
	{
		printf("thread creation fail");
		exit(0);
	}
	pthread_detach(p_thread);
	//>>


	//<<check port and create server socket
	if(argc != 2)
	{
		printf("usage : %s [port]\n",argv[0]);
		exit(0);
	}

	if((server_fd = socket(AF_INET, SOCK_STREAM,0)) == -1 )
	{
		printf("Server : Can't open stream socket\n");
		exit(0);
	}
	memset(&server_addr,0x00,sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(atoi(argv[1]));
	if(bind(server_fd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0)
	{
		printf("Server : Can't bind local address.\n");
		exit(0);
	}
	if(listen(server_fd,5) < 0)
	{
		printf("Server : connection fail.\n");
		exit(0);
	}
	//>>


	memset(buffer, 0x00,sizeof(buffer));
	printf("Server : waiting connection.\n");
	len = sizeof(client_addr);
	socketFds[0] = server_fd;





	while(1)
	{
	//tcp zone
	//start observate to socket_fds and accept client<<
	//socket_fds initialize<<

		FD_ZERO(socket_fds);
		FD_SET(socketFds[0],socket_fds);
		for(i = 1 ; i < MAX_SOCKET; i++)
		{
			if(socketFds[i]>0)
			{
				FD_SET(socketFds[i],socket_fds);
			}
		}
		//>>
		select(socketFds[0]+MAX_SOCKET,socket_fds,NULL,NULL,&tv);


		//if server socket has transformed(client want to accept)
		//do accept<<
		if(FD_ISSET(socketFds[0],socket_fds))
		{
			client_fd = accept(server_fd,(struct sockaddr *)&client_addr,&len);
			if(client_fd < 0)
			{
				printf("Server : accept failed.\n");
				exit(0);
			}
			for(i = 0 ; i < MAX_SOCKET ; i++)
			{
				if(socketFds[i]<=0)
				{
					socketFds[i] = client_fd;
					break;
				}
			}
			FD_SET(client_fd,socket_fds);
			inet_ntop(AF_INET,&client_addr.sin_addr,temp,sizeof(temp));
			printf("server : %s client connected.\n",temp);
			for(i = 0 ; i < MAX_SOCKET ; i++)
			{
				if(0==memcmp(&server_addrs[i],&empty_addr,sizeof(empty_addr)))
				{
					server_addrs[i] = client_addr;
					break;
				}
			}
			read(client_fd,buffer,msg_size);
			printf("message : %s",buffer);
		}
		//>>


		//if clinet socket has transformed(client want to send data)
		//do read or recive<<
		for(i = 1; i < MAX_SOCKET ; i++ )
		{
			if(socketFds[i]<=0)
			{
				continue;
			}
			if(FD_ISSET(socketFds[i],socket_fds))
			{
				memset(buffer,0x00,sizeof(BUF_LEN));
				read(socketFds[i],buffer,msg_size);
				printf("message : %s from %d\n",buffer,socketFds[i]);
			}
		}
		//>>
		//tcp zone end


		//udp zone star



		memset(&detect_Packet,0x00,sizeof(detect_Packet));
		memset(&touch_Packet,0x00,sizeof(touch_Packet));

		checkSensorAndCreatePacket(&detect_Packet,DETECT_SENSOR);
		checkSensorAndCreatePacket(&touch_Packet,TOUCH_SENSOR);



		if(0==memcmp(detect_Packet.data,"YES",sizeof("YES")))
		{
			printf("send detect alarm\n");
			udpSend(&detect_Packet);
		}
		if(0==memcmp(touch_Packet.data,"YES",sizeof("YES")))
		{
			printf("send touch alarm\n");
			udpSend(&touch_Packet);
		}



	}
	close(server_fd);
	return 0;
}
