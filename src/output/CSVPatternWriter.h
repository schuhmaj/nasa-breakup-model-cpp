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
#include "spdlog/spdlog.h"

class CSVPatternWriter : public OutputWriter {

    const static std::map<char, std::function<void(const Satellite &sat, std::stringstream &stream)>> functionMap;

    const static std::map<char, std::string> headerMap;

    std::vector<std::function<void(const Satellite &sat, std::stringstream &stream)>> _myToDo;

    std::vector<std::string> _myHeader;

    std::shared_ptr<spdlog::logger> _logger;

public:

    /**
     * Creates a new CSVPatternWriter to a file.
     * @param filename - string
     * @param pattern - the pattern (have a look at the headerMap, which char has which meaning)
     */
    explicit CSVPatternWriter(const std::string &filename, const std::string &pattern)
            : _logger{spdlog::basic_logger_mt<spdlog::async_factory>("CSVPatternWriter_" + filename, filename, true)},
              _myToDo{} {
        _logger->set_pattern("%v");
        for (char c : pattern) {
            _myToDo.push_back(functionMap.at(c));
            _myHeader.push_back(headerMap.at(c));
        }
    }

    /**
    * De-Registers the logger of the CSVPatternWriter, to ensure that a similar CSVPatternWriter
     * can be constructed once again.
    */
    ~CSVPatternWriter() override {
        spdlog::drop(_logger->name());
    }

    void printResult(const std::vector<Satellite> &satelliteCollection) const override;

    /**
    * Function required for testing the logger.
    * Flushes immediately the content, to be called after printResult(...)
    * Kills the advantage of asynchronous logging!
    */
    void flush() {
        _logger->flush();
    }
};
