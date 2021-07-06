#pragma once

#include "OutputWriter.h"
#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <utility>
#include <memory>
#include "model/Satellite.h"
#include "util/UtilityContainer.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/fmt/ostr.h"

class CSVWriter : public OutputWriter {

    std::shared_ptr<spdlog::logger> _logger;

    bool _withKepler;

public:

    using OutputWriter::OutputWriter;

    CSVWriter() : CSVWriter("breakupResult.csv") {}

    explicit CSVWriter(const std::string &filename) : CSVWriter(filename, false) {}

    CSVWriter(const std::string &filename, bool withKepler)
            : _logger{spdlog::basic_logger_mt<spdlog::async_factory>("CSVWriter_" + filename, filename, true)},
              _withKepler{withKepler} {
        _logger->set_pattern("%v");
    }

    void printResult(const std::vector<Satellite> &satelliteCollection) override;

    void printResult(const Breakup &breakup) override;

private:

    void printStandard(const std::vector<Satellite> &satelliteCollection) const;

    void printKepler(const std::vector<Satellite> &satelliteCollection) const;

};

