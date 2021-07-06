#include "FancyCSVWriter.h"

const std::map<char, std::function<void(const Satellite &sat, std::stringstream &stream)>> FancyCSVWriter::functionMap{
        {'A', [](const Satellite &sat, std::stringstream &stream) -> void{stream << sat.getArea();}},
        {'I', [](const Satellite &sat, std::stringstream &stream) -> void{stream << sat.getId();}},
        {'M', [](const Satellite &sat, std::stringstream &stream) -> void{stream << sat.getMass();}}
        //TODO, add mappings
};

void FancyCSVWriter::printResult(const std::vector<Satellite> &satelliteCollection) {
    for (const auto &sat : satelliteCollection) {
        std::stringstream stream{};
        for (const auto &function : _myToDo) {
            function(sat, stream);
            stream << ',';
        }
        _logger->info(stream.str());
    }
}
