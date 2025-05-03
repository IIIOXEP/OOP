#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

// Базовый класс Entity
class Entity {
protected:
    std::string name;
    int health;

public:
    Entity(const std::string& name = "", int health = 0) : name(name), health(health) {}
    virtual ~Entity() {}

    virtual void displayInfo() const {
        std::cout << "Name: " << name << ", Health: " << health;
    }

    // Сериализация в поток
    virtual void save(std::ofstream& out) const {
        out << name << '\n' << health << '\n';
    }

    // Статический метод для создания объекта из потока
    static Entity* load(std::ifstream& in);

    // Метод возвращает тип объекта (для сохранения)
    virtual std::string getType() const = 0;
};

// Класс Player
class Player : public Entity {
    int level;

public:
    Player(const std::string& name = "", int health = 0, int level = 0)
        : Entity(name, health), level(level) {}

    void displayInfo() const override {
        std::cout << "Player - Name: " << name << ", Health: " << health << ", Level: " << level << std::endl;
    }

    void save(std::ofstream& out) const override {
        out << getType() << '\n';
        Entity::save(out);
        out << level << '\n';
    }

    static Player* loadPlayer(std::ifstream& in) {
        std::string name;
        int health, level;

        std::getline(in, name);
        in >> health;
        in.ignore(); // пропускаем символ новой строки
        in >> level;
        in.ignore();

        return new Player(name, health, level);
    }

    std::string getType() const override {
        return "Player";
    }
};

// Класс Enemy
class Enemy : public Entity {
    std::string enemyType;

public:
    Enemy(const std::string& name = "", int health = 0, const std::string& enemyType = "")
        : Entity(name, health), enemyType(enemyType) {}

    void displayInfo() const override {
        std::cout << "Enemy - Name: " << name << ", Health: " << health << ", Type: " << enemyType << std::endl;
    }

    void save(std::ofstream& out) const override {
        out << getType() << '\n';
        Entity::save(out);
        out << enemyType << '\n';
    }

    static Enemy* loadEnemy(std::ifstream& in) {
        std::string name, enemyType;
        int health;

        std::getline(in, name);
        in >> health;
        in.ignore();
        std::getline(in, enemyType);

        return new Enemy(name, health, enemyType);
    }

    std::string getType() const override {
        return "Enemy";
    }
};

// Реализация статического метода load для базового Entity
Entity* Entity::load(std::ifstream& in) {
    std::string type;
    if (!std::getline(in, type)) {
        return nullptr; 
    }

    if (type == "Player") {
        return Player::loadPlayer(in);
    }
    else if (type == "Enemy") {
        return Enemy::loadEnemy(in);
    }
    else {
        throw std::runtime_error("Unknown entity type: " + type);
    }
}

// GameManager с указателями на Entity
template <typename T>
class GameManager {
private:
    std::vector<T> entities;

public:
    ~GameManager() {
        for (auto e : entities) {
            delete e;
        }
    }

    void addEntity(const T& entity) {
        entities.push_back(entity);
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity->displayInfo();
        }
    }

    std::vector<T>& getEntities() {
        return entities;
    }
};

// Функция сохранения
void saveToFile(const GameManager<Entity*>& manager, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing.");
    }

    for (const auto& entity : manager.getEntities()) {
        entity->save(file);
    }
}

// Функция загрузки
void loadFromFile(GameManager<Entity*>& manager, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        throw std::runtime_error("Failed to open file for reading.");
    }

    // Очистим старые данные
    for (auto e : manager.getEntities()) {
        delete e;
    }
    manager.getEntities().clear();

    while (true) {
        Entity* entity = Entity::load(file);
        if (!entity) {
            break;
        }
        manager.addEntity(entity);
    }
}

int main() {
    GameManager<Entity*> manager;
    manager.addEntity(new Player("Hero", 100, 5));
    manager.addEntity(new Enemy("Goblin", 50, "Orc"));

    std::cout << "Исходные данные:" << std::endl;
    manager.displayAll();

    saveToFile(manager, "game_save.txt");

    GameManager<Entity*> loadedManager;
    loadFromFile(loadedManager, "game_save.txt");

    std::cout << "\nДанные после загрузки:" << std::endl;
    loadedManager.displayAll();

    return 0;
}
