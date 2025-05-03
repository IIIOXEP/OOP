#include <iostream>
#include <queue>
#include <stdexcept>

template <typename T>
class Queue {
private:
    std::queue<T> data;

public:
    void push(const T& value) {
        data.push(value);
    }

    void pop() {
        if (data.empty()) {
            throw std::out_of_range("Попытка вызова pop() у пустой очереди");
        }
        data.pop();
    }

    T& front() {
        if (data.empty()) {
            throw std::out_of_range("Очередь пуста, нет элементов для доступа");
        }
        return data.front();
    }

    bool empty() const {
        return data.empty();
    }
};

int main() {
    Queue<int> q;

    try {
        q.pop(); // Попытка удалить элемент из пустой очереди, вызовет исключение
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    // Дополнительная проверка - нормальная вставка и извлечение
    q.push(10);
    std::cout << "Первый элемент очереди: " << q.front() << std::endl;
    q.pop();

    try {
        q.pop(); // Опять вызываем pop для пустой очереди
    }
    catch (const std::out_of_range& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
