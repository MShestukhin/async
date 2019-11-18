#ifndef LOGGER_H
#define LOGGER_H
#include <fstream>
#include <iostream>
#include <sstream>
#include "observer.h"
#include <thread>
#include <mutex>

static std::string file_name;
class logger : public IObserver
{
    vector<string> cmd_local_buf;
public:
    static int iter;
    static vector<string> cmd_str;
    static bool first_thread;
    logger();
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
        first_thread=true;
        std::ofstream ifs(file_name.c_str(), std::ios_base::in | std::ios_base::app);
    }
};

#endif // LOGGER_H
