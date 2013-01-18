#ifndef SEGMENTED_ALGORITHM_TRANSFORM_HPP
#define SEGMENTED_ALGORITHM_TRANSFORM_HPP

#include "aux/transform1.hpp"
#if 0
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <algorithm>

namespace segmented
{
  namespace aux
  {
    //This version only cares if T is segmented.
    template <typename T, typename U, typename V, typename F,
              typename Tag, typename U_TAG, typename V_TAG>
    std::pair<U,V> transform(T it1, T end1, U it2, V out, F f,
        segmented_tag, U_TAG, V_TAG)
    {
      if(it1 == end1)
        return std::make_pair(it2, out);

      typedef typename segment_iterator<T>::type out_it;
      out_it sfirst1 = segment_iterator<T>::get(it1);
      out_it slast1  = segment_iterator<T>::get(end1);

      if(sfirst == slast)
      {
        return aux::transform(
          local_iterator<T>::get(it1), local_iterator<T>::get(end1),
          it2, out, f);
      }
      else
      {
        boost::tie(it2, out) = aux::transform(
          local_iterator<T>::get(it), boost::end(*sfirst), it2, out, f);
        while(++sfirst != slast)
        {
          boost::tie(it2, out) = aux::transform(
              boost::begin(*sfirst), boost::end(*sfirst), it2, out, f);
        }
        return aux::transform(
              boost::begin(*slast), local_iterator<T>::get(end), it2, out, f);
      }
    }
    //This version only cares if U is segmented, U's local iterators are
    // random access, and T is random access:
    template <typename T, typename U, typename V, typename F,
              typename Tag,
              typename T_SEGMENT_TAG,
              typename V_SEGMENT_TAG>
    std::pair<U,V> transform(T it1, T end1, U it2, V out, F f,
        T_TAG, segmented_tag, V_TAG)
    {
      while(it1 != end1)
      {
        local_iterator<U>::type lit2 = local_iterator<U>::get(it2);
        boost::tie(lit2, out) = aux::transform(
      }
      return std::make_pair(it2, out);

      typedef typename segment_iterator<T>::type out_it;
      out_it sfirst1 = segment_iterator<T>::get(it1);
      out_it slast1  = segment_iterator<T>::get(end1);

      if(sfirst == slast)
      {
        return aux::transform(
          local_iterator<T>::get(it1), local_iterator<T>::get(end1),
          it2, out, f);
      }
      else
      {
        boost::tie(it2, out) = aux::transform(
          local_iterator<T>::get(it), boost::end(*sfirst), it2, out, f);
        while(++sfirst != slast)
        {
          boost::tie(it2, out) = aux::transform(
              boost::begin(*sfirst), boost::end(*sfirst), it2, out, f);
        }
        return aux::transform(
              boost::begin(*slast), local_iterator<T>::get(end), it2, out, f);
      }
    }

    template <typename T, typename U, typename V, typename F,
      typename T_TRAVERSE_TAG, typename U_SEGMENT_TAG,
      typename U_TRAVERSE_TAG, typename V_SEGMENT_TAG,
      typename V_TRAVERSE_TAG>
    std::pair<U,V> transform(T it1, T end1, U it2, V out, F f,
        not_segmented_tag, T_TRAVERSE_TAG,
        U_SEGMENT_TAG,     U_TRAVERSE_TAG,
        V_SEGMENT_TAG,     V_TRAVERSE_TAG)
    {
      //NOTE: It would be nice to use the std version here, but when this is
      // called from a segmented version we need the incremented it2 and out.
      //return std::transform(it1, end1, it2, out, f);
      for(; it1 != end1; ++it1, ++it2, ++out)
        *out = f(*it1, *it2);
      return std::make_pair(it2, out);
    }

    template <typename T, typename U, typename V, typename F,
      typename T_TRAVERSE_TAG, typename U_SEGMENT_TAG,
      typename U_TRAVERSE_TAG, typename V_SEGMENT_TAG,
      typename V_TRAVERSE_TAG>
    std::pair<U,V> transform(T it1, T end1, U it2, V out, F f,
        not_segmented_tag, T_TRAVERSE_TAG,
        U_SEGMENT_TAG,     U_TRAVERSE_TAG,
        V_SEGMENT_TAG,     V_TRAVERSE_TAG)
    {
      //NOTE: It would be nice to use the std version here, but when this is
      // called from a segmented version we need the incremented it2 and out.
      //return std::transform(it1, end1, it2, out, f);
      for(; it1 != end1; ++it1, ++it2, ++out)
        *out = f(*it1, *it2);
      return std::make_pair(it2, out);
    }

    template <typename T, typename U, typename V, typename F>
    V transform(T it1, T end1, U it2, V out, F f)
    {
      typedef typename segmented_iterator_tag<T>::type t_tag;
      typedef typename segmented_iterator_tag<U>::type u_tag;
      typedef typename segmented_iterator_tag<V>::type v_tag;
      return transform(
          it1, end1, it2, out, f, t_tag(), u_tag(), v_tag()).second;
    }
  }//end aux

  template <typename T, typename U, typename V, typename F>
  V transform(T it1, T end1, U it2, V out, F f)
  { return aux::transform(it1, end1, it2, out, f); }

}//end segmented

#endif
#endif
