#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define SERVER_FIFO "FIFO2server"

struct msg{
	int type;
	long srcID;
	long desID;
	char content[100];
};
