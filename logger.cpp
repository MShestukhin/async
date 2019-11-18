#include "logger.h"
#include <unistd.h>

logger::logger()
{
}

void logger::init(std::string path_log){
    path="/";
    file_name=currentDateTime("%d.%m.%Y")+".log";
    std::ofstream ifs(file_name.c_str(), std::ios_base::in | std::ios_base::app);
}

std::string logger::currentDateTime(std::string format) {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), format.c_str(), &tstruct);
    return buf;
}
void logger::info(std::string s){
//    msg(s,"INFO");
}

mutex lk;
static int a=0;
void iter_t(std::ofstream &ifs,vector<string> &cmd_local){
    if(a<cmd_local.size()){
        lk.lock();
        ifs<<cmd_local.at(a);
        if(a<cmd_local.size()-1)
            ifs<<", ";
        a++;
        lk.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return iter_t(ifs,cmd_local);
    }
    return;
}

void logger::run_threads(vector<string> cmd_local){
    std::ofstream ifs(file_name.c_str(), std::ios_base::in | std::ios_base::app);
    a=0;
    if (ifs.is_open())
    {
        ifs <<"bulk: ";
    }
    std::thread thr(cmd_line_log_thread, std::ref(cmd_local));
    std::thread thr1(iter_t,std::ref(ifs),std::ref(cmd_local));
    std::thread thr2(iter_t,std::ref(ifs),std::ref(cmd_local));
    thr.join();
    thr1.join();
    thr2.join();
    ifs <<"\n";
    ifs.close();
}

std::string get_line(std::vector<string> &cmd_local){
    std::string sum_cmd="bulk: ";
    for (auto str = cmd_local.begin(); str != cmd_local.end(); ++str) {
        sum_cmd=sum_cmd+*str;
        if(str!=cmd_local.end()-1)
            sum_cmd+=", ";
    }
    return sum_cmd;
}

void logger::cmd_line_log_thread(vector<string> &cmd_local){
    std::string sum_cmd="bulk: ";
    for (auto str = cmd_local.begin(); str != cmd_local.end(); ++str) {
        sum_cmd=sum_cmd+*str;
        if(str!=cmd_local.end()-1)
            sum_cmd+=", ";
    }
    std::cout<<sum_cmd<<"\n";

}

void logger::file_log_thread_one(vector<string> &cmd_local){
        msg(get_line(cmd_local));
}

void logger::file_log_thread_two(vector<string> &cmd_local){
        msg(get_line(cmd_local));
}


bool logger::first_thread;
std::vector<std::string> logger::cmd_str;

void logger::msg(std::string s){
    std::ofstream ifs(file_name.c_str(), std::ios_base::in | std::ios_base::app);
     if (ifs.is_open())
     {
         ifs << s+"\n";
         ifs.close();
     }
     else
         std::cout << "Error opening file\n";
}



