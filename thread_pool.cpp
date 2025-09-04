#include "common.h"

ThreadPool::ThreadPool(size_t numOfThreads) : stop{false}{
    for(size_t i=0; i<numOfThreads; i++){
        workers.emplace_back([this]{
            while(true){
                function<void()> task {}; 
                
                // lock scope
                {
                    unique_lock<mutex> lk(queueMutex); 
                    condition.wait(lk, [this]{return stop || !tasks.empty();});

                    // closure and cleanup:
                    if(stop && tasks.empty())
                     return;

                    task = tasks.front(); 
                    tasks.pop(); 
                }
                // exiting the loop before executing the actual task
                // so that the process is actually parallel and not blocked : 
                task(); 
                }
            });
        }        
    }    

ThreadPool::~ThreadPool(){
    stop = true; 
    condition.notify_all(); 

    // join every thread to main. 
    for(auto& worker : workers ){
        worker.join(); 
    }
}

void ThreadPool::enqueue(function<void()> task){
    {
        unique_lock<mutex> lk(queueMutex);
        tasks.push(move(task));
    }
    condition.notify_one(); 
}

// int main(){
//     ThreadPool pool(3);
//     pool.enqueue([]{cout << "task 1 being executed \n";});
//     pool.enqueue([]{cout << "task 2 being executed \n";});
//     pool.enqueue([]{cout << "task 3 being executed \n";});
//     pool.enqueue([]{cout << "task 4 being executed \n";});
//     pool.enqueue([]{cout << "task 5 being executed \n";});
//     cout << "main thread exit" << endl;

//     return 0;
// }