#pragma once

#include "Breakup.h"

class Explosion : public Breakup {

public:

    using Breakup::Breakup;

    void run() override;

};

