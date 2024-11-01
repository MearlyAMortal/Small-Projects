#include <pthread.h>
#include <mutex>
#include <condition_variable>

class Semaphore {
public:
  Semaphore(int init_value) {
    n = init_value;
  }

  void sem_wait();
  void sem_post();

private:
  int n;
  std::mutex sm;
  std::condition_variable waitCondition;
};


void Semaphore::sem_wait(){
  std::unique_lock<std::mutex> lk(sm);

  while( n == 0 ){
    // Spinning
    waitCondition.wait(lk);
  }
  n -= 1;
  lk.unlock();
}

void Semaphore::sem_post(){
  sm.lock();
  n += 1;
  waitCondition.notify_one();
  sm.unlock();
}
