#ifndef INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_
#define INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_


#include <memory>
#include <unordered_map>
#include <utility>

#include <meevax/vstream/graphix_impl.hpp>


namespace meevax::graph {


// template <typename C> class visual_node;
// template <typename C> using visual_edge = std::unique_ptr<meevax::visual_node<C>>;


template <typename T, typename C = char>
class labeled_tree
  : public T,
    public std::unordered_map<std::basic_string<C>, std::unique_ptr<T>>
{
  using node_type = T;
  using edge_type = std::unique_ptr<T>;

public:
  template <typename... Ts>
  explicit labeled_tree(Ts&&... args)
    : node_type {std::forward<Ts>(args)...}
  {}

  auto& operator[](std::basic_string<C>&& node_name)
  {
    if ((*this).find(node_name) == (*this).end())
    {
      edge_type edge {new node_type {*this}};
      (*this).emplace(node_name, std::move(edge));
    }

    return *(*this).at(node_name);
  }
};


} // namespace meevax


#endif
