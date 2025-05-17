#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <stdexcept>

// ------------------------- Базовый класс Пользователь -------------------------
class User {
private:
    std::string name;
    int id;
    int accessLevel;

public:
    User(const std::string& name, int id, int accessLevel) {
        if (name.empty() || accessLevel < 0) {
            throw std::invalid_argument("Недопустимое имя или уровень доступа");
        }
        this->name = name;
        this->id = id;
        this->accessLevel = accessLevel;
    }

    virtual ~User() = default;

    std::string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    void setName(const std::string& name) { 
        if (name.empty()) throw std::invalid_argument("Имя не может быть пустым");
        this->name = name; 
    }
    void setId(int id) { this->id = id; }
    void setAccessLevel(int level) { 
        if (level < 0) throw std::invalid_argument("Уровень доступа не может быть отрицательным");
        this->accessLevel = level; 
    }

    virtual void displayInfo() const {
        std::cout << "Имя: " << name << ", ID: " << id << ", Уровень доступа: " << accessLevel << std::endl;
    }
};

// ------------------------- Производные классы -------------------------
class Student : public User {
private:
    std::string group;
public:
    Student(const std::string& name, int id, int accessLevel, const std::string& group)
        : User(name, id, accessLevel), group(group) {}

    void displayInfo() const override {
        User::displayInfo();
        std::cout << "Группа: " << group << std::endl;
    }
};

class Teacher : public User {
private:
    std::string department;
public:
    Teacher(const std::string& name, int id, int accessLevel, const std::string& department)
        : User(name, id, accessLevel), department(department) {}

    void displayInfo() const override {
        User::displayInfo();
        std::cout << "Кафедра: " << department << std::endl;
    }
};

class Administrator : public User {
private:
    std::string role;
public:
    Administrator(const std::string& name, int id, int accessLevel, const std::string& role)
        : User(name, id, accessLevel), role(role) {}

    void displayInfo() const override {
        User::displayInfo();
        std::cout << "Роль: " << role << std::endl;
    }
};

// ------------------------- Класс Ресурс -------------------------
class Resource {
private:
    std::string resourceName;
    int requiredAccessLevel;
public:
    Resource(const std::string& name, int level) : resourceName(name), requiredAccessLevel(level) {}

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }

    std::string getName() const { return resourceName; }
    int getRequiredAccessLevel() const { return requiredAccessLevel; }
};

// ------------------------- Шаблонная система контроля доступа -------------------------
template <typename T>
class AccessControlSystem {
private:
    std::vector<std::shared_ptr<T>> users;
    std::vector<Resource> resources;

public:
    void addUser(std::shared_ptr<T> user) {
        users.push_back(user);
    }

    void addResource(const Resource& resource) {
        resources.push_back(resource);
    }

    void displayUsers() const {
        for (const auto& user : users) {
            user->displayInfo();
        }
    }

    void saveUsersToFile(const std::string& filename) const {
        std::ofstream ofs(filename);
        for (const auto& user : users) {
            ofs << user->getName() << "," << user->getId() << "," << user->getAccessLevel() << "\n";
        }
        ofs.close();
    }

    void loadUsersFromFile(const std::string& filename) {
        std::ifstream ifs(filename);
        std::string name;
        int id, accessLevel;
        while (ifs >> name >> id >> accessLevel) {
            users.push_back(std::make_shared<User>(name, id, accessLevel));
        }
        ifs.close();
    }

void checkAccessToResource(const std::string& resourceName) const {
        auto it = std::find_if(resources.begin(), resources.end(), [&resourceName](const Resource& r) {
            return r.getName() == resourceName;
        });
        if (it != resources.end()) {
            for (const auto& user : users) {
                std::cout << user->getName() << (it->checkAccess(*user) ? " имеет" : " не имеет") << " доступ к ресурсу " << resourceName << std::endl;
            }
        } else {
            std::cerr << "Ресурс не найден." << std::endl;
        }
    }

    void searchUserByName(const std::string& name) const {
        for (const auto& user : users) {
            if (user->getName() == name) {
                user->displayInfo();
                return;
            }
        }
        std::cout << "Пользователь не найден." << std::endl;
    }

    void sortUsersByAccessLevel() {
        std::sort(users.begin(), users.end(), [](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
            return a->getAccessLevel() < b->getAccessLevel();
        });
    }
};

// ------------------------- Главная функция -------------------------
int main() {
    try {
        AccessControlSystem<User> system;

        system.addUser(std::make_shared<Student>("Алиса", 1, 2, "ИКБО-01-22"));
        system.addUser(std::make_shared<Teacher>("Др.Борис", 2, 4, "Физика"));
        system.addUser(std::make_shared<Administrator>("Карина", 3, 5, "ИТ-администратор"));

        system.addResource(Resource("Библиотека", 1));
        system.addResource(Resource("Лаборатория", 3));
        system.addResource(Resource("Серверная", 5));

        std::cout << "\n-- Список пользователей --\n";
        system.displayUsers();

        std::cout << "\n-- Проверка доступа к Лаборатории --\n";
        system.checkAccessToResource("Лаборатория");

        std::cout << "\n-- Поиск пользователя 'Алиса' --\n";
        system.searchUserByName("Алиса");

        std::cout << "\n-- Пользователи, отсортированные по уровню доступа --\n";
        system.sortUsersByAccessLevel();
        system.displayUsers();
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }
    return 0;
}
