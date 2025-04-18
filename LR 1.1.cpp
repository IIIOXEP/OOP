#include <iostream>
#include <string>

class Character {
private:
    std::string name;  // Приватное поле: имя персонажа
    int health;        // Приватное поле: уровень здоровья
    int attack;        // Приватное поле: уровень атаки
    int defense;       // Приватное поле: уровень защиты
    const int maxHealth; // Максимальный уровень здоровья

public:
    // Конструктор для инициализации данных
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d), maxHealth(100) {}

    // Метод для получения уровня здоровья
    int getHealth() const {
        return health;
    }

    // Метод для вывода информации о персонаже
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    // Метод для атаки другого персонажа
    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.takeDamage(damage); // используем метод takeDamage вместо прямого изменения здоровья
            std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
        }
        else {
            std::cout << name << " attacks " << enemy.name << ", but it has no effect!" << std::endl;
        }
    }

    // Метод для увеличения уровня здоровья
    void heal(int amount) {
        health += amount;
        if (health > maxHealth) {
            health = maxHealth; // Не позволяем превышать максимальное здоровье
        }
    }

    // Метод для уменьшения уровня здоровья
    void takeDamage(int amount) {
        health -= amount;
        if (health < 0) {
            health = 0; // Не позволяём здоровью опускаться ниже 0
        }
    }
};

int main() {
    // Создаем объекты персонажей
    Character hero("Hero", 100, 20, 10);
    Character monster("Goblin", 50, 15, 5);

    // Выводим информацию о персонажах
    hero.displayInfo();
    monster.displayInfo();

    // Герой атакует монстра
    hero.attackEnemy(monster);
    monster.displayInfo();

    // Монстр получает урон
    monster.takeDamage(10);
    monster.displayInfo();

    // Герой лечит себя
    hero.takeDamage(30); // Уменьшаем здоровье героя для тестирования
    hero.displayInfo();
    hero.heal(20); // Лечение на 20
    hero.displayInfo();

    // Пробуем лечить героя больше его максимального здоровья
    hero.heal(100);
    hero.displayInfo();

    return 0;
}
