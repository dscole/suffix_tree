#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <algorithm>
#include <iostream>

using namespace std;

struct Node
{
  unsigned mIndex;
  vector<pair<char, std::unique_ptr<Node>>> mChildren;

  static int num_nodes;
  Node()
  {
    num_nodes++;
  }

  ~Node()
  {
    num_nodes--;
  }
};

/*static*/ int Node::num_nodes = 0;

std::unique_ptr<Node> GetSuffixTree(char const* s)
{
  if (!s) return std::unique_ptr<Node>();

  auto head = make_unique<Node>();
  Node* current = head.get();

  while (*s)
  {
    auto* suffix = s;
    auto current_suffix = current;
    while (*suffix)
    {
      auto it = lower_bound(current_suffix->mChildren.begin(), current_suffix->mChildren.end(), *suffix,
                            [](auto& lhs, auto& rhs)
                            {
                              return lhs.first < rhs.first;
                            });
      if (it->first == *suffix)
      {
        current_suffix = it->second.get();
      }
      else
      {
        auto new_it = current_suffix->mChildren.emplace(it, *suffix, make_unique<Node>());
        current_suffix = new_it->second.get();
      }
      suffix++;
    }
    s++;
  }
  return head;
}

void PrintSuffixTree(Node* node)
{

}

int main(int __attribute__((unused)) argc, char *argv[])
{
  auto suffix_tree = GetSuffixTree(argv[1]);
  cout << Node::num_nodes << " nodes were created" << endl;
  cout << "These are all the suffixes" << endl;

  PrintSuffixTree(suffix_tree.get());

  return 0;
}
