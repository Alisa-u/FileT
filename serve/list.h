#include<assert.h>
#include<malloc.h>
#include<stdio.h>
typedef struct Node
{
	char *str;
	Node *next;
	int c;
}Node;

void Init_list(Node ** head);

void Add_list(Node *head,char *str);

int Getcount_list(Node *head);

void Add_listC(Node*head,int c);

char* Getstr_list(Node *head,int index);
int Getc_list(Node *head);
void Clean_list(Node *head);

void Destroy_list(Node *head);

int Getstr_fd(Node *head,int index);
