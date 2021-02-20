#ifndef __QQIPC
#define __QQIPC

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>

#define SERVER_FIFO "FIFO2server"
#define USER_FIFO_PATH "./userFIFO/"
#define USER_FIFO_FOLDER "userFIFO"
#define CONTENT_SIZE 100

struct msg{
	int type;
	long srcID;
	long desID;
	char content[CONTENT_SIZE];
	char name[30];
};

void printMsg(struct msg* buffer){
	printf("type: %d\tsrcID: %ld\tdesID: %ld\ncontent: %s\tname: %s\n",buffer->type, buffer->srcID, buffer->desID, buffer->content, buffer->name);
}

#endif
