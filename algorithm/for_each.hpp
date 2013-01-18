#ifndef SEGMENTED_ALGORITHM_FOR_EACH_HPP
#define SEGMENTED_ALGORITHM_FOR_EACH_HPP

#include "segmented_iterator_traits.hpp"
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/utility.hpp>
#include <algorithm>

namespace segmented
{
  template <typename T, typename F> F for_each(T it, T end, F f);

  namespace aux
  {
    template <typename F, typename R>
    struct ref_function
    {
      F& f;
      explicit ref_function(F& f) : f(f) {}

      void operator()(R r) const
      {
        f(r);
      }
    };//end ref_function_obj

    template <typename T, typename F>
    F for_each(T it, T end, F f, segmented_tag)
    {
      if(it == end)
        return f;

      typedef typename segment_iterator<T>::type out_it;
      out_it sfirst = segment_iterator<T>::get(it);
      out_it slast  = segment_iterator<T>::get(end);

      if(sfirst == slast)
      {
        return segmented::for_each(
            local_iterator<T>::get(it), local_iterator<T>::get(end), f);
      }
      else
      {
        segmented::for_each(local_iterator<T>::get(it), boost::end(*sfirst),
          ref_function<F, typename std::iterator_traits<T>::reference>(f));
        while(++sfirst != slast)
        {
          segmented::for_each(boost::begin(*sfirst), boost::end(*sfirst),
            ref_function<F, typename std::iterator_traits<T>::reference>(f));
        }
        segmented::for_each(boost::begin(*slast), local_iterator<T>::get(end),
          ref_function<F, typename std::iterator_traits<T>::reference>(f));
        return f;
      }
    }

    template <typename T, typename F>
    F for_each(T it, T end, F f, not_segmented_tag)
    { return std::for_each(it, end, f); }

  }//end aux

  template <typename T, typename F>
  F for_each(T it, T end, F f)
  {
    return aux::for_each(it, end, f,
        typename segmented_iterator_tag<T>::type());
  }

}//end segmented

#endif
