#include "qqIPC.h"

long thisID = -1;
char userName[30];
int serverfd;
int receivefd;

int sendMessage(struct msg* data2send){
	if(write(serverfd, data2send, sizeof(struct msg)) == -1){
		perror("send fail");
		return -1;
	}
	return 0;
}
void loginsuccess(long id){
	thisID = id;
	printf("login success> your id is %ld\n", thisID);
}

void userUpdate(char* userinfo){
	
}

void receiveMsg(struct msg* buffer){
	printMsg(buffer);
}

void receiveFromFIFO(){
	printf("waiting FIFO\n");
	struct msg buffer = {-1};
	int ret = read(receivefd, &buffer, sizeof(struct msg));
	if(ret == -1)return;
	printf("receive>");
	printMsg(&buffer);
	switch(buffer.type){
		case 11: loginsuccess(buffer.desID);break;
		case 12: userUpdate(buffer.content);break;
		case 13: receiveMsg(&buffer);break;
	}
}

int login(){
	struct msg buffer = {1, -1, -1};
	strcpy(buffer.content, userName);
	printf("send login: %s\n", buffer.content);
	sendMessage(&buffer);
	char path[30] = USER_FIFO_PATH;
	receivefd = open(strcat(path, userName), O_CREAT | O_RDONLY | O_NONBLOCK, 0666);
	if(receivefd == -1){
		perror("open receive fail");
		exit(1);
	}

}

void logout(){
	struct msg logoutmsg = {4, thisID, -1};
	sendMessage(&logoutmsg);
}

int sendMessageTo(char* content, long desid){
	struct msg data2send = {3, thisID, desid};
	strcpy(data2send.content, content);
	return sendMessage(&data2send);
}

int main(){
	if(access(SERVER_FIFO, F_OK)){
		perror("open server");
		exit(1);
	}
	serverfd = open(SERVER_FIFO, O_WRONLY | O_CREAT, 0666);
	if(serverfd == -1){
		perror("open FIFO");
		exit(1);
	}
	printf("What's your name:");
	scanf("%s", userName);
	login();
	while(thisID == -1){
		printf("thisID: %ld\n", thisID);
		receiveFromFIFO();
		sleep(1);
	}
	char * content = "hello, QQIPC";
	printf("thisID:%ld\n", thisID);
	sleep(2);
	logout();
}
