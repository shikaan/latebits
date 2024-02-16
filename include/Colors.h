#pragma once

#include <SFML/Graphics.hpp>

using namespace std;

namespace df {
enum Color {
  UNDEFINED_COLOR = -1,
  BLACK = 0,
  DARK_BLUE,
  DARK_PURPLE,
  DARK_GREEN,
  BROWN,
  DARK_GRAY,
  LIGHT_GRAY,
  WHITE,
  RED,
  ORANGE,
  YELLOW,
  GREEN,
  BLUE,
  INDIGO,
  PINK,
  PEACH,
};

const Color COLOR_DEFAULT = WHITE;

// Maps an hexadecimal character to a Color
auto fromHex(char c) -> Color;

// Converts a Color to a SFML Color
auto toSFColor(Color c) -> sf::Color;
}  // namespace df
