#pragma once

#include "Breakup.h"

/**
 * A Explosion Breakup of one satellite.
 */
class Explosion : public Breakup {

public:

    using Breakup::Breakup;

private:
    void generateFragments() override;

    void characteristicLengthDistribution() override;

    void assignParent() override;

    void deltaVelocityDistribution() override;

};

