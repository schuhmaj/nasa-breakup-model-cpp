#include "Breakup.h"

void Breakup::run() {
    //0. Step: Prepare constants, etc.
    this->init();

    //1. Step: Generate the new Satellites
    this->calculateFragmentCount();

    //2. Step: Assign every new Satellite a value for L_c
    this->characteristicLengthDistribution();

    //3. Step: Calculate the A/M (area-to-mass-ratio), A (area) and M (mass) values for every Satellite
    this->areaToMassRatioDistribution();

    this->enforceMassConservation();

    //4. Step: Assign parent and by doing that assign each fragment a base velocity
    this->assignParentProperties();

    //5. Step: Calculate the Ejection velocity for every Satellite
    this->deltaVelocityDistribution();

    //6. Step: As a last step set the _currentMaxGivenID to the new valid value
    _currentMaxGivenID += _output.size();
}

Breakup &Breakup::setSeed(std::optional<unsigned long> seed) {
    if (seed.has_value()) {
        _fixRNG = std::mt19937 {seed.value()};
    } else {
        _fixRNG = std::nullopt;
    }
    return *this;
}

void Breakup::generateFragments(size_t fragmentCount, const std::array<double, 3> &position) {
    //Just in case of a rerun - reset the outputMass to zero
    _outputMass = 0;
    _output = Satellites{_currentMaxGivenID+1, SatType::DEBRIS, position, fragmentCount};
}

void Breakup::characteristicLengthDistribution() {
    std::for_each(std::execution::par_unseq, _output._characteristicLength.begin(), _output._characteristicLength.end(),
                  [&](double &lc) {
        lc = calculateCharacteristicLength();
    });
}

void Breakup::areaToMassRatioDistribution() {
    auto tupleView = _output.getAreaMassTuple();
    std::for_each(std::execution::par_unseq, tupleView.begin(), tupleView.end(),
                  [&](auto &tuple) {
        //Order in the tuple: 0: L_c | 1: A/M | 2: Area | 3: Mass
        //Calculate the A/M value in [m^2/kg]
        std::get<1>(tuple) = calculateAreaMassRatio(std::get<0>(tuple));
        //Calculate the area A in [m^2]
        std::get<2>(tuple) = calculateArea(std::get<0>(tuple));
        //Calculate the mass m in [kg]
        std::get<3>(tuple) = calculateMass(std::get<2>(tuple), std::get<1>(tuple));
    });
}

void Breakup::enforceMassConservation() {
    //Enforce Mass Conservation if the output mass is greater than the input mass
    _outputMass = std::accumulate(_output._mass.begin(), _output._mass.end(), 0.0);
    size_t oldSize = _output.size();
    size_t newSize = _output.size();
    while (_outputMass > _inputMass) {
        _outputMass -= _output._mass.back();
        newSize -= 1;
        _output._mass.pop_back();
    }
    if (oldSize != newSize) {
        spdlog::warn("The simulation reduced the number of fragments because the mass budget was exceeded. "
                     "In other words: The random behaviour has produced heavier fragments");
        _output.resize(newSize);
    } else if (_enforceMassConservation) {
        //This is written in an else if, because if the former condition was true, we already had too many fragments
        //But we only need to check this here when no fragments had to be removed.
        while (_outputMass < _inputMass) {
            //Order in the tuple: 0: L_c | 1: A/M | 2: Area | 3: Mass

            //Create new element and assign values
            auto tuple = _output.appendElement();
            std::get<0>(tuple) = calculateCharacteristicLength();
            std::get<1>(tuple) = calculateAreaMassRatio(std::get<0>(tuple));
            std::get<2>(tuple) = calculateArea(std::get<0>(tuple));
            std::get<3>(tuple) = calculateMass(std::get<2>(tuple), std::get<1>(tuple));

            //Calculate new mass
            _outputMass += std::get<3>(tuple);
        }
        //Remove the element which has lead to the exceeding of the mass budget
        _outputMass -= _output._mass.back();
        _output.popBack();
        spdlog::warn("The simulation increased the number of fragments to enforce the mass conservation.");
    }
}

void Breakup::deltaVelocityDistribution() {
    using namespace util;
    auto tupleView = _output.getVelocityTuple();
    std::for_each(std::execution::par_unseq, tupleView.begin(), tupleView.end(),
                  [&](auto &tuple) {
        //Order in the tuple: 0: A/M | 1: Velocity | 2: Ejection Velocity
        //Calculates the velocity as a scalar based on Equation 11/ 12
        const double chi = log10(std::get<0>(tuple));
        const double mu = _deltaVelocityFactorOffset.first * chi + _deltaVelocityFactorOffset.second;
        static constexpr double sigma = 0.4;
        std::normal_distribution normalDistribution{mu, sigma};
        double velocity = std::pow(10, getRandomNumber(normalDistribution));

        //Transform the scalar velocity into a cartesian vector
        std::get<2>(tuple) = calculateVelocityVector(velocity);
        std::get<1>(tuple) = std::get<1>(tuple) + std::get<2>(tuple);
    });
}

double Breakup::calculateCharacteristicLength() {
    using util::transformUniformToPowerLaw;
    static std::uniform_real_distribution<> uniformRealDistribution{0.0, 1.0};
    const double y = getRandomNumber(uniformRealDistribution);
    return transformUniformToPowerLaw(_minimalCharacteristicLength, _maximalCharacteristicLength, _lcPowerLawExponent, y);
}

double Breakup::calculateAreaMassRatio(double characteristicLength) {
    using namespace util;
    const double logLc = std::log10(characteristicLength);

    double areaToMassRatio{0};

    if (characteristicLength > 0.11) {          //Case bigger than 11 cm
        std::normal_distribution n1{mu_1(_satType, logLc), sigma_1(_satType, logLc)};
        std::normal_distribution n2{mu_2(_satType, logLc), sigma_2(_satType, logLc)};

        areaToMassRatio = std::pow(10, alpha(_satType, logLc) * getRandomNumber(n1) +
        (1 - alpha(_satType, logLc)) * getRandomNumber(n2));
    } else if (characteristicLength < 0.08) {   //Case smaller than 8 cm
        std::normal_distribution n{mu_soc(logLc), sigma_soc(logLc)};

        areaToMassRatio = std::pow(10, getRandomNumber(n));
    } else {                                    //Case between 8 cm and 11 cm
        std::normal_distribution n1{mu_1(_satType, logLc), sigma_1(_satType, logLc)};
        std::normal_distribution n2{mu_2(_satType, logLc), sigma_2(_satType, logLc)};
        std::normal_distribution n{mu_soc(logLc), sigma_soc(logLc)};

        double y1 = std::pow(10, alpha(_satType, logLc) * getRandomNumber(n1) +
                                 (1 - alpha(_satType, logLc)) * getRandomNumber(n2));
        double y0 = std::pow(10, getRandomNumber(n));

        areaToMassRatio = y0 + (characteristicLength - 0.08) * (y1 - y0) / (0.03);
    }

    return areaToMassRatio;
}

double Breakup::calculateArea(double characteristicLength) {
    if (characteristicLength < 0.00167) {
        return 0.540424 * characteristicLength * characteristicLength;
    } else {
        return 0.556945 * std::pow(characteristicLength, 2.0047077);
    }
}

double Breakup::calculateMass(double area, double areaMassRatio) {
    return area / areaMassRatio;
}

std::array<double, 3> Breakup::calculateVelocityVector(double velocity) {
    std::uniform_real_distribution<> uniformRealDistribution{0.0, 1.0};

    double u = getRandomNumber(uniformRealDistribution) * 2.0 - 1.0;
    double theta = getRandomNumber(uniformRealDistribution) * 2.0 * util::PI;
    double v = std::sqrt(1.0 - u * u);

    return std::array<double, 3>
            {{v * std::cos(theta) * velocity, v * std::sin(theta) * velocity, u * velocity}};
}
