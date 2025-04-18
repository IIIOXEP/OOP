#include <iostream>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // Конструктор
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Character " << name << " created!\n";
    }

    // Деструктор
    ~Character() {
        std::cout << "Character " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    // Конструктор
    Monster(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {
        std::cout << "Monster " << name << " created!\n";
    }

    // Деструктор
    ~Monster() {
        std::cout << "Monster " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }
};

// Новый класс Weapon
class Weapon {
private:
    std::string name;
    int damage;
    float weight;

public:
    // Конструктор
    Weapon(const std::string& n, int d, float w)
        : name(n), damage(d), weight(w) {
        std::cout << "Weapon " << name << " created!\n";
    }

    // Деструктор
    ~Weapon() {
        std::cout << "Weapon " << name << " destroyed!\n";
    }

    // Метод для вывода информации об оружии
    void displayInfo() const {
        std::cout << "Weapon Name: " << name << ", Damage: " << damage
            << ", Weight: " << weight << " kg" << std::endl;
    }
};

int main() {
    // Создание объектов
    Character hero("Hero", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);

    // Создание объектов оружия
    Weapon sword("Sword", 25, 3.5);
    Weapon axe("Axe", 30, 5.0);
    Weapon bow("Bow", 15, 2.0);

    // Вывод информации о персонаже и монстре
    hero.displayInfo();
    goblin.displayInfo();

    // Вывод информации об оружии
    sword.displayInfo();
    axe.displayInfo();
    bow.displayInfo();

    return 0;
}
