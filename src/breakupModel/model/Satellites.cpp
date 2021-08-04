#include "Satellites.h"

std::vector<Satellite> Satellites::getAoS() const {
    std::vector<Satellite> vector{};
    size_t size = this->size();
    size_t id = _startID;
    vector.reserve(size);

    auto nameIt = _name.begin();
    auto lcIt = _characteristicLength.begin();
    auto amIt = _areaToMassRatio.begin();
    auto mIt = _mass.begin();
    auto aIt = _area.begin();
    auto vIt = _velocity.begin();
    auto evIt = _ejectionVelocity.begin();

    for (; lcIt != _characteristicLength.end(); ++nameIt, ++lcIt, ++amIt, ++mIt, ++aIt, ++vIt, ++evIt) {
        vector.emplace_back(id++, *nameIt, _satType, *lcIt, *amIt, *mIt, *aIt, *vIt, *evIt, _position);
    }
    return vector;
}

