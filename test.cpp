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

    TEST(LongHelp) {   //--help
        const char* args[] = {"test", "--help"};
        Interface iface(2, const_cast<char**>(args));
        CHECK(!iface.getDescription().empty());
    }

    TEST(HelpAndFile) { //несколько параметров
        const char* args[] = {"test", "-h", "-f", "base.txt"};
        Interface iface(4, const_cast<char**>(args));
        CHECK(!iface.getDescription().empty());
    }

    TEST(FileAndHelp) {//несколько параметров в другом порядке
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

								// Сценарий тестирования рабочих параметров
SUITE(WorkParamsTest) {  //-port

  TEST(ShortPort) {
    const char* args[] = {"test", "-p", "33333"};
    Interface iface(3, const_cast<char**>(args));
    CHECK_EQUAL(iface.getPort(), 33333);
  }

  TEST(LongPort) {  //--port
    const char* args[] = {"test", "--port", "33333"};
    Interface iface(3, const_cast<char**>(args));
    CHECK_EQUAL(iface.getPort(), 33333);
  }

  TEST(PortAndDataFileAndLogFile) {
    const char* args[] = {"test", "-p", "33333", "-f", "data.txt", "-e", "error.txt"};
    Interface iface(7, const_cast<char**>(args));
    CHECK_EQUAL(iface.getPort(), 33333);
    CHECK_EQUAL(iface.getFileName(), "data.txt");
    CHECK_EQUAL(iface.getErrorFile(), "error.txt");
  }

  TEST(DataFileAndLogFileAndPort) {
    const char* args[] = {"test", "-f", "data.txt", "-e", "error.txt", "-p", "33333"};
    Interface iface(7, const_cast<char**>(args));
    CHECK_EQUAL(iface.getPort(), 33333);
    CHECK_EQUAL(iface.getFileName(), "data.txt");
    CHECK_EQUAL(iface.getErrorFile(), "error.txt");
  }

  TEST(DefaultValues) {
    const char* args[] = {"test"};
    Interface iface(1, const_cast<char**>(args));
    CHECK_EQUAL(iface.getPort(), 33333); // Предполагаем стандартный порт
    CHECK_EQUAL(iface.getFileName(), "/etc/vcalc.conf");
    CHECK_EQUAL(iface.getErrorFile(), "/var/log/vcalc.log");
  }

  TEST(MissingPort) {
    const char* args[] = {"test", "-f", "data.txt", "-e", "error.txt"};
    Interface iface(4, const_cast<char**>(args));
    CHECK_EQUAL(iface.getPort(), 33333);
  }

  TEST(InvalidPort) {
    const char* args[] = {"test", "-p", "65536", "-f", "data.txt", "-e", "error.txt"};
    CHECK_THROW(Interface iface(7, const_cast<char**>(args)), std::invalid_argument);
	}
}

/*													// Сценарий тестирования исключений
SUITE(ErrorTest)
{
    TEST(OpenNonExistentFile) {
        Error errors;
        CHECK_THROW(errors.er("non_existent_file.txt", "error.txt"), RuntimeError);
    }

    TEST(InvalidPort) {
        const char* args[] = {"test", "-p", "65536"};
        CHECK_THROW(Interface iface(3, const_cast<char**>(args)), std::invalid_argument);
    }

    TEST(InvalidFileName) {
        const char* args[] = {"test", "-f", ""};
        CHECK_THROW(Interface iface(3, const_cast<char**>(args)), std::invalid_argument);
    }

    TEST(InvalidErrorFile) {
        const char* args[] = {"test", "-e", ""};
        CHECK_THROW(Interface iface(3, const_cast<char**>(args)), std::invalid_argument);
    }
}
*/														// Сценарий тестирования калькулятора
SUITE(CalculatorTest)
{
    // Тест на нормальный случай
    TEST(ProcessVector_NormalCase) {
        std::vector<double> vector = {-4000.0, -5000.0, -6000.0, -7000.0};
        double expected_product = 8.4e+14; 

        double result = Calculator::processVector(vector);
        CHECK_EQUAL(expected_product, result);
    }

    // Тест на пустой вектор
    TEST(ProcessVector_EmptyVector) {
        std::vector<double> vector;
        double expected_product = 1.0; // Предполагаем, что произведение пустого вектора равно 1

        double result = Calculator::processVector(vector);
        CHECK_EQUAL(expected_product, result);
    }

    // Тест на вектор с одним элементом
    TEST(ProcessVector_SingleElement) {
        std::vector<double> vector = {5.0};
        double expected_product = 5.0;

        double result = Calculator::processVector(vector);
        CHECK_EQUAL(expected_product, result);
    }

    // Тест на вектор с нулевым элементом
    TEST(ProcessVector_ZeroElement) {
        std::vector<double> vector = {2.0, 0.0, 4.0};
        double expected_product = 0.0;

        double result = Calculator::processVector(vector);
        CHECK_EQUAL(expected_product, result);
    }

    // Тест на большой ввод
    TEST(ProcessVector_LargeInput) {
        std::vector<double> vector(1000000, 1.0);
        double expected_product = 1.0; // Произведение всех единиц равно 1

        double result = Calculator::processVector(vector);
        CHECK_EQUAL(expected_product, result);
    }
}

int main() {
    return UnitTest::RunAllTests();
}
