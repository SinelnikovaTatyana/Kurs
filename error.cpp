#include "server.h"
#include <ctime>
#include <fstream>
#include <iostream>

void Error::errors(std::string error, std::string name) {
    try {
        std::ofstream file;
        file.open(name, std::ios::app);
        if (!file.is_open()) {
            throw std::runtime_error("Ошибка открытия файла для записи ошибок: " + name);
        }

        time_t seconds = time(NULL);
        tm* timeinfo = localtime(&seconds);
        file << error << ':' << asctime(timeinfo) << std::endl;
        std::cout << "error: " << error << std::endl;

        file.close(); // Закрываем файл после записи
    } catch (const std::runtime_error& e) {
        std::cerr << "Ошибка записи в файл ошибок: " << e.what() << std::endl;
    }
}

int Error::er(std::string fileName, std::string file_error) {
    try {
        std::fstream file;
        file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        file.open(fileName);
        return 1;
    } catch (const std::runtime_error& e) {
        std::string error = "Ошибка открытия файла: " + std::string(e.what());
        errors(error, file_error);
        throw std::runtime_error("Ошибка открытия файла: " + fileName);
    }
}
