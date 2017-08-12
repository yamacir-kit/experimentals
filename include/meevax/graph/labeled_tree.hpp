#ifndef INCLUDED_MEEVAX_GRAPH_LABELED_TREE_HPP_
#define INCLUDED_MEEVAX_GRAPH_LABELED_TREE_HPP_


#include <memory>
#include <unordered_map>
#include <utility>


namespace meevax::graph {


// TODO
// 動くけどあまりに可読性が低いので命名の見直しや型の定義（特にコイツが酷い）を直すこと
// そもそもの原因は基底クラスの名前がクソ長いこと
// クラス外でノード、エッジのテンプレート型エイリアスをかけることも検討すること


template <typename Key, typename Mapped>
class labeled_tree
  : public Mapped,
    public std::unordered_map<Key, std::unique_ptr<meevax::graph::labeled_tree<Key,Mapped>>>
{
  using node_type = meevax::graph::labeled_tree<Key,Mapped>;
  using edge_type = std::unique_ptr<node_type>;

public:
  template <typename... Ts>
  explicit labeled_tree(Ts&&... args)
    : Mapped {std::forward<Ts>(args)...}
  {}

  auto& operator[](Key&& node_name)
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
