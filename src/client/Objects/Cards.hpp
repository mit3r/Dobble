#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "nlohmann/json.hpp"

using json = nlohmann::json;

class Cards {
  private:
      std::unordered_map<unsigned short, std::string> sourcesMap;

  public:
      Cards(std::string cardsFilePath);
      std::string getPath(unsigned short id);

      ~Cards();
};
