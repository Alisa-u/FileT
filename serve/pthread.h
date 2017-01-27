#ifndef __THREAD1_H
#define __THREAD1_H

#include <vector>
#include <list>
#include <cstring>
#include <pthread.h>
#include <iostream>
using namespace std;
class Task
{
	protected:
		int data;
	public:
		CTaskChunk(){}
		virtual int Run1() = 0 ;//纯虚函数
	public:
		virtual ~CTaskChunk(){}//虚析够函数
}	

class CThreadPool
{
public:
	CThreadPoolS(int threadNum = 10);
	void AddTask(CTaskChunk *task);   /** 把任务添加到任务队列中 */
	int QuitAll();                              /** 使线程池中的线程退出 */
	int GetTaskSize();                          /** 获取当前任务队列中的任务数 */
protected:
	static void* ThreadFunc(void * threadData); /** 新线程的线程回调函数 */
	int Create();                               /** 创建线程池中的线程 */

private:
	static  std::list<CTaskChunk *> m_workqueue;      /** 任务列表 */
	static	bool shutdown;                      /** 线程退出标志 */         
	int 	m_iThreadNum;                       /** 线程池中启动的线程数 */
	pthread_t	*pthread_id;		    /** 线程文件描述符数组**/
	static pthread_mutex_t m_pthreadMutex;      /** 线程同步锁 */
	static pthread_cond_t m_pthreadCond;        /** 线程同步的条件变量 */   
};

