#pragma once

#include "Breakup.h"

class Collision : public Breakup {

    bool _isCatastrophic;

public:

    using Breakup::Breakup;

private:
    void generateFragments() override;

    void characteristicLengthDistribution() override;

    void deltaVelocityDistribution() override;

};

