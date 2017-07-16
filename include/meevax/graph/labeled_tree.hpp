#ifndef INCLUDED_MEEVAX_GRAPH_LABELED_TREE_HPP_
#define INCLUDED_MEEVAX_GRAPH_LABELED_TREE_HPP_


#include <memory>
#include <unordered_map>
#include <utility>


namespace meevax::graph {


template <typename S, typename T>
class labeled_tree
  : public T,
    public std::unordered_map<S, std::unique_ptr<meevax::graph::labeled_tree<S,T>>>
{
  using node_type = meevax::graph::labeled_tree<S,T>;
  using edge_type = std::unique_ptr<meevax::graph::labeled_tree<S,T>>;

public:
  template <typename... Ts>
  explicit labeled_tree(Ts&&... args)
    : T {std::forward<Ts>(args)...}
  {}

  auto& operator[](S&& node_name)
  {
    if ((*this).find(node_name) == (*this).end())
    {
      (*this).emplace(node_name, edge_type {new node_type {*this}});
    }

    return *(*this).at(node_name);
  }
};


} // namespace meevax::graph


#endif
