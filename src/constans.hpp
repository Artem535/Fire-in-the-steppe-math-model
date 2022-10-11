#pragma once

namespace Constans {

struct Plot {
  static constexpr int countCell = 12;
  static constexpr int maxRangeValue = 2;
  static constexpr int minRangeValue = 0;
};

struct Table {
  static constexpr int countCell = 12;
  static constexpr int minWidthCell = 24;
  static constexpr int minHeightCell = 24;
  static constexpr int width = minWidthCell * countCell;
  static constexpr int height = minHeightCell * countCell;
};
} // namespace Constans
