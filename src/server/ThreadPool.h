
// Created by Elad Aharon on 14/01/18.
// ID: 311200786
//

#ifndef EX04_THREADPOOL_H
#define EX04_THREADPOOL_H


#include "Task.h"
#include <queue>
#include <pthread.h>
using namespace std;

class ThreadPool {
public:
    ThreadPool(int threadsNum);
    void addTask(Task *task);
    void terminate();
    virtual ~ThreadPool();
private:
    queue<Task *> tasksQueue;
    pthread_t* threads;
    void executeTasks();
    bool stopped;
    pthread_mutex_t lock;
    static void *execute(void *arg);
};


#endif //EX04_THREADPOOL_H
