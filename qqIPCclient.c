#include "qqIPC.h"
#include "list.h"

long thisID = -1;
char userName[30];
int serverfd;
int receivefd;
struct node* userList = NULL;

int sendMessage(struct msg* data2send){
	if(write(serverfd, data2send, sizeof(struct msg)) == -1){
		perror("send fail");
		return -1;
	}
	return 0;
}
void loginsuccess(long id){
	thisID = id;
	printf("login success\n");
}

void userUpdate(char* userinfo){
	struct node* cur = userList;
	while(cur->next != NULL)cur = cur -> next;
	cur -> next = (struct node*) malloc(sizeof(struct node));
	cur = cur -> next;
	memcpy(cur, (struct node*) userinfo, sizeof(struct node));
	cur -> next = NULL;
}

void receiveMsg(struct msg* buffer){
	printf("Message from %s: %s", buffer->name, buffer->content);
}

void getUserInfo(){
	destroyList(userList, 0);
	userList = NULL;
	userList = initList(userList);
	struct msg buffer = {2, thisID, -1};
	sendMessage(&buffer);
}

int receiveFromFIFO(){
	struct msg buffer = {-1};
	int ret = read(receivefd, &buffer, sizeof(struct msg));
	if(buffer.type == -1)return -1;
	//printf("receive>");
	//printMsg(&buffer);
	switch(buffer.type){
		case 11: loginsuccess(buffer.desID);break;
		case 12: userUpdate(buffer.content);break;
		case 121: return 1;
		case 13: receiveMsg(&buffer);break;
	}
	return 0;
}

int login(){
	struct msg buffer = {1, -1, -1};
	strcpy(buffer.content, userName);
	sendMessage(&buffer);
	char path[30] = USER_FIFO_PATH;
	receivefd = open(strcat(path, userName), O_CREAT | O_RDONLY | O_NONBLOCK, 0666);
	if(receivefd == -1){
		perror("open receive fail");
		exit(1);
	}

}

void logout(int t){
	struct msg logoutmsg = {4, thisID, -1, "logout"};
	sendMessage(&logoutmsg);
	printf("logout\n");
	exit(0);
}

int sendMessageTo(long desid, char* content){
	struct msg data2send = {3, thisID, desid};
	strcpy(data2send.content, content);
	strcpy(data2send.name, userName);
	return sendMessage(&data2send);
}

void boardcast(char* content){
    while(receiveFromFIFO() != 1)usleep(30000);
    struct node* cur = userList -> next;
    printf("boardcast: %s\n", content);
    while(cur != NULL){
        //printf("to %ld\n", cur -> id);
        sendMessageTo( cur -> id, content);
        cur = cur -> next;
    }
}

int main(){
	signal(SIGINT, logout);
	signal(SIGSTOP, logout);
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
	printf("waiting FIFO\n");
	while(thisID == -1){
		receiveFromFIFO();
		sleep(1);
	}
	getUserInfo();
	int flag = fcntl(STDIN_FILENO, F_GETFL);     
        flag |= O_NONBLOCK;
	fcntl(STDIN_FILENO, F_SETFL, flag);

	char content[CONTENT_SIZE];
	int size = 1;
	while(1){
		if(receiveFromFIFO() == 1) getUserInfo();
		strcmp(content, "");
		size = read(STDIN_FILENO, content, CONTENT_SIZE);
		if(size > 0){
			content[size] = '\0';
			//printf("|%s|\n", content);
			if(strcmp(content, "exit") == 0 ||strcmp(content, "exit\n") == 0){
				break;
			}
			else if(strcmp(content, "user\n") == 0){
                while(receiveFromFIFO() != 1)usleep(30000);
                printList(userList);
                getUserInfo();
                continue;
			}
            boardcast(content);
            getUserInfo();
		}
		usleep(30000);
	}
	logout(1);
}
