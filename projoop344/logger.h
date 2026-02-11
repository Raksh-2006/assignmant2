 
/*
Name: Rakshith Singh
Email: rsrakshith-singh@myseneca.ca
Student ID: 131037244
Date: 1/29/26

I declare that this submission is the result of my own work and I only copied the code
that my professor provided to complete my assignments. This submitted piece of work
has not been shared with any other student or 3rd party content provider.
*/

#ifndef SENECA_LOGGER_H
#define SENECA_LOGGER_H

#include <iostream>
#include "event.h"

namespace seneca {

    class Logger {
        Event* m_events{ nullptr };
        size_t m_size{ 0 };

    public:
        Logger() = default;
        ~Logger();

        // Disable copy operations
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;

        // Enable move operations
        Logger(Logger&& other) noexcept;
        Logger& operator=(Logger&& other) noexcept;

        void addEvent(const Event& event);

        friend std::ostream& operator<<(std::ostream& os, const Logger& logger);
    };

}

#endif
