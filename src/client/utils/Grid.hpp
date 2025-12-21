#include <SFML/System.hpp>

#pragma once

class Grid {
  private:
  unsigned int rows = 1;
  unsigned int columns = 1;
  sf::Vector2f layoutSize;
  sf::Vector2f cellSize = sf::Vector2f(0, 0);
  sf::Vector2f padding = sf::Vector2f(0, 0);
  sf::Vector2f gap = sf::Vector2f(0, 0);

  bool flowHorizontal = true;
  bool dirty = true;

  unsigned int cellsCount = 0;
  sf::Vector2f* cells;

  sf::Vector2f& getCell(const unsigned int index) {
    if (index >= cellsCount) throw std::out_of_range("Grid::getCell: Index out of range");
    if (flowHorizontal) return cells[index];

    unsigned int x = index / this->rows;
    unsigned int y = index % this->rows;
    return cells[y * this->columns + x];
  }

  sf::Vector2f& getCell(const unsigned int x, const unsigned int y) {
    if (x >= this->columns || y >= this->rows) throw std::out_of_range("Grid::getCell: Coordinates out of range");
    return cells[y * this->columns + x];
  }

  public:
  void calculateLayout() {
    if (cellsCount != 0) delete[] cells, cellsCount = 0;
    cellsCount = rows * columns;
    cells = new sf::Vector2f[cellsCount];

    if (cellSize.x <= 0.f) cellSize.x = (layoutSize.x - padding.x * 2.f - gap.x * (columns - 1)) / static_cast<float>(columns);
    if (cellSize.y <= 0.f) cellSize.y = (layoutSize.y - padding.y * 2.f - gap.y * (rows - 1)) / static_cast<float>(rows);

    sf::Vector2f totalGridSize = sf::Vector2f(cellSize.x * columns, cellSize.y * rows) + sf::Vector2f(gap.x * (columns - 1), gap.y * (rows - 1));

    sf::Vector2f calculatedSize = totalGridSize + padding * 2.f;
    if (calculatedSize.x > layoutSize.x || calculatedSize.y > layoutSize.y) throw std::logic_error("Grid::calculateLayout: Cell size with gaps and padding exceeds grid size");

    // Adjust padding to center the grid within the available grid size
    padding = (layoutSize - totalGridSize) * 0.5f;

    sf::Vector2f position;
    for (unsigned int y = 0; y < rows; y++) {
      position.y = padding.y + y * (cellSize.y + gap.y);

      for (unsigned int x = 0; x < columns; x++) {
        position.x = padding.x + x * (cellSize.x + gap.x);

        this->getCell(x, y) = position;
      }
    }

    dirty = false;
  }

  void setPadding(const sf::Vector2f padding) { this->padding = padding, dirty = true; }
  void setGap(const sf::Vector2f gap) { this->gap = gap, dirty = true; }
  void setRows(const unsigned int rows) { this->rows = rows, dirty = true; }
  void setColumns(const unsigned int columns) { this->columns = columns, dirty = true; }
  void setCellSize(const sf::Vector2f cellSize) { this->cellSize = cellSize, dirty = true; }
  void setLayoutSize(const sf::Vector2f layoutSize) { this->layoutSize = layoutSize, dirty = true; }
  void setHorizontalFlow() { this->flowHorizontal = true, dirty = true; }
  void setVerticalFlow() { this->flowHorizontal = false, dirty = true; }

  sf::Vector2f getPadding() const { return padding; }
  sf::Vector2f getGap() const { return gap; }
  sf::Vector2f getCellSize() const { return cellSize; }
  sf::Vector2f getGridSize() const { return layoutSize; }

  sf::Vector2f getCellPosition(const unsigned int row, const unsigned int column) {
    if (dirty) throw std::logic_error("Grid::getCell: Layout is dirty, call calculateLayout() before accessing cells");
    return getCell(row, column);
  }
  sf::Vector2f getCellPosition(const unsigned int index) {
    if (dirty) throw std::logic_error("Grid::getCell: Layout is dirty, call calculateLayout() before accessing cells");
    return getCell(index);
  }
};