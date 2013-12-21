#ifndef SEGMENTED_TEST_SCOPED_TIMER_HPP
#define SEGMENTED_TEST_SCOPED_TIMER_HPP

#include <boost/chrono.hpp>

namespace segmented
{
  class scoped_timer
  {
    boost::chrono::high_resolution_clock::time_point start_;

  public:
    scoped_timer() : start_(boost::chrono::high_resolution_clock::now()) {}

    ~scoped_timer()
    {
      auto stop = boost::chrono::high_resolution_clock::now();
      std::cout << boost::chrono::duration<double>(stop - start_) << std::endl;
    }
  };//end scoped_timer

}//end segmented

#endif
