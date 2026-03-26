#include "guild.h"
#include <iostream>
#include <utility>

namespace seneca {

    Guild::Guild() = default;

    Guild::Guild(const char* name) : m_name(name ? name : "") {}

    Guild::Guild(const Guild& other) {
        copyFrom(other);
    }

    Guild::Guild(Guild&& other) noexcept {
        moveFrom(other);
    }

    Guild& Guild::operator=(const Guild& other) {
        if (this != &other) {
            clear();
            copyFrom(other);
        }
        return *this;
    }

    Guild& Guild::operator=(Guild&& other) noexcept {
        if (this != &other) {
            clear();
            moveFrom(other);
        }
        return *this;
    }

    Guild::~Guild() {
        clear();
    }

    void Guild::clear() {
        delete[] m_members;          // Guild does NOT own Characters
        m_members = nullptr;
        m_size = 0;
        m_name.clear();
    }

    void Guild::copyFrom(const Guild& other) {
        m_name = other.m_name;
        m_size = other.m_size;

        if (m_size == 0) {
            m_members = nullptr;
            return;
        }

        m_members = new Character * [m_size] {};
        for (size_t i = 0; i < m_size; ++i) {
            m_members[i] = other.m_members[i];   // shallow copy (non-owning)
        }
    }

    void Guild::moveFrom(Guild& other) noexcept {
        m_name = std::move(other.m_name);
        m_members = other.m_members;
        m_size = other.m_size;

        other.m_members = nullptr;
        other.m_size = 0;
        other.m_name.clear();
    }

    void Guild::addMember(Character* c) {
        if (!c) return;

        for (size_t i = 0; i < m_size; ++i) {
            if (m_members[i] == c) return; // already in guild
        }

        Character** tmp = new Character * [m_size + 1] {};
        for (size_t i = 0; i < m_size; ++i) tmp[i] = m_members[i];
        tmp[m_size] = c;

        delete[] m_members;
        m_members = tmp;
        ++m_size;

        c->setHealthMax(c->getHealthMax() + 300);
    }

    void Guild::removeMember(const std::string& c) {
        if (!m_members || m_size == 0) return;

        size_t idx = m_size;
        for (size_t i = 0; i < m_size; ++i) {
            if (m_members[i] && m_members[i]->getName() == c) {
                idx = i;
                break;
            }
        }
        if (idx == m_size) return;

        Character* who = m_members[idx];
        who->setHealthMax(who->getHealthMax() - 300);

        for (size_t i = idx; i + 1 < m_size; ++i) {
            m_members[i] = m_members[i + 1];
        }

        --m_size;

        if (m_size == 0) {
            delete[] m_members;
            m_members = nullptr;
            return;
        }

        Character** tmp = new Character * [m_size] {};
        for (size_t i = 0; i < m_size; ++i) tmp[i] = m_members[i];

        delete[] m_members;
        m_members = tmp;
    }

    Character* Guild::operator[](size_t idx) const {
        if (idx >= m_size) return nullptr;
        return m_members[idx];
    }

    void Guild::showMembers() const {
        if (!m_members || m_size == 0) {
            std::cout << "No guild.\n";
            return;
        }

        std::cout << "[Guild] " << m_name << "\n";
        for (size_t i = 0; i < m_size; ++i) {
            std::cout << "    " << (i + 1) << ": " << *m_members[i] << "\n";
        }
    }

} // namespace seneca
