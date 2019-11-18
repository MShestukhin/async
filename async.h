#pragma once

#include <cstddef>
#include "logger.h"
namespace async {

using handle_t = void *;

struct buf {
    vector<string> cmd_str;
    int iter=0;
    int local_iter=0;
    buf(size_t n){
        iter=n;
    }
};

class Reflector: public IObserver // Prints the observed string into cout
{
    buf *buf_ptr;
public:

    Reflector(buf &bb){
        buf_ptr=&bb;
    }

    virtual void handleEvent(const SupervisedString& ref)
    {
        if(ref.get() != "}" && ref.get() !="{")
            buf_ptr->cmd_str.push_back(ref.get());
    }
};

class Counter: public IObserver // Prints the length of observed string into cout
{

    buf *buf_ptr;
public:

    Counter(buf &bb){
        buf_ptr=&bb;
    }
  virtual void handleEvent(const SupervisedString& ref)
  {
      buf_ptr->iter--;
  }
};

class ObjCounter: public IObserver // Prints the length of observed string into cout
{
    buf *buf_ptr;
public:
    ObjCounter(buf &bb){
        buf_ptr=&bb;
    }
//  static int local_iter;
  virtual void handleEvent(const SupervisedString& ref)
  {
        if(ref.get()=="{"){
            buf_ptr->local_iter=buf_ptr->local_iter+1;
            buf_ptr->iter=buf_ptr->local_iter;
        }
        if(ref.get()=="}"){
            buf_ptr->local_iter=buf_ptr->local_iter-1;
            buf_ptr->iter=buf_ptr->local_iter;
        }

  }
};

class IterLookup: public IObserver // Prints the length of observed string into cout
{
    buf *buf_ptr;
public:
    IterLookup(buf &bb){
        buf_ptr=&bb;
    }
  virtual void handleEvent(const SupervisedString& ref)
  {
        if(buf_ptr->iter == 0){
            logger::run_threads(buf_ptr->cmd_str);
            buf_ptr->cmd_str.clear();
        }
  }
};

handle_t connect(std::size_t bulk);
void receive(handle_t handle, const char *data, std::size_t size);
void disconnect(handle_t handle);

}
