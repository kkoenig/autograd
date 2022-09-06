#include <fmt/format.h>

#include <fstream>

#include "autograd/autograd.h"

// usage: bazel run //:demo -- $(pwd)/graph.dot && dot $(pwd)/graph.dot
// -Tpng -o $(pwd)/images/graph.png"
int main(int argc, char** argv) {
  ag::Graph g;

  // forward pass
  auto& a = g.value(-2.1f);
  auto& b = g.value(2.2f);
  auto& c = g.value(1.1f);
  auto& d = a * b;
  auto& e = a * b;
  auto& f = c * d + e;

  // backward pass
  f.zero_grad();
  f.backward();

  std::ofstream os(argv[1]);
  if (os.good()) {
    fmt::print("Writing graph to {}\n", argv[1]);
    g.write_dot(os);
    os.close();
  }
  return 0;
}
