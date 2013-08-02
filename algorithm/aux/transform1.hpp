
#ifndef SEGMENTED_ALGORITHM_AUX_TRANSFORM1_HPP
#define SEGMENTED_ALGORITHM_AUX_TRANSFORM1_HPP

#include "segmented_iterator_traits.hpp"
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <algorithm>

namespace segmented
{
  namespace aux
  {
    template <typename T> struct is_random_access
      : boost::is_base_of<std::random_access_iterator_tag,
          typename std::iterator_traits<T>::iterator_category> {};

    template <typename T, typename U, typename F>
    U transform1(T it, T end, U out, F f);

    //This version only cares if T is segmented.  U's segmentation
    // is ignored, since we have no way of knowing how the segments of U
    // align with the segments of T.
    template <typename T, typename U, typename F>
    U transform1(T it, T end, U out, F f, segmented_tag, not_segmented_tag)
    {
      if(it == end)
        return out;

      typedef typename segment_iterator<T>::type t_segment_type;
      t_segment_type sfirst = segment_iterator<T>::get(it);
      t_segment_type slast  = segment_iterator<T>::get(end);

      if(sfirst == slast)
      {
        return aux::transform1(
          local_iterator<T>::get(it), local_iterator<T>::get(end), out, f);
      }
      else
      {
        out = aux::transform1(
          local_iterator<T>::get(it), boost::end(*sfirst), out, f);
        while(++sfirst != slast)
        {
          out = aux::transform1(
              boost::begin(*sfirst), boost::end(*sfirst), out, f);
        }
        return aux::transform1(
              boost::begin(*slast), local_iterator<T>::get(end), out, f);
      }
    }
    template <typename T, typename U, typename F>
    std::pair<T,U> transform1(T it, T end, U out, U out_end, F f,
        boost::mpl::true_)
    {
      typedef typename std::iterator_traits<T>::difference_type t_d_t;
      typedef typename std::iterator_traits<U>::difference_type u_d_t;
      t_d_t t_dist = end - it;
      u_d_t u_dist = out_end - out;
      t_dist = std::min(t_dist, u_dist);
      end = it + t_dist;
      out_end = out + t_dist;
      while(it != end)
      {
        *out = f(*it);
        ++it;
        ++out;
      }
      return std::make_pair(end, out_end);
    }
    template <typename T, typename U, typename F>
    std::pair<T,U> transform1(T it, T end, U out, U out_end, F f,
        boost::mpl::false_)
    {
      while(it != end && out != out_end)
      {
        *out = f(*it);
        ++it;
        ++out;
      }
      return std::make_pair(it, out);
    }
    template <typename T, typename U, typename F>
    std::pair<T,U> transform1(T it, T end, U out, U out_end, F f,
        not_segmented_tag)
    {
      return aux::transform1(it, end, out, out_end, f,
          boost::mpl::bool_<is_random_access<T>::value
                            && is_random_access<U>::value>());
    }
    template <typename T, typename U, typename F>
    std::pair<T,U> transform1(T it, T end, U out, U out_end, F f, segmented_tag)
    {
      if(out == out_end)
        return std::make_pair(it, out);

      typedef typename segment_iterator<U>::type u_segment_type;
      typedef typename local_iterator<U>::type u_local_type;
      typedef typename segmented_iterator_traits<u_local_type>::type
        local_segmentation;
      u_segment_type u_segment_it = segment_iterator<U>::get(out);
      u_segment_type u_segment_last = segment_iterator<U>::get(out_end);

      if(u_segment_it == u_segment_last)
      {
        return aux::transform1(it, end, local_iterator<U>::get(out),
            local_iterator<U>::get(out_end), f, local_segmentation());
      }
      else
      {
        u_local_type u_local_it = local_iterator<U>::get(out);
        u_local_type u_local_end = boost::end(*u_segment_it);
        boost::tie(it, u_local_it) = aux::transform1(it, end,
            u_local_it, u_local_end, f, local_segmentation());

        while(it != end && ++u_segment_it != u_segment_last)
        {
          boost::tie(it, u_local_it) = aux::transform1(it, end,
              boost::begin(*u_segment_it), boost::end(*u_segment_it),
              out, f, local_segmentation());
        }

        boost::tie(it, u_local_it) = aux::transform1(it, end,
            boost::begin(*u_segment_last), local_iterator<U>::get(out_end),
            f, local_segmentation());

        return std::make_pair(it, compose(u_segment_last, u_local_it));
      }
    }

    template <typename T, typename U, typename F>
    U transform1(T it, T end, U out, F f, not_segmented_tag, segmented_tag)
    {
      typedef typename segment_iterator<U>::type u_segment_type;
      typedef typename local_iterator<U>::type u_local_type;

      u_segment_type u_segment_it = segment_iterator<U>::get(out);
      u_local_type u_local_it = local_iterator<U>::get(out);
      u_local_type u_local_end = boost::end(u_segment_it);

      while(it != end)
      {
        while(u_local_it == u_local_end)
        {
          ++u_segment_it;
          u_local_it = boost::begin(u_segment_it);
          u_local_end = boost::end(u_segment_it);
        }
        boost::tie(it, u_local_it) =
          aux::transform1(it, end, u_local_it, u_local_end, f);
      }

      return compose(u_segment_it, u_local_it);
    }
    template <typename T, typename U, typename F>
    U transform1(T it, T end, U out, F f, not_segmented_tag, not_segmented_tag)
    { return std::transform(it, end, out, f); }

    template <typename T, typename U, typename F>
    U transform1(T it, T end, U out, F f)
    {
      typedef typename segmented_iterator_traits<T>::type t_seg_tag;
      typedef typename segmented_iterator_traits<U>::type u_seg_tag;

      return transform1(it, end, out, f, t_seg_tag(), u_seg_tag());
    }
  }//end aux

  template <typename T, typename U, typename F>
  U transform(T it, T end, U out, F f)
  { return aux::transform1(it, end, out, f); }

}//end segmented

#endif
