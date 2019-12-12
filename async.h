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

class Reflector: public IObserver // Prints the observed string into cout
{
    buf *buf_ptr;
public:

    void set(buf *bb){
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

    void set(buf *bb){
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
    static int local_iter;
    void set(buf *bb){
        buf_ptr=bb;
    }
//  static int local_iter;
  virtual void handleEvent(const SupervisedString& ref)
  {
        if(ref.get()=="{"){
            local_iter=local_iter+1;
            buf_ptr->iter=local_iter;
        }
        if(ref.get()=="}"){
            local_iter=local_iter-1;
            buf_ptr->iter=local_iter;
        }
  }
};

class IterLookup: public IObserver // Prints the length of observed string into cout
{
    buf *buf_ptr;
public:
    void set(buf *bb){
        buf_ptr=bb;
    }
  virtual void handleEvent(const SupervisedString& ref)
  {
        if(buf_ptr->iter == 0){
            Logger::run_threads(buf_ptr->cmd_str);
            buf_ptr->cmd_str.clear();
        }
  }
};

struct singleton {
    Counter cnt;
    Reflector refl;
    IterLookup iterLookup;
    ObjCounter objCnt;
    Logger log;
    SupervisedString str;
    buf *b;
    string cmd;
    singleton(std::size_t bulk) {
        b=new buf(bulk);
        cnt.set(b);
        refl.set(b);
        iterLookup.set(b);
        objCnt.set(b);
        str.add(iterLookup);
        str.add(refl);
        str.add(cnt);
        str.add(objCnt);
        str.add(log);
    }
    void remove_cnt(){
        str.remove(cnt);
    }
    void add_cnt(){
        str.add(cnt);
    }

    void data_worker(const char *data, int sum_cmd){
    //    buf *b=mapa.find((SupervisedString*)&handle)->second;
        while (*data){
            if(*data!='\n')
                cmd.push_back(*data);
            else {
                if(cmd == "}" || cmd =="{"){
                    str.remove(cnt);
                    if(!ObjCounter::local_iter)
                        b->cmd_str.clear();
                }
                str.reset(cmd);
                str.remove(log);
                if(b->iter == 0){
                    b->iter=sum_cmd;
                    str.remove(cnt);
                    str.add(cnt);
                }
                cmd.clear();
            }
              data++;
        }
    }
};

singleton connect(std::size_t bulk);
void receive(singleton &handle, const char *data, std::size_t size);
void disconnect(SupervisedString handle);

}
