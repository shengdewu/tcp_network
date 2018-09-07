#include <pthread.h>
//读写锁

class rwlock
{
public:
    rwlock(){ pthread_rwlock_init(&rwlck_, nullptr); }
    ~rwlock(){ pthread_rwlock_destroy(&rwlck_); }

    void wlock(){ pthread_rwlock_wrlock(&rwlck_); }
    void rlock(){ pthread_rwlock_rdlock(&rwlck_); }
    void unlock(){ pthread_rwlock_unlock(&rwlck_); };

private:
    pthread_rwlock_t rwlck_;
};

class auto_rwlock
{
public:
    auto_rwlock(rwlock *rwlck, bool wlck = true):rwlck_(rwlck){ if(wlck){ rwlck_->wlock(); } else{ rwlck_->rlock(); } }
    ~auto_rwlock(){ if(rwlck_){ rwlck_->unlock(); } }

private:
    rwlock *rwlck_;
};