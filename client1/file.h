#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <dirent.h> 
#include <vector>
using namespace std;
class BigFile
{
public:				
   	unsigned long get_file_size(const char *path);//获取文件大小
	vector<char *> Split_File(char *filename,int size);	//将文件分割成size大小的块
        int  Get_Chunk_Num(vector<char *>block);  		// 获取块的个数
};

