#ifndef INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_
#define INCLUDED_MEEVAX_VSTREAM_BASIC_VSTREAM_HPP_


#include <memory>
#include <unordered_map>
#include <utility>

#include <meevax/vstream/graphix_impl.hpp>


namespace meevax {


template <typename C> class visual_node;
template <typename C> using visual_edge = std::unique_ptr<meevax::visual_node<C>>;


template <typename C>
class visual_node
  : public meevax::graphix_impl,
    public std::unordered_map<std::basic_string<C>, meevax::visual_edge<C>>
{
public:
  template <typename... Ts>
  explicit visual_node(Ts&&... args)
    : meevax::graphix_impl {std::forward<Ts>(args)...}
  {}

  auto& operator[](std::basic_string<C>&& node_name)
  {
    if ((*this).find(node_name) == (*this).end())
    {
      meevax::visual_edge<C> edge {new meevax::visual_node<C> {*this}};
      (*this).emplace(node_name, std::move(edge));
    }

    return *(*this).at(node_name);
  }
};


} // namespace meevax


#endif
