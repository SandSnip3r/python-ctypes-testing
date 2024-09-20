#include <iostream>
#include <map>
#include <queue>
#include <set>

struct Position {
  Position() = default;
  Position(int r, int c) : row(r), col(c) {}
  int row, col;
};

bool operator<(const Position &p1, const Position &p2) {
  if (p1.row == p2.row) {
    return p1.col < p2.col;
  }
  return p1.row < p2.row;
}

int posToLinear(int row, int col, int width) {
  return row * width + col;
}

extern "C" {

  float cppMult(int int_param, float float_param) {
    float return_value = int_param * float_param;
    std::cout << "    In cmult : int: " << int_param << " float " << float_param << " returning  " << return_value << std::endl;
    return return_value;
  }

  int32_t shortestPathLength(int32_t *start, int32_t *goal, int32_t *grid, int32_t height, int32_t width) {
    std::cout << "Asked to find the shortest path for a " << height << 'x' << width << " grid" << std::endl;
    std::cout << "Start " << start[0] << ',' << start[1] << ", and goal is " << goal[0] << ',' << goal[1] << std::endl;
    
    std::set<Position> visited;
    std::set<Position> pushed;
    std::map<Position, Position> previous;
    std::queue<Position> bfsQueue;
    bfsQueue.emplace(start[0], start[1]);
    while (!bfsQueue.empty()) {
      const Position current = bfsQueue.front();

      // Check if we found the goal
      if (current.row == goal[0] && current.col == goal[1]) {
        std::cout << "Found goal!" << std::endl;
        auto getPathLength = [&]() {
          Position tmpCurrent = current;
          int steps = 0;
          auto it = previous.find(tmpCurrent);
          while (it != previous.end()) {
            ++steps;
            tmpCurrent = it->second;
            it = previous.find(tmpCurrent);
          }
          return steps;
        };
        return getPathLength();
      }

      bfsQueue.pop();

      visited.emplace(current);

      auto pushNextPosition = [&](const Position &nextPosition) {
        if (visited.count(nextPosition) != 0) {
          // Already visited
          return;
        }
        if (pushed.count(nextPosition) != 0) {
          // Already pushed
          return;
        }
        previous[nextPosition] = current;
        bfsQueue.emplace(nextPosition);
        pushed.insert(nextPosition);
      };

      // Directions for moving: up, right, down, left (this is the order preferred by Pathery)
      if (current.row > 0) {
        // Try up
        if (grid[posToLinear(current.row-1, current.col, width)] == 0) {
          // Can move here
          pushNextPosition(Position(current.row-1, current.col));
        }
      }
      if (current.col < width-1) {
        // Try right
        if (grid[posToLinear(current.row, current.col+1, width)] == 0) {
          // Can move here
          pushNextPosition(Position(current.row, current.col+1));
        }
      }
      if (current.row < height-1) {
        // Try down
        if (grid[posToLinear(current.row+1, current.col, width)] == 0) {
          // Can move here
          pushNextPosition(Position(current.row+1, current.col));
        }
      }
      if (current.col > 0) {
        // Try right
        if (grid[posToLinear(current.row, current.col-1, width)] == 0) {
          // Can move here
          pushNextPosition(Position(current.row, current.col-1));
        }
      }
    }
    return 0;
  }

}