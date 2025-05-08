#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <algorithm>
#include <stdexcept>

class User {
private:
    std::string name;
    int id;
    int accessLevel;
public:
    User(const std::string& name, int id, int level)
        : name(name), id(id), accessLevel(level) {
        if (name.empty() || id < 0 || level < 0) throw std::invalid_argument("Invalid user data");
    }
    virtual ~User() = default;
    std::string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }
    void setName(const std::string& n) { if (n.empty()) throw std::invalid_argument("Empty name"); name = n; }
    void setId(int i) { if (i < 0) throw std::invalid_argument("Negative ID"); id = i; }
    void setAccessLevel(int l) { if (l < 0) throw std::invalid_argument("Negative access level"); accessLevel = l; }
    virtual void displayInfo() const {
        std::cout << "Name: " << name << " | ID: " << id << " | Level: " << accessLevel;
    }
    virtual std::string getType() const = 0;
};

class Student : public User {
private:
    std::string group;
public:
    Student(const std::string& name, int id, int level, const std::string& group)
        : User(name, id, level), group(group) {
        if (group.empty()) throw std::invalid_argument("Empty group");
    }
    void setGroup(const std::string& g) { if (g.empty()) throw std::invalid_argument("Empty group"); group = g; }
    std::string getGroup() const { return group; }
    void displayInfo() const override {
        User::displayInfo();
        std::cout << " | Group: " << group << std::endl;
    }
    std::string getType() const override { return "Student"; }
};

class Teacher : public User {
private:
    std::string department;
public:
    Teacher(const std::string& name, int id, int level, const std::string& dept)
        : User(name, id, level), department(dept) {
        if (dept.empty()) throw std::invalid_argument("Empty department");
    }
    void setDepartment(const std::string& d) { if (d.empty()) throw std::invalid_argument("Empty department"); department = d; }
    std::string getDepartment() const { return department; }
    void displayInfo() const override {
        User::displayInfo();
        std::cout << " | Department: " << department << std::endl;
    }
    std::string getType() const override { return "Teacher"; }
};

class Administrator : public User {
private:
    std::string role;
public:
    Administrator(const std::string& name, int id, int level, const std::string& role)
        : User(name, id, level), role(role) {
        if (role.empty()) throw std::invalid_argument("Empty role");
    }
    void setRole(const std::string& r) { if (r.empty()) throw std::invalid_argument("Empty role"); role = r; }
    std::string getRole() const { return role; }
    void displayInfo() const override {
        User::displayInfo();
        std::cout << " | Role: " << role << std::endl;
    }
    std::string getType() const override { return "Administrator"; }
};

class Resource {
private:
    std::string name;
    int requiredLevel;
public:
    Resource(const std::string& name, int level)
        : name(name), requiredLevel(level) {
        if (name.empty() || level < 0) throw std::invalid_argument("Invalid resource data");
    }
    std::string getName() const { return name; }
    int getRequiredLevel() const { return requiredLevel; }
    bool checkAccess(const User& u) const {
        return u.getAccessLevel() >= requiredLevel;
    }
};

template<typename U, typename R>
class AccessControlSystem {
private:
    std::vector<std::shared_ptr<U>> users;
    std::vector<std::shared_ptr<R>> resources;
public:
    void addUser(std::shared_ptr<U> u) { users.push_back(u); }
    void addResource(std::shared_ptr<R> r) { resources.push_back(r); }
    std::shared_ptr<U> findUserByName(const std::string& name) const {
        auto it = std::find_if(users.begin(), users.end(), [&](auto& u) { return u->getName() == name; });
        return it != users.end() ? *it : nullptr;
    }
    std::shared_ptr<U> findUserById(int id) const {
        auto it = std::find_if(users.begin(), users.end(), [&](auto& u) { return u->getId() == id; });
        return it != users.end() ? *it : nullptr;
    }
    void sortUsersByLevel() {
        std::sort(users.begin(), users.end(), [](auto& a, auto& b) { return a->getAccessLevel() < b->getAccessLevel(); });
    }
    void checkAllAccess() const {
        for (const auto& u : users) {
            for (const auto& r : resources) {
                std::cout << u->getName()
                    << (r->checkAccess(*u) ? " has access to " : " denied access to ")
                    << r->getName() << std::endl;
            }
        }
    }
    void displayAllUsers() const {
        for (const auto& u : users) u->displayInfo();
    }
    void save(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("Cannot open file for saving");
        out << users.size() << '\n';
        for (const auto& u : users) {
            out << u->getType() << ' ' << u->getName() << ' ' << u->getId()
                << ' ' << u->getAccessLevel();
            if (u->getType() == "Student") out << ' ' << static_cast<Student*>(u.get())->getGroup();
            if (u->getType() == "Teacher") out << ' ' << static_cast<Teacher*>(u.get())->getDepartment();
            if (u->getType() == "Administrator") out << ' ' << static_cast<Administrator*>(u.get())->getRole();
            out << '\n';
        }
        out << resources.size() << '\n';
        for (const auto& r : resources) {
            out << r->getName() << ' ' << r->getRequiredLevel() << '\n';
        }
    }
    void load(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) throw std::runtime_error("Cannot open file for loading");
        size_t ucount; in >> ucount;
        users.clear();
        for (size_t i = 0; i < ucount; ++i) {
            std::string type, name, extra;
            int id, level;
            in >> type >> name >> id >> level;
            if (type == "Student") { in >> extra; addUser(std::make_shared<Student>(name, id, level, extra)); }
            else if (type == "Teacher") { in >> extra; addUser(std::make_shared<Teacher>(name, id, level, extra)); }
            else if (type == "Administrator") { in >> extra; addUser(std::make_shared<Administrator>(name, id, level, extra)); }
        }
        size_t rcount; in >> rcount;
        resources.clear();
        for (size_t i = 0; i < rcount; ++i) {
            std::string rname; int lvl;
            in >> rname >> lvl;
            addResource(std::make_shared<R>(rname, lvl));
        }
    }
};

int main() {
    try {
        AccessControlSystem<User, Resource> acs;
        acs.addUser(std::make_shared<Student>("Alice", 1, 1, "CS101"));
        acs.addUser(std::make_shared<Teacher>("Bob", 2, 2, "Math"));
        acs.addUser(std::make_shared<Administrator>("Carol", 3, 3, "IT"));

        acs.addResource(std::make_shared<Resource>("Library", 1));
        acs.addResource(std::make_shared<Resource>("Lab", 2));
        acs.addResource(std::make_shared<Resource>("AdminPanel", 3));

        std::cout << "All users before sort:\n";
        acs.displayAllUsers();

        acs.sortUsersByLevel();
        std::cout << "\nAll users after sort:\n";
        acs.displayAllUsers();

        std::cout << "\nAccess check:\n";
        acs.checkAllAccess();

        acs.save("data.txt");
        std::cout << "\nData saved to data.txt" << std::endl;


        AccessControlSystem<User, Resource> acs2;
        acs2.load("data.txt");
        std::cout << "\nLoaded users:\n";
        acs2.displayAllUsers();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}