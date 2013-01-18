#ifndef SEGMENTED_SEGMENTED_ITERATOR_TRAITS_HPP
#define SEGMENTED_SEGMENTED_ITERATOR_TRAITS_HPP

#include <boost/mpl/and.hpp>
#include <boost/mpl/has_xxx.hpp>

namespace segmented
{
  template <typename iterator>
  struct local_iterator
  {
    typedef typename iterator::local_iterator type;

    static type get(iterator it)
    { return it.get_local_iterator(); }

  };//end local_iterator

  template <typename iterator>
  struct segment_iterator
  {
    typedef typename iterator::segment_iterator type;

    static type get(iterator it)
    { return it.get_segment_iterator(); }

  };//end segment_iterator

  struct segmented_tag {};
  struct not_segmented_tag {};

  template <typename iterator>
  struct segmented_iterator_traits
  {
    typedef not_segmented_tag type;
  };

}//end segmented

#endif

