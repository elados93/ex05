
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
    /**
     * constructor
     * @param threadsNum is the number of threads that in the pool
     */
    ThreadPool(int threadsNum);

    /**
     * this function adds tasks to the queue that the pool should excute
     * by their turn.
     * @param task  is the task to add.
     */
    void addTask(Task *task);

    /**
     * this function close all the threads in the pool and delete the mutex.
     */
    void terminate();

    /**
     * distructor
     */
    virtual ~ThreadPool();
private:
    queue<Task *> tasksQueue;
    pthread_t* threads;

    /**
     * loop all the threads in the pool and checks if there is any task to excute.
     */
    void executeTasks();

    /**
     * signify if there pool should stop executing.
     */
    bool stopped;
    pthread_mutex_t lock;

    /**
     * execute a specific task.
     * @param arg is the parameters needed to execute the task.
     */
    static void *execute(void *arg);
};


#endif //EX04_THREADPOOL_H
