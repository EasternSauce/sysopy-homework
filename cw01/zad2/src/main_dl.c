#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/times.h>
#include <dlfcn.h> //dynamic linking
#include "list.h"

static clock_t startTime;
static struct tms startCPU;
static clock_t prevTime;
static struct tms prevCPU;


void controlPoint(int i){
	printf("\nCONTROL POINT %d\n", i);
	clock_t currentTime;
	struct tms currentCPU;

	currentTime = times(&currentCPU);
	int clocksPerSec = 100;

	printf("Difference with starting time:\n");
	printf("Real Time: %f, User Time: %f System Time: %f\n",
			(double)(currentTime - startTime)/clocksPerSec,
			(double)(currentCPU.tms_utime - startCPU.tms_utime)/clocksPerSec,
			(double)(currentCPU.tms_stime - startCPU.tms_stime)/clocksPerSec);

	if(i != 0){
		printf("Difference with previous time:\n");
		printf("Real Time: %f, User Time: %f System Time: %f\n",
				(double)(currentTime - prevTime)/clocksPerSec,
				(double)(currentCPU.tms_utime - prevCPU.tms_utime)/clocksPerSec,
				(double)(currentCPU.tms_stime - prevCPU.tms_stime)/clocksPerSec);
	}
	prevTime = currentTime;
	prevCPU = currentCPU;
	printf("END OF CONTROL POINT %d\n\n", i);

}

void printList(Node* head){
	printf("Printing the list...\n");
	while(head != NULL){
		printf("Name: %s, Surname: %s, Birth date: %s, Email: %s, Phone: %s, Address %s\n", head->firstname, head->lastname, head->birthdate, head->email, head->phone, head->address);
		head = head->next;
	}
}

int main(){

	void *handle;
	Node * (*createList)(char*, char*, char*, char*, char*, char*);
	void (*deleteList)(Node **);
	void (*insertAtHead)(Node **head, char*, char*, char*, char*, char*, char*);
	//void (*removeAtHead)(Node**);
	void (*insertAtTail)(Node*, char*, char*, char*, char*, char*, char*);
	void (*removeAtTail)(Node*);
	Node * (*find)(char*, Node*, Field);
	void (*sort)(Node**, Field);

	char *error;

	handle = dlopen("liblist.so", RTLD_LAZY);
	if(!handle){
		fprintf(stderr, "%s\n", dlerror());
		exit(1);
	}

	createList = dlsym(handle, "createList");
	deleteList = dlsym(handle, "deleteList");
	insertAtHead = dlsym(handle, "insertAtHead");
	//removeAtHead = dlsym(handle, "removeAtHead");
	insertAtTail = dlsym(handle, "insertAtTail");
	removeAtTail = dlsym(handle, "removeAtTail");
	find = dlsym(handle, "find");
	sort = dlsym(handle, "sort");

	if((error = dlerror()) != NULL){
		fprintf(stderr, "%s\n", error);
		exit(1);
	}


	startTime = times(&startCPU);
	//first name, last name, birth date, email, phone, address
	controlPoint(0);
	Node *head = createList("Kermit", "Shaffer", "19/10/1993", "Vestibulum.ante@euplacerat.ca", "470-8271", "133-2681 Eu Road");
	insertAtTail(head, "Indira", "Ayers", "05/09/1979", "semper@eu.com", "1-118-526-3527", "754-8716 Nunc. Ave");
	insertAtTail(head, "Rigel", "Mejia", "09/09/1982", "urna.suscipit.nonummy@litora.com", "421-9584", "6528 Enim. St.");
	insertAtTail(head, "Keiko", "Kramer", "19/10/1984", "commodo.tincidunt@arcuVestibulum.org", "371-2805", "4339 Nec Rd.");
	insertAtTail(head, "Tatum", "Owens", "14/05/1982", "a@sedorci.ca", "193-3189", "299-8787 In Road");
	insertAtTail(head, "Trevor", "Schroeder", "13/11/1975", "euismod.et@quispedeSuspendisse.org", "1-634-499-4547", "P.O. Box 950, 6936 Libero. St.");
	insertAtTail(head, "Rose", "Conrad", "02/05/1978", "in.cursus.et@ligula.ca", "967-6680", "188-3372 Fringilla Road");
	insertAtTail(head, "Amber", "Garcia", "03/11/1981", "auctor@justo.com", "1-853-773-6787", "602-9112 Et Av.");
	insertAtTail(head, "Levi", "Horne", "12/03/1988", "diam.nunc.ullamcorper@Maecenasornare.ca", "619-9986", "P.O. Box 869, 5758 Sem St.");
	insertAtTail(head, "Nayda", "Wyatt", "25/08/1984", "dolor@neque.co.uk", "1-181-758-7840", "Ap #650-542 Nunc Rd.");
	insertAtTail(head, "Aladdin", "Mendoza", "12/08/1985", "in@euenimEtiam.edu", "615-9704", "317-8135 Lobortis Avenue");

	printList(head);

	sort(&head, LASTNAME);
	printf("\n\nSorted by last name:\n");
	printList(head);

	controlPoint(1);

	printf("Adding or removing to head or tail of the list\n");
	printf("List before adding:\n");
	printList(head);

	removeAtTail(head);
	removeAtTail(head);
	removeAtTail(head);
	insertAtHead(&head, "Garrison", "Franco", "24/02/1995", "massa.Integer@auctorvitaealiquet.co.uk", "292-4723", "2113 Metus. Avenue");
	insertAtHead(&head, "Quintessa", "Webster", "28/02/1985", "metus.Aenean.sed@orcilobortisaugue.org", "1-839-354-0185", "Ap #223-7354 Laoreet Road");
	insertAtHead(&head, "Autumn", "Franco", "09/10/1986", "odio.Phasellus.at@ullamcorpermagnaSed.co.uk", "790-2893", "271 Erat, Avenue");
	
	printf("\n\nList after adding and removing:\n");
	printList(head);

	printf("\nFinding element with phone number 967-6680\n");
	Node* found = find("967-6680", head, PHONE); 
	printf("Found a person named %s %s\n", found->firstname, found->lastname);


	printf("\nFinding a person named Tatum 10000000 times\n");
	for(int i = 0; i < 10000000; i++){
		find("Tatum", head, FIRSTNAME);
	}

	controlPoint(2);


	printf("\nSelection sorting the list 5000000 times\n");
	for(int i = 0; i < 1000000; i++){
		sort(&head, PHONE);
		sort(&head, LASTNAME);
		sort(&head, FIRSTNAME);
		sort(&head, PHONE);
		sort(&head, ADDRESS);
		sort(&head, EMAIL);
	}
	
	deleteList(&head);

	controlPoint(3);

	return 0;

}
