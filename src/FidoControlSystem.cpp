#include "../include/FidoControlSystem.h"


#include "../include/Backpropagation.h"
#include "../include/LSInterpolator.h"


using namespace rl;

FidoControlSystem::FidoControlSystem(int stateDimensions, int actionDimensions, Action minAction, Action maxAction) : WireFitQLearn(stateDimensions, actionDimensions, 1, 12, 4, minAction, maxAction, 11, new rl::LSInterpolator(), net::Backpropagation(), 0.95, 0.4)  { }