
// Created by Elad Aharon on 14/01/18.
// ID: 311200786
//

#ifndef EX04_TASK_H
#define EX04_TASK_H

class Task {
public:
    Task(void *(*func)(void *arg), void *arg) :
            func(func), arg(arg) {}

    void execute() {
        func(arg);
    }

    virtual ~Task() {}

private:
    void *(*func)(void *arg);
    void *arg;
};


#endif //EX04_TASK_H
