#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    int data;
    struct node* prev;
    struct node* next;
} Node;

typedef Node* listPointer;

void insert(listPointer *list, int data) {
    listPointer newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->prev = NULL;
    newNode->next = NULL;

    if (*list == NULL) { // empty list
        *list = newNode;
        newNode->next = newNode;
        newNode->prev = newNode;
    } else {
        listPointer curr = *list;
        listPointer prev = NULL;
        int found = 0;

        while (curr->next != *list) {
            if (curr->data == data) {
                free(newNode);
                return; // duplicate, do nothing
            } else if (curr->data > data) {
                found = 1;
                break; // found insertion point
            }

            prev = curr;
            curr = curr->next;
        }

        if (found || (curr->data > data)) { // insert in middle or beginning
            newNode->next = curr;
            newNode->prev = curr->prev;
            curr->prev->next = newNode;
            curr->prev = newNode;

            if (curr == *list) { // insert at beginning
                *list = newNode;
            }
        } else { // insert at the end
            newNode->next = curr->next;
            newNode->prev = curr;
            curr->next = newNode;
            (*list)->prev = newNode;
        }
    }
}



void Ndelete(listPointer* list, int data) {
    if (*list == NULL) {
        return; // empty list
    } else {
        listPointer curr = *list;
        while (curr != NULL) {
            if (curr->data == data) {
                if (curr == *list) { // delete at beginning
                    *list = curr->next;
                    if (*list != NULL) {
                        (*list)->prev = NULL;
                    }
                } else if (curr->next == NULL) { // delete at end
                    curr->prev->next = NULL;
                } else { // delete in middle
                    curr->prev->next = curr->next;
                    curr->next->prev = curr->prev;
                }
                //curr= NULL;
                return;
            }
            curr = curr->next;
        }
    }
    printf("ok");
}

void print_forward(listPointer list) {
    listPointer curr;
    FILE* outfile;
    outfile = fopen("hw2_result.txt", "a");
    if (list) {
        curr = list;
        while (1) {
            fprintf(outfile, "%d ", curr->data);
            printf("%d ", curr->data);
            curr = curr->next;
            if (curr == list)
                break;
        }
    }
    fprintf(outfile, "\n");
    printf("\n");
    fclose(outfile);
    
}

void print_reverse(listPointer list) {
 listPointer curr;
 FILE *outfile;
 outfile = fopen("hw2_result.txt", "a");
 if(list) {
 curr = list->prev;
 while(curr != list) {
 fprintf(outfile, "%d ", curr->data);
 printf("%d ", curr->data);
 curr = curr->prev;
 }
fprintf(outfile, "%d ", curr->data);
 printf("%d ", curr->data);
 }
 fprintf(outfile, "\n");
 printf("\n");
 fclose(outfile);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("usage: %s input_filename\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("The input file does not exist.\n");
        return 1;
    }

    listPointer list = NULL;
    char command[10];
    int data;

    while (fscanf(file, "%s", command) != EOF) {
        if (strcmp(command, "INSERT") == 0) {
            fscanf(file, "%d", &data);
            insert(&list, data);
        } else if (strcmp(command, "DELETE") == 0) {
            fscanf(file, "%d", &data);
            Ndelete(&list, data);
        } else if (strcmp(command, "ASCEND") == 0) {
            print_forward(list);
        } else if (strcmp(command, "DESCEND") == 0) {
            print_reverse(list);
        }
    }

    fclose(file);
    return 0;
}
