#pragma once

#include "Breakup.h"

class Explosion : public Breakup {

public:

    using Breakup::Breakup;

private:
    void generateFragments() override;

    void characteristicLengthDistribution() override;

    void deltaVelocityDistribution() override;

};

