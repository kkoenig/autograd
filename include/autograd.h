#pragma once
#include <deque>
#include <vector>

namespace ag {

enum class Operation { PARAM, SUM, PRODUCT };

class Graph;
struct Value {
  Value(Graph&,
        const std::uint32_t id,
        const float value,
        const Operation op,
        std::function<void(Value&)> grad_fn);
  Value(const Value&) = delete;
  Value(Value&&) = delete;

  const Value& operator+(const Value& rhs) const;
  const Value& operator*(const Value& rhs) const;

  void zero_grad() const;
  void backward() const;

  Graph& g;
  const std::uint32_t id;
  const float value;
  const Operation op;
  const std::function<void(Value&)> grad_fn;
  mutable float grad = 0.f;
};

class Graph {
 public:
  Value& value(const float value);

  // visit backward nodes topologically
  template <class F>
  void visit_backward(const std::uint32_t root, F&& f);

  void write_dot(std::ostream&);

  Value& make_value(const float value,
                    const Operation op,
                    const Value* lhs_parent,
                    const Value* rhs_parent,
                    std::function<void(Value&)> grad_fn);

 private:
  std::deque<Value> values;

  struct Edge {
    std::uint32_t from;
    std::uint32_t to;
  };

  std::deque<Edge> edges;
};

}  // namespace ag
