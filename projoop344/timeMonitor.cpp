/*
Name: Rakshith Singh
Email: rsrakshith-singh@myseneca.ca
Student ID: 131037244
Date: 1/29/26

I declare that this submission is the result of my own work and I only copied the code
that my professor provided to complete my assignments. This submitted piece of work
has not been shared with any other student or 3rd party content provider.
*/

#include "timeMonitor.h"

namespace seneca {

    void TimeMonitor::startEvent(const char* name) {
        m_eventName = name;
        m_start = std::chrono::steady_clock::now();
    }

    Event TimeMonitor::stopEvent() {
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_start);
        return Event(m_eventName, duration);
    }

}
