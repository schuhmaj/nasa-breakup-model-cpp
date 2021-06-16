#include "Breakup.h"

#include <iostream>

void Breakup::run() {
    //1. Step: Remove all old stuff which there possible is and makes the Breakup ready to go
    this->init();

    //2. Step: Generate the new Satellites
    this->generateFragments();

    //3. Step: Assign every new Satellite a value for L_c
    this->characteristicLengthDistribution();

    //4. Step: Calculate the A/M (area-to-mass-ratio), A (area) and M (mass) values for every Satellite
    this->areaToMassRatioDistribution();

    //5. Step: Calculate the Ejection velocity for every Satellite
    this->deltaVelocityDistribution();

}

void Breakup::init() {
    _output.clear();
}

void Breakup::characteristicLengthDistribution(double powerLawExponent) {
    using util::transformUniformToPowerLaw;
    //Init a random device, which then generates the seed for the mt19937-generator (pseudo-random) which
    //then feeds the uniform distribution
    std::random_device rd;
    std::mt19937 generator{rd()};
    std::uniform_real_distribution<> uniformRealDistribution{0.0, 1.0};

    std::for_each(_output.begin(),
                  _output.end(),
                  [&](Satellite &sat) {
                      double y = uniformRealDistribution(generator);
                      double L_c = transformUniformToPowerLaw(_minimalCharacteristicLength,
                                                              _maximalCharacteristicLength,
                                                              powerLawExponent,
                                                              y);
                      sat.setCharacteristicLength(L_c);
                  });
}

void Breakup::areaToMassRatioDistribution() {
    //Init a random device, which then generates the seed for the mt19937-generator (pseudo-random) which
    //then feeds the uniform distribution
    std::random_device rd;
    std::mt19937 generator{rd()};

    //TODO Refactor and make it smaller
    std::for_each(_output.begin(),
                  _output.end(),
                  [&](Satellite &sat) {
                    double lc = sat.getCharacteristicLength();

                    //Get the parameter alpha, mu, sigma
                    auto param = getParameterAM(lc);
                    std::normal_distribution n1{param[1], param[2]};
                    std::normal_distribution n2{param[3], param[4]};

                    //Calculate the A/M value in [m^2/kg]
                    double areaToMassRatio =
                            std::pow(param[0] * n1(generator) + (1-param[0]) * n2(generator), 10);

                    //Calculate the are A in [m^2]
                    double area = 0;
                    if (lc < 0.00167) {
                        area = 0.540424 * lc * lc;
                    } else {
                        area = 0.556945 * std::pow(lc, 2.0047077);
                    }

                    //Calculate the mass m in [kg]
                    double mass = area / areaToMassRatio;

                    //Finally set every value in the satellite
                    sat.setAreaToMassRatio(areaToMassRatio);
                    sat.setArea(area);
                    sat.setMass(mass);
                });

}

std::array<double, 5> Breakup::getParameterAM(double characteristicLength) {
    std::array<double, 5> res{};

    double logLc = std::log10(characteristicLength);

    //alpha
    if (logLc <= -1.95) {
        res[0] = 0;
    } else if(logLc >= 0.55) {
        res[0] = 1;
    } else{
        res[0] = 0.3 + 0.4 * (logLc + 1.2);
    }

    //mu_1
    if (logLc <= -1.1) {
        res[1] = -0.6;
    } else if(logLc >= 0.0) {
        res[1] = -0.95;
    } else {
        res[1] = -0.6 - 0.318 * (logLc + 1.1);
    }

    //sigma_1
    if (logLc <= -1.3) {
        res[2] = 0.1;
    } else if(logLc >= -0.3) {
        res[2] = 0.3;
    } else {
        res[2] = 0.1 + 0.2 * (logLc + 1.3);
    }

    //mu_2
    if (logLc <= -0.7) {
        res[3] = -1.2;
    } else if(logLc >= -0.1) {
        res[3] = -2.0;
    } else {
        res[3] = -1.2 - 1.333 * (logLc + 0.7);
    }

    //sigma_2
    if (logLc <= -0.5) {
        res[4] = 0.5;
    } else if(logLc >= -0.3) {
        res[4] = 0.3;
    } else {
        res[4] = 0.5 - (logLc + 0.5);
    }

    return res;
}
