#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define SERVER_FIFO "FIFO2server"
#define USER_FIFO_PATH "./userFIFO/"

struct msg{
	int type;
	long srcID;
	long desID;
	char content[100];
};

void printMsg(struct msg* buffer){
	printf("type: %d\tsrcID: %ld\tdesID: %ld\tcontent: %s\n",buffer->type, buffer->srcID, buffer->desID, buffer->content);
}
