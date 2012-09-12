
#include "algorithm/for_each.hpp"
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
  typedef segmented_vector<float> sv_float;
  sv_float::segment_list segs(5);
  segs[1] += 1.f,2.f,3.f;
  segs[3] += 4.f,5.f,6.f;
  sv_float sv(segs);

  counter c = segmented::for_each(sv.begin(), sv.end(), counter());
  BOOST_TEST(std::abs(c.count - 21.f) < 1e-6f);

  return boost::report_errors();
}

