#ifndef SEGMENTED_ALGORITHM_FILL_HPP
#define SEGMENTED_ALGORITHM_FILL_HPP

#include "segmented_iterator_traits.hpp"
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <algorithm>

namespace segmented
{
  template <typename T, typename V> void fill(T it, T end, const V& v);

  namespace aux
  {
    template <typename T, typename V>
    void fill(T it, T end, const V& v, segmented_tag)
    {
      typedef typename segment_iterator<T>::type out_it;
      out_it sfirst = segment_iterator<T>::get(it);
      out_it slast  = segment_iterator<T>::get(end);

      if(sfirst == slast)
      {
        if(it == end)
          return;
        segmented::fill(
          local_iterator<T>::get(it), local_iterator<T>::get(end), v);
      }
      else
      {
        segmented::fill(local_iterator<T>::get(it), boost::end(*sfirst), v);
        while(++sfirst != slast)
        {
          segmented::fill(boost::begin(*sfirst), boost::end(*sfirst), v);
        }
        segmented::fill(boost::begin(*slast), local_iterator<T>::get(end), v);
      }
    }

    template <typename T, typename V>
    void fill(T it, T end, const V& v, not_segmented_tag)
    { std::fill(it, end, v); }

  }//end aux

  template <typename T, typename V>
  void fill(T it, T end, const V& v)
  {
    aux::fill(it, end, v, typename segmented_iterator_traits<T>::type());
  }

}//end segmented

#endif

