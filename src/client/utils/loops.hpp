

#define RUN_EVERY_N_MS(ms, code)                     \
  {                                                \
    static sf::Clock clock;                        \
    static sf::Time accumulated = sf::Time::Zero; \
    accumulated += clock.restart();                 \
    if (accumulated.asMilliseconds() >= ms) {      \
      code;                                        \
      accumulated = sf::Time::Zero;                \
    }                                              \
  }