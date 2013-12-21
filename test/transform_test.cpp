
#include "algorithm/transform.hpp"
#include "segmented_vector.hpp"
#include "scoped_timer.hpp"

#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <cmath>
#include <iostream>

using namespace segmented;

namespace
{
  struct MultByFive
  {
    int operator()(int i)
    { return i * 5; }
  };//end MultByFive
}//end namespace

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
  segmented::transform(
      sv.begin(), sv.end(), std::back_inserter(s), MultByFive());

  //TODO: Check values of s.

  sv_int emptyList;
  segmented::transform(
      emptyList.begin(), emptyList.end(), s.begin(), MultByFive());
  static const int num_segs = 5000;
  static const int local_size = 5000;
  sv_int::segment_list l_segs(num_segs, std::vector<int>(local_size));
  sv_int large_segs(l_segs);
  std::generate(large_segs.begin(), large_segs.end(), &std::rand);
  std::vector<int> all;
  std::copy(large_segs.begin(), large_segs.end(), std::back_inserter(all));

  //Case 0:
  std::vector<int> stdcase0(num_segs * local_size);
  std::vector<int> segcase0(num_segs * local_size);
  std::cout << "One input segmented, output not-segmented time:" << std::endl;
  std::cout << "std: ";
  {
    scoped_timer st;
    std::transform(large_segs.begin(), large_segs.end(), stdcase0.begin(),
      MultByFive());
  }

  std::cout << "seg: ";
  {
    scoped_timer st;
    segmented::transform(large_segs.begin(), large_segs.end(),
        segcase0.begin(), MultByFive());
  
  }

  BOOST_TEST(std::equal(stdcase0.begin(), stdcase0.end(), segcase0.begin()));

  //Case 1:
  sv_int stdcase1(l_segs);
  sv_int segcase1(l_segs);
  std::cout << "One input not-segmented, output-segmented time:" << std::endl;
  std::cout << "std: ";
  {
    scoped_timer st;
    std::transform(all.begin(), all.end(), stdcase1.begin(), MultByFive());
  }

  std::cout << "seg: ";
  {
    scoped_timer st;
    segmented::transform(all.begin(), all.end(),
        segcase1.begin(), MultByFive());
  }


  std::cout << "One input segmented, output segmented time:" << std::endl;

  std::cout << "One input not segmented, output not segmented time:" << std::endl;
  BOOST_TEST(std::equal(stdcase1.begin(), stdcase1.end(), segcase1.begin()));

  return boost::report_errors();
}


