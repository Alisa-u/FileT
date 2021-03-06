#include <stdio.h>
#include <map>
#include <vector>
#include <iostream>
#include "list.h"
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <sys/sendfile.h>
#include <fstream>
#include <set>
#include "msg.h"
#include "file.h"
#include <semaphore.h>
#define CPath "/home/youyou/cvte/filetongbu/client/"
#define CFPath "/home/youyou/cvte/filetongbu/client/"
using namespace std;
#define   CMD          1
#define   GETFILE      2
#define   PUTFILE      3
#define   EXIT         4
#define   CD           5
static pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER; //静态初始化
static pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;

int sockfd;
int get_cmd(char *str)
{
	if(strcmp(str,"exit")==0)
	{
		return EXIT;
	}
	else if(strcmp(str,"get")==0)
	{
		return GETFILE;
	}
	else if(strcmp(str,"put")==0)
	{
		return PUTFILE;
	}
	else
	{
		return CMD;
	}
}

void sendtoserve(Node *head,int sockfd)
{
	if(head==NULL)
	{
		return;
	}
	char sendbuff[521]={0};
	int maxnode=Getcount_list(head);//插入链表中的结点个数
	int i=0;
	for(;i<maxnode;++i)
	{
		strcpy(sendbuff,Getstr_list(head,i));//将一个个的结点放到sendbuff中，以#分隔 :cp#main.c#test.c   ls#    
		strcat(sendbuff,"#");
	}

	if(strncmp(sendbuff,"ls",2)==0)//判断如果sendbuff的前两个字符是ls，就给它加上--color，以确定它可以在ls时有颜色显示
	{
		strcat(sendbuff,"--color");
	}

	send(sockfd,sendbuff,strlen(sendbuff),0);//将sendbuff发送个客户端
	char recvbuff[521]={0};
	int n=recv(sockfd,recvbuff,521,0);//接受来自服务器的
	
	if(n==0)
	{
		printf("serve close");
		return;
	}
	char *p=strtok(recvbuff,"#");
	if(p==NULL)
	{
		printf("recv error\n");
		return;
	}
	if(strcmp(p,"0")==0)
	{
		p=strtok(NULL,"#");
		//printf("***%s\n",sendbuff);
		if(strncmp(sendbuff,"ls",2)==0)//将ls打印出来的内容打印成一行
		{
			char *s=p;
			while(*s!='\0')
			{
				if(*s=='\n')
				{
					*s=' ';
				}
				s++;
			}
			
		}
		printf("%s\n",p);
			return;
		
	}
	else if(strcmp(p,"1")==0)//如果是1，则不做任何处理返回
	{
		return;
	}
	else
	{
		printf("recv data error");
	}
	return;
}

void getfile(Node *head,int sockfd)//get sem.c
{
	if(head==NULL)
	{
		return;
	}
	char sendbuff[521]={0};
	int maxnode=Getcount_list(head);//插入链表中的结点个数
	int i=0;
	//printf("%s\n",Getstr_list(head,0));
	//printf("%s\n",Getstr_list(head,1));
	for(;i<maxnode;++i)
	{
		strcat(sendbuff,Getstr_list(head,i));//将一个个的结点放到sendbuff中，以#分隔:get#sem.c#
		strcat(sendbuff,"#");
	}
	printf("sendbuff=%s\n",sendbuff);//调试信息
	send(sockfd,sendbuff,strlen(sendbuff),0);//将  get#sem.c#   发送到服务器
	
	int fd=open(Getstr_list(head,1),O_WRONLY|O_CREAT,0600);//打开或者创建sem.c文件
	assert(fd!=-1);
	int pipefd[2];
	pipe(pipefd);//创建管道
	while(1)
	{
		char recvbuff[256]={0};
		int length=0;
		int l=recv(sockfd,&length,4,0);//接收来自服务器中收到的文件大小
		cout<<"~~length~~:"<<length<<endl;
		cout<<"~~begin put~~"<<endl;
		int n=splice(sockfd,NULL,pipefd[1],NULL,length,0);//将这些信息按length长度从sockfd描述附上拷到pipefd[1]写中	
		if(n==0)
		{
			continue;
		}
		splice(pipefd[0],NULL,fd,NULL,n,0);//将这些从管道的读端读到fd文件中
		break;	
	}
}

char clientbuff[100]={0};
vector<char *> md5vec;
pthread_t id[5];//5个线程id
char path[40] = {0};//存放当前要传递的文件名
char fpath[40];
void *work(void *);
BigFile bgfile;//定义了一个文件类的对象
void send_one_block(char *blockname);
vector<char *>fileblock;
void putfile(Node*head,int sockfd)//put a.c
{
	if(head==NULL)
	{
		return;
	}
	char sendbuff[512]={0};
	int maxnode=Getcount_list(head);
	cout<<maxnode<<endl;;
	int i=0;
	//printf("%s\n",Getstr_list(head,0));
	//printf("%s\n",Getstr_list(head,1));
	for(;i<maxnode;++i)
	{
		strcat(sendbuff,Getstr_list(head,i));//put#a.c#
		strcat(sendbuff,"#");
	}

	printf("sendbuff=%s\n",sendbuff);//调试信息

	send(sockfd,sendbuff,strlen(sendbuff)+1,0);//将put#a.c#发送给serve
	strcpy(path,CFPath);
	strcat(path,Getstr_list(head,1));
	//sprintf(path,"./%s",Getstr_list(head,1));
	
	//将md5校验码写入md5file中
	char tmpmd5[100]={0};
	strcpy(tmpmd5,CFPath);
	strcat(tmpmd5,Getstr_list(head,1));
	cout<<"tmpmd5:"<<tmpmd5<<endl;
	char md5file[100]={0};
	strcpy(md5file,CPath);
	strcat(md5file,"cli_md5");
	cout<<"md5file:"<<md5file<<endl;
	char str[100]={0};
	sprintf(str,"md5sum %s > %s",tmpmd5,md5file);//将md5码写入一个临时文件里
	cout<<"str:"<<str<<endl;
	system(str);

	char md5[40]={0};
 	ifstream infile(md5file,ios::in);				//将文件的MD5值写到temp文件中
        infile.getline(md5,33);
      

        char str1[50];
       
        cout<<"md5"<<md5<<endl;

 
	strcat(clientbuff,md5);
	strcat(clientbuff,"#");
	char *s=NULL;char *ptr=NULL;
	
	strcpy(fpath,path);
	if((ptr=strtok(path,"/"))!=NULL)
	{
		s=ptr;
	}
	while((ptr=strtok(NULL,"/"))!=NULL)//strtok默认其内就有一个指针，保存当前截取的位置
	{
		s=ptr;
	}
	strcat(clientbuff,s);
	strcat(clientbuff,"*");//md5#s*
	
	cout<<"+++++++++++++++++client:+++++++++++++"<<clientbuff<<endl;

	
	int fp=open(fpath,O_RDONLY);//打开a.c
	struct stat stat_file;//stat是一个结构体
	fstat(fp,&stat_file);
	cout<<stat_file.st_size<<endl;
	memset(sendbuff,0,512);
     
	sprintf(sendbuff,"[T04@0@%ld]",stat_file.st_size);
	send(sockfd,sendbuff,strlen(sendbuff)+1,0);
	//sprintf(sendbuff,"S03#",length);
	cout<<"sendbuff:"<<sendbuff<<endl;
	memset(sendbuff,0,512);
	recv(sockfd,sendbuff,511,0);
  	cout<<"sendbuff:"<<sendbuff<<endl;//ok
	
 	if(strncmp(sendbuff,"OK",2) == 0)
	{
		cout<<"+++fpath+++"<<fpath<<endl;
		unsigned long filesize=bgfile.get_file_size(fpath);
		cout<<"~~filesize:"<<filesize<<endl;
		char buff[10]={0};
		if(filesize<1024*1024)
		{
			strcpy(buff,"small");			
			send(sockfd,buff,sizeof(buff),0);
			cout<<"small"<<endl;
			while( 1 )
	   	 	{
			       int n = 0;
		      	       if ((n = sendfile(sockfd,fp,NULL,256))>0)
		      	       {
					continue;
		       	       }
				else
				{
					break;
				}
	    		}
		}
		else
		{
			strcpy(buff,"big");			
			send(sockfd,buff,sizeof(buff),0);
			cout<<"big file"<<endl;
			cout<<"~~~~~~~~~~~~~~~~"<<endl;

			cout<<"path"<<fpath<<endl;
			fileblock=bgfile.Split_File(fpath,50);//对文件分块，然后将其放到block文件夹中，将分块的小文件名放到fileblock的vector中
			if(!fileblock.empty())
			{
				cout<<"block is not empty!"<<endl;
			}
			int count=fileblock.size();
			send(sockfd,&count,sizeof(count),0);
			cout<<"block count:"<<count<<endl;
			while(!fileblock.empty())
			{
				cout<<"---------send_one_block------"<<endl;
				pthread_mutex_lock(&mutex);//加锁
				char *blockname=fileblock.back();
				fileblock.pop_back();
				pthread_mutex_unlock(&mutex);//解锁
				cout<<"blockname:"<<blockname<<endl;//block文件分块的名字
				send_one_block(blockname);
				char buff[4]={0};
				recv(sockfd,&buff,sizeof(buff),0);
				cout<<buff<<endl;//文件块发送成功的返回ok信息
			}
		}
	}
	close(fp);
}


void send_one_block(char *blockname)
{
	
	char bockpath[100]={0};
	strcpy(bockpath,CPath);
	strcat(bockpath,"block/");
	strcat(bockpath,blockname);
	cout<<"bockpath:"<<bockpath<<endl;
		
	
	
	unsigned long blocfilesize=bgfile.get_file_size(path);

	int fd = open(bockpath,O_RDONLY);
	if( fd == -1 )
        {
	   cout<<"open fail"<<endl;
        }
	struct stat stat_file;//stat是一个结构体
	fstat(fd,&stat_file);
	cout<<"stat_file.st_size"<<stat_file.st_size<<endl;
		cout<<"++++++++++++++++++=="<<endl;
		char sendlength[512]={0};	
		sprintf(sendlength,"[T04@0@%ld]",stat_file.st_size);
		strcat(sendlength,"*");
		strcat(sendlength,blockname);
		cout<<"sendlength:"<<sendlength<<endl;
		send(sockfd,sendlength,strlen(sendlength),0);
		char sendbuff[512]={0};
		memset(sendbuff,0,512);
		recv(sockfd,sendbuff,511,0);
	    	cout<<"sendbuff:"<<sendbuff<<endl;
	
	 	if(strcmp(sendbuff,"OK") == 0)
		{
			cout<<"------------"<<endl;
			while( 1 )
	   	 	{
				int n = 0;				
				if ((n = sendfile(sockfd,fd,NULL,256))>0)
				{
						cout<<"nnn2:"<<n<<endl;
						continue;
				}
				else
				{
					break;
				}
			}
		}
		close(fd);
}
void recv_one_block(int fd)
{
	cout<<"begin recv one block"<<endl;
	char recvallbuff[256]={0};
	
	int l=recv(fd,recvallbuff,255,0);//接收来自服务器中收到的文件大小+文件名
	if(l == 0)
	{
		cout<<"recv error"<<endl;
	}
	
	cout<<"recvbuff:"<<recvallbuff<<endl;//调试信息
	char *ptr=strtok(recvallbuff,"*");
	cout<<"+++++++++++++++"<<ptr<<endl;
	char chunkname[20]={0};
	char recvbuff[30]={0};
	strcpy(recvbuff,ptr);
	while((ptr=strtok(NULL,"*"))!=NULL)
	{
		strcpy(chunkname,ptr);
	}
	
	
	char buf[10]={0};
	
	char path[100] = {0};
	strcpy(path,CPath);
	strcat(path,"Tblock/");
	strcat(path,chunkname);
	cout<<"open path:"<<path<<endl;
	int filerec = open(path,O_WRONLY|O_CREAT,0600);         //在本地创建文件
	if( filerec == -1 )
	{
			cout<<"create chunk faile"<<endl;
			exit(0);
	}
     
	char *p = recvbuff + 5;
	long  length = 0;
	if (strncmp(p,"0",1) == 0 )
	{
		cout<<"-------------2"<<endl;
		 p[strlen(p)-1] = 0;
		 length = atol(p+2);
		cout<<"length:"<<length<<endl; 
	}
	int pipefd[2];                                              //创建管道
	pipe(pipefd);
	send(fd,"OK",2,0);
	long count = 0;
	 while( 1 )
	 {
		int n = splice(fd,NULL,pipefd[1],NULL,256,SPLICE_F_MORE);
		if ( n <= 0 )
		{
			printf("down file error\n");
			break;
		}
		count += n;
	
		//printf("count=%d,n=%d\n",count,n);
		int num = splice(pipefd[0],NULL,filerec,NULL,n,SPLICE_F_MORE);
		//printf("num=%d\n",num);
		if ( num < n )
		{
			splice(pipefd[0],NULL,filerec,NULL,(n-num),0);
		}
		if ( count == length )
		{
		 	printf("\ndown file finish\n");
			send(fd,"come",4,0);
			break;
		}
	}	
	close(filerec);
	close(pipefd[0]);
	close(pipefd[1]);	
}
void fun(int sig)
{
	vector<char *> servec;
	cout<<"client test"<<endl;
	char keytime[20];
	char buff[1024]={0};
	strcpy(keytime,"check md5");
	send(sockfd,keytime,sizeof(keytime),0);	
	int res=recv(sockfd,buff,sizeof(buff),0);
	cout<<"~~~~~~~~~server_buff~~~~~:"<<buff<<endl;
	char *ptr;	
	char *wptr;
	if((ptr=strtok(buff,"*"))!=NULL)
	{
		servec.push_back(ptr);
	}
	while((wptr=strtok(NULL,"*"))!=NULL)
	{
		servec.push_back(wptr);
	}
	vector<char *>::iterator serit=servec.begin();
	
	for(;serit!=servec.end();++serit)
	{
		cout<<"server:"<<*serit<<endl;
	}
	
	char *cptr;	
	char *cwptr;
	if((cptr=strtok(clientbuff,"*"))!=NULL)
	{
		md5vec.push_back(cptr);
	}
	while((cwptr=strtok(NULL,"*"))!=NULL)
	{
		md5vec.push_back(cwptr);
	}
	vector<char *>::iterator cliit=md5vec.begin();
	
	for(;cliit!=md5vec.end();++cliit)
	{
		cout<<"client:"<<*cliit<<endl;
	}
	alarm(100);
	char putbuff[1024]={0};
	vector<char *>::iterator sit=servec.begin();
	vector<char *>::iterator cit=md5vec.begin();
	for(;sit!=servec.end();sit++)//ser
	{
		bool tag=false;
		for(;cit!=md5vec.end();++cit)//cli
		{
			cout<<"cit"<<*cit<<endl;
			cout<<"sit"<<*sit<<endl;
			if(strcmp(*cit,*sit)==0)
			{
				cout<<"-----true equal-----"<<endl;
				tag=true;
				break;
			}
		}
		if(tag==false)
		{
			cout<<*sit<<endl;
			strcat(putbuff,*sit);
	//		strcat(putbuff,"!");
		}
	}
	if(strlen(putbuff)==0)
	{
		cout<<"----equal----"<<endl;
		send(sockfd,"equal",6,0);
	}
	else
	{
		cout<<"putbuff"<<putbuff<<endl;
		send(sockfd,putbuff,sizeof(putbuff),0);
		cout<<"get"<<putbuff<<endl;
		char *s=NULL;char *ptr=NULL;
		cout<<"putbuff:"<<putbuff<<endl;
		char *myargv[20]={0};
		char *pptr=NULL;char *wptr=NULL;
		if((pptr=strtok(putbuff,"*"))!=NULL)
		{
	   		myargv[0]=pptr;
		}
		int i=1;
		while((wptr=strtok(NULL,"*"))!=NULL)
	        {
		    myargv[i++]=wptr;
	        }
		char tmp[40]={0};
		myargv[i]='\0';
		vector<char*>tmpvec;
		for(int i=0;myargv[i]!='\0';i++)
		{
			char *path;char *ptrpath;
			if((path=strtok(myargv[i],"#"))!=NULL)
			{
				ptrpath=path;	
			}
			while((path=strtok(NULL,"#"))!=NULL)
			{
				ptrpath=path;
				strcpy(tmp,CPath);
				strcat(tmp,ptrpath);
			}
			tmpvec.push_back(tmp);
		}
		cout<<"out"<<endl;
		while(!tmpvec.empty())
		{
			char *sbuff=tmpvec.front();
			tmpvec.pop_back();
			cout<<"----------sbuff---------:"<<sbuff<<endl;
			int res=open(sbuff,O_WRONLY|O_CREAT,0600);//打开或者创建sem.c文件
			assert(res!=-1);
			int pipefd[2];
			pipe(pipefd);//创建管道
			char recvbuff[256]={0};
			long int length=0;
			
			int l=recv(sockfd,recvbuff,255,0);//接收来自服务器中收到的文件大小
			
			cout<<"recvbuff:"<<recvbuff<<endl;
			if(l<0)
			{
				perror("recv error");
			}
			char *p = recvbuff + 5;
		    	if (strncmp(p,"0",1) == 0 )
		       	{
		       		p[strlen(p)-1] = 0;
		       		length = atol(p+2);
				cout<<"length:"<<length<<endl; 
			}
			send(sockfd,"OK",3,0);
			if(length<1024*1024)
			{
				cout<<"++++++++++++++i++++++++++++++"<<i<<endl;
				int count = 0;
				while(1)
				{
					int n = splice(sockfd,NULL,pipefd[1],NULL,256,SPLICE_F_MORE);
				
					if(n<0)
					{
					    	printf("down file error\n");
					    	break;	
					}
				
					count += n;
					printf("count=%d,n=%d\n",count,n);
					int num = splice(pipefd[0],NULL,res,NULL,n,SPLICE_F_MORE);
					printf("num=%d\n",num);
					if ( num < n )
					{
				    		splice(pipefd[0],NULL,res,NULL,(n-num),0);
					}
					if ( count == length )
					{
				   	  cout<<"\ndown file finish\n"<<endl;
				    	  break;
					}				
	      			}	
			}
			else
			{
				cout<<"-----big_file-----"<<endl;
				int count=0;
				recv(sockfd,&count,sizeof(count),0);
				cout<<"+++count:+++"<<count<<endl;
				for(int i=0;i<count;++i)
				{
					recv_one_block(sockfd);
					send(sockfd,"ok",3,0);
				}
				
				char catfile[128]={0};
				for(int i=0;i<count;++i)
				{
					sprintf(catfile,"cat Tblock/00%d>>%s",i,myargv[1]);
					cout<<"++++++catfile++++++:"<<catfile<<endl;
					system(catfile);
				}
			}
		}

	}
	servec.clear();md5vec.clear();

}
void ShowLoginMain()
{                            
	cout<<"*********main menu*********"<<endl;
	cout<<"1.login :"<<endl;
	cout<<"2.register:"<<endl;
	cout<<"3.exit:"<<endl;
	cout<<"***************************"<<endl;
}
bool doUserLogin()//id+password
{
	size_t size=-1;
	
	LOGIN Lidpsd;
	bzero(&Lidpsd,sizeof(Lidpsd));	
	cout<<"id:"<<endl;
	cin>>Lidpsd.userid;

	cout<<"passwd:"<<endl;
	cin>>Lidpsd.password;
	
	cout<<"begin logining...\n";
	size=send(sockfd,&Lidpsd,sizeof(Lidpsd),0);

	if(size==-1)
		throw"login faile";
	char buff[1024]={0};
	size=recv(sockfd,buff,sizeof(buff),0);
	cout<<buff;
	if(strcmp(buff,"LOGIN") == 0)
	{
		cout<<"already login\n"<<endl;
		return true;
	}
	else if(strcmp(buff,"NEGATION") == 0)
	{
		cout<<"This user does not exist\n"<<endl;
		bool Log_Reg();
	}
}
bool Log_Reg();
bool doUserRegist()//用户名+密码
{
	size_t Rid=-1;
	REGISTER Ridpsd;
	

	cout<<"username:"<<endl;

	cin>>Ridpsd.username;

	cout<<"passwd:"<<endl;

	cin>>Ridpsd.password;
	//将用户名和密码传给服务器
	Rid=send(sockfd,&Ridpsd,sizeof(Ridpsd),0);
	if(Rid==-1)
		throw"register faile";
	int id;
	//接收来自服务器传来的id
	Rid=recv(sockfd,&id,sizeof(id),0);
	cout<<"id:"<<id<<endl;
	cout<<"."<<endl;	
	cout<<"****register successful..*"<<endl;
	cout<<"username:"<<Ridpsd.username<<endl;
	cout<<"passwd:"<<Ridpsd.password<<endl;
	cout<<"id:"<<id<<endl;
	cout<<"***************************"<<endl;
	Log_Reg();
		
	return true;
}
bool Log_Reg()
{
	char ch;
	cout<<"please enter '1.Log' or '2.Reg' :\n";
	cin.sync();
	cin>>ch;
	int n=send(sockfd,(char *)&ch,sizeof(ch),0);
	
	cout<<"n:"<<n<<endl;
	if(ch=='1')
	{
		bool res=doUserLogin();
		return res;
	}
	else if(ch=='2')
	{
		bool res=doUserRegist();
		return res;
	}
}

int main()
{
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	assert(sockfd!=-1);

	struct sockaddr_in saddr;
	memset(&saddr,sizeof(saddr),0);
	saddr.sin_family=AF_INET;//地址族
	saddr.sin_port=htons(6500);//用和服务器一样的端口号.6500指的是要链接的服务器是6500，客户端的端口号是随机分配
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");	
	
	int res=connect(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
	assert(res!=-1);
	/*
	while(1)
	{
		ShowLoginMain();
	
		bool res=Log_Reg();
		if(res)
		{
			break;
		}
	}
	*/
	Node *head=NULL;
	Init_list(&head);//初始化链表
	while(1)
	{
		signal(SIGALRM,fun);
		alarm(100);//由alarm和setitimer函数设置的实时闹钟一旦超时，将触发SIGALRM信号，然后调用fun函数
		char buff[128]={0};
		fgets(buff,128,stdin);
		buff[strlen(buff)-1]=0;//去掉\n给就是给它的位赋值成0就行
		char *ptr=strtok(buff," ");
		if(ptr==NULL)
		{
			continue;
		}
		Add_list(head,ptr);//cp main.c test.c将它们按“ ”截取分别放到链表里  ls     get sem.c(下载)     put a.c（上传）
		while((ptr=strtok(NULL,""))!=NULL)
		{
			Add_list(head,ptr);
		}
		int val=get_cmd(Getstr_list(head,0));
		switch(val)//选择是命令  下载文件   退出   上传文件
		{
			case CMD:
				sendtoserve(head,sockfd);
					break;
			case GETFILE:
				getfile(head,sockfd);
					break;
			case EXIT:
				Destroy_list(head);
				exit(0);
					break;
			case PUTFILE:
				putfile(head,sockfd);
		}
		 Clean_list(head);
	}
	close(sockfd);
}









