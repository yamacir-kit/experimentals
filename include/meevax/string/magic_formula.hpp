#ifndef INCLUDED_MEEVAX_VSTREAM_MAGIC_FORMULA_HPP_
#define INCLUDED_MEEVAX_VSTREAM_MAGIC_FORMULA_HPP_


#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include <boost/algorithm/string.hpp>

#ifndef NDEBUG
#include <meevax/string/runtime_typename.hpp>
#endif


namespace meevax {


template <typename C,
          template <typename...> class ExternalContainer = std::vector,
          template <typename...> class InternalContainer = std::basic_string>
class magic_formula
  : public ExternalContainer<InternalContainer<C>>
{
public:
  template <typename... Ts>
  explicit magic_formula(Ts&&... args)
    : ExternalContainer<InternalContainer<C>> {std::forward<Ts>(args)...}
  {
#ifndef NDEBUG
    std::cout << "[debug] meevax::magic_formula::magic_formula(Ts&&...) - \e[0;33m";

    for (const auto& v : *this)
    {
      std::cout << v << (&v != &(*this).back() ? "\e[0;38;5;059m_\e[0;33m" : "\e[0m\n");
    }

    std::cout << "        \e[0mtype detail: \e[0;32m"
              << meevax::runtime_typename<C>(*this) << "\e[0m\n";
#endif
  }

  magic_formula(const InternalContainer<C>& string)
  {
    boost::algorithm::split(*this, string, boost::is_any_of(" "));

#ifndef NDEBUG
    std::cout << "[debug] meevax::magic_formula::magic_formula(const InternalContainer<C>&) - \e[0;33m";

    for (const auto& v : *this)
    {
      std::cout << v << (&v != &(*this).back() ? "\e[0;38;5;059m_\e[0;33m" : "\e[0m\n");
    }

    std::cout << "        \e[0mtype detail: \e[0;32m"
              << meevax::runtime_typename<C>(*this) << "\e[0m\n";
#endif
  }
};


} // namespace meevax


#endif
