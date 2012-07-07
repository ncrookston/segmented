
#include "segmented_vector.hpp"
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <iostream>

namespace segmented
{
  namespace aux
  {
    template <typename T, typename F>
    F for_each(T it, T end, F f, boost::mpl::true_)
    {
      if(it == end)
        return f;

      typedef typename outer_iterator<T>::type out_it;
      out_it sfirst = outer_iterator<T>::get(it);
      out_it slast  = outer_iterator<T>::get(end);

      if(sfirst == slast)
      {
        f = for_each(
          inner_iterator<T>::get(it), inner_iterator<T>::get(end), f);
      }
      else
      {
        f = for_each(inner_iterator<T>::get(it), boost::end(*sfirst), f);
        while(++sfirst != slast)
        {
          f = for_each(boost::begin(*sfirst), boost::end(*sfirst), f);
        }
        f = for_each(boost::begin(*slast), inner_iterator<T>::get(end), f);
      }
      return f;
    }

    template <typename T, typename F>
    F for_each(T it, T end, F f, boost::mpl::false_)
    { return std::for_each(it, end, f); }
  }//end aux

  template <typename T, typename F>
  F for_each(T it, T end, F f)
  { return aux::for_each(it, end, f, typename is_segmented_iterator<T>::type()); }

}//end segmented

using namespace segmented;
using namespace boost::assign;

struct print_float { void operator()(float f) { std::cout << f << ' '; } };
int main()
{
  typedef segmented_vector<float> sv_int;
  sv_int::segment_list segs;/*(5);
  segs[1] += 1.f,2.f,3.f;
  segs[3] += 4.f,5.f,6.f;*/
  sv_int sv(segs);

  std::for_each(sv.begin(), sv.end(), print_float());
  std::cout << std::endl;
  segmented::for_each(sv.begin(), sv.end(), print_float());
  std::cout << std::endl;

  return 0;
}

