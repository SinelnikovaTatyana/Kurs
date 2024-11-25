#include "server.h"

#include <vector>
#include <limits>
#include <cstdint>

int Calculator::calc(int work_sock) {
    try {
        uint32_t Quantity;
        uint32_t Length;
        double Vector_number;
        double Product;

        // Получаем количество векторов
        ssize_t bytes_received = recv(work_sock, &Quantity, sizeof(Quantity), 0);
        if (bytes_received <= 0) {
            throw RuntimeError("Ошибка при получении количества векторов");
        }

        for (uint32_t j = 0; j < Quantity; j++) {
            // Получаем длину вектора
            bytes_received = recv(work_sock, &Length, sizeof(Length), 0);
            if (bytes_received <= 0) {
                throw RuntimeError("Ошибка при получении длины вектора");
            }
            Product = 1.0; // Начинаем с 1 для произведения

            for (uint32_t i = 0; i < Length; i++) {
                bytes_received = recv(work_sock, &Vector_number, sizeof(Vector_number), 0);
                if (bytes_received <= 0) {
                    throw RuntimeError("Ошибка при получении элемента вектора");
                }
                Product *= Vector_number; 
            }

            // Отправляем результат произведения
            ssize_t bytes_sent = send(work_sock, &Product, sizeof(Product), 0);
            if (bytes_sent <= 0) {
                throw RuntimeError("Ошибка при отправке результата");
            }
        }

        close(work_sock);
        return 1;
    } catch (const RuntimeError& e) {
        std::cerr << "Ошибка в калькуляторе: " << e.what() << std::endl;
        close(work_sock); // Закрываем сокет в случае ошибки
        return -1;
    }
}
