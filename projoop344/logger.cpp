/*
Name: Rakshith Singh
Email: rsrakshith-singh@myseneca.ca
Student ID: 131037244
Date: 1/29/26

I declare that this submission is the result of my own work and I only copied the code
that my professor provided to complete my assignments. This submitted piece of work
has not been shared with any other student or 3rd party content provider.
*/

#include "logger.h"

namespace seneca {

    Logger::~Logger() {
        delete[] m_events;
        m_events = nullptr;
        m_size = 0;
    }

    Logger::Logger(Logger&& other) noexcept {
        *this = std::move(other);
    }

    Logger& Logger::operator=(Logger&& other) noexcept {
        if (this != &other) {
            delete[] m_events;

            m_events = other.m_events;
            m_size = other.m_size;

            other.m_events = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    void Logger::addEvent(const Event& event) {
        Event* temp = new Event[m_size + 1];

        for (size_t i = 0; i < m_size; ++i) {
            temp[i] = m_events[i];
        }

        temp[m_size] = event;

        delete[] m_events;
        m_events = temp;
        ++m_size;
    }

    std::ostream& operator<<(std::ostream& os, const Logger& logger) {
        for (size_t i = 0; i < logger.m_size; ++i) {
            os << logger.m_events[i] << '\n';
        }
        return os;
    }

}
