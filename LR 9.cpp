#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

// ------------------ Класс Character -------------------
class Character {
private:
    std::string name;
    int health;
    int attackPower;
    int defense;
    int level;
    int experience;

public:
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attackPower(a), defense(d), level(1), experience(0) {}

    void attackEnemy(Character& enemy) {
        int damage = attackPower - enemy.defense;
        if (damage <= 0) damage = 1;  // минимум урона 1
        enemy.takeDamage(damage);
        std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) {
            health = 0;
            throw std::runtime_error(name + " has died!");
        }
    }

    void heal(int amount) {
        health += amount;
        if (health > 100) health = 100;
        std::cout << name << " heals for " << amount << " HP!" << std::endl;
    }

    void gainExperience(int exp) {
        experience += exp;
        while (experience >= 100) {
            level++;
            experience -= 100;
            std::cout << name << " leveled up to level " << level << "!" << std::endl;
        }
    }

    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense
            << ", Level: " << level << ", Experience: " << experience << std::endl;
    }

    std::string getName() const { return name; }
    int getHealth() const { return health; }
};

// ------------------ Базовый класс Monster -------------------
class Monster {
protected:
    std::string name;
    int health;
    int attackPower;
    int defense;

public:
    Monster(std::string n, int h, int a, int d)
        : name(n), health(h), attackPower(a), defense(d) {}

    virtual ~Monster() = default;

    virtual void attack(Character& character) {
        int damage = attackPower - character.defense;
        if (damage <= 0) damage = 1;  // минимум урона 1
        character.takeDamage(damage);
        std::cout << name << " attacks " << character.getName() << " for " << damage << " damage!" << std::endl;
    }

    virtual void displayInfo() const {
        std::cout << "Monster: " << name << ", HP: " << health
            << ", Attack: " << attackPower << ", Defense: " << defense << std::endl;
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) {
            health = 0;
            throw std::runtime_error(name + " has died!");
        }
    }

    bool isAlive() const {
        return health > 0;
    }
};

// ------------------ Класс Goblin -------------------
class Goblin : public Monster {
public:
    Goblin()
        : Monster("Goblin", 30, 10, 5) {}
};

// ------------------ Класс Dragon -------------------
class Dragon : public Monster {
public:
    Dragon()
        : Monster("Dragon", 100, 25, 15) {}
};

// ------------------ Класс Skeleton (задание 1) -------------------
class Skeleton : public Monster {
public:
    Skeleton()
        : Monster("Skeleton", 40, 15, 10) {}
};

// ------------------ Класс Inventory (задание 2) -------------------
class Inventory {
private:
    std::vector<std::string> items;
public:
    void addItem(const std::string& item) {
        items.push_back(item);
        std::cout << item << " добавлен в инвентарь." << std::endl;
    }

    void removeItem(const std::string& item) {
        auto it = std::find(items.begin(), items.end(), item);
        if (it != items.end()) {
            items.erase(it);
            std::cout << item << " удален из инвентаря." << std::endl;
        }
        else {
            std::cout << item << " не найден в инвентаре." << std::endl;
        }
    }

    void displayInventory() const {
        std::cout << "Инвентарь: ";
        if (items.empty()) {
            std::cout << "Пустой" << std::endl;
            return;
        }
        for (const auto& i : items) {
            std::cout << i << ", ";
        }
        std::cout << std::endl;
    }
};

// ------------------ Шаблонный класс Logger (задание 5) -------------------
template<typename T>
class Logger {
private:
    std::ofstream file;
public:
    Logger(const std::string& filename) {
        file.open(filename, std::ios::app);
        if (!file) {
            throw std::runtime_error("Не удалось открыть файл лога");
        }
    }
    ~Logger() {
        if (file.is_open()) file.close();
    }

    void log(const T& message) {
        file << message << std::endl;
    }
};

// ------------------ Класс Game -------------------
class Game {
private:
    Character hero;
    Inventory inventory;
    Logger<std::string> logger;

public:
    Game(std::string playerName)
        : hero(playerName, 100, 20, 10), logger("game_log.txt") {}

    void start() {
        std::cout << "Начало игры!" << std::endl;
        hero.displayInfo();
        inventory.displayInventory();
    }

    void battle(Monster& monster) {
        std::cout << "Бой с монстром: " << std::endl;
        monster.displayInfo();

        try {
            while (hero.getHealth() > 0 && monster.isAlive()) {
                hero.attackEnemy(monster);
                logger.log(hero.getName() + " атакует " + monster.name);

                if (!monster.isAlive()) {
                    std::cout << monster.name << " повержен!" << std::endl;
                    hero.gainExperience(50);
                    logger.log(monster.name + " повержен.");
                    break;
                }

                monster.attack(hero);
                logger.log(monster.name + " атакует " + hero.getName());
            }
        }
        catch (const std::runtime_error& ex) {
            std::cout << "Исключение: " << ex.what() << std::endl;
            logger.log(std::string("Исключение: ") + ex.what());
        }
    }

    void saveProgress(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Ошибка сохранения прогресса!" << std::endl;
            return;
        }
        // Просто пример записи, можно улучшать сериализацию
        file << hero.getName() << "\n"
            << hero.getHealth() << "\n"
            << hero.level << "\n"  // например, сделать геттер или friend-класс
            << "\n"; // Можно добавить больше данных
        std::cout << "Прогресс сохранён в " << filename << std::endl;
    }

    void loadProgress(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Ошибка загрузки прогресса!" << std::endl;
            return;
        }
        // Для примера: чтение данных (нужно реализовать соответствующие методы для установки)
        std::string name;
        int health;
        int lvl;

        file >> name >> health >> lvl;

        std::cout << "Прогресс загружен: " << name << ", HP=" << health << ", Level=" << lvl << std::endl;
    }

    Inventory& getInventory() { return inventory; }
    Character& getHero() { return hero; }
};

int main() {
    Game game("Hero");

    // Начинаем игру
    game.start();

    // Работа с инвентарём
    Inventory& inv = game.getInventory();
    inv.addItem("Меч");
    inv.addItem("Зелье лечения");
    inv.displayInventory();
    inv.removeItem("Меч");
    inv.displayInventory();

    // Создаём монстров
    Goblin goblin;
    Dragon dragon;
    Skeleton skeleton;  // новый тип

    // Бой с гоблином
    game.battle(goblin);

    // Бой с драконом
    game.battle(dragon);

    // Бой со скелетом
    game.battle(skeleton);

    // Сохранение прогресса
    game.saveProgress("savegame.txt");

    // Загрузка прогресса (демо)
    game.loadProgress("savegame.txt");

    return 0;
}

