#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "file.h"
#include <dirent.h> 
#include <string.h>
#include <fcntl.h>
using namespace std;
#define PATH "/home/youyou/cvte/filetongbu/client/"
unsigned long BigFile::get_file_size(const char *path)  
{  
    unsigned long filesize = -1;      
    struct stat statbuff;  
    if(stat(path, &statbuff) < 0){  
        return filesize;  
    }else{  
        filesize = statbuff.st_size;  
    }  
    return filesize;  
}  
vector<char*> BigFile::Split_File(char *filename,int size)
{
	vector<char *>block;
	cout<<"~~~~~~~~~~~~~Split_File~~~~~~~~~"<<endl;
	char folderName[50] = {0};//拼接文件夹名
	cout<<"file name:"<<filename<<endl;
	strcat(folderName,PATH);
	strcat(folderName,"block");
	cout<<"folderName:"<<folderName<<endl;
	char str0[50];//创建文件夹
	sprintf(str0,"mkdir %s",folderName);
	cout<<"str0"<<str0<<endl;
	system(str0);
	char str2[100];//分割文件
	sprintf(str2,"split -a 3 -d -b %dm  %s  %s/",size,filename,folderName);//将名字为filename的文件分成大小为size的块，将其放入folderName中
	system(str2);
	

	 struct dirent *ptr=NULL;
	DIR *dir;
	dir=opendir(folderName);
	while((ptr=readdir(dir))!=NULL)
	{
		if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)
		{
			continue;
		}
		cout<<ptr->d_name<<endl;
		block.push_back(ptr->d_name);
	}
	return block;
}
int  BigFile::Get_Chunk_Num(vector<char *>block)
{
	return block.size();
}
