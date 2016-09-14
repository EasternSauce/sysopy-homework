#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"


Node * createNode(char * firstname, char * lastname, char * birthdate, char * email, char * phone, char * address){
	Node * node = (Node *)malloc(sizeof(Node));
	node->next = NULL;
	node->prev = NULL;
	node->firstname = firstname;
	node->lastname = lastname;
	node->birthdate = birthdate;
	node->email = email;
	node->phone = phone;
	node->address = address;
	return node;
}

Node * createList(char * firstname, char * lastname, char * birthdate, char * email, char * phone, char * address){
	return createNode(firstname, lastname, birthdate, email, phone, address);
}

void deleteList(Node ** head){
	Node * current = *head;
	while(current != NULL){
		Node * temp = current->next;
		free(current);
		current = temp;
	}
	*head = NULL;
}

void insertAtHead(Node ** head, char * firstname, char * lastname, char * birthdate, char * email, char * phone, char * address){
	Node * node = createNode(firstname, lastname, birthdate, email, phone, address);
	if(*head == NULL){
		*head = node;
		return;
	}
	(*head)->prev = node;
	node->next = *head;
	*head = node;
}

void removeAtHead(Node ** head){
	if(*head == NULL){
		return;
	}
	(*head)->next->prev = NULL;
	*head = (*head)->next;
}


void insertAtTail(Node * head, char * firstname, char * lastname, char * birthdate, char * email, char * phone, char * address){

	Node * node = createNode(firstname, lastname, birthdate, email, phone, address);

	if(head == NULL){
		head = node;
		return;
	}
	Node * tail = head;
	while(tail->next != NULL) tail = tail->next;
	tail->next = node;
	node->prev = tail;
}

void removeAtTail(Node * head){
	if(head == NULL){
		return;
	}

	Node * tail = head;
	while(tail->next != NULL) tail = tail->next;

	tail->prev->next = NULL;
}

Node * find(char* x, Node * head, Field field){
		while(head != NULL){
			char word[255];
			switch(field){
			case FIRSTNAME:
				strcpy((char *)word, (char *)head->firstname);
				break;
			case LASTNAME:
				strcpy((char *)word, (char *)head->lastname);
				break;
			case BIRTHDATE:
				strcpy((char *)word, (char *)head->birthdate);
				break;
			case EMAIL:
				strcpy((char *)word, (char *)head->email);
				break;
			case PHONE:
				strcpy((char *)word, (char *)head->phone);
				break;
			case ADDRESS:
				strcpy((char *)word, (char *)head->address);
				break;
			default:
				break;
			}
		if(strcmp((char *)word, x) == 0) return head;
		head = head->next;
	}
	return NULL;
}

Node * removeMax(Node ** head, Field field){
	Node * prev = NULL;
	Node * max = *head;
	Node * current = *head;
	while(current != NULL){
		char word[255];
		char maxword[255];
		switch(field){
		case FIRSTNAME:
			strcpy((char *)word, (char *)current->firstname);
			strcpy((char *)maxword, (char *)max->firstname);
			break;
		case LASTNAME:
			strcpy((char *)word, (char *)current->lastname);
			strcpy((char *)maxword, (char *)max->lastname);
			break;
		case BIRTHDATE:
			strcpy((char *)word, (char *)current->birthdate);
			strcpy((char *)maxword, (char *)max->birthdate);
			break;
		case EMAIL:
			strcpy((char *)word, (char *)current->email);
			strcpy((char *)maxword, (char *)max->email);
			break;
		case PHONE:
			strcpy((char *)word, (char *)current->phone);
			strcpy((char *)maxword, (char *)max->phone);
			break;
		case ADDRESS:
			strcpy((char *)word, (char *)current->address);
			strcpy((char *)maxword, (char *)max->address);
			break;
		default:
			break;
		}

		if(strcmp(word, maxword) > 0){ //compare strings
			prev = current->prev; //new prev of max
			max = current; //new max
		}
		current = current->next;
	}
	if(max != NULL){
		if(prev == NULL) *head = (*head)->next; //fix the head
		if(prev != NULL) prev->next = max->next; //set nexts
		if(max->next != NULL) max->next->prev = max->prev; //set prevs
		max->next = NULL;
		max->prev = NULL;
	}
	return max;
}

void sort(Node ** head, Field field){
	Node * sorted = NULL; //head of sorted list

	while(*head != NULL){

		Node * inserted = removeMax(head, field); //get max node

		if(sorted == NULL){ //place it in sorted list
			sorted = inserted;
		}
		else{
			inserted->next = sorted;
			sorted->prev = inserted;
			sorted = inserted;
		}
	}
	*head = sorted; //return sorted list
}


