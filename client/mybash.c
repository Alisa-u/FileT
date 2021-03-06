#include"list.h"
#include"mybash.h"
#define LINE_MAX 256
#define PATH "/home/youyou/2015/shell/mybin/" 
#define myargc 10

int get_cmd(char *str);

void do_run(Node *head,int cmd);
		
void do_cd(Node *head);
					
void do_pwd();
	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printfinfo()//打印信息[]
{
	char *str="$";
	int id=getuid();//获取用户id函数  uid_t getuid(void);
	if(id==0)
	{
		str="#";
	}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	struct passwd * p=getpwuid(id);//getpwuid是一个结构体
     /* getpwuid()在<pwd.h>头文件中，获取用户信息，得到passwd结构体,getuid()在<sys/types.h>头文件中，获取启动程序用户的uid号
        struct passwd * getpwname(const char * name);返回指针，指向passwd结构体。name：用户登录名
      struct passwd{             ; 用户信息的编程接口
           char * pw_name          ;用户登录名
           uid_t  pw_uid           ;uid号 
           gid_t  pw_gid           ;gid号
           char * pw_dir           ;用户家目录
           char * pw_gecos         ;用户全名
           char * pw_shell         ;用户默认shell
    */
	if(p==NULL)//获取信息失败
	{
		printf("[mybash:]$ d");
		fflush(stdout);
	}
	char *name =p->pw_name;//如果成功就可以指向用户名
//////////////////////////////////////////////////////////////////
	char hostname[128]={0};
	if(gethostname(hostname,128)==-1)//获取主机名 int gethostname(char *name,size_t namelen);
	{	
		strcpy(hostname,"HOSTNAME");
	}
////////////////////////////////////////////////////////////////
	char path[256]={0};
	if(getcwd(path,256)==NULL)//获取当前路径  char *getcwd(char *buff,size_t size);
	{
		printf("[mybash:]");
		fflush(stdout);
	}
////////////////////////////////////////////////////////////////////////////
	char *s="/";
	char *ptr=NULL;
	if((ptr=strtok(path,"/"))!=NULL)//截取函数 char *strtok(char *str, const char *delim);
	{
		s=ptr;
	}
	while((ptr=strtok(NULL,"/"))!=NULL)//strtok默认其内就有一个指针，保存当前截取的位置
	{
		s=ptr;
	}
	printf("[%s@%s %s]%s ",p->pw_name,hostname,s,str);
	fflush(stdout);
}


int main()
{
	Node *head=NULL;
	Init_list(&head);
	while(1)
	{
		char buff[LINE_MAX]={0};
		printfinfo();//给出提示信息
		fgets(buff,128,stdin);//ls pwd ./main
		buff[strlen(buff)-1]=0;
		
		/*
		char *ptr=strtok(buff," ");

		if(ptr==NULL)
		{
			continue;
		}
		Add_list(head,ptr);
		while((ptr=strtok(NULL," "))!=NULL)
		{
			Add_list(head,ptr);//截到一个不为空的数据压入链表中
		}
		////////////////////////////////////
*/
		char *ptr=strtok(buff," ");

		if(ptr==NULL)
		{
			continue;
		}
		Add_list(head,ptr);
		while((ptr=strtok(NULL," "))!=NULL)
		{
			Add_list(head,ptr);//截到一个不为空的数据压入链表中
		}
		int cmd=get_cmd(Getstr_list(head,0));
		//printf("%s\n",Getstr_list(head,1));
		switch(cmd)
		{
			case RUN_STR:
			case CMD_STR:
			     do_run(head,cmd);
				break;
			case CD:
			     do_cd(head);
				break;
			case EXIT:
			     Clean_list(head);
			     exit(0);
				break;			
			default:
			     printf("cmd is error\n");
			
		}
		Clean_list(head);
	}
	
}
int get_cmd(char *str)  //分类提取
{
	if(str == NULL)
	{
		return ERR;
	}
	if(strcmp(str,"cd")==0)
	{
		return CD;
	}
	else if( strcmp(str,"exit")==0)
	{
		return EXIT;
	}
	else if( strncmp(str,"./",2) == 0 || strncmp(str,"/",1 == 0))
	{
		return RUN_STR;   
	}
	else
	{
		return CMD_STR;
	}
}
//////////////////////////////////////////////////////////////////////////////
void do_run(Node *head,int cmd)
{
	char path[128]={0};
	if(head==NULL||cmd==ERR)
	{
		perror("bash argv is error");
		return;
	}
	if(cmd==CMD_STR)
	{
		strcpy(path,PATH);
	}
//#define PATH "/home/youyou/2015/shell/mybin/" 
	strcat(path,Getstr_list(head,0));//获取第一个结点的内容
	
	pid_t pid = fork();
	if(pid==-1)
	{
		perror("fork is error\n");
	}
	else if(pid==0)
	{
		int i=0;
		char *myargv[myargc]={0};
		int num=Getcount_list(head);
		if(num>=myargc)
		{
			printf("argc is error\n");
		}
		for(;i<num;++i)
		{
			myargv[i]=Getstr_list(head,i);
		}
		execv(path,myargv);
		perror("mybash:is error");
		exit(0);
	}
	wait(NULL);
}

void do_cd(Node *head)
{
	if(head==NULL)
	{
		return;
	}
	if(chdir(Getstr_list(head,1))==-1)//int chdir(const char *path);类似shell中的cd命令 
	{
		perror("bash cd:");
	}
}




etstr_list(head,1))==-1)//int chdir(const char *path);类似shell中的cd命令 
	{
		perror("bash cd:");
	}
}




