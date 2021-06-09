#pragma once

#include "Breakup.h"

class Explosion : public Breakup {

public:

    using Breakup::Breakup;

private:
    int fragmentCount() override;

    void sizeDistribution() override;

    void deltaVelocityDistribution() override;

};

