#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "packet.h"
#include "extern.h"

int initSensor()
{
	if(wiringPiSetup()==1) return 1;
	pinMode(TOUCH_SENSOR,INPUT);
	pinMode(DETECT_SENSOR,INPUT);
	pinMode(LED1_PIN,OUTPUT);
	pinMode(LED2_PIN,OUTPUT);
}


void checkSensorAndCreatePacket(TYPE_PACKET* tempPacket,int sensor)
{

	int res = digitalRead(sensor);

	//printf("check Sensor : %s : %d ------\n",(sensor == TOUCH_SENSOR) ? ("Touch sensor") : ("Detect sensor"),res);

	digitalWrite(( sensor == TOUCH_SENSOR ) ? LED1_PIN : LED2_PIN ,res);

	makeCmdInfo(tempPacket,sensor,res ? "YES":"NO");

	//printCmd(tempPacket);

	return ;
}

