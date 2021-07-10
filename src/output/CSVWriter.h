#pragma once

#include "OutputWriter.h"
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <utility>
#include "model/Satellite.h"
#include "util/UtilityContainer.h"

class CSVWriter : public OutputWriter {

    const std::string _filename{"breakupResult.csv"};

    bool _withKepler{false};

public:

    using OutputWriter::OutputWriter;

    explicit CSVWriter(std::string filename)
            : _filename{std::move(filename)} {}

    CSVWriter(std::string filename, bool withKepler)
            : _filename{std::move(filename)},
              _withKepler{withKepler} {}


    void printResult(const std::vector<Satellite> &satelliteCollection) override;

    void printResult(const Breakup &breakup) override;

private:

    void csvLine(std::ostream &ostream, const Satellite &satellite);

    void csvHeader(std::ostream &ostream) const;

};

