#include <iostream>
#include <string>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    // Перегрузка оператора ==
    bool operator==(const Character& other) const {
        return name == other.name && health == other.health;
    }

    // Перегрузка оператора <<
    friend std::ostream& operator<<(std::ostream& os, const Character& character) {
        os << "Character: " << character.name << ", HP: " << character.health
            << ", Attack: " << character.attack << ", Defense: " << character.defense;
        return os;
    }
};

class Weapon {
private:
    std::string name;
    int damage;
    float weight;

public:
    Weapon(const std::string& n, int d, float w)
        : name(n), damage(d), weight(w) {
        std::cout << "Weapon " << name << " created!\n";
    }

    ~Weapon() {
        std::cout << "Weapon " << name << " destroyed!\n";
    }

    void displayInfo() const {
        std::cout << "Weapon Name: " << name << ", Damage: " << damage
            << ", Weight: " << weight << " kg" << std::endl;
    }

    // Перегрузка оператора +
    Weapon operator+(const Weapon& other) const {
        return Weapon("Combined " + this->name + " & " + other.name,
            this->damage + other.damage,
            (this->weight + other.weight) / 2); // Средний вес
    }

    // Перегрузка оператора >
    bool operator>(const Weapon& other) const {
        return this->damage > other.damage;
    }

    // Перегрузка оператора <<
    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon) {
        os << "Weapon: " << weapon.name << ", Damage: " << weapon.damage
            << ", Weight: " << weapon.weight << " kg";
        return os;
    }
};

int main() {
    Character hero("Hero", 100, 20, 10);
    Weapon sword("Sword", 25, 3.5);
    Weapon bow("Bow", 15, 2.0);

    // Вывод информации о персонаже
    std::cout << hero << std::endl;

    // Создание нового оружия путем сложения
    Weapon combinedWeapon = sword + bow;

    // Вывод информации о комбинированном оружии
    combinedWeapon.displayInfo();

    // Сравнение двух оружий
    if (sword > bow) {
        std::cout << sword << " has more damage than " << bow << std::endl;
    }
    else {
        std::cout << bow << " has more damage than " << sword << std::endl;
    }

    return 0;
}
