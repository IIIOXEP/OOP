#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

class Entity {
protected:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    // Метод атаки
    virtual void attack(Entity& target) {
        int damage = attack - target.defense;
        if (damage > 0) {
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    // Метод лечения
    virtual void heal(int amount) {
        health += amount;
        std::cout << name << " heals for " << amount << " HP!\n";
    }

    // Метод вывода информации
    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    virtual ~Entity() {}
};

class Character : public Entity {
public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {}

    // Переопределение метода attack
    void attack(Entity& target) override {
        int damage = attack - target.defense;
        if (damage > 0) {
            // Шанс на критический удар (20%)
            if (rand() % 100 < 20) {
                damage *= 2;
                std::cout << "Critical hit! ";
            }
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    // Переопределение метода heal
    void heal(int amount) override {
        health += amount;
        std::cout << name << " heals for " << amount << " HP!\n";
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        std::cout << "Character: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {}

    // Переопределение метода attack
    void attack(Entity& target) override {
        int damage = attack - target.defense;
        if (damage > 0) {
            // Шанс на ядовитую атаку (30%)
            if (rand() % 100 < 30) {
                damage += 5; // Дополнительный урон от яда
                std::cout << "Poisonous attack! ";
            }
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    // Переопределение метода displayInfo
    void displayInfo() const override {
        std::cout << "Monster: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

// Новый класс Boss, наследованный от Monster
class Boss : public Monster {
public:
    Boss(const std::string& n, int h, int a, int d)
        : Monster(n, h, a, d) {}

    // Переопределение метода атаки для Boss
    void attack(Entity& target) override {
        int damage = attack - target.defense;
        if (damage > 0) {
            damage += 10; // Удар кувалдой наносит дополнительный урон
            target.health -= damage;
            std::cout << name << " uses Hammer Strike on " << target.name << " for " << damage << " damage!\n";
        }
        else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(0))); // Инициализация генератора случайных чисел

    // Создание объектов
    Character hero("Hero", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);
    Boss boss("Boss", 150, 30, 20); // Создание объекта Boss

    // Массив указателей на базовый класс
    Entity* entities[] = { &hero, &goblin, &boss };

    // Полиморфное поведение
    for (auto& entity : entities) {
        entity->displayInfo(); // Вывод информации о сущности
    }

    // Бой между персонажем и монстрами
    hero.attack(goblin);
    goblin.attack(hero);
    boss.attack(hero);

    // Применяем лечение
    hero.heal(20);
    goblin.heal(10);
    boss.heal(15);

    return 0;
}
