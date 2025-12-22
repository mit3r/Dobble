
#include "Components/EmptyButton.hpp"
#include "Components/Label.hpp"
#include "utils/Grid.hpp"

#pragma once

class LabelButton : public EmptyButton {
  public:
  Label label = Label("", *LayoutManager::fontRoboto, 16);
  Grid grid = Grid();

  LabelButton() {
    grid.calculateLayout();
  }

  void setPosition(const sf::Vector2f& position) {
    EmptyButton::setPosition(position);
    this->label.setPosition(position + this->grid.getCellPosition(0));
  }

  void setSize(const sf::Vector2f& size) {
    EmptyButton::setSize(size);
    this->grid.setLayoutSize(size);
    this->grid.setRows(1);
    this->grid.setColumns(1);
    this->grid.setCellSize(this->label.getLocalBounds().getSize());
    this->grid.calculateLayout();
    this->label.setPosition(this->getPosition() + this->grid.getCellPosition(0));
  }

  void draw(sf::RenderTarget& window, sf::RenderStates states) const {
    EmptyButton::draw(window, states);
    window.draw(this->label, states);
  }
};