#pragma once

#include "output/OutputWriter.h"

#include <string>
#include <functional>
#include "model/Satellite.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/fmt/ostr.h"

/**
 * Creates vtk file of the given input.
 */
class VTKWriter : public OutputWriter {

    std::shared_ptr<spdlog::logger> _logger;

public:

    VTKWriter() : VTKWriter("breakupResult.vtu") {}

    explicit VTKWriter(const std::string &filename)
            : _logger{spdlog::basic_logger_mt<spdlog::async_factory>("VTKWriter_" + filename, filename, true)} {
        _logger->set_pattern("%v");
    }

    void printResult(const std::vector<Satellite> &satelliteCollection) override;

private:

    /**
     * Prints a property of the points.
     * @tparam T - the number type (double, float, int, etc.)
     * @param name - name of the property, e.g. mass
     * @param property - the property (normally a getter of an satellite)
     * @param satelliteCollection - the data
     */
    template<typename T>
    void printProperty(const std::string &name, const std::function<T(const Satellite &satellite)> &property,
                       const std::vector<Satellite> &satelliteCollection) {
        _logger->info(R"(        <DataArray Name="{}" NumberOfComponents="1" format="ascii" type="Float32">)", name);
        for (const auto &sat : satelliteCollection) {
            _logger->info("          {}", property(sat));
        }
        _logger->info(R"(        </DataArray>)");
    }

    /**
     * Prints a property of the points.
     * @tparam T - the number type (double, float, int, etc.)
     * @param name - name of the property, e.g. mass
     * @param property - the property (normally a getter of an satellite), here return value cartesian vector
     * @param satelliteCollection - the data
     * @related I do not use the >> overload because of ADL (https://en.cppreference.com/w/cpp/language/adl)
     */
    template<typename T>
    void printProperty(const std::string &name, const std::function<std::array<T, 3>(const Satellite &satellite)> &property,
                       const std::vector<Satellite> &satelliteCollection) {
        _logger->info(R"(<DataArray Name="{}" NumberOfComponents="3" format="ascii" type="Float32">)", name);
        for (const auto &sat : satelliteCollection) {
            const auto &array = property(sat);
            _logger->info("          {} {} {}", array[0], array[1], array[2]);
        }
        _logger->info(R"(</DataArray>)");
    }

    /**
     * Prints the Header of the VTK file.
     * @param size - the number of points
     */
    void printHeader(size_t size);

    /**
     * Prints the Footer of the VTK file.
     */
    void printFooter();


};
