#ifndef INCLUDED_MEEVAX_GRAPH_DYNAMIC_TREE_HPP
#define INCLUDED_MEEVAX_GRAPH_DYNAMIC_TREE_HPP


#include <memory>
#include <type_traits>
#include <unordered_map>
#include <utility>


namespace meevax::graph {


// TODO
// 動くけどあまりに可読性が低いので命名の見直しや型の定義（特にコイツが酷い）を直すこと
// そもそもの原因は基底クラスの名前がクソ長いこと
// クラス外でノード、エッジのテンプレート型エイリアスをかけることも検討すること

// TODO
// Mapped型が要件を満たしていることをSFINAEで確認するよう修正すること
// - dynamic_tree 自体の作成時の引数はそのままルートノードへのMapped型のコンストラクタ引数となること
// - operator[] によって子ノードが作成される時、親ノードへの参照とノード名が渡されること

template <typename Key, typename Mapped>
class dynamic_tree
  : public Mapped,
    public std::unordered_map<Key, std::unique_ptr<meevax::graph::dynamic_tree<Key, Mapped>>>
{
  using node_type = meevax::graph::dynamic_tree<Key, Mapped>;
  using edge_type = std::unique_ptr<node_type>;

public:
  template <typename... Ts>
  explicit dynamic_tree(Ts&&... args)
    : Mapped {std::forward<Ts>(args)...}
  {}

  auto& operator[](Key&& node_name)
  {
    using unordered_map = std::unordered_map<Key, edge_type>;

    if (unordered_map::find(node_name) == unordered_map::end())
    {
      unordered_map::emplace(
        node_name, edge_type {create(*this, node_name)}
      );
    }

    return *unordered_map::at(node_name);
  }

private:
  template <typename... Ts,
            typename = typename std::enable_if<
                                  std::is_constructible<Mapped, Ts...>::value
                                >::type>
  auto create(Ts&&... args)
  {
    return new node_type {std::forward<Ts>(args)...};
  }

  auto create(Mapped&& parent_node, Key&& node_name)
  {
    return new node_type {std::forward<Mapped>(parent_node)};
  }
};


} // namespace meevax::graph


#endif
