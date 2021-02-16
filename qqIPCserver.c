#include "qqIPC.h"

void printMsg(struct msg* buffer){
	printf("---------type: %d  srcID: %ld  desID: %ld  content: %s\n", buffer->type, buffer->srcID, buffer->desID, buffer->content);
}

int login(long srcID, char* content){
	printf("login>");
	printf("---------username：%s\n", content);
	return 0;
}

int sendLogInfo(long target){
	return 0;
}

int diliverMessage(struct msg* buffer){
	struct msg senddata = *buffer;
	long tempid = senddata.srcID;
	senddata.srcID = senddata.desID;
	senddata.desID = tempid;
	printf("try to send>");
	printMsg(&senddata);
	return 0;
}

int logout(long id){
	return 0;
}

int readMsgFromFIFO(int fd){
	struct msg buffer;
	int receiveSize = read(fd, &buffer, sizeof(struct msg));
	if(receiveSize == -1){
		perror("read from FIFO");
		exit(1);
	}
	else if(receiveSize == 0){
		return 0;
	}
	printMsg(&buffer);
	switch(buffer.type){
		case 1: login(buffer.srcID, buffer.content);break;
		case 2: sendLogInfo(buffer.srcID);break;
		case 3: diliverMessage(&buffer);break;
		case 4: logout(buffer.srcID);break;
	}
	return 0;
}

int main(){
	if(access(SERVER_FIFO, F_OK))mkfifo(SERVER_FIFO, 0666);
	int receivefd = open(SERVER_FIFO, O_RDONLY | O_CREAT, 0666);
	if(receivefd == -1){
		perror("open fifo");
		exit(1);
	}
	while(1){
		readMsgFromFIFO(receivefd);
		sleep(1);
	}
	return 0;
}
