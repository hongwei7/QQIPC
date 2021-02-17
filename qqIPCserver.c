#include "qqIPC.h"
#include "list.h"
#define USER_MAX_NUM 200

int receivefd;
struct node* userList = NULL;
int sendfd[USER_MAX_NUM] = {0};


void makeFIFO2client(char * userName, long id){
	char path[20] = USER_FIFO_PATH;
	strcat(path, userName);
	if(access(path, F_OK))mkfifo(path, 0666);
	close(sendfd[id]);
	sendfd[id] = open(path, O_WRONLY | O_CREAT, 0666); 	
	printf("the login port is %d \n", sendfd[id]);
	if(sendfd[id] == -1){
		perror("makeFIFO2client");
		exit(1);
	}
}

void sendMessage(struct msg* buffer){
	printf("send to %d >", sendfd[buffer->desID]);
	printMsg(buffer);
	int ret = write(sendfd[buffer->desID], buffer, sizeof(struct msg));
	if(ret == -1){
		perror("send to client");
		exit(1);
	}
}

void login(char* content){
	printf("login>");
	long id = loginByUserName(userList, content);
	printf("---------username：%s id:%ld\n", content, id);
	struct msg loginsuccess = {11, -1, id, "login"};
	makeFIFO2client(content, id);
	sendMessage(&loginsuccess);
}

int sendLogInfo(long target){
	return 0;
}

int diliverMessage(struct msg* buffer){
	struct msg senddata = *buffer;
	long tempid = senddata.srcID;
	senddata.srcID = senddata.desID;
	senddata.desID = tempid;
	return 0;
}

void unlinkFIFO(long id, char* userName){
	char path[20] = USER_FIFO_PATH;
	strcat(path, userName);
	unlink(path);
}

int logout(long id){
	struct node* user = logoutById(userList, id);
	close(sendfd[id]);
	unlinkFIFO(id, user->userName);
	free(user);
	return 0;
}

int readMsgFromFIFO(){
	struct msg buffer = {-1};
	int receiveSize = read(receivefd, &buffer, sizeof(struct msg));
	if(receiveSize == -1){
		//perror("read from FIFO");
	}
	else if(receiveSize == 0){
		return 0;
	}
	if(buffer.type != -1)printMsg(&buffer);
	switch(buffer.type){
		case 1: login(buffer.content);break;
		case 2: sendLogInfo(buffer.srcID);break;
		case 3: diliverMessage(&buffer);break;
		case 4: logout(buffer.srcID);break;
	}
	return 0;
}

int main(){
	userList = initList(userList);
	if(access(SERVER_FIFO, F_OK))mkfifo(SERVER_FIFO, 0666);
	receivefd = open(SERVER_FIFO, O_RDONLY | O_CREAT | O_NONBLOCK, 0666);
	if(receivefd == -1){
		perror("open fifo");
		exit(1);
	}
	while(1){
		readMsgFromFIFO();
		printList(userList);
		sleep(1);
	}
	close(receivefd);
	destroyList(userList);
	return 0;
}
