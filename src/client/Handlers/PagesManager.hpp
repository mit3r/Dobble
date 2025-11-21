#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

#ifndef PAGES_MANAGER_HPP
#define PAGES_MANAGER_HPP

class Page {
  private:
  public:
  virtual ~Page() = default;

  /** Method to initialize the page */
  virtual void start(sf::RenderWindow& window) = 0;

  /** Method to clean up the page */
  virtual void end(sf::RenderWindow& window) = 0;

  /** Method to handle events */
  virtual void handleEvent(sf::RenderWindow& window, const sf::Event& event) = 0;

  /** Method to render the page */
  virtual void render(sf::RenderWindow& window) = 0;
};

class PagesManager {
  private:
  std::unordered_map<std::string, std::unique_ptr<Page>> pages;
  Page* currentPage;

  public:
  template <typename T, typename... Args>
  void add(const std::string& name, Args&&... args) {
    static_assert(std::is_base_of_v<Page, T>, "T must derive from Page");
    pages[name] = std::make_unique<T>(std::forward<Args>(args)...);

    if (!currentPage)
      currentPage = pages[name].get();
  }

  void changeTo(const std::string& name, sf::RenderWindow& window);

  void handleEvent(sf::RenderWindow& window, const sf::Event& event);
  void render(sf::RenderWindow& window);
};

#endif  // PAGES_MANAGER_HPP