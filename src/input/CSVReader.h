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
 *
 * @related For further information about the Idea. Code is partly adapted from here
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

    /**
     * Parses one cell of the CSV line file stream to a type V by using its >> operator.
     * @tparam V - the value to be extracted, should have an >> operator overload
     * @param istream - the line stream
     * @param value - the value extracted (non-const)
     * @return true to not cause any issues in getTuple
     */
    template<typename V>
    bool parseCell(std::istream &istream, V &value) {
        std::string cell;
        std::getline(istream, cell, ',');
        std::stringstream cellStream {cell};
        cellStream >> value;
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
        std::initializer_list<bool>{parseCell(lineStream, std::get<I>(tuple)) ...};
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
            while (this->nextLine(fileStream, t)) {
                lines.push_back(t);
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
