/*
Name: Rakshith Singh
Email: rsrakshith-singh@myseneca.ca
Student ID: 131037244
Date: 1/29/26

I declare that this submission is the result of my own work and I only copied the code
that my professor provided to complete my assignments. This submitted piece of work
has not been shared with any other student or 3rd party content provider.
*/

#include "event.h"
#include "settings.h"

#include <iomanip>

namespace seneca {

    Event::Event(const char* name, const std::chrono::nanoseconds& duration)
        : m_name(name ? name : ""), m_duration(duration) {
    }

    std::ostream& operator<<(std::ostream& os, const Event& ev) {
        static int counter = 0;
        ++counter;

        // Determine displayed units + field width based on g_settings
        long long value = 0;
        int width = 11; // default for nanoseconds
        const std::string& units = g_settings.m_time_units;

        if (units == "seconds") {
            value = std::chrono::duration_cast<std::chrono::seconds>(ev.m_duration).count();
            width = 2;
        }
        else if (units == "milliseconds") {
            value = std::chrono::duration_cast<std::chrono::milliseconds>(ev.m_duration).count();
            width = 5;
        }
        else if (units == "microseconds") {
            value = std::chrono::duration_cast<std::chrono::microseconds>(ev.m_duration).count();
            width = 8;
        }
        else { 
            value = ev.m_duration.count();
            width = 11;
        }

        os << std::setw(2) << std::right << counter << ": "
            << std::setw(40) << std::right << ev.m_name
            << " -> "
            << std::setw(width) << std::right << value
            << ' ' << units;

        return os;
    }

}
