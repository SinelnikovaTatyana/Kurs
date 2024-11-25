#include "server.h"
#include <iostream>
#include <stdexcept>

int main(int argc, char *argv[]) {
    try {
        Interface ui(argc, argv);
        std::string file_name = ui.getFileName();
        int port = ui.getPort();
        std::string file_error = ui.getErrorFile();

        Error errors;
        if (errors.er(file_name, file_error) == 12) {
            throw RuntimeError("Ошибка открытия файла: " + file_name);
        }

        Server server(errors);
        Authorized authorized(errors);
        Calculator calculator(errors);
        std::string error;
        int s = server.self_addr(error, file_error, port);
        if (s == -1) {
            throw RuntimeError("Ошибка при настройке сервера: " + error);
        }

        while (true) {
            int work_sock = server.client_addr(s, error, file_error);
            if (work_sock == -1) {
                continue; // Пропускаем итерацию, если произошла ошибка
            }

            if (authorized.authorized(work_sock, file_name, file_error) != 1) {
                continue; // Пропускаем итерацию, если авторизация не пройдена
            }

            if (calculator.calc(work_sock) != 1) {
                continue; // Пропускаем итерацию, если произошла ошибка в калькуляторе
            }
        }
    } catch (const RuntimeError& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Непредвиденная ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
