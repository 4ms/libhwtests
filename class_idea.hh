#include <concepts>

enum class ButtonError {
  None,
  StuckDown,
  NoisyPress,
  NoisyRelease,
  MultipleHighs,
  Unknown,
};

template <typename T>
concept BoardC = requires(T t, unsigned chan) {
                   { t.read_button(chan) } -> std::same_as<bool>;
                   t.set_indicator(chan, bool{});
                   t.set_error_indicator(chan, ButtonError{});
                 };

template <BoardC Board> class ButtonTester {
  Board board;
  unsigned cur_chan = 0;

public:
  bool check() {
    board.read_button(cur_chan);
    cur_chan++;
    return true;
  }
};

////// in project:

struct SWNButtonIO {
  bool read_button(unsigned num) { return false; }
  void set_indicator(unsigned num, bool high) {}
  void set_error_indicator(unsigned num, ButtonError err) {}
};

void test_buttons() {
  ButtonTester<SWNButtonIO> checker;

  while (!checker.check()) {
  }
}
