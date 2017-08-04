extern void makeCmdinfo(TYPE_PACKET* , int , char *);
extern void printCmd(TYPE_PACKET*);
extern int initSensor();
extern void checkSensorAndCreatePacket(TYPE_PACKET*,int sensor);

#define TOUCH_SENSOR 4
#define DETECT_SENSOR 5
#define LED1_PIN 1
#define LED2_PIN 3
