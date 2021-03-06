#include <iostream>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <hiredis/hiredis.h>

using namespace std;

class MyRedis
{
public:
	MyRedis(char *str,int s);
	void Redis_ZSet(char *command); 	//zset函数调用的命令
	vector< char* > Redis_Get_ZSet(char *command);	//
	void Redis_add_Hash(char *command);	//向Hash数据结构中添加数据
	char* Redis_hgetall_Hash(char *command);//将Hash中的域和value值全部打印
	void Redis_add_Set(char *command);	//向set中添加数据结构
	void Redis_Smember_Set(char *command);	//将set中的value打印出来
	vector<char*> Redis_Lrange_List(char *command,char *Value);
	void Redis_add_List(char *command);
	~MyRedis();
private:
	redisContext* c;
};
