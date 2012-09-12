#ifndef SEGMENTED_ALGORITHM_TRANSFORM_HPP
#define SEGMENTED_ALGORITHM_TRANSFORM_HPP

#include "segmented_iterator_traits.hpp"
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <algorithm>

namespace segmented
{
  template <typename T, typename U, typename F> F transform(T, T, U, F);

  namespace extension
  {
    //This version only cares if T is segmented.  U's segmentation
    // is ignored, since we have no way of knowing how the segments of U
    // align with the segments of T.  If U and T's segments are aligned,
    // we can be more efficient -- that is captured by a different tag type.
    template <typename T, typename U, typename F>
    U transform(T it, T end, U out, F f, segmented_tag)
    {
      if(it == end)
        return out;

      typedef typename segment_iterator<T>::type out_it;
      out_it sfirst = segment_iterator<T>::get(it);
      out_it slast  = segment_iterator<T>::get(end);

      if(sfirst == slast)
      {
        return segmented::transform(
          local_iterator<T>::get(it), local_iterator<T>::get(end), out, f);
      }
      else
      {
        out = segmented::transform(
          local_iterator<T>::get(it), boost::end(*sfirst), out, f);
        while(++sfirst != slast)
        {
          out = segmented::transform(
              boost::begin(*sfirst), boost::end(*sfirst), out, f);
        }
        return segmented::transform(
              boost::begin(*slast), local_iterator<T>::get(end), out, f);
      }
    }

    template <typename T, typename U, typename F>
    U transform(T it, T end, U out, F f, not_segmented_tag)
    { return std::transform(it, end, out, f); }

  }//end extension

  template <typename T, typename U, typename F>
  U transform(T it, T end, U out, F f)
  {
    return extension::transform(it, end, out, f,
        typename segmented_iterator_tag<T>::type());
  }

}//end segmented

#endif
#endif
