#pragma once

#include <cstddef>
//#include "logger.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "observer.h"
#include <thread>
#include <mutex>
namespace async {

static std::string file_name;
class Logger : public IObserver
{
public:
    static int iter;
    static vector<string> cmd_str;
    std::string currentDateTime(std::string format);
    std::string path;
    void init(std::string path_log);
    void info(std::string s);
    static void msg(std::string s);
    static void run_threads(vector<string> cmd_local);
    static void cmd_line_log_thread(vector<string> &cmd_local);
    static void file_log_thread_one(vector<string> &cmd_local);
    static void file_log_thread_two(vector<string> &cmd_local);
    virtual void handleEvent(const SupervisedString& ref)
    {
        time_t     now = time(0);
        time_t seconds;
        time(&seconds);

        std::stringstream ss;
        ss << seconds;

        file_name="bulk"+ss.str()+".log";
        std::ofstream ifs(file_name.c_str(), std::ios_base::in | std::ios_base::app);
    }
};

//using handle_t = void *;

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

    Reflector(buf *bb){
        buf_ptr=bb;
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

    Counter(buf *bb){
        buf_ptr=bb;
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
    ObjCounter(buf *bb){
        buf_ptr=bb;
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
    IterLookup(buf *bb){
        buf_ptr=bb;
    }
  virtual void handleEvent(const SupervisedString& ref)
  {
        if(buf_ptr->iter == 0){
            vector<string> bb;
            Logger::run_threads(buf_ptr->cmd_str);
            buf_ptr->cmd_str.clear();
        }
  }
};
SupervisedString connect(std::size_t bulk);
void receive(SupervisedString handle, const char *data, std::size_t size);
void disconnect(SupervisedString handle);

}
