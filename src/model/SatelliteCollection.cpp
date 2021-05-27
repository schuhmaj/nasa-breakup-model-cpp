#include "SatelliteCollection.h"

void SatelliteCollection::push_back(const Satellite &satellite) {
    _satellites.push_back(satellite);
}

void SatelliteCollection::push_back(Satellite &&satellite) {
    _satellites.push_back(satellite);
}

void SatelliteCollection::remove(Satellite &satellite) {
    auto iter = _satellites.begin();
    while (*iter != satellite) {
        ++iter;
    }
    std::swap(*iter, _satellites.back());
    _satellites.pop_back();
}

long SatelliteCollection::size() {
    return _satellites.size();
}
