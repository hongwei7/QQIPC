#ifndef __LINK
#define __LINK

#include "qqIPC.h"

struct node{
	long id;
	char userName[30];
	struct node* next;
};

void destroyList(struct node* head, int ifunlink){
	if(head == NULL)return;
	struct node* back = head->next;
	while(back != NULL){
		char path[30] = USER_FIFO_PATH;
		if(ifunlink == 1)unlink(strcat(path, back->userName));
		free(head);
		head = back;
		back = back -> next;
	}
	free(head);
}

struct node* initList(struct node* head){
	if(head != NULL){
		destroyList(head, 0);	
	}
	head = (struct node*) malloc(sizeof(struct node));
	head->id = -1;
	strcpy(head->userName, "server");
	head -> next = NULL;
	return head;
}

long loginByUserName(struct node* head, char* userName){
	if(head == NULL){
		printf("error: invalid list\n");
		exit(1);
	}
	struct node* prev = head;
	head = head->next;
	while(head != NULL){
		if(strcmp(head->userName, userName) == 0){
			return head->id;
		}
		head = head->next;
		prev = prev->next;
	}
	prev->next = (struct node*) malloc(sizeof(struct node));
	head = prev->next;
	head->id = prev->id + 1;
	strcpy(head->userName, userName);
	head->next = NULL;
	printf("id created: %ld", head->id);
	return head->id;
}

struct node* logoutById(struct node* head, long id){
	if(head == NULL || head->next == NULL)return NULL;
	struct node* prev = head;
	head = head->next;
	while(head != NULL){
		if(head->id == id){
			prev -> next = head -> next;
			return head;
		}
		head = head -> next;
		prev = prev -> next;
	}
	return NULL;
}

void printList(struct node* head){
	printf("----------Users----------\n");
	while(head != NULL){
		printf("id: %ld\tuserName: %s\n", head->id, head->userName);
		head = head->next;
	}
	printf("-------------------------\n");
	return;
}

#endif
