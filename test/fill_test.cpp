
#include "algorithm/fill.hpp"
#include "scoped_timer.hpp"
#include "segmented_vector.hpp"

#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <cmath>
#include <iostream>

using namespace segmented;
using namespace boost::assign;

namespace
{

struct counter
{
  counter() : count(0.f) {}

  void operator()(float f)
  { count += f; }

  float count;
};

}//end namespace

int main()
{
  typedef segmented_vector<int> sv_int;
  sv_int::segment_list segs(3, std::vector<int>(3, 5));
  segs[1].clear();
  sv_int sv(segs);

  segmented::fill(sv.begin(), sv.end(), 3);
  sv_int::iterator sIt = sv.begin();
  BOOST_TEST(*sIt++ == 3);
  BOOST_TEST(*sIt++ == 3);
  BOOST_TEST(*sIt++ == 3);
  BOOST_TEST(*sIt++ == 3);
  BOOST_TEST(*sIt++ == 3);
  BOOST_TEST(*sIt++ == 3);
  BOOST_TEST(sIt == sv.end());


  {
    typedef segmented_vector<int> sv_int;
    sv_int::segment_list t_segs(10000, std::vector<int>(1000, 5));
    sv_int t_sv(t_segs);
    {
      scoped_timer st;
      std::fill(t_sv.begin(), t_sv.end(), 3);
    }
  }

  {

    typedef segmented_vector<int> sv_int;
    sv_int::segment_list t_segs(10000, std::vector<int>(1000, 5));
    sv_int t_sv(t_segs);

    {
      scoped_timer st;
      segmented::fill(t_sv.begin(), t_sv.end(), 3);
    }
  }

  return boost::report_errors();
}

