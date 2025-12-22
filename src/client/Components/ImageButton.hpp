
#include "Components/EmptyButton.hpp"
#include "Components/HasEvents.hpp"

class ImageButton : public EmptyButton {
  private:
  sf::Texture texture;
  sf::Sprite sprite;

  public:
  ImageButton() {}

  bool loadImageFromFile(const std::string& filePath) {
    if (!this->texture.loadFromFile(filePath)) {
      return false;
    }
    this->sprite.setTexture(this->texture);
    return true;
  }

  void setPosition(const sf::Vector2f& position) {
    EmptyButton::setPosition(position);
    this->sprite.setPosition(position);
  }

  void setSize(const sf::Vector2f& size) {
    EmptyButton::setSize(size);
  }

  void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
    target.draw(this->sprite, states);
    EmptyButton::draw(target, states);
  }
};