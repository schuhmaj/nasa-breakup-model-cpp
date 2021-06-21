#pragma once

#include "OutputWriter.h"
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include "model/Satellite.h"
#include "util/UtilityContainer.h"

class CSVWriter : public OutputWriter {

    const std::string _filename{"breakupResult.csv"};

public:

    using OutputWriter::OutputWriter;

    explicit CSVWriter(const std::string &filename)
        : _filename{filename} {}


    void printResult(const std::vector<Satellite> &satelliteCollection) override;

    void printResult(const Breakup &breakup) override;

private:

    static inline void csvLine(std::ostream &ostream, const Satellite &satellite);

    static inline void csvHeader(std::ostream &ostream);

};

