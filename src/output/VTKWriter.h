#pragma once

#include "output/OutputWriter.h"

#include <string>
#include "model/Satellite.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/fmt/ostr.h"

class VTKWriter : public OutputWriter {

    std::shared_ptr<spdlog::logger> _logger;

public:

    //TODO Change default filename
    VTKWriter() : VTKWriter("todo.vtk") {}

    explicit VTKWriter(const std::string &filename)
            : _logger{spdlog::basic_logger_mt<spdlog::async_factory>("VTKWriter_" + filename, filename, true)} {
        _logger->set_pattern("%v");
    }

    void printResult(const std::vector<Satellite> &satelliteCollection) override;

};
