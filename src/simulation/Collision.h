#pragma once

#include "Breakup.h"

class Collision : public Breakup {

public:

    using Breakup::Breakup;

private:
    void generateFragments() override;

    void sizeDistribution() override;

    void deltaVelocityDistribution() override;

};

