#pragma once

#include "OutputWriter.h"

class CSVWriter : public OutputWriter {

public:

    using OutputWriter::OutputWriter;

    void printResult() override;

};

