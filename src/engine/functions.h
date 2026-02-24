#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <random>
#include <chrono>

class Functions {
public:
    Functions();

    double computeST(double S, double r, double q, double sigma, double T, double Z);
    double computeCall(double K, double r, double T, double ST);

    double generateStandardNormal();
    
private:
    unsigned seed;
    std::mt19937_64 generator;
    std::normal_distribution<double> distribution;
};

#endif // FUNCTIONS_H
