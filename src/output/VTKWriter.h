#pragma once

#include "output/OutputWriter.h"

#include <string>
#include <functional>
#include "model/Satellite.h"
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"

/**
 * Creates vtk file of the given input.
 * Can be visualized e.g. with ParaView.
 */
class VTKWriter : public OutputWriter {

    std::shared_ptr<spdlog::logger> _logger;

public:

    VTKWriter() : VTKWriter("breakupResult.vtu") {}

    explicit VTKWriter(const std::string &filename)
            : _logger{spdlog::basic_logger_mt<spdlog::async_factory>("VTKWriter_" + filename, filename, true)} {
        _logger->set_pattern("%v");
    }

    /**
    * De-Registers the logger of the VTKWriter, to ensure that a similar VTKWriter can be constructed once again.
    */
    ~VTKWriter() override{
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

private:

    /**
     * Prints a property of the points.
     * @tparam Property - the type of the property
     * @tparam Data - the class which contains this property
     * @param name - name of the property, e.g. mass
     * @param property - the property (normally a getter of an satellite)
     * @param dataCollection - the data
     */
    template<typename Property, typename Data>
    void printProperty(const std::string &name, const std::function<Property(const Data &data)> &property,
                       const std::vector<Data> &dataCollection) const {
        _logger->info(R"(        <DataArray Name="{}" NumberOfComponents="1" format="ascii" type="Float32">)", name);
        for (const auto &date : dataCollection) {
            _logger->info("          {}", property(date));
        }
        _logger->info(R"(        </DataArray>)");
    }

    /**
     * Prints a property of the points.
     * @tparam Property - the type of the property
     * @tparam Data - the class which contains this property
     * @param name - name of the property, e.g. mass
     * @param property - the property (normally a getter of an satellite), here return value cartesian vector
     * @param dataCollection - the data
     * @related I do not use the << overload because of ADL (https://en.cppreference.com/w/cpp/language/adl)
     */
    template<typename Property, typename Data>
    void printProperty(const std::string &name, const std::function<std::array<Property, 3>(const Data &data)> &property,
                       const std::vector<Data> &dataCollection) const {
        _logger->info(R"(        <DataArray Name="{}" NumberOfComponents="3" format="ascii" type="Float32">)", name);
        for (const auto &date : dataCollection) {
            const auto &array = property(date);
            _logger->info("          {} {} {}", array[0], array[1], array[2]);
        }
        _logger->info(R"(        </DataArray>)");
    }

    /**
     * Prints the Header of the VTK file.
     * @param size - the number of points
     */
    void printHeader(size_t size) const;

    /**
     * Prints the separator between point properties and point position in cell.
     */
    void printSeparator() const;

    /**
     * Prints the Footer of the VTK file.
     */
    void printFooter() const;


};
