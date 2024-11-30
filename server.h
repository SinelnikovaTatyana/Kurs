#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <cryptopp/cryptlib.h>
#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <csignal>
#include <cstdlib>
#include <getopt.h>
#include <cstdint>
#include <filesystem>
#include <limits>

class Error {
public:
    Error();
    static void errors(std::string error, std::string name);
    static int er(std::string fileName, std::string file_error);
};

class Server {
public:
    Server(Error& err) : errors(err) {}
    int self_addr(std::string& error, std::string& file_error, int port);
    int client_addr(int s, std::string& error, std::string& file_error);

private:
    Error& errors;
};

class Authorized {
public:
    Authorized(Error& err) : e_error(err) {}
    void msgsend(int work_sock, const std::string& mess);
    std::string SHA(const std::string& sah);
    std::string salt_generator(const std::size_t length);
    int authorized(int work_sock, std::string fileName, std::string file_error);

private:
    Error& e_error;
};

class Calculator {
public:
    static double processVector(const std::vector<double>& vectorValues);
};

class RuntimeError : public std::invalid_argument {
public:
    explicit RuntimeError(const std::string& what_arg) : std::invalid_argument(what_arg) {}
    explicit RuntimeError(const char* what_arg) : std::invalid_argument(what_arg) {}
};
class Interface {
public:
    Interface() : fileName("base.txt"), port(33333), file_error("error.txt"), description("") {};
    std::string getFileName() const;
    int getPort() const;
    std::string getErrorFile() const;
    std::string getDescription() const;
	bool parseArguments(int argc, char* argv[]);
private:
    std::string fileName;
    int port;
    std::string file_error;
    std::string description;

    
};
#endif // SERVER_H
