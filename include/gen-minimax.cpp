#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <deque>
#include <iostream>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <vector>

constexpr static inline int MAX_SCORE = 10;

enum PlayerState {
  PLAYER_X = 0,
  PLAYER_O = 1,
};

enum class GameWinPosition : uint8_t {
  NONE_ = 0,
  ROW_0 = 1,
  ROW_1 = 2,
  ROW_2 = 3,
  COL_0 = 4,
  COL_1 = 5,
  COL_2 = 6,
  DIA_1 = 7,
  DIA_2 = 8,
  DRAW_ = 9,
};

struct GameState {
  std::uint32_t crosses : 9;
  std::uint32_t noughts : 9;
  std::uint32_t player : 1;

  bool operator==(const GameState &other) const {
    return crosses == other.crosses && noughts == other.noughts && player == other.player;
  }
};

std::ostream &operator<<(std::ostream &out, const GameState &state) {
  return out << std::hex << "0b" << std::bitset<9>(state.crosses) << " 0b" << std::bitset<9>(state.noughts) << std::dec
             << " " << state.player;
}

static inline GameState makeState(uint16_t crosses, uint16_t noughts) {
  return GameState{
      static_cast<uint32_t>(crosses & 0b111111111),
      static_cast<uint32_t>(noughts & 0b111111111),
      PLAYER_X,
  };
}

static inline GameState makeState() {
  return makeState(0, 0);
}

static inline bool isCellEmpty(GameState state, uint8_t row, uint8_t col) {
  uint32_t mask = 1 << (row * 3 + col);
  return !((state.crosses & mask) || (state.noughts & mask));
}

static inline PlayerState whoHasCell(GameState state, uint8_t row, uint8_t col) {
  uint32_t mask = 1 << (row * 3 + col);
  if (state.crosses & mask) {
    return PLAYER_X;
  } else if (state.noughts & mask) {
    return PLAYER_O;
  }
  __builtin_unreachable();
}

static inline GameState setCellWithPlayer(GameState state, uint8_t row, uint8_t col, PlayerState player) {
  uint32_t mask = 1 << (row * 3 + col);
  if (player == PLAYER_X) {
    state.crosses |= mask;
  } else if (player == PLAYER_O) {
    state.noughts |= mask;
  }
  return state;
}

static inline GameState unsetCellWithPlayer(GameState state, uint8_t row, uint8_t col, PlayerState player) {
  uint32_t mask = 1 << (row * 3 + col);
  if (player == PLAYER_X && (state.crosses & mask)) {
    state.crosses ^= mask;
  } else if (player == PLAYER_O && (state.noughts & mask)) {
    state.noughts ^= mask;
  }
  return state;
}

static inline GameState trySetCell(GameState state, uint8_t row, uint8_t col) {
  uint32_t mask = 1 << (row * 3 + col);
  if (state.player == PLAYER_X && !(state.noughts & mask)) {
    state.player = PLAYER_O * !(state.crosses & mask);
    state.crosses |= mask;
  } else if (state.player == PLAYER_O && !(state.crosses & mask)) {
    state.player = PLAYER_X * !(state.noughts & mask);
    state.noughts |= mask;
  }
  return state;
}

#define ELEM(s, i, j) ((((s) & (1 << ((i)*3 + (j)))) >> ((i)*3 + (j))) & 0b1)
#define ROW(matrix, i)                                                                                                 \
  (ELEM(matrix, (i), 0) != 0 && ELEM(matrix, (i), 0) == ELEM(matrix, (i), 1) &&                                        \
      ELEM(matrix, (i), 1) == ELEM(matrix, (i), 2))
#define COL(matrix, i)                                                                                                 \
  (ELEM(matrix, 0, (i)) != 0 && ELEM(matrix, 0, (i)) == ELEM(matrix, 1, (i)) &&                                        \
      ELEM(matrix, 1, (i)) == ELEM(matrix, 2, (i)))

static inline GameWinPosition tryDetectWinOf(uint16_t matrix) {
  if (ROW(matrix, 0)) { return GameWinPosition::ROW_0; }
  if (ROW(matrix, 1)) { return GameWinPosition::ROW_1; }
  if (ROW(matrix, 2)) { return GameWinPosition::ROW_2; }
  if (COL(matrix, 0)) { return GameWinPosition::COL_0; }
  if (COL(matrix, 1)) { return GameWinPosition::COL_1; }
  if (COL(matrix, 2)) { return GameWinPosition::COL_2; }
  if (ELEM(matrix, 0, 0) != 0 && ELEM(matrix, 0, 0) == ELEM(matrix, 1, 1) && ELEM(matrix, 1, 1) == ELEM(matrix, 2, 2)) {
    return GameWinPosition::DIA_1;
  }
  if (ELEM(matrix, 0, 2) != 0 && ELEM(matrix, 0, 2) == ELEM(matrix, 1, 1) && ELEM(matrix, 1, 1) == ELEM(matrix, 2, 0)) {
    return GameWinPosition::DIA_2;
  }

  return GameWinPosition::NONE_;
}

static inline GameWinPosition tryDetectWin(GameState state) {
  GameWinPosition pos = tryDetectWinOf(state.crosses);
  if (pos != GameWinPosition::NONE_) {
    return pos;
  } else if ((pos = tryDetectWinOf(state.noughts)) != GameWinPosition::NONE_) {
    return pos;
  } else if ((state.crosses | state.noughts) == 0b111111111) {
    return GameWinPosition::DRAW_;
  }
  return GameWinPosition::NONE_;
}

// https://stackoverflow.com/questions/109023/count-the-number-of-set-bits-in-a-32-bit-integer
static inline uint8_t numberOfSetBits(uint32_t i) {
  // Java: use int, and use >>> instead of >>. Or use Integer.bitCount()
  // C or C++: use uint32_t
  i = i - ((i >> 1) & 0x55555555);                // add pairs of bits
  i = (i & 0x33333333) + ((i >> 2) & 0x33333333); // quads
  i = (i + (i >> 4)) & 0x0F0F0F0F;                // groups of 8
  return (i * 0x01010101) >> 24;                  // horizontal sum of bytes
}

static inline uint8_t setPlacesX(GameState state) {
  return numberOfSetBits(state.crosses);
}

static inline uint8_t setPlacesO(GameState state) {
  return numberOfSetBits(state.noughts);
}

static inline uint8_t setPlaces(GameState state) {
  return numberOfSetBits(state.crosses) + numberOfSetBits(state.noughts);
}

namespace std {
template <>
struct hash<GameState> {
  std::size_t operator()(const GameState &state) const {
    return std::hash<std::uint32_t>{}((state.crosses << 9) | state.noughts);
  }
};
} // namespace std

struct GameStateTreeNode {
  GameState state{};
  bool is_winning{};
  std::vector<GameStateTreeNode *> children{};
  GameStateTreeNode *parent{};

  GameStateTreeNode() noexcept = default;
  GameStateTreeNode(GameState state) : state{state} {}
  GameStateTreeNode(GameState state, std::vector<GameStateTreeNode *> children, GameStateTreeNode *parent)
      : state{state}, children{std::move(children)}, parent{parent} {}
  GameStateTreeNode(GameState state, std::size_t size_hint, GameStateTreeNode *parent) : state{state}, parent{parent} {
    children.reserve(size_hint);
  }

  void set_win(bool is_winning_) { is_winning = is_winning_; }
  void add_child(GameStateTreeNode *child) { children.emplace_back(child); }
};

struct GameStateTree {
  std::unordered_map<GameState, GameStateTreeNode *> state_map{};
  std::unordered_map<GameStateTreeNode *, std::unique_ptr<GameStateTreeNode>> nodes{};
  GameStateTreeNode *root{};

  GameStateTree() noexcept = default;

  GameStateTreeNode *push_node(GameState state, std::size_t size_hint, GameStateTreeNode *parent) {
    bool is_first = state_map.size() == 0;
    if (state_map.find(state) == state_map.end()) {
      std::unique_ptr<GameStateTreeNode> node = std::make_unique<GameStateTreeNode>(state, size_hint, parent);
      state_map.emplace(state, node.get());
      nodes.emplace(node.get(), std::move(node));
    }
    GameStateTreeNode *node = state_map.find(state)->second;
    if (is_first) { root = node; }
    return node;
  }

  GameStateTreeNode *push_node(GameState state, std::vector<GameStateTreeNode *> children, GameStateTreeNode *parent) {
    bool is_first = state_map.size() == 0;
    if (state_map.find(state) == state_map.end()) {
      std::unique_ptr<GameStateTreeNode> node = std::make_unique<GameStateTreeNode>(state, std::move(children), parent);
      state_map.emplace(state, node.get());
      nodes.emplace(node.get(), std::move(node));
    }
    GameStateTreeNode *node = state_map.find(state)->second;
    if (is_first) { root = node; }
    return node;
  }

  GameStateTreeNode *push_node(GameState state, GameStateTreeNode *parent) {
    auto *node = push_node(state, std::vector<GameStateTreeNode *>{}, parent);
    if (parent != nullptr) { parent->add_child(node); }
    return node;
  }

  GameStateTreeNode *push_node(GameState state, GameStateTreeNode *parent, bool is_winning) {
    auto *node = push_node(state, parent);
    node->set_win(is_winning);
    return node;
  }

  std::size_t numSolutions() { return nodes.size(); }

  std::size_t numWinningSolutions() {
    return std::count_if(state_map.begin(), state_map.end(), [](const auto &x) { return x.second->is_winning; });
  }
};

int getScore(GameState state, uint8_t depth, PlayerState curr, PlayerState player, PlayerState opponent) {
  GameWinPosition pos = tryDetectWin(state);
  if (pos == GameWinPosition::NONE_) {
    return 0;
  } else if (curr == player) {
    return depth - MAX_SCORE;
  } else if (curr == opponent) {
    return MAX_SCORE - depth;
  } else {
    __builtin_unreachable();
  }
}

int minimax(GameState state, GameStateTree &tree, GameStateTreeNode *parent, int depth, bool isMax, PlayerState player,
    PlayerState opponent) {
  int score = getScore(state, depth, isMax ? player : opponent, player, opponent);
  if (score > 0) {
    parent->add_child(tree.push_node(state, parent, true));
    return score;
  }
  if (score < 0) {
    parent->add_child(tree.push_node(state, parent, false));
    return score;
  }
  if (setPlaces(state) == 9) {
    parent->add_child(tree.push_node(state, parent, false));
    return 0;
  }

  if (isMax) {
    int best = -1000;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (isCellEmpty(state, i, j)) {
          state = setCellWithPlayer(state, i, j, player);
          auto *node = tree.push_node(state, parent, false);
          parent->add_child(node);
          best = std::max(best, minimax(state, tree, node, depth + 1, false, player, opponent));
          state = unsetCellWithPlayer(state, i, j, player);
        }
      }
    }
    return best;
  } else {
    int best = 1000;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (isCellEmpty(state, i, j)) {
          state = setCellWithPlayer(state, i, j, opponent);
          auto *node = tree.push_node(state, parent, false);
          parent->add_child(node);
          best = std::min(best, minimax(state, tree, node, depth + 1, true, player, opponent));
          state = unsetCellWithPlayer(state, i, j, opponent);
        }
      }
    }
    return best;
  }
}

std::pair<int, int> findBestMove(
    GameStateTree &tree, GameStateTreeNode *node, GameState state, PlayerState player, PlayerState opponent) {
  int bestVal = -1000;
  std::pair<int, int> bestMove{-1, -1};
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (isCellEmpty(state, i, j)) {
        state = setCellWithPlayer(state, i, j, player);
        int val = minimax(state, tree, node, 0, false, player, opponent);
        state = unsetCellWithPlayer(state, i, j, player);
        if (val > bestVal) {
          bestVal = val;
          bestMove.first = i;
          bestMove.second = j;
        } else if (val == bestVal) {
          if (tryDetectWin(state) != GameWinPosition::NONE_ && state.player == player) {
            bestVal = val;
            bestMove.first = i;
            bestMove.second = j;
          }
        }
      }
    }
  }
  std::cout << bestVal << '\n';

  return bestMove;
}

GameStateTree getAllSolutions() {
  GameStateTree tree{};
  GameStateTreeNode *root = tree.push_node(makeState(), nullptr);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      GameState state = trySetCell(makeState(), i, j);
      GameStateTreeNode *node = tree.push_node(state, root);
      findBestMove(tree, node, state, PLAYER_O, PLAYER_X);
    }
  }
  return tree;
}

void prettyPrint(GameState state) {
  std::cout << "\n-------------\n";
  for (int i = 0; i < 3; i++) {
    std::cout << "| ";
    for (int j = 0; j < 3; j++) {
      if (!isCellEmpty(state, i, j)) {
        std::cout << (whoHasCell(state, i, j) == PLAYER_X ? 'X' : 'O') << " | ";
      } else {
        std::cout << "- | ";
      }
    }
    std::cout << "\n-------------\n";
  }
}

int main() {
  GameStateTree tree{};
  GameStateTreeNode *root = tree.push_node(makeState(), nullptr);
  GameState state = makeState(0b001001000, 0b100110000);
  PlayerState player = PLAYER_O;
  PlayerState opponent = player == PLAYER_X ? PLAYER_O : PLAYER_X;
  auto best = findBestMove(tree, root, state, player, opponent);
  std::cout << best.first << ' ' << best.second << '\n';
  prettyPrint(setCellWithPlayer(state, best.first, best.second, player));
  // auto mytree = getAllSolutions();
  // std::cout << mytree.numSolutions() << '\n';
  // std::cout << mytree.numWinningSolutions() << '\n';
  // std::cout << mytree.root->children.size() << '\n';
  // GameState state = makeState();
  // for (int i = 0; i < 3; i++) {
  //     for (int j = 0; j < 3; j++) {
  //         std::cout << i << ' ' << j << '\n';
  //         std::cout << state << '\n';
  //         std::cout << setCellWithPlayer(state, i, j, PLAYER_X) << '\n';
  //         std::cout << unsetCellWithPlayer(setCellWithPlayer(state, i, j, PLAYER_X), i, j, PLAYER_X) << '\n';
  //         assert(state == unsetCellWithPlayer(setCellWithPlayer(state, i, j, PLAYER_X), i, j, PLAYER_X));
  //         assert(state == unsetCellWithPlayer(setCellWithPlayer(state, i, j, PLAYER_O), i, j, PLAYER_O));
  //     }
  // }
}
