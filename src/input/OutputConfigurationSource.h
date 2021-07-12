#pragma once

#include <vector>
#include <memory>
#include "output/OutputWriter.h"

/**
 * Pure virtual interface for the definition of OutputSources
 */
class OutputConfigurationSource {

public:

    /**
     * Returns a vector of pointers to OutputTargets defined by a given configuration source.
     * @return OutputTargets
     */
    virtual std::vector<std::shared_ptr<OutputWriter>> getOutputTargets () const = 0;

    /**
    * Returns a vector of pointers to InputTargets defined by a given configuration source.
    * @return InputTargets
    */
    virtual std::vector<std::shared_ptr<OutputWriter>> getInputTargets () const = 0;

};