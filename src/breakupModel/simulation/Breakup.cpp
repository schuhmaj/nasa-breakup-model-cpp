#include "Breakup.h"

void Breakup::run() {
    //1. Step: Generate the new Satellites
    this->calculateFragmentCount();

    //2. Step: Assign every new Satellite a value for L_c
    this->characteristicLengthDistribution();

    //3. Step: Calculate the A/M (area-to-mass-ratio), A (area) and M (mass) values for every Satellite
    this->areaToMassRatioDistribution();

    //4. Step: Assign parent and by doing that assign each fragment a base velocity
    this->assignParentProperties();

    //5. Step: Calculate the Ejection velocity for every Satellite
    this->deltaVelocityDistribution();

    //6. Step: As a last step set the _currentMaxGivenID to the new valid value
    _currentMaxGivenID += _output.size();
}

Breakup &Breakup::setSeed(unsigned long seed) {
    _randomNumberGenerator = std::mt19937{seed};
    return *this;
}

void
Breakup::generateFragments(size_t fragmentCount, const std::array<double, 3> &position) {
    //Just in case of a rerun - reset the outputMass to zero
    _outputMass = 0;
    _output = Satellites{_currentMaxGivenID+1, SatType::DEBRIS, position, fragmentCount};
}

void Breakup::characteristicLengthDistribution(double powerLawExponent) {
    using util::transformUniformToPowerLaw;
    std::uniform_real_distribution<> uniformRealDistribution{0.0, 1.0};
    for (auto &lc : _output._characteristicLength) {
        const double y = uniformRealDistribution(_randomNumberGenerator);
        lc = transformUniformToPowerLaw(_minimalCharacteristicLength, _maximalCharacteristicLength, powerLawExponent, y);
    }
}

void Breakup::areaToMassRatioDistribution() {
    auto lcIt = _output._characteristicLength.begin();
    auto amIt = _output._areaToMassRatio.begin();
    auto areaIt = _output._area.begin();
    auto massIt = _output._mass.begin();
    size_t size = 0;
    for (; amIt != _output._areaToMassRatio.end(); ++lcIt, ++amIt, ++areaIt, ++massIt, ++size) {
        const double lc = *lcIt;

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
        *amIt = areaToMassRatio;
        *areaIt = area;
        *massIt = mass;

        //Stop calculating further values if the produced mass already exceeds the input mass
        //and erases the elements which are no longer needed
        _outputMass += mass;
        if (_outputMass > _inputMass) {
//            spdlog::warn("The simulation reduced the number of fragments because the mass budget was exceeded. "
//                         "In other words: The random behaviour has produced heavier fragments");
            _outputMass -= mass;
            _output.resize(size);
            break;
        }
    }
}

void Breakup::deltaVelocityDistribution(double factor, double offset) {
    using namespace util;
    auto amIt = _output._areaToMassRatio.begin();
    auto evIt = _output._ejectionVelocity.begin();
    auto vIt = _output._velocity.begin();
    for (; amIt != _output._areaToMassRatio.end(); ++amIt, ++evIt, ++vIt) {
        //Calculates the velocity as a scalar based on Equation 11/ 12
        const double chi = log10(*amIt);
        const double mu = factor * chi + offset;
        static constexpr double sigma = 0.4;
        std::normal_distribution normalDistribution{mu, sigma};
        double velocity = std::pow(10, normalDistribution(_randomNumberGenerator));

        //Transform the scalar velocity into a cartesian vector
        *evIt = calculateVelocityVector(velocity);
        *vIt = *vIt + *evIt;
    }
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
