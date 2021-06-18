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
                      const double y = uniformRealDistribution(generator);
                      const double L_c = transformUniformToPowerLaw(_minimalCharacteristicLength,
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

    std::for_each(_output.begin(),
                  _output.end(),
                  [&](Satellite &sat) {
                    const double lc = sat.getCharacteristicLength();

                    //Calculate the A/M value in [m^2/kg]
                    const double areaToMassRatio = calculateAM(lc, generator);

                    //Calculate the are A in [m^2]
                    double area = 0;
                    if (lc < 0.00167) {
                        area = 0.540424 * lc * lc;
                    } else {
                        area = 0.556945 * std::pow(lc, 2.0047077);
                    }

                    //Calculate the mass m in [kg]
                    const double mass = area / areaToMassRatio;

                    //Finally set every value in the satellite
                    sat.setAreaToMassRatio(areaToMassRatio);
                    sat.setArea(area);
                    sat.setMass(mass);
                });

}

void Breakup::deltaVelocityDistribution(double factor, double offset) {
    std::random_device rd;
    std::mt19937 generator{rd()};

    std::for_each(_output.begin(),
                  _output.end(),
                  [&](Satellite &sat) {
                      const double chi = log10(sat.getAreaToMassRatio());
                      const double mu = factor * chi + offset;
                      static constexpr double sigma = 0.4;
                      std::normal_distribution normalDistribution{mu, sigma};
                      double velocity = std::pow(10, normalDistribution(generator));

                      //TODO Create cartesian vector from scalar ejection velocity (see Python)
                      //Currently, we just set the first component of the velocity vector
                      sat.setVelocity({velocity, 0 , 0});
                  });
}

double Breakup::calculateAM(double characteristicLength, std::mt19937 &generator) {
    using namespace util;
    const double logLc = std::log10(characteristicLength);

    double areaToMassRatio{0};

    if (characteristicLength > 0.11) {          //Case bigger than 11 cm
        std::normal_distribution n1{mu_1(_satType, logLc), sigma_1(_satType, logLc)};
        std::normal_distribution n2{mu_2(_satType, logLc), sigma_2(_satType, logLc)};
        
        areaToMassRatio = std::pow(10, alpha(_satType, logLc) * n1(generator) + (1-alpha(_satType, logLc)) * n2(generator));
    } else if (characteristicLength < 0.08) {   //Case smaller than 8 cm
        std::normal_distribution n{mu_soc(logLc), sigma_soc(logLc)};

        areaToMassRatio = std::pow(10, n(generator));
    } else {                                    //Case between 8 cm and 11 cm
        //Bridge Function required here
    }

    return areaToMassRatio;
}
