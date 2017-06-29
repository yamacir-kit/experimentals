#ifndef INCLUDED_MEEVAX_VSTREAM_MAGIC_FORMULA_HPP_
#define INCLUDED_MEEVAX_VSTREAM_MAGIC_FORMULA_HPP_


#include <string>
#include <utility>
#include <vector>


namespace meevax {


template <typename C,
          template <typename...> class SequenceContainer = std::vector,
          template <typename...> class String = std::basic_string>
class magic_formula
  : public SequenceContainer<String<C>>
{
public:
  template <typename... Ts>
  explicit magic_formula(Ts&&... args)
    : SequenceContainer<String<C>> {std::forward<Ts>(args)...}
  {
#ifndef NDEBUG
    std::cout << "[debug] meevax::magic_formula::magic_formula() - ";
    for (const auto& v : *this) { std::cout << v << (&v != &(*this).back() ? ' ' : '\n'); }
#endif
  }
};


} // namespace meevax


#endif
