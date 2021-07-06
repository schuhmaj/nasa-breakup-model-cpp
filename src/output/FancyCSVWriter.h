#pragma once

#include "OutputWriter.h"
#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <utility>
#include <memory>
#include <map>
#include <functional>
#include "model/Satellite.h"
#include "util/UtilityContainer.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/fmt/ostr.h"

class FancyCSVWriter : public OutputWriter {

    const static std::map<char, std::function<void(const Satellite &sat, std::stringstream &stream)>> functionMap;

    std::vector<std::function<void(const Satellite &sat, std::stringstream &stream)>> _myToDo;

    std::shared_ptr<spdlog::logger> _logger;

public:

    explicit FancyCSVWriter(const std::string &filename, const std::string &pattern)
            : _logger{spdlog::basic_logger_mt<spdlog::async_factory>("CSVWriter_" + filename, filename, true)},
              _myToDo{} {
        _logger->set_pattern("%v");
        for (char c : pattern) {
            _myToDo.push_back(functionMap.at(c));
        }
    }

    void printResult(const std::vector<Satellite> &satelliteCollection) override;
};
