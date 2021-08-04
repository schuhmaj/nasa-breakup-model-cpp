#include "Breakup.h"

void Breakup::run() {
    //1. Step: Remove all old stuff which there possible is and makes the Breakup ready to go
    this->prepare();

    //2. Step: Generate the new Satellites
    this->generateFragments();

    //3. Step: Assign every new Satellite a value for L_c
    this->characteristicLengthDistribution();

    //4. Step: Calculate the A/M (area-to-mass-ratio), A (area) and M (mass) values for every Satellite
    this->areaToMassRatioDistribution();

    //5. Step: Assign parent and by doing that assign each fragment a base velocity
    this->assignParentProperties();

    //6. Step: Calculate the Ejection velocity for every Satellite
    this->deltaVelocityDistribution();

}

Breakup &Breakup::setSeed(unsigned long seed) {
    _randomNumberGenerator = std::mt19937{seed};
    return *this;
}

void Breakup::prepare() {
    _output.clear();
}

void
Breakup::createFragments(size_t fragmentCount, const std::array<double, 3> &position) {
    _output.resize(fragmentCount, Satellite(SatType::DEBRIS, position));
    std::for_each(_output.begin(), _output.end(),
                  [&](Satellite &sat) { sat.setId(++_currentMaxGivenID); });
}

void Breakup::characteristicLengthDistribution(double powerLawExponent) {
    using util::transformUniformToPowerLaw;

    std::uniform_real_distribution<> uniformRealDistribution{0.0, 1.0};
    std::for_each(_output.begin(),
                  _output.end(),
                  [&](Satellite &sat) {
                      const double y = uniformRealDistribution(_randomNumberGenerator);
                      const double L_c = transformUniformToPowerLaw(_minimalCharacteristicLength,
                                                                    _maximalCharacteristicLength,
                                                                    powerLawExponent,
                                                                    y);
                      sat.setCharacteristicLength(L_c);
                  });
}

void Breakup::areaToMassRatioDistribution() {
    auto satIt = _output.begin();
    for (; satIt != _output.end(); ++satIt) {
        const double lc = satIt->getCharacteristicLength();

        //Calculate the A/M value in [m^2/kg]
        const double areaToMassRatio = calculateAM(lc);

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
        satIt->setAreaToMassRatio(areaToMassRatio);
        satIt->setArea(area);
        satIt->setMass(mass);

        //Stop calculating further values if the produced mass already exceeds the input mass
        //and erases the elements which are no longer needed
        _outputMass += mass;
        if (_outputMass > _inputMass) {
//            spdlog::warn("The simulation reduced the number of fragments because the mass budget was exceeded. "
//                         "In other words: The random behaviour has produced heavier fragments");
            _outputMass -= mass;
            _output.erase(satIt, _output.end());
            break;
        }
    }
}

void Breakup::deltaVelocityDistribution(double factor, double offset) {
    std::for_each(_output.begin(),
                  _output.end(),
                  [&](Satellite &sat) {
                      //Calculates the velocity as an scalar based on Equation 11/ 12
                      const double chi = log10(sat.getAreaToMassRatio());
                      const double mu = factor * chi + offset;
                      static constexpr double sigma = 0.4;
                      std::normal_distribution normalDistribution{mu, sigma};
                      double velocity = std::pow(10, normalDistribution(_randomNumberGenerator));

                      //Transform the scalar velocity into a cartesian vector
                      auto velocityVector = calculateVelocityVector(velocity);
                      sat.addEjectionVelocity(velocityVector);
                  });
}

double Breakup::calculateAM(double characteristicLength) {
    using namespace util;
    const double logLc = std::log10(characteristicLength);

    double areaToMassRatio{0};

    if (characteristicLength > 0.11) {          //Case bigger than 11 cm
        std::normal_distribution n1{mu_1(_satType, logLc), sigma_1(_satType, logLc)};
        std::normal_distribution n2{mu_2(_satType, logLc), sigma_2(_satType, logLc)};

        areaToMassRatio = std::pow(10, alpha(_satType, logLc) * n1(_randomNumberGenerator) +
                                       (1 - alpha(_satType, logLc)) * n2(_randomNumberGenerator));
    } else if (characteristicLength < 0.08) {   //Case smaller than 8 cm
        std::normal_distribution n{mu_soc(logLc), sigma_soc(logLc)};

        areaToMassRatio = std::pow(10, n(_randomNumberGenerator));
    } else {                                    //Case between 8 cm and 11 cm
        std::normal_distribution n1{mu_1(_satType, logLc), sigma_1(_satType, logLc)};
        std::normal_distribution n2{mu_2(_satType, logLc), sigma_2(_satType, logLc)};
        std::normal_distribution n{mu_soc(logLc), sigma_soc(logLc)};

        double y1 = std::pow(10, alpha(_satType, logLc) * n1(_randomNumberGenerator) +
                                 (1 - alpha(_satType, logLc)) * n2(_randomNumberGenerator));
        double y0 = std::pow(10, n(_randomNumberGenerator));

        areaToMassRatio = y0 + (characteristicLength - 0.08) * (y1 - y0) / (0.03);
    }

    return areaToMassRatio;
}

std::array<double, 3> Breakup::calculateVelocityVector(double velocity) {
    std::uniform_real_distribution<> uniformRealDistribution{0.0, 1.0};

    double u = uniformRealDistribution(_randomNumberGenerator) * 2.0 - 1.0;
    double theta = uniformRealDistribution(_randomNumberGenerator) * 2.0 * util::PI;
    double v = std::sqrt(1.0 - u * u);

    return std::array<double, 3>
            {{v * std::cos(theta) * velocity, v * std::sin(theta) * velocity, u * velocity}};
}
