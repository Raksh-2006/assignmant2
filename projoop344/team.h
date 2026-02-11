#ifndef SENECA_TEAM_H
#define SENECA_TEAM_H

#include <string>
#include <iostream>
#include "character.h"

namespace seneca {

    class Team {
        std::string m_name{};
        Character** m_members{};
        size_t      m_size{};

        void clear() {
            if (m_members) {
                for (size_t i = 0; i < m_size; ++i)
                    delete m_members[i];
                delete[] m_members;
            }
            m_members = nullptr;
            m_size = 0;
            m_name.clear();
        }

        void copyFrom(const Team& other) {
            m_name = other.m_name;
            m_size = other.m_size;
            if (m_size == 0) {
                m_members = nullptr;
                return;
            }
            m_members = new Character * [m_size] {};
            for (size_t i = 0; i < m_size; ++i)
                m_members[i] = other.m_members[i]->clone();
        }

        void moveFrom(Team& other) noexcept {
            m_name = std::move(other.m_name);
            m_members = other.m_members;
            m_size = other.m_size;

            other.m_members = nullptr;
            other.m_size = 0;
            other.m_name.clear();
        }

    public:
        Team() = default;

        Team(const char* name) : m_name(name ? name : "") {}

        Team(const Team& other) { copyFrom(other); }

        Team(Team&& other) noexcept { moveFrom(other); }

        Team& operator=(const Team& other) {
            if (this != &other) {
                clear();
                copyFrom(other);
            }
            return *this;
        }

        Team& operator=(Team&& other) noexcept {
            if (this != &other) {
                clear();
                moveFrom(other);
            }
            return *this;
        }

        ~Team() { clear(); }

        void addMember(const Character* c) {
            if (!c) return;

            for (size_t i = 0; i < m_size; ++i)
                if (m_members[i]->getName() == c->getName())
                    return;

            Character** tmp = new Character * [m_size + 1] {};
            for (size_t i = 0; i < m_size; ++i)
                tmp[i] = m_members[i];

            tmp[m_size] = c->clone();

            delete[] m_members;
            m_members = tmp;
            ++m_size;
        }

        void removeMember(const std::string& c) {
            if (!m_members || m_size == 0) return;

            size_t idx = m_size;
            for (size_t i = 0; i < m_size; ++i) {
                if (m_members[i]->getName() == c) {
                    idx = i;
                    break;
                }
            }
            if (idx == m_size) return;

            delete m_members[idx];

            for (size_t i = idx; i + 1 < m_size; ++i)
                m_members[i] = m_members[i + 1];

            --m_size;

            if (m_size == 0) {
                delete[] m_members;
                m_members = nullptr;
                return;
            }

            Character** tmp = new Character * [m_size] {};
            for (size_t i = 0; i < m_size; ++i)
                tmp[i] = m_members[i];

            delete[] m_members;
            m_members = tmp;
        }

        Character* operator[](size_t idx) const {
            if (idx >= m_size) return nullptr;
            return m_members[idx];
        }

        void showMembers() const {
            if (!m_members || m_size == 0) {
                std::cout << "No team.\n";
                return;
            }

            std::cout << "[Team] " << m_name << "\n";
            for (size_t i = 0; i < m_size; ++i) {
                std::cout << "    " << (i + 1) << ": " << *m_members[i] << "\n";
            }
        }
    };

} // namespace seneca

#endif
