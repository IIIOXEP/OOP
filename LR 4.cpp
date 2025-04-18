#include <iostream>
#include <memory>
#include <string>
#include <vector>

class Inventory {
private:
    std::vector<std::unique_ptr<std::string>> items; // Используем вектор уникальных указателей на строки

public:
    // Метод для добавления предмета в инвентарь
    void addItem(const std::string& item) {
        items.push_back(std::make_unique<std::string>(item));
    }

    // Метод для отображения предметов инвентаря
    void displayInventory() const {
        std::cout << "Inventory contains:\n";
        for (const auto& item : items) {
            std::cout << "- " << *item << std::endl; // Разыменование уникального указателя для получения значения
        }
    }
};

int main() {
    Inventory inventory; // Создаем инвентарь

    // Добавляем предметы в инвентарь
    inventory.addItem("Sword");
    inventory.addItem("Potion");
    inventory.addItem("Bow");

    // Отображаем инвентарь
    inventory.displayInventory();

    return 0;
}
