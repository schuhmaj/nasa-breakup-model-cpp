#pragma once

#include "Breakup.h"

/**
 * A Explosion Breakup of one satellite.
 */
class Explosion : public Breakup {

public:

    using Breakup::Breakup;

private:
    void calculateFragmentCount() override;

    void characteristicLengthDistribution() override;

    void assignParentProperties() override;

    void deltaVelocityDistribution() override;

};

