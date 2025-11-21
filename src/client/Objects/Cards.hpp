#include <fstream>
#include <iostream>
#include <string>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Cards {
  private:
    public:
  Cards(std::string cardsFilePath);
  ~Cards();
};
