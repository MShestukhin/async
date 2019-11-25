#include "async.h"
#include <string>
namespace async {

void Logger::init(std::string path_log){
    path="/";
    file_name=currentDateTime("%d.%m.%Y")+".log";
    std::ofstream ifs(file_name.c_str(), std::ios_base::in | std::ios_base::app);
}

std::string Logger::currentDateTime(std::string format) {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), format.c_str(), &tstruct);
    return buf;
}
void Logger::info(std::string s){
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

void Logger::run_threads(vector<string> cmd_local){
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

void Logger::cmd_line_log_thread(vector<string> &cmd_local){
    std::string sum_cmd="bulk: ";
    for (auto str = cmd_local.begin(); str != cmd_local.end(); ++str) {
        sum_cmd=sum_cmd+*str;
        if(str!=cmd_local.end()-1)
            sum_cmd+=", ";
    }
    std::cout<<sum_cmd<<"\n";

}

void Logger::file_log_thread_one(vector<string> &cmd_local){
        msg(get_line(cmd_local));
}

void Logger::file_log_thread_two(vector<string> &cmd_local){
        msg(get_line(cmd_local));
}

void Logger::msg(std::string s){
    std::ofstream ifs(file_name.c_str(), std::ios_base::in | std::ios_base::app);
     if (ifs.is_open())
     {
         ifs << s+"\n";
         ifs.close();
     }
     else
         std::cout << "Error opening file\n";
}


buf *b=new buf(5);
Counter cnt(b);
Reflector refl(b);
IterLookup iterLookup(b);
ObjCounter objCnt(b);
Logger log;
int sum_cmd=0;
SupervisedString connect(std::size_t bulk) {
    sum_cmd=bulk;
//    b=new buf(bulk);
    SupervisedString str;
    str.add(iterLookup);
    str.add(refl);
    str.add(cnt);
    str.add(objCnt);
    str.add(log);
    return str;
}

void receive(SupervisedString handle, const char *data, std::size_t size) {
    string cmd;
    while (*data){
        if(*data!='\n')
            cmd.push_back(*data);
        else {
            if(cmd == "}" || cmd =="{"){
                handle.remove(cnt);
                if(!b->local_iter)
                    b->cmd_str.clear();
            }
            handle.reset(cmd);
            handle.remove(log);
            if(b->iter == 0){
                b->iter=sum_cmd;
                handle.remove(cnt);
                handle.add(cnt);
            }
            cmd.clear();
        }
          data++;
    }
}

void disconnect(SupervisedString handle) {
}
}

