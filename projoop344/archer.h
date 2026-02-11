/*
Name: Rakshith Singh
Email: rsrakshith-singh@myseneca.ca
Student ID: 131037244
Date: 2/10/26

I declare that this submission is the result of my own work and I only copied the code
that my professor provided to complete my assignments. This submitted piece of work
has not been shared with any other student or 3rd party content provider.
*/
#ifndef SENECA_ARCHER_H
#define SENECA_ARCHER_H

#include <iostream>
#include "health.h"
#include "characterTpl.h"

namespace seneca {

    template <typename Weapon_t>
    class Archer : public CharacterTpl<seneca::SuperHealth> {
        int m_baseDefense{};
        int  m_baseAttack{};
        Weapon_t m_weapon{};

    public:
        Archer(const char* name, int healthMax, int baseAttack, int baseDefense, Weapon_t weapon)
            : CharacterTpl<seneca::SuperHealth>(name, healthMax)
            , m_baseDefense(baseDefense)
            , m_baseAttack(baseAttack)
            , m_weapon(weapon) {
        }

        int getAttackAmnt() const override {
            return static_cast<int>(1.3 * m_baseAttack);
        }

        int getDefenseAmnt() const override {
            return static_cast<int>(1.2 * m_baseDefense);
        }

        Character* clone() const override {
            return new Archer(*this);
        }

        void attack(Character* enemy) override {
            std::cout << this->getName() << " is attacking " << enemy->getName() << ".\n";

            int dmg = getAttackAmnt();

            std::cout << "    Archer deals " << dmg << " ranged damage!\n";
            enemy->takeDamage(dmg);
        }

        void takeDamage(int dmg) override {
            std::cout << this->getName() << " is attacked for " << dmg << " damage.\n";
            std::cout << "    Archer has a defense of " << getDefenseAmnt()
                << ". Reducing damage received.\n";

            dmg -= getDefenseAmnt();
            if (dmg < 0) dmg = 0;

            CharacterTpl<seneca::SuperHealth>::takeDamage(dmg);
        }
    };

} 
#endif
