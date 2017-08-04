typedef struct packet{
	int flag;
	int sys_id;	//SYSTEM ID
	int sen_id;	//SENSOR ID
	unsigned int timestamp;
	char data[4];
}TYPE_PACKET;

//typedef struct packet __attribute__((packed)) TYPE_PACKET;

#define CMD_INFO 1
#define CMD_CONTROL 2
#define CMD_INTERVAL 3
