#include"list.h"

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

int Getstr_fd(Node *head,int index)
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
	return p->c;
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

void Add_listC(Node*head,int c)
{
	if(head==NULL || c==0)
	{
		return;
	}
	Node *p=(Node *)malloc(sizeof(Node));
	assert(p!=NULL);
	p->c=c;
	
	
	Node *q=head;
	for(;q->next!=NULL;q=q->next)
	{
		;
	}
	q->next=p;
	p->next=NULL;
}

int Getc_list(Node *head)
{
	/*assert(head != NULL);
	int c = 1;
	Node *p =  head->next;
	if(p == NULL)
	{
		return -1;
	}
	head->next = p->next;
	c = p->c;
	free(p);
	return c;*/
	assert(head != NULL);
	int c = 1;
	Node *p =  head->next;
	if(p == NULL)
	{
		return -1;
	}
	c = p->c;
	return c;
}
void Destroy_list(Node *head)
{
	Clean_list(head);
	free(head);
}

