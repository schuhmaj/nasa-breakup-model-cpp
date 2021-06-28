#include "TLEReader.h"


std::vector<std::pair<size_t, std::array<double, 6>>> TLEReader::getIDKepler() {
    std::vector<std::pair<size_t, std::array<double, 6>>> result{};

    std::ifstream fileStream{_filename};

    while (!fileStream.eof()) {
        result.push_back(getTwoLine(fileStream));
    }

    return result;
}

std::pair<size_t, std::array<double, 6>> TLEReader::getTwoLine(std::istream &istream) {
    size_t id;
    std::array<double, 6> keplerElements{};


    return std::make_pair(id, keplerElements);
}
