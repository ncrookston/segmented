#ifndef SEGMENTED_SEGMENTED_ITERATOR_TRAITS_HPP
#define SEGMENTED_SEGMENTED_ITERATOR_TRAITS_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace segmented
{
  template <typename iterator>
  struct inner_iterator
  {
    typedef typename iterator::inner_iterator type;

    static type get(iterator it)
    { return it.get_inner(); }

  };//end inner_iterator

  template <typename iterator>
  struct outer_iterator
  {
    typedef typename iterator::outer_iterator type;

    static type get(iterator it)
    { return it.get_outer(); }

  };//end inner_iterator

  BOOST_MPL_HAS_XXX_TRAIT_DEF(inner_iterator)
  BOOST_MPL_HAS_XXX_TRAIT_DEF(outer_iterator)

  template <typename T>
  struct is_segmented_iterator
    : boost::mpl::and_<typename has_inner_iterator<T>::type, typename has_outer_iterator<T>::type> {};

}//end segmented

#endif

