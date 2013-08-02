
#include "algorithm/transform.hpp"
#include "segmented_vector.hpp"
#include "scoped_timer.hpp"

#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <cmath>
#include <iostream>

using namespace segmented;
using namespace boost::assign;

int main()
{
  typedef segmented_vector<int> sv_int;
  sv_int::segment_list segs(3, std::vector<int>(3));
  segs[0][0] = 0;
  segs[0][1] = 1;
  segs[0][2] = 2;

  segs[1][0] = 3;
  segs[1][1] = 4;
  segs[1][2] = 5;

  segs[2][0] = 6;
  segs[2][1] = 7;
  segs[2][2] = 8;
  sv_int sv(segs);

  std::vector<int> s;
  segmented::transform(sv.begin(), sv.end(), 
      std::back_inserter(s), [](int i)
  { return i * 2; });

  std::for_each(s.begin(), s.end(), [](int i)
  { std::cout << i << std::endl; });

  sv_int::segment_list l_segs(5000, std::vector<int>(1000));
  sv_int large_segs(l_segs);
  std::vector<int> normal(5000 * 1000);
  std::vector<int> segmented(5000 * 1000);

  {
    scoped_timer st;
    std::transform(large_segs.begin(), large_segs.end(), normal.begin(),
      [](int i) { return i * 5; });
  }

  {
    scoped_timer st;
    segmented::transform(large_segs.begin(), large_segs.end(),
        segmented.begin(), [](int i) { return i * 5; });
  
  }

  BOOST_TEST(std::equal(normal.begin(), normal.end(), segmented.begin()));

  return boost::report_errors();
}


