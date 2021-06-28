#pragma once

#include <utility>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <exception>

/**
 * Provides the functionality to read an CSV file into an two dimensional container.
 */
class CSVReader {

    const std::string _filename;

    bool _hasHeader;

public:

    CSVReader(std::string filename, bool hasHeader)
            : _filename(std::move(filename)),
              _hasHeader(hasHeader) {}

    virtual ~CSVReader() = default;


    /**
     * Returns the lines of the CSV file in an two dimensional vector. The first vector encapsulates
     * the lines, whereas the second interior encapsulates the cells.
     * @return vector of tokenized lines
     */
    std::vector<std::vector<std::string>> getLines();

    /**
     * Returns the header of the CSV file
     * @return vector of header tokens
     * @throws throws an exception if the CSV file does not contain an header
     */
    std::vector<std::string> getHeader();

private:

    /**
     * Tokenizes one line separated by a comma ',' delimiter of a CSV file.
     * @param istream - the input stream
     * @return tokenized line as vector
     */
    static std::vector<std::string> nextLine(std::istream &istream);

};
