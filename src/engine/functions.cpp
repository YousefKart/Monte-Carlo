#include "functions.h"
#include <cmath>

Functions::Functions() {
    seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator = std::mt19937_64(seed);
    distribution = std::normal_distribution<double>(0.0, 1.0);
}

double Functions::computeST(double S, double r, double q, double sigma, double T, double Z) {
    return S * std::exp(T * (r - q - sigma * sigma / 2) + sigma * std::sqrt(T) * Z);
}

double Functions::computeCall(double K, double r, double T, double ST) {
    return std::exp(-r * T) * std::max(ST - K, 0.0);
}

double Functions::generateStandardNormal() {
    return distribution(generator);
}
