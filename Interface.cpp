#include "server.h"
#include <getopt.h>
#include <iostream>
#include <stdexcept>

Interface::Interface(int argc, char* argv[]) {
    try {
        parseArguments(argc, argv);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при парсинге аргументов: " << e.what() << std::endl;
        exit(1);
    }
}

std::string Interface::getFileName() const {
    return fileName;
}

int Interface::getPort() const {
    return port;
}

std::string Interface::getErrorFile() const {
    return file_error;
}

std::string Interface::getDescription() const {
    return description;
}

void Interface::parseArguments(int argc, char* argv[]) {
    const struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"file", required_argument, 0, 'f'},
        {"port", required_argument, 0, 'p'},
        {"error", required_argument, 0, 'e'},
        {0, 0, 0, 0}
    };

    int option;
    int option_index = 0;
    fileName = "/etc/vcalc.conf";
    port = 33333;
    file_error = "/var/log/vcalc.log";
    description = "";

    while ((option = getopt_long(argc, argv, "hf:p:e:", long_options, &option_index)) != -1) {
        switch (option) {
            case 'h':
                description = "Векторный калькулятор выполняющий действие нахождения произведения векторов\n"
                              "Инструкция: -f База данных пользователей -p Порт -e Файл ошибок\n"
                              "-h Помощь\n"
                              "-f Название файла\n"
                              "-p Порт\n"
                              "-e Файл ошибок\n";
                return;
            case 'f': {
                fileName = std::string(optarg);
            }
                break;
            case 'p': {
                try {
                    port = std::stoi(std::string(optarg));
                    if (port < 0 || port > 65535) {
                        throw std::invalid_argument("Неверный порт");
                    }
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Ошибка: неверный формат порта" << std::endl;
                    exit(1);
                }
            }
                break;
            case 'e': {
                file_error = std::string(optarg);
            }
                break;
            case '?':
                std::cout << "Неверно введен параметр. Используйте -h для помощи." << std::endl;
                exit(1);
            default:
                std::cout << "Неизвестный параметр. Используйте -h для помощи." << std::endl;
                exit(1);
        }
    }

    if (description.empty()) {
        description = "Векторный калькулятор выполняющий действие нахождения произведения векторов\n"
                      "Инструкция: -f База данных пользователей -p Порт -e Файл ошибок\n"
                      "-h Помощь\n"
                      "-f Название файла\n"
                      "-p Порт\n"
                      "-e Файл ошибок\n";
    }
}
