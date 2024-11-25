#include <UnitTest++/UnitTest++.h>
#include <UnitTest++/TestReporterStdout.h>
#include "server.h"
#include <vector>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <limits>
#include <iostream>

// Сценарий тестирования справки
SUITE(HelpTest)
{
    TEST(ShortHelp) //-help
    {
        const char* args[] = {"test", "-h"};
        Interface iface(2, const_cast<char**>(args));
        CHECK(!iface.getDescription().empty());
    }

    TEST(LongHelp) {
        const char* args[] = {"test", "--help"};
        Interface iface(2, const_cast<char**>(args));
        CHECK(!iface.getDescription().empty());
    }

    TEST(HelpAndFile) {
        const char* args[] = {"test", "-h", "-f", "base.txt"};
        Interface iface(4, const_cast<char**>(args));
        CHECK(!iface.getDescription().empty());
    }

    TEST(FileAndHelp) {
        const char* args[] = {"test", "-f", "base.txt", "-h"};
        Interface iface(4, const_cast<char**>(args));
        CHECK(!iface.getDescription().empty());
    }

    TEST(HelpAndPort) {
        const char* args[] = {"test", "-h", "-p", "8080"};
        Interface iface(4, const_cast<char**>(args));
        CHECK(!iface.getDescription().empty());
    }

    TEST(PortAndHelp) {
        const char* args[] = {"test", "-p", "8080", "-h"};
        Interface iface(4, const_cast<char**>(args));
        CHECK(!iface.getDescription().empty());
    }

    TEST(HelpAndErrorFile) {
        const char* args[] = {"test", "-h", "-e", "error.txt"};
        Interface iface(4, const_cast<char**>(args));
        CHECK(!iface.getDescription().empty());
    }

    TEST(ErrorFileAndHelp) {
        const char* args[] = {"test", "-e", "error.txt", "-h"};
        Interface iface(4, const_cast<char**>(args));
        CHECK(!iface.getDescription().empty());
    }
}

// Сценарий тестирования калькулятора
SUITE(CalculatorTest)
{
    // Тест на нормальный случай
    TEST(ProcessVector_NormalCase) {
        int sv[2];
        socketpair(AF_UNIX, SOCK_STREAM, 0, sv);

        uint32_t quantity = 1;
        uint32_t length = 4;
        double vector[] = {-4000.0, -5000.0, -6000.0, -7000.0};
        double expected_product = 8.4e+14; 

        send(sv[0], &quantity, sizeof(quantity), 0);
        send(sv[0], &length, sizeof(length), 0);
        for (double num : vector) {
            send(sv[0], &num, sizeof(num), 0);
        }

        Error error;
        Calculator calc(error);
        calc.calc(sv[1]);

        double result;
        recv(sv[0], &result, sizeof(result), 0);
        CHECK_EQUAL(expected_product, result);

        close(sv[0]);
        close(sv[1]);
    }

    // Тест на пустой вектор
    TEST(ProcessVector_EmptyVector) {
        int sv[2];
        socketpair(AF_UNIX, SOCK_STREAM, 0, sv);

        uint32_t quantity = 1;
        uint32_t length = 0;
        double expected_product = 1.0; // Предполагаем, что произведение пустого вектора равно 1

        send(sv[0], &quantity, sizeof(quantity), 0);
        send(sv[0], &length, sizeof(length), 0);

        Error error;
        Calculator calc(error);
        calc.calc(sv[1]);

        double result;
        recv(sv[0], &result, sizeof(result), 0);
        CHECK_EQUAL(expected_product, result);

        close(sv[0]);
        close(sv[1]);
    }

    // Тест на вектор с одним элементом
    TEST(ProcessVector_SingleElement) {
        int sv[2];
        socketpair(AF_UNIX, SOCK_STREAM, 0, sv);

        uint32_t quantity = 1;
        uint32_t length = 1;
        double vector[] = {5.0};
        double expected_product = 5.0;

        send(sv[0], &quantity, sizeof(quantity), 0);
        send(sv[0], &length, sizeof(length), 0);
        send(sv[0], &vector[0], sizeof(vector[0]), 0);

        Error error;
        Calculator calc(error);
        calc.calc(sv[1]);

        double result;
        recv(sv[0], &result, sizeof(result), 0);
        CHECK_EQUAL(expected_product, result);

        close(sv[0]);
        close(sv[1]);
    }

    // Тест на вектор с нулевым элементом
    TEST(ProcessVector_ZeroElement) {
        int sv[2];
        socketpair(AF_UNIX, SOCK_STREAM, 0, sv);

        uint32_t quantity = 1;
        uint32_t length = 3;
        double vector[] = {2.0, 0.0, 4.0};
        double expected_product = 0.0;

        send(sv[0], &quantity, sizeof(quantity), 0);
        send(sv[0], &length, sizeof(length), 0);
        for (double num : vector) {
            send(sv[0], &num, sizeof(num), 0);
        }

        Error error;
        Calculator calc(error);
        calc.calc(sv[1]);

        double result;
        recv(sv[0], &result, sizeof(result), 0);
        CHECK_EQUAL(expected_product, result);

        close(sv[0]);
        close(sv[1]);
    }

    // Тест на большой ввод
    TEST(ProcessVector_LargeInput) {
        int sv[2];
        socketpair(AF_UNIX, SOCK_STREAM, 0, sv);

        uint32_t quantity = 1;
        uint32_t length = 1000000;
        std::vector<double> vector(length, 1.0);
        double expected_product = 1.0; // Произведение всех единиц равно 1

        send(sv[0], &quantity, sizeof(quantity), 0);
        send(sv[0], &length, sizeof(length), 0);
        for (double num : vector) {
            send(sv[0], &num, sizeof(num), 0);
        }

        Error error;
        Calculator calc(error);
        calc.calc(sv[1]);

        double result;
        recv(sv[0], &result, sizeof(result), 0);
        CHECK_EQUAL(expected_product, result);

        close(sv[0]);
        close(sv[1]);
    }
}

int main() {
    return UnitTest::RunAllTests();
}
