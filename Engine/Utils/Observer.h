#ifndef __OBSERVER_H__
#define __OBSERVER_H__

#include <unordered_set>

namespace ScapeEngine
{

class Observer
{
public:
    Observer();
    virtual ~Observer();

    virtual void update() = 0;
};

class Subject
{
public:
    Subject();
    virtual ~Subject();

    virtual void addObserver(Observer* observer);
    virtual void removeObserver(Observer* observer);

    virtual void notifyObservers();

protected:
    typedef std::unordered_set<Observer*> ObserverSet;
    ObserverSet mObservers;
};
}

#endif /* __OBSERVER_H__ */
