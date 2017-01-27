typedef struct data
{
	char *str;
	struct data *next;
}Node;

void Init_list(Node ** head);

void Add_list(Node *head,char *str);

int Getcount_list(Node *head);

char* Getstr_list(Node *head,int index);

void Clean_list(Node *head);

void Destroy_list(Node *head);
