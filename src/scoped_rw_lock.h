#ifndef __SCOPED_RW_LOCK_H_
#define __SCOPED_RW_LOCK_H_

// this code is in public domain
// http://blog.modp.com/2008/12/scoped-readerwriter-locks-using.html

#include <boost/thread/shared_mutex.hpp>

class scoped_read_lock
{
private:
  boost::shared_mutex&  rwlock;
public:
  scoped_read_lock(boost::shared_mutex& lock)
    : rwlock(lock) { rwlock.lock_shared(); }
  ~scoped_read_lock() { rwlock.unlock_shared(); }  
};

class scoped_write_lock {
private:
  boost::shared_mutex& rwlock;
public:
  scoped_write_lock(boost::shared_mutex& lock)
    : rwlock(lock) { rwlock.lock(); }
  ~scoped_write_lock() { rwlock.unlock(); }
};

#endif