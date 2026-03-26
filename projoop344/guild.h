#ifndef SENECA_GUILD_H
#define SENECA_GUILD_H

#include <string>
#include <cstddef>
#include "character.h"

namespace seneca {

    class Guild {
        std::string m_name{};
        Character** m_members{};
        size_t      m_size{};

        void clear();
        void copyFrom(const Guild& other);
        void moveFrom(Guild& other) noexcept;

    public:
        Guild();
        Guild(const char* name);

        Guild(const Guild& other);
        Guild(Guild&& other) noexcept;

        Guild& operator=(const Guild& other);
        Guild& operator=(Guild&& other) noexcept;

        ~Guild();

        void addMember(Character* c);
        void removeMember(const std::string& c);

        Character* operator[](size_t idx) const;

        void showMembers() const;
    };

} // namespace seneca

#endif
