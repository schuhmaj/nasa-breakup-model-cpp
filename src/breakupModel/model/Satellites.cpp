#include "Satellites.h"

std::vector<std::tuple<std::shared_ptr<const std::string> &, double &, double &, double &, double &, std::array<double, 3> &, std::array<double, 3> &>>
Satellites::getAsTuple() {
    std::vector<std::tuple<std::shared_ptr<const std::string> &, double &, double &, double &, double &, std::array<double, 3> &, std::array<double, 3> &>> vector{};
    size_t size = _characteristicLength.size();
    vector.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        vector.emplace_back(_name[i] ,_characteristicLength[i], _areaToMassRatio[i], _mass[i], _area[i],
                            _ejectionVelocity[i], _velocity[i]);
    }
    return vector;
}

std::vector<std::tuple<const std::shared_ptr<const std::string> &, const double &, const double &,
        const double &, const double &, const std::array<double, 3> &, const std::array<double, 3> &>>
Satellites::getAsTuple() const {
    std::vector<std::tuple<const std::shared_ptr<const std::string> &, const double &, const double &,
    const double &, const double &, const std::array<double, 3> &, const std::array<double, 3> &>> vector{};
    size_t size = _characteristicLength.size();
    vector.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        vector.emplace_back(_name[i] ,_characteristicLength[i], _areaToMassRatio[i], _mass[i], _area[i],
                            _ejectionVelocity[i], _velocity[i]);
    }
    return vector;
}

std::vector<Satellite> Satellites::getAoS() const {
    std::vector<Satellite> vector{};
    size_t size = _characteristicLength.size();
    size_t id = _startID;
    vector.reserve(size);
    for (auto const &[namePtr, lc, am, m, a, ev, v] : this->getAsTuple()) {
        vector.emplace_back(id++, namePtr, _satType, lc, am, m, a, ev, v, _position);
    }
    return vector;
}

