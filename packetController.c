#include "packet.h"
#include "extern.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define SYS_ID 2

void makeCmdInfo(TYPE_PACKET* tempPacket, int sensor, char *tempdata)
{
	int i = 0;
	tempPacket->flag = CMD_INFO;
	tempPacket->sys_id = SYS_ID;
	tempPacket->sen_id = sensor;
	tempPacket->timestamp = time(NULL);

	for(i=0; i < 4 ; i++){
		tempPacket->data[i]=tempdata[i];
	}

}

void printCmd(TYPE_PACKET* dataPacket)
{
	printf("%d %d %d %d %s",dataPacket->flag,dataPacket->sys_id,dataPacket->sen_id,dataPacket->timestamp,dataPacket->data);
}
