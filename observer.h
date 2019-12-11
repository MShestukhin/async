#ifndef OBSERVER_H
#define OBSERVER_H
#include <list>
#include <string>
#include <vector>
using namespace std;
class SupervisedString;

struct buf {
    vector<string> cmd_str;
    int iter=0;
    int local_iter=0;
    buf(size_t n){
        iter=n;
    }
};

class IObserver
{
public:
    virtual void handleEvent(const SupervisedString&) = 0;
};

class SupervisedString // Observable class
{
    string _str;
    list<IObserver*> _observers;

    void _Notify()
    {
        for(auto& observer: _observers)
        {
            observer->handleEvent(*this);
        }
    }

public:
    void add(IObserver& ref)
    {
        _observers.push_front(&ref);
    }

    void remove(IObserver& ref)
    {
        _observers.remove(&ref);
    }

    const string& get() const
    {
        return _str;
    }

    void reset(string str)
    {
        _str = str;
        _Notify();
    }
};

#endif // OBSERVER_H
