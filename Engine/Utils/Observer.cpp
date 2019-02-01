#include "Observer.h"

namespace ScapeEngine
{

Observer::Observer() {}

Observer::~Observer() {}

Subject::Subject() : mObservers() {}

Subject::~Subject() {}

void Subject::addObserver(Observer* observer) { mObservers.insert(observer); }

void Subject::removeObserver(Observer* observer) { mObservers.erase(observer); }

void Subject::notifyObservers()
{
    for (Observer* observer : mObservers)
    {
        observer->update();
    }
}
}
