#include "CSVReader.h"


std::vector<std::vector<std::string>> CSVReader::getLines() {
    //Open fileStream stream
    std::ifstream fileStream{_filename};
    std::vector<std::vector<std::string>> lines{};

    //Skip header if the fileStream has a header
    if (_hasHeader) {
        nextLine(fileStream);
    }

    //Read row by row
    while (!fileStream.eof()) {
        lines.push_back(nextLine(fileStream));
    }

    return lines;
}

std::vector<std::string> CSVReader::getHeader() {
    if (_hasHeader) {
        std::ifstream file{_filename};
        return nextLine(file);
    } else {
        throw std::invalid_argument{"The CSV file has no header!"};
    }
}

std::vector<std::string> CSVReader::nextLine(std::istream &istream) {
    std::vector<std::string> row{};

    std::string line;
    std::getline(istream, line);
    std::stringstream lineStream{line};

    while (!lineStream.eof()) {
        std::string cell;
        std::getline(lineStream, cell, ',');
        row.push_back(cell);
    }

    return row;
}
