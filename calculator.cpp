#include "server.h"

#include <vector>
#include <limits>
#include <cstdint>

double Calculator::processVector(const std::vector<double>& vectorValues) {
    double product = 1.0;

    for (double value : vectorValues) {
        product *= value;
    }

    return product;
}
