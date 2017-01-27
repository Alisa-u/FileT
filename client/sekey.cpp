#include"msg.h"
ude"list.h"
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




#include <errno.h>
#define IXPATH "/home/youyou/2015/sousuo/index"
#define SIZE 128
#define linemax 256
extern MyRedis myredis;
char* DealPath(char *src,char *des)//处理路径   
{
   char *newfile = NULL;
   newfile = (char *)malloc(strlen(src)+strlen(des)+3);
   assert(newfile != NULL);
   if(src[strlen(src)-1] == '/')
   {
      strcpy(newfile, des);
      strcat(newfile, src);
   }
   else
   {
       strcpy(newfile, des);
       strcat(newfile, "/");
       strcat(newfile, src);
   }
   return newfile;
}
char *BF(const char *str,const char *substr)
{             
	int sum = 0;
	const char *p = str;                                                                           
	const char *q = substr;                                                                   
	while((*p!='\0') && (*q!='\0'))
	{
		if(*p == *q)                                                                                
		{
			sum++;
			q++;
			p++;
		}
		else                                                                                                
		{                                                                                                     
			p = p-sum+1;                                                                             
			q = substr;      
			sum = 0;                                              
		}
	}
	if(*q == '\0')
	{
		return (char *)(p-sum);
	}
	return NULL;
}

char* Del_substr(char *str,const char *substr)
{
	assert(str!=NULL && substr!=NULL);
	char *p = BF(str,substr);
	const char *q = NULL;
	int len = 0;
	if(p==NULL)
	{
		return 0;
	}

	for(q=substr;*q!='\0';q++)
	{
		len++;
	}

	for(q=p+len;*q!='\0';q++,p++)
	{
		*p=*q;
	}
	*p='\0';
	return str;
}


vector<keycount>::iterator myfind(vector<keycount>::iterator first,vector<keycount>::iterator last,const int&val)
{
	for(;first!=last;++first)
	{
		if((*first).wordid==val)
		{
			return first;
		}
	}
	return last;
}
void Indexkey(int c)
{
	vector<keycount> ckeyvec;//存放所以关键词id和这个关键词在这篇文章中出现的次数
	vector<wordiddf> dfvec;//存放所有关键词id和df的一个vector
	DocId did;//文档
		
	did.AddDocId();
	
	Wordid wd;//关键词
	WHashTable ht;
	wd.Init_Hash(ht);
	
	Inverted itv;//倒排
	HashTable *pt=new HashTable();
	itv.Init_Hash(pt);
	
	DIR *dp=NULL;
	char nowpath[SIZE]={0};
	strcpy(nowpath,IXPATH);
	if((dp=opendir(nowpath))==NULL)//DIR* opendir(const char *path);打开一个目录，在失败时候返回空指针
	{
		perror("opendir is error\n");
		exit(1);
	}
	
	struct dirent *dir=NULL;
	while((dir=readdir(dp))!=NULL)//index
	{
		if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
	  	 {
  			  continue;
        	 }
		 DIR *dpin=NULL;
		
		char *indexpaths=DealPath(dir->d_name,nowpath);
		
		if((dpin=opendir(indexpaths))==NULL)
		{
			perror("opendir is error\n");
			exit(1);
		}		
		struct dirent *dirin=NULL;
		while((dirin=readdir(dpin))!=NULL)//读取index0123目录
		{
			int allcount=0;

			//cout<<dir->d_name;
			if(strcmp(dirin->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
		  	 {
	  			  continue;
			 }
			if(strncmp(dirin->d_name,"result_",6)!=0)
			{
				continue;
			}
			char pathname[SIZE]={0};
			char *newpath=DealPath(dirin->d_name,indexpaths);
			FILE* wordFile = fopen(newpath,"r");
	 		char line[linemax];
	 		//cout<<"newpath:"<<newpath;
			keycount kc;
	 		while(fgets(line,linemax,wordFile))
			{
				char *p = NULL;
	 			char ptr[100] = {'\0'};
				if((p = strtok(line," "))!=NULL)//截取函数 char *strtok(char *str, const char *delim);
				{
					allcount++;
					memset(ptr,0,100);
					strcpy(ptr,p);
					//cout<<"================="<<ptr<<"++"<<allcount;
					//cout<<"ptr:"<<ptr;
					setlocale(LC_ALL,"zh_CN.UTF-8");
					wchar_t wcharkey[256]={'\0'};
					mbstowcs(wcharkey,ptr,strlen(ptr));
					//printf("wcharkey=%ls\n",wcharkey);
					wd.Insert_Hash(ht,wcharkey);//将截取的关键词插入关键词的哈希表中
					
					int keywordid=wd.Searchid(ht,wcharkey);
					//cout<<"关键词id:"<<keywordid<<endl;
				
					kc.wordid=keywordid;
					kc.kcounts=1;
					vector<keycount>::iterator itk=myfind(ckeyvec.begin(),ckeyvec.end(),kc.wordid);
				
					if(itk==ckeyvec.end())//如果这个关键词是第一次出现，就将kcounts=1直接插入
					{
						ckeyvec.push_back(kc);
					}
					else
					{
						((*itk).kcounts)++;//否则(*itk).kcounts)++
					}
				
				}
				while((p=strtok(NULL," "))!=NULL)//strtok默认其内就有一个指针，保存当前截取的位置
				{
					allcount++;
					memset(ptr,0,100);
					strcpy(ptr,p);
					//cout<<"================="<<ptr<<"++"<<allcount;
					//cout<<"ptr:"<<ptr<<sizeof(ptr)<<endl;
					setlocale(LC_ALL,"zh_CN.UTF-8");
					wchar_t wcharkey[256]={'\0'};
					mbstowcs(wcharkey,ptr,strlen(ptr));

					//printf("wcharkey=%ls\n",wcharkey);
					int rec = wd.Insert_Hash(ht,wcharkey);
					//cout<<"rec::"<<rec<<endl;
				
					int keywordid=wd.Searchid(ht,wcharkey);
					//cout<<"关键词id:"<<keywordid<<endl;

				
					kc.wordid=keywordid;
					kc.kcounts=1;
					vector<keycount>::iterator itk=myfind(ckeyvec.begin(),ckeyvec.end(),kc.wordid);
					if(itk==ckeyvec.end())
					{
						ckeyvec.push_back(kc);
					}
					else
					{
						((*itk).kcounts)++;
					}
				}
			}
			
			key wordkey;
			value v1;
			vector<keycount>::iterator ittv=ckeyvec.begin();
			for(;ittv!=ckeyvec.end();++ittv)
			{
				wordkey.wordid= (*ittv).wordid;
				v1.infor.TF=(((*ittv).kcounts)/allcount);//根据关键词出现的次数算出tf
				//cout<<"次数"<<(*ittv).kcounts<<"--"<<"allcount:"<<allcount<<"--"<<"TF:"<<v1.infor.TF<<endl;
				int pdocid=did.SearchDocId(newpath);
				v1.infor.docid=pdocid;
				 bool res=itv.Insert_Hash(pt,wordkey,v1);//将wordid、tf、docid插入到pt表中
				//cout<<"newpath:"<<newpath<<"文档id号："<<pdocid<<endl;			
			}
			
			ckeyvec.clear();
		}
	}
	//cout<<"所有的docid："<<did.MaxDocid()<<endl;
	/////////////////////////////将关键词和wordid的表打印出来///////////////////////////////

	cout<<"------------------stringkeys表信息---------------------"<<endl;//
	int i=0;
	for(;i<WM;++i)
	{
		HashNode *p=ht.table[i];
		while(p!=NULL)
		{
			cout<<"+++++++++++++++++++++++++++++++"<<endl;
			printf("stringkey:%ls",p->data.stringkey);
			cout<<"pos:"<<i<<"  "<<"wid:"<<p->data.wordid<<endl;
			p=p->next;
		}
	}

	/////////////////////////////将wordid和docid的表打印出来/////////////////////////////////
	cout<<"---------------------wordid---docdid表信息---------------------"<<endl;//
	for(int i=0;i<M;++i)
	{
			map< key,vector<value> > mapnode=(pt)->table[i];
			map< key,vector<value> >::iterator tit=mapnode.begin();
				for(;tit!=mapnode.end();++tit)
			{
				//cout<<"插入的wordid:"<<tit->first.wordid<<endl;
				vector<value>::iterator it3=tit->second.begin();
				//cout<<"+++++++++++++++++++++++"<<endl;
				
				//cout<<"vector大小:"<<tit->second.size();
				for(;it3!=tit->second.end();++it3)
				{
					//cout<<"插入的docid:"<<it3->infor.docid<<"插入的TF:"<<it3->infor.TF<<endl;
				}
				//算DF并将wordid和df一同push_back()到dfvec中
				float allmaxdocid=did.MaxDocid();
				//cout<<"allmaxdocid"<<allmaxdocid<<endl;
				int alldocid=tit->second.size();
				//cout<<"alldocid"<<alldocid<<endl;
				float df=allmaxdocid/alldocid;
				float dfresult= log(df); 
				wordiddf wofd;
				wofd.wordid=tit->first.wordid;
				wofd.df=dfresult;
				dfvec.push_back(wofd);
				
			}
		
	}
		
		closedir(dp);
/////////////////////////////将wordid和df的表打印出来/////////////////////////////////

		vector<wordiddf>::iterator fdit=dfvec.begin();
		for(;fdit!=dfvec.end();++fdit)
		{
			cout<<"wordid:"<<(*fdit).wordid<<"--"<<"DF:"<<(*fdit).df<<endl;
		} 

		//////////////////////////////输入到redis中//////////////////////////////////
		cout<<"//////////////////输入到redis中///////////////////"<<endl;
		int ri=0;
		for(;ri<WM;++ri)
		{
			HashNode *p=ht.table[ri];
			while(p!=NULL)
			{
				printf("stringkey:%ls",p->data.stringkey);
				int redid=wd.Searchid(ht,p->data.stringkey);
			  	key redwordkey;
				redwordkey.wordid=redid;
				vector<value> reddoc;
				reddoc=itv.FindDocV(pt,redwordkey);
				int redpos=itv.Hash( redwordkey.wordid);
				map<key,vector<value> > remap=pt->table[redpos];
				map<key,vector<value> >::iterator rit1=remap.find(redwordkey);
				vector<value>::iterator rit3=rit1->second.begin();
				for(;rit3!=rit1->second.end();++rit3)
				{
					cout<<"doc id:"<<rit3->infor.docid<<endl;
					int udocdid=rit3->infor.docid;
			    	 	//cout<<"u:"<<udocdid<<endl;
					char upath[40]={0};
					strcpy(upath,did.SearPath(udocdid));
					cout<<"查找的文档："<<upath<<endl;
		     			char* userpath=Del_substr(upath,"result_");
			    		
					double ftdf=0;
					cout<<"TF:"<<rit3->infor.TF;
		
					vector<wordiddf>::iterator rdfit=dfvec.begin();
					for(;rdfit!=dfvec.end();++rdfit)
					{
						if(rdfit->wordid==redid)
						{
							cout<<"DF:"<<rdfit->df<<endl;
							ftdf=(rit3->infor.TF)*rdfit->df;
							break;
						}
					}
					char reds[128]={0};
					cout<<"DF*TF:"<<ftdf<<endl;
					sprintf(reds,"zadd %ls %lf %s",p->data.stringkey,ftdf,userpath);
					myredis.Redis_ZSet(reds);  
					cout<<reds<<endl;
				}
				p=p->next;
			}
		}

}	

void SearchKey(int c)
{
	while(1)
	{
		char userkey[128]={0};
		cout<<"userkey:"<<strlen(userkey)<<endl;
		cout<<"++++++11"<<endl;	
		
		while(1)
		{
			int n=recv(c,userkey,128,0);
			if(n!=-1)
			{
				break;
			}
		}
		cout<<"userkey:"<<userkey<<endl;
		if(strncmp(userkey,"timeout",7)==0)
		{
			cout<<"client time out!"<<endl;
			break;
			
		}
		cout<<userkey<<endl;
		setlocale(LC_ALL,"zh_CN.UTF-8");
		wchar_t userwcharkey[256]={'\0'};
		mbstowcs(userwcharkey,userkey,strlen(userkey));
		printf("%ls\n",userwcharkey);
		char redser[128]={0};
		sprintf(redser,"zrange %ls 0 -1 withscores",userwcharkey);
		cout<<redser<<endl;
		vector<char*> vecred(0);
		vecred=myredis.Redis_Get_ZSet(redser);
		char sendbuff[1024]={0};
		vector<char*>::iterator itrs=vecred.begin();
		for(;itrs!=vecred.end();itrs=itrs+2)
		{
			
			strcat(sendbuff,*itrs);
			strcat(sendbuff,"#");
		}
		cout<<"sendbuff"<<sendbuff<<endl;
		send(c,sendbuff,strlen(sendbuff),0);
		break;
	}
}
