#ifndef INCLUDED_MEEVAX_SYSTEM_IO_SYSTEM_HPP
#define INCLUDED_MEEVAX_SYSTEM_IO_SYSTEM_HPP


#include <iostream>
#include <string>
#include <type_traits>

#include <meevax/concepts/is_standard_container.hpp>
#include <meevax/utility/noncopyable.hpp>
#include <meevax/utility/unmovable.hpp>


namespace meevax::system {


template <typename CharType>
class code
{
};


template <typename CharType, template <typename...> typename StandardContainer,
          typename = typename std::enable_if<
                                meevax::concepts::is_standard_container<
                                  StandardContainer<CharType>
                                >::value
                              >::type>
class io_system
  : public StandardContainer<StandardContainer<meevax::system::code<CharType>>>
{
public: // member types
  using char_type = typename std::basic_string<CharType>::value_type;

public:
  explicit io_system()
  {}

  void  read(std::basic_istream<char_type>& istream);
  void write(std::basic_ostream<char_type>& ostream);
};


} // namespace meevax::system


#endif // #ifndef INCLUDED_MEEVAX_SYSTEM_IO_SYSTEM_HPP

