#include "Fido/Fido.h"
#include "Fido/Simulator/Simlink.h"

int main() {
  rl::FidoControlSystem learner = rl::FidoControlSystem(1, 2, {-1, -1}, {1, 1}, 0.4);
  Simlink simulator;
  int goodIterations = 0;

  while(goodIterations < 5) {
    rl::Action action = learner.chooseBoltzmanAction({ simulator.isLeftOfLine() }, 0.2);

    double originalDistance = simulator.distanceFromLine();

    sf::Vector2f displacement =  sf::Vector2f(action[0], action[1]);
    sf::Vector2f newPosition = simulator.robot.getPosition() + displacement;
    simulator.robot.setPosition(newPosition);

    double newDistance = simulator.distanceFromLine();
    double rewardValue = (fabs(originalDistance) - fabs(newDistance - originalDistance)) / sqrt(2);
    learner.applyReinforcementToLastAction(rewardValue, { simulator.isLeftOfLine() });

    if(simulator.distanceFromLine() < 80) goodIterations++;
    else goodIterations = 0;
  }
}
