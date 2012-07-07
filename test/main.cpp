
#include "algorithm/for_each.hpp"
#include "segmented_vector.hpp"

#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <iostream>

using namespace segmented;
using namespace boost::assign;

struct print_float { void operator()(float f) { std::cout << f << ' '; } };
int main()
{
  typedef segmented_vector<float> sv_int;
  sv_int::segment_list segs(5);
  segs[1] += 1.f,2.f,3.f;
  segs[3] += 4.f,5.f,6.f;
  sv_int sv(segs);

  std::for_each(sv.begin(), sv.end(), print_float());
  std::cout << std::endl;
  segmented::for_each(sv.begin(), sv.end(), print_float());
  std::cout << std::endl;

  return 0;
}

