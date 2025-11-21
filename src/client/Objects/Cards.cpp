#include "Cards.hpp"

Cards::Cards(std::string cardsFilePath) {
  std::ifstream cardsFile(cardsFilePath);
  json cardsJson = json::parse(cardsFile);

  for (auto& card : cardsJson.items()) {
    std::cout << card.key() << ": " << card.value() << std::endl;
  }
}

Cards::~Cards() {
}