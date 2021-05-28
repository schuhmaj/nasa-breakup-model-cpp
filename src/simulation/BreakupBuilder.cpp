#include "BreakupBuilder.h"

BreakupBuilder &BreakupBuilder::addBreakupType(BreakupType breakupType) {
    _breakupType = breakupType;
    return *this;
}

BreakupBuilder &BreakupBuilder::addInput(const std::shared_ptr<InputReader> &inputReader) {
    _inputReader = inputReader;
    return *this;
}

std::unique_ptr<Breakup> BreakupBuilder::getResult() {
    //TODO
    return nullptr;
}
