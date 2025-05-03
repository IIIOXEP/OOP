#include <iostream>
#include <deque>    // используем deque для хранения элементов

template <typename T>
class Queue {
private:
    std::deque<T> data;

public:
    // Добавление элемента в очередь
    void push(const T& item) {
        data.push_back(item);
    }

    // Удаление элемента из очереди
    void pop() {
        if (data.empty()) {
            std::cerr << "Ошибка: Попытка удаления из пустой очереди!" << std::endl;
            return;
        }
        data.pop_front();
    }

    // Получение первого элемента очереди (без удаления)
    T& front() {
        if (data.empty()) {
            throw std::out_of_range("Очередь пуста");
        }
        return data.front();
    }

    // Проверка, пустая ли очередь
    bool empty() const {
        return data.empty();
    }

    // Вывод всех элементов (для наглядности)
    void display() const {
        for (const T& el : data) {
            std::cout << el << ' ';
        }
        std::cout << std::endl;
    }
};

int main() {
    // Очередь строк
    Queue<std::string> stringQueue;
    stringQueue.push("apple");
    stringQueue.push("banana");
    stringQueue.push("cherry");

    std::cout << "Строки в очереди: ";
    stringQueue.display();

    stringQueue.pop();
    std::cout << "После pop: ";
    stringQueue.display();

    // Очередь чисел
    Queue<int> intQueue;
    intQueue.push(10);
    intQueue.push(20);
    intQueue.push(30);

    std::cout << "Числа в очереди: ";
    intQueue.display();

    intQueue.pop();
    std::cout << "После pop: ";
    intQueue.display();

    return 0;
}
