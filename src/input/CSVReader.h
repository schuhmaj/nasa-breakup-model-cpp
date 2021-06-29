#pragma once

#include <utility>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <exception>
#include <tuple>
#include "model/Satellite.h"

/**
 * Provides the functionality to read an CSV file into an container of tuples.
 * Generally the following types are implemented to be paresd from a CSV file:
 * double, float, int, long, size_t, char, std::string, SatType
 *
 * @note Add more types by overloading the function readCell
 * @attention Any type is implemented via static_cast which might lead to issues!
 * @related For further information, about the Idea. Code partly adapted from here
 * https://stackoverflow.com/questions/34314806/parsing-a-c-string-into-a-tuple [accessed 29.06.2021]
 */
template<typename ...T>
class CSVReader {

    const std::string _filename;

    bool _hasHeader;

public:

    CSVReader(std::string filename, bool hasHeader)
            : _filename(std::move(filename)),
              _hasHeader(hasHeader) {}

    virtual ~CSVReader() = default;


private:

    template<typename V>
    bool readCell(std::istream &istream, V &value) {
        std::string cell;
        std::getline(istream, cell, ',');
        value = static_cast<V>(cell);
        return true;
    }

    bool readCell(std::istream &istream, double &value) {
        std::string cell;
        std::getline(istream, cell, ',');
        value = cell.empty() ? 0 : std::stod(cell);
        return true;
    }

    bool readCell(std::istream &istream, float &value) {
        std::string cell;
        std::getline(istream, cell, ',');
        value = cell.empty() ? 0 : std::stof(cell);
        return true;
    }

    bool readCell(std::istream &istream, int &value) {
        std::string cell;
        std::getline(istream, cell, ',');
        value = cell.empty() ? 0 : std::stoi(cell);
        return true;
    }

    bool readCell(std::istream &istream, long &value) {
        std::string cell;
        std::getline(istream, cell, ',');
        value = cell.empty() ? 0 : std::stol(cell);
        return true;
    }

    bool readCell(std::istream &istream, size_t &value) {
        std::string cell;
        std::getline(istream, cell, ',');
        std::stringstream stream{cell};
        stream >> value;
        return true;
    }

    bool readCell(std::istream &istream, char &value) {
        std::string cell;
        std::getline(istream, cell, ',');
        value = cell[0];
        return true;
    }

    bool readCell(std::istream &istream, std::string &value) {
        std::string cell;
        std::getline(istream, cell, ',');
        value = cell;
        return true;
    }

    bool readCell(std::istream &istream, SatType &value) {
        std::string cell;
        std::getline(istream, cell, ',');
        value = Satellite::stringToSatType.at(cell);
        return true;
    }


    /**
     * Creates the tuple for a given lineStream.
     * @tparam Tuple - the tuple to be filled
     * @tparam I - index sequence (packed as long as parameter list T)
     * @param lineStream - a CSV line/ row
     * @param tuple the tuple to be filled
     */
    template<typename Tuple, typename std::size_t... I>
    void getTuple(std::istream &lineStream, Tuple &tuple, std::index_sequence<I...>) {
        std::initializer_list<bool>{readCell(lineStream, std::get<I>(tuple)) ...};
    }

    /**
     * Returns the next line of the CSV file as a tuple
     * @tparam Tuple - the tuple to be filled
     * @param fileStream - the CSV input stream
     * @param tuple - the tuple to be filled
     * @return success of parsing a new element
     */
    template<typename Tuple>
    bool nextLine(std::istream &fileStream, Tuple &tuple) {
        std::string line;
        std::getline(fileStream, line);
        if (!line.empty()) {
            std::stringstream lineStream{line};
            this->getTuple(lineStream, tuple, std::make_index_sequence<sizeof...(T)>{});
            return true;
        }
        return false;
    }

public:

    /**
     * Returns the lines of the CSV file in a vector. Each line is tokenized into an tuple with the corresponding types.
     * @return vector of tokenized lines
     */
    std::vector<std::tuple<T...>> getLines() {
        //Open fileStream stream
        std::ifstream fileStream{_filename};
        std::vector<std::tuple<T...>> lines{};
        std::tuple<T...> t;

        //Skip header if the fileStream has a header
        if (_hasHeader) {
            std::string line;
            std::getline(fileStream, line);
        }

        //Read row by row
        try {
            while (!fileStream.eof()) {
                if (this->nextLine(fileStream, t)) {
                    lines.push_back(t);
                }
            }
        } catch (std::exception &e) {
            throw std::invalid_argument{e.what()};
        }

        return lines;
    }

    /**
     * Returns the header of the CSV file as a vector of strings.
     * @return vector of strings
     * @throws an exception if the CSV file does not have an header
     */
    std::vector<std::string> getHeader() {
        if (_hasHeader) {
            std::vector<std::string> header{};
            std::ifstream fileStream{_filename};
            std::string line;
            std::getline(fileStream, line);
            std::stringstream lineStream{line};
            for (size_t i = 0; i < sizeof...(T); ++i){
                std::string cell;
                std::getline(lineStream, cell, ',');
                header.push_back(cell);
            }
            return header;
        } else {
            throw std::invalid_argument{"The CSV file has no header!"};
        }
    }
};
