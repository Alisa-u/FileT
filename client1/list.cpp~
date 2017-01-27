#include"list.h"
#include<assert.h>
#include<malloc.h>
void Init_list(Node ** head)
{
	if(head==NULL)	
	{
		return;
	}
	Node *p=(Node*)malloc(sizeof(Node));
	assert(p!=NULL);
	
	p->next=NULL;
	*head=p;
}

void Add_list(Node *head,char *str)
{
	if(head==NULL || str==NULL)
	{
		return;
	}
	Node *p=(Node *)malloc(sizeof(Node));
	assert(p!=NULL);
	p->str=str;
	
	
	Node *q=head;
	for(;q->next!=NULL;q=q->next)
	{
		;
	}
	q->next=p;
	p->next=NULL;
}

int Getcount_list(Node *head)//不包括头结点
{
	if(head==NULL)
	{
		return 0;
	}
	int count=0;
	Node *p=head->next;
	for(;p!=NULL;p=p->next)
	{
		++count;
	}
	return count;
}

char* Getstr_list(Node *head,int index)
{
	if(head==NULL || index>=Getcount_list(head) || index<0)
	{
		return NULL;
	}
	Node *p=head->next;
	int i=0;
	for(;i<index;i++)
	{
		p=p->next;
	}
	return p->str;
}

void Clean_list(Node *head)
{
	if(head==NULL)
	{
		return;
	}
	Node *p=head->next;
	while(p!=NULL)
	{
		head->next=p->next;
		free(p);
		p=head->next;
	}
}

void Destroy_list(Node *head)
{
	Clean_list(head);
	free(head);
}

