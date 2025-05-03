#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>
#include <cstdlib>

// Класс персонажа
class Character {
public:
    std::string name;
    int health;
    int attackPower;
    int defense;

    Character(const std::string& n, int h, int atk, int def)
        : name(n), health(h), attackPower(atk), defense(def) {}

    void displayInfo() const {
        std::cout << name << " HP: " << health << std::endl;
    }
};

// Класс монстра
class Monster {
public:
    std::string name;
    int health;
    int attackPower;
    int defense;

    Monster(const std::string& n, int h, int atk, int def)
        : name(n), health(h), attackPower(atk), defense(def) {}

    void displayInfo() const {
        std::cout << name << " HP: " << health << std::endl;
    }
};

// Мьютекс для синхронизации доступа к здоровью
std::mutex battleMutex;

// Функция боя
void fight(Character& hero, Monster& monster) {
    while (true) {
        {
            std::lock_guard<std::mutex> lock(battleMutex);

            if (hero.health <= 0 || monster.health <= 0) {
                break;
            }

            // Герой атакует монстра
            int damageToMonster = hero.attackPower - monster.defense;
            if (damageToMonster < 0) damageToMonster = 0;
            monster.health -= damageToMonster;
            std::cout << hero.name << " наносит " << damageToMonster << " урона " << monster.name << std::endl;

            if (monster.health <= 0) {
                std::cout << monster.name << " повержен!" << std::endl;
                break;
            }

            // Монстр атакует героя
            int damageToHero = monster.attackPower - hero.defense;
            if (damageToHero < 0) damageToHero = 0;
            hero.health -= damageToHero;
            std::cout << monster.name << " наносит " << damageToHero << " урона " << hero.name << std::endl;

            if (hero.health <= 0) {
                std::cout << hero.name << " побежден!" << std::endl;
                break;
            }
        }
        // Задержка между раундами боя
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int main() {
    Character hero("Hero", 100, 20, 5);
    Monster monster("Goblin", 80, 15, 3);

    std::cout << "Бой начинается!" << std::endl;

    std::thread fightThread(fight, std::ref(hero), std::ref(monster));

    fightThread.join(); // Ждем окончания боя

    std::cout << "Итог боя:" << std::endl;
    hero.displayInfo();
    monster.displayInfo();

    return 0;
}
