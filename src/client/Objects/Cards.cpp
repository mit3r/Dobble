#include "Cards.hpp"

Cards::Cards(std::string cardsFilePath) {
  std::ifstream cardsFile(cardsFilePath);
  json cardsJson = json::parse(cardsFile);

  std::cout << "Loaded card:" << std::endl;
  for (auto& card : cardsJson.items()) {
    uint16_t id = static_cast<uint16_t>(std::stoi(card.key()));
    std::string src = card.value().get<std::string>();
    this->sourcesMap[id] = src;

    std::cout << std::setfill(' ') << std::setw(4);
    std::cout << id << ": " << src << std::endl;
  }
}

std::string Cards::getPath(unsigned short id) {
  return this->sourcesMap[id];
}

Cards::~Cards() {
}