#include "VTKWriter.h"

void VTKWriter::printResult(const std::vector<Satellite> &satelliteCollection) {
    this->printHeader(satelliteCollection.size());

    this->printProperty<double>("mass", &Satellite::getMass, satelliteCollection);
    this->printProperty<double>("area", &Satellite::getArea, satelliteCollection);
    this->printProperty<double>("area-to-mass", &Satellite::getAreaToMassRatio, satelliteCollection);
    this->printProperty<double>("velocity", &Satellite::getVelocity, satelliteCollection);
    this->printProperty<double>("position", &Satellite::getPosition, satelliteCollection);

    this->printFooter();
}

void VTKWriter::printHeader(size_t size) {
    _logger->info(R"(<?xml version="1.0" encoding="UTF-8" standalone="no" ?>)");
    _logger->info(R"(<VTKFile byte_order="LittleEndian" type="UnstructuredGrid" version="0.1">)");
    _logger->info(R"(  <UnstructuredGrid>)");
    _logger->info(R"(    <Piece NumberOfCells="0" NumberOfPoints="{}">)", size);
    _logger->info(R"(      <PointData>)");
}

void VTKWriter::printFooter() {
    _logger->info(R"(      </Points>)");
    _logger->info(R"(      <Cells>)");
    _logger->info(R"(        <DataArray Name="types" NumberOfComponents="0" format="ascii" type="Float32"/>)");
    _logger->info(R"(      </Cells>)");
    _logger->info(R"(    </Piece>)");
    _logger->info(R"(  </UnstructuredGrid>)");
    _logger->info(R"(</VTKFile>)");
}

