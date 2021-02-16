#include "qqIPC.h"

long thisID = -1;
char userName[30];
int serverfd;

int sendMessage(struct msg* data2send){
	if(write(serverfd, data2send, sizeof(struct msg)) == -1){
		perror("send fail");
		return -1;
	}
	return 0;
}

int login(){
	struct msg buffer = {1, -1, 0};
	strcpy(buffer.content, userName);
	printf("send login: %s\n", buffer.content);
	return sendMessage(&buffer);
}

int sendMessageTo(char* content, long desid){
	struct msg data2send = {3, thisID, desid};
	strcpy(data2send.content, content);
	return sendMessage(&data2send);
}

int main(){
	if(access(SERVER_FIFO, F_OK))mkfifo(SERVER_FIFO, 0666);
	else{
		printf("server not found\n");
		exit(1);
	}
	serverfd = open(SERVER_FIFO, O_WRONLY | O_CREAT, 0666);
	if(serverfd == -1){
		perror("open FIFO");
		exit(1);
	}
	printf("What's your name:");
	scanf("%s", userName);
	if(login() != 0){
		printf("login failed!\n");
		exit(1);
	}
	char * content = "hello, QQIPC";
	while(1){
		sleep(1);
		int ret = sendMessageTo(content, -1);
		if(ret == -1){
			perror("write to server");
			exit(1);
		}
	}	
}
