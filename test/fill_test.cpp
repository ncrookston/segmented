
#include "algorithm/fill.hpp"
#include "segmented_vector.hpp"

#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <cmath>
#include <iostream>

using namespace segmented;
using namespace boost::assign;

struct counter
{
  counter() : count(0.f) {}

  void operator()(float f)
  { count += f; }

  float count;
};

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

  return boost::report_errors();
}

