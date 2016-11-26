#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <algorithm>
#include <iostream>
#include <cstring>

using namespace std;

#if __cplusplus <= 201103L
namespace std_compat {
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
}

using namespace std_compat;
#endif

struct Node {
  using Edge = pair<char, std::unique_ptr<Node>>;

  unsigned mIndex;
  vector<Edge> mChildren;

  static int num_nodes;
  Node() { num_nodes++; }

  ~Node() { num_nodes--; }
};

/*static*/ int Node::num_nodes = 0;

std::unique_ptr<Node> GetSuffixTree(char const *s) {
  if (!s)
    return std::unique_ptr<Node>();

  auto head = make_unique<Node>();
  Node *current = head.get();

  while (*s) {
    auto *suffix = s;
    auto current_suffix = current;
    while (*suffix) {
      pair<char, std::unique_ptr<Node>> pcp{*suffix, nullptr};
      auto it = lower_bound(current_suffix->mChildren.begin(),
                            current_suffix->mChildren.end(), pcp,
                            [](Node::Edge const& lhs, Node::Edge const& rhs) {
                              return lhs.first < rhs.first;
                            });
      if (it != current_suffix->mChildren.end() and it->first == *suffix) {
        current_suffix = it->second.get();
      } else {
        auto new_it =
            current_suffix->mChildren.emplace(it, *suffix, make_unique<Node>());
        current_suffix = new_it->second.get();
      }
      suffix++;
    }
    current_suffix->mChildren.emplace_back('\0', nullptr);
    s++;
  }
  return head;
}

void PrintSuffixTree(Node *node, char *buffer, char *current) {
  for (auto &c_n : node->mChildren) {
    *current = c_n.first;
    if (c_n.first == '\0') cout << buffer << endl;
    else                   PrintSuffixTree(c_n.second.get(), buffer, current);
  }
}

int main(int __attribute__((unused)) argc, char *argv[]) {
  auto suffix_tree = GetSuffixTree(argv[1]);
  cout << Node::num_nodes << " nodes were created" << endl;
  cout << "These are all the suffixes" << endl;

  vector<char> buffer(strlen(argv[1]));
  PrintSuffixTree(suffix_tree.get(), buffer.data(), buffer.data());

  suffix_tree.release();
  cout << Node::num_nodes << " nodes still used" << endl;

  return 0;
}
