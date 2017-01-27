#include "redis.h" 
MyRedis::MyRedis(char *str,int s)
{
	c = redisConnect(str,s);
	if (c->err)
	{
		cout<<"redisConnect error"<<endl;	
	} 
}
MyRedis::~MyRedis()
{
	redisFree(c);
}
void MyRedis::Redis_ZSet(char *command)  
{
 	if (REDIS_OK != redisAppendCommand(c,command))
	{		
		 //redisFree(c);
		 cout<<"command add error"<<endl;
	}
	redisReply* reply = NULL;
	if (REDIS_OK != redisGetReply(c,(void**)&reply))  //处理缓冲区里面的命令,错误判断每次调用redisGetReply只能执行一次命令，执行完之后用freeReplyObject用来释放reply.							
	{
        	cout<<"command execute error"<<endl;
        	freeReplyObject(reply);
        	//redisFree(c);
    	}
	freeReplyObject(reply);
	cout<<"command execute success"<<endl;
}

vector<char*> MyRedis::Redis_Get_ZSet(char *command)
{
	vector<char *> vecred;
	redisReply* r = (redisReply*)redisCommand(c,command);
	if(NULL == r)
	{
		cout<<"----"<<endl;
        	exit(1);
	}
	if(r->type != REDIS_REPLY_ARRAY)
	{
		cout<<"++++++"<<endl;
		freeReplyObject(r);
       		
        	exit(1);
	}
	cout<<"****************"<<endl;
	char buff[1024]={0};
	for (int i = 0; i < r->elements; ++i)
	{
		redisReply* childReply = r->element[i];
        	if (childReply->type == REDIS_REPLY_STRING)
		{
			vecred.push_back(childReply->str);
		}
	}
		vector<char*>::iterator itrs=vecred.begin();
		for(;itrs!=vecred.end();itrs++)
		{
			cout<<"*用户："<<*itrs<<endl;
		}
	return vecred;
}


void MyRedis::Redis_add_List(char *command)
{
	redisReply* r = (redisReply*)redisCommand(c,command);
	if (NULL == r)//则表示客户端和服务器之间出现严重错误如链接断开，必须重新链接
	{
        	return;
    }
	if(r->type != REDIS_REPLY_INTEGER) //插入成功会返回插入的个数,如不是int类型说明语句执行错误
	{
		freeReplyObject(r);
       		//redisFree(c);
        	return;
	}
	int length = r->integer;
	freeReplyObject(r);
	cout<<"length:"<<length;
}

vector<char*> MyRedis::Redis_Lrange_List(char *command,char *Value)
{
	redisReply* r = (redisReply*)redisCommand(c,command);
	if(r->type != REDIS_REPLY_ARRAY)    //返回值是数组类型的，数组里面放的是value值
	{
			freeReplyObject(r);
        	exit(1);
	}
	vector<char *> valuevec;
	for (int i = 0; i < r->elements; ++i)
	{
		redisReply* childReply = r->element[i];
        	if (childReply->type == REDIS_REPLY_STRING)
		{
          		cout<<"list--value"<<childReply->str<<endl;
			char scmpbuff[1024] = {0};
			strcpy(scmpbuff,childReply->str);
			char *ptr = NULL;
			if((ptr = strtok(scmpbuff,"#")) != NULL)
			{
	    			if(strcmp(Value,ptr) == 0)
				{
					valuevec.push_back(childReply->str);
				}
			}	
		}
	}
	return valuevec;
	freeReplyObject(r);
}

void MyRedis::Redis_add_Hash(char *command)    //如果是插入操作
{
	redisReply* r = (redisReply*)redisCommand(c,command);
	if (NULL == r)//则表示客户端和服务器之间出现严重错误如链接断开，必须重新链接
	{
        	//redisFree(c);
        	return;
    	}
	if(r->type != REDIS_REPLY_INTEGER) //插入成功会返回插入的个数,如不是int类型说明语句执行错误
	{
		freeReplyObject(r);
       		redisFree(c);
        	return;
	}
	int length = r->integer;
	cout<<"length:"<<length;
	freeReplyObject(r);
}

char* MyRedis::Redis_hgetall_Hash(char *command)  //获取某个hash表中的field和value
{
	redisReply* r = (redisReply*)redisCommand(c,command);
	if (NULL == r)//则表示客户端和服务器之间出现严重错误如链接断开，必须重新链接
	{
		cout<<"redis********"<<endl;
        	//redisFree(c);
        	return NULL;
    	}
	
	if(r->type != REDIS_REPLY_ARRAY)
	{
		cout<<"*******"<<endl;
		freeReplyObject(r);
       		//redisFree(c);
        	return NULL;
	}
	cout<<"redis____"<<endl;
	for (int i = 0; i < r->elements; ++i)
	{
		redisReply* childReply = r->element[i];
        	if (childReply->type == REDIS_REPLY_STRING)
		{
          		cout<<"feild--value"<<childReply->str<<endl;
			return childReply->str;
		}
	}
	freeReplyObject(r);
}

void MyRedis::Redis_add_Set(char *command)
{
	redisReply* r = (redisReply*)redisCommand(c,command);
	if (NULL == r)//则表示客户端和服务器之间出现严重错误如链接断开，必须重新链接
	{
        	//redisFree(c);
        	return;
    	}
	if(r->type != REDIS_REPLY_INTEGER) //插入成功会返回插入的个数,如不是int类型说明语句执行错误
	{
			freeReplyObject(r);
       		//redisFree(c);
        	return;
	}
	int length = r->integer;
	cout<<"length:"<<length;
	freeReplyObject(r);
	
}
void MyRedis::Redis_Smember_Set(char *command)
{
	redisReply* r = (redisReply*)redisCommand(c,command);
	if (NULL == r)//则表示客户端和服务器之间出现严重错误如链接断开，必须重新链接
	{
        	//redisFree(c);
        	return;
    	}
	if(r->type != REDIS_REPLY_ARRAY)
	{
			freeReplyObject(r);
       		//redisFree(c);
        	return;
	}
	for (int i = 0; i < r->elements; ++i)
	{
		redisReply* childReply = r->element[i];
        	if (childReply->type == REDIS_REPLY_STRING)
		{
          		cout<<"set--value"<<childReply->str<<endl;
		}
	}
	freeReplyObject(r);
}


