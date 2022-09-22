#include <stdio.h>
#include <stdlib.h>
struct Node { int data; struct Node* next; };


int main() { 
           struct Node* head ;
           head = malloc(sizeof(struct Node));
           head->data = 10;
           
           struct Node* n2  ;
           n2 = malloc(sizeof(struct Node));
           n2->data = 5;
           head->next = n2;
           n2->next = NULL;
           
           while (head != NULL) { 
		   printf( "%d" , head->data);
		    head = head->next;
			 } 
              return 0;
			  }

