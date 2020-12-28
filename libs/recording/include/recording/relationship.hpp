#pragma once

enum tx_relationship { PARENT_CHILD = 0, PREDECESSOR_SUCCESSOR };

static constexpr std::array<std::string_view, 2> tx_relationship_literal = {{"PARENT/CHILD", "PRED/SUCC"}};
static inline constexpr const char* relationship_name(tx_relationship rel) {
  return (tx_relationship_literal[rel].data());
}

static constexpr inline std::string_view get_parent(char const* hier_name) {
  std::string_view ret(hier_name);
  auto pos = ret.rfind('.');
  if (pos == std::string::npos) {
    return ret;
  } else {
    return ret.substr(0, pos);
  }
}