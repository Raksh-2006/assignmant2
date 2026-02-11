/*
Name: Rakshith Singh
Email: rsrakshith-singh@myseneca.ca
Student ID: 131037244
Date: 2/10/26

I declare that this submission is the result of my own work and I only copied the code
that my professor provided to complete my assignments. This submitted piece of work
has not been shared with any other student or 3rd party content provider.
*/

#ifndef SENECA_CHARACTERTPL_H
#define SENECA_CHARACTERTPL_H

#include <iostream>
#include "character.h"

namespace seneca {

    template <typename T>
    class CharacterTpl : public Character {
        int m_healthMax{};
        T   m_health{};

    public:
        CharacterTpl(const char* name, int healthMax)
            : Character(name), m_healthMax(healthMax), m_health{} {
            m_health = m_healthMax;
        }

        void takeDamage(int dmg) override {
            m_health -= dmg;

            int cur = static_cast<int>(m_health);
            if (cur < 0) {
                // For numeric T this clamps to 0.
                // For InfiniteHealth, operator= does nothing (still 1).
                m_health = 0;
                cur = static_cast<int>(m_health);
            }

            if (cur <= 0) {
                std::cout << "    " << getName() << " has been defeated!\n";
            }
            else {
                std::cout << "    " << getName() << " took " << dmg
                    << " damage, " << cur << " health remaining.\n";
            }
        }

        int getHealth() const override {
            return static_cast<int>(m_health);
        }

        int getHealthMax() const override {
            return m_healthMax;
        }

        void setHealth(int health) override {
            if (health < 0) health = 0;
            if (health > m_healthMax) health = m_healthMax;
            m_health = health;
        }

        void setHealthMax(int health) override {
            if (health < 0) health = 0;
            m_healthMax = health;
            m_health = m_healthMax;
        }
    };

} // namespace seneca

#endif
