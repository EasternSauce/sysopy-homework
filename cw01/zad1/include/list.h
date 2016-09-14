#ifndef LIST_H
#define LIST_H

typedef enum {FIRSTNAME, LASTNAME, BIRTHDATE, EMAIL, PHONE, ADDRESS} Field;

typedef struct Node{
	struct Node *prev;
	struct Node *next;
	char *firstname;
	char *lastname;
	char *birthdate;
	char *email;
	char *phone;
	char *address;
} Node;

Node * createList(char *firstname, char *lastname, char *birthdate, char *email, char *phone, char *address);
void deleteList(Node **head);
void insertAtHead(Node **head, char *firstname, char *lastname, char *birthdate, char *email, char *phone, char *address);
void removeAtHead(Node **head);
void insertAtTail(Node* head, char *firstname, char *lastname, char *birthdate, char *email, char *phone, char *address);
void removeAtTail(Node *head);
Node * find(char* x, Node *head, Field field);
void sort(Node **head, Field field);


#endif
