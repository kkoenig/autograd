#include <fmt/format.h>

#include <fstream>

#include "include/autograd.h"

namespace ag {

Value& Graph::value(const float value) {
  return make_value(value, Operation::PARAM, nullptr, nullptr, [](Value&) {});
}

Value& Graph::make_value(const float value,
                         const Operation op,
                         const Value* lhs_parent,
                         const Value* rhs_parent,
                         std::function<void(Value&)> grad_fn) {
  const std::uint32_t next_id = values.size();
  if (lhs_parent && rhs_parent) {
    edges.emplace_back(Edge{lhs_parent->id, next_id});
    edges.emplace_back(Edge{rhs_parent->id, next_id});
  }
  values.emplace_back(*this, next_id, value, op, std::move(grad_fn));
  return values.back();
}

Value::Value(Graph& g,
             const std::uint32_t id,
             const float value,
             const Operation op,
             std::function<void(Value&)> grad_fn)
    : g(g), id(id), value(value), op(op), grad_fn(std::move(grad_fn)) {}

const Value& Value::operator+(const Value& rhs) const {
  auto grad_fn = [lhs = this, rhs = &rhs](Value& v) {
    lhs->grad += v.grad;
    rhs->grad += v.grad;
  };
  return g.make_value(value + rhs.value, Operation::SUM, this, &rhs, grad_fn);
}

const Value& Value::operator*(const Value& rhs) const {
  auto grad_fn = [lhs = this, rhs = &rhs](Value& v) {
    lhs->grad += v.grad * rhs->value;
    rhs->grad += v.grad * lhs->value;
  };
  return g.make_value(value * rhs.value, Operation::PRODUCT, this, &rhs,
                      grad_fn);
}

void Value::backward() const {
  grad = 1.f;
  g.visit_backward(id, [&](Value& v) { v.grad_fn(v); });
}

void Value::zero_grad() const {
  g.visit_backward(id, [](Value& v) { v.grad = 0.f; });
}

template <class F>
void Graph::visit_backward(const std::uint32_t root, F&& on_visit) {
  std::vector<std::vector<std::uint32_t>> b_adj(values.size());
  std::vector<std::vector<std::uint32_t>> f_adj(values.size());
  for (auto&& e : edges) {
    b_adj[e.to].emplace_back(e.from);
    f_adj[e.from].emplace_back(e.to);
  }

  std::vector<bool> visited(values.size());
  const auto helper = [&](auto&& recurse, const std::uint32_t id) -> void {
    if (visited[id]) {
      return;
    }
    visited[id] = true;

    for (auto&& forward_child : f_adj[id]) {
      recurse(recurse, forward_child);
    }

    on_visit(values[id]);

    for (auto&& backward_child : b_adj[id]) {
      recurse(recurse, backward_child);
    }
  };
  helper(helper, root);
}

namespace {
const char* to_string(const Operation op) {
  switch (op) {
    case Operation::SUM:
      return "+";
    case Operation::PRODUCT:
      return "*";
    case Operation::PARAM:
      return "?";
  }
}
}  // namespace

// https://graphviz.org/pdf/dotguide.pdf
void Graph::write_dot(std::ostream& os) {
  os << R"(
digraph g {
node [shape=Mrecord]
fontname="Helvetica,Arial,sans-serif"
node [fontname="Helvetica,Arial,sans-serif"]
rankdir = "LR"
edge [fontname="Helvetica,Arial,sans-serif"]
)";
  for (auto& v : values) {
    const auto label = fmt::format("{}|i{}|{{{{value|{}}}|{{grad|{}}}}}",
                                   to_string(v.op), v.id, v.value, v.grad);
    os << fmt::format("{} [style=bold, label=\"{}\"]", v.id, label)
       << std::endl;
  }
  os << std::endl;

  for (auto& e : edges) {
    os << fmt::format("{} -> {}", e.from, e.to) << std::endl;
  }
  os << "}" << std::endl;
}
}  // namespace ag
