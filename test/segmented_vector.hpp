#ifndef SEGMENTED_TEST_SEGMENTED_VECTOR_HPP
#define SEGMENTED_TEST_SEGMENTED_VECTOR_HPP

#include "segmented_iterator_traits.hpp"
#include <boost/assert.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/optional.hpp>
#include <boost/range/empty.hpp>
#include <boost/range/iterator.hpp>
#include <boost/range/reference.hpp>
#include <boost/range/value_type.hpp>
#include <vector>

namespace segmented
{
  namespace aux
  {
    template <typename segment_range>
    struct const_correct_typedefs
    {
      typedef typename boost::range_iterator<segment_range>::type
        segment_iterator;
      typedef typename boost::mpl::eval_if_c<
        boost::is_const<segment_range>::value,
        boost::add_const<typename boost::range_value<
          typename boost::remove_const<segment_range>::type >::type>,
        boost::range_value<segment_range>
      >::type local_range;

      typedef typename boost::range_iterator<local_range>::type local_iterator;
      typedef typename boost::iterator_reference<local_iterator>::type local_reference;
      typedef typename boost::remove_const<local_reference>::type local_value;
    };//end const_correct_typedefs
  }//end aux

  template <typename segment_range>
  class flattened_iterator
    : public boost::iterator_facade<
        flattened_iterator<segment_range>,
        typename aux::const_correct_typedefs<segment_range>::local_value,
        boost::forward_traversal_tag //TODO: More types.
      >
  {
    typedef aux::const_correct_typedefs<segment_range> typedefs;
  public:
    typedef typename typedefs::segment_iterator segment_iterator;
    typedef typename typedefs::local_range local_range;
    typedef typename typedefs::local_iterator local_iterator;
    typedef typename typedefs::local_reference local_reference;

    flattened_iterator() : opt_in_(), out_(), out_end_()
    {}
    flattened_iterator(segment_iterator out, segment_iterator out_end,
                       boost::optional<local_iterator> opt_in)
      : opt_in_(opt_in), out_(out), out_end_(out_end)
    {
      if(opt_in_)
        make_valid_in();
    }

    template <typename U>
    flattened_iterator(flattened_iterator<U> const& other)
      : opt_in_(other.opt_in_), out_(other.out_), out_end_(other.out_end_)
    {}

    segment_iterator get_segment_iterator() const
    { return out_; }

    local_iterator get_local_iterator() const
    { return *opt_in_; }

  private:
    friend class boost::iterator_core_access;
    template <typename> friend class flattened_iterator;

    template <typename U>
    bool equal(flattened_iterator<U> const& other) const
    { return other.out_ == out_ && other.opt_in_ == opt_in_; }

    void increment()
    {
      BOOST_ASSERT(opt_in_);
      ++*opt_in_;
      make_valid_in();
    }

    void make_valid_in()
    {
      BOOST_ASSERT(opt_in_);
      while(out_ != out_end_ && *opt_in_ == out_->end())
        *opt_in_ = (++out_)->begin();
    }

    local_reference dereference() const
    { BOOST_ASSERT(opt_in_ && *opt_in_ != out_->end()); return **opt_in_; }

    boost::optional<local_iterator> opt_in_;
    segment_iterator out_, out_end_;
  };//end flattened_iterator

  template <typename T>
  struct segmented_iterator_traits<flattened_iterator<T> >
  {
    typedef segmented_tag type;

    flattened_iterator<T> compose(
        typename flattened_iterator<T>::segment_iterator s_it,
        typename flattened_iterator<T>::segment_iterator s_end,
        typename flattened_iterator<T>::local_iterator l_it)
    { return flattened_iterator<T>(s_it, s_end, l_it); }
  };

  //Non-industrial-stregth segmented vector.
  template <typename T>
  class segmented_vector
  {
  public:
    typedef std::vector<T> segment;
    typedef std::vector<segment> segment_list;

    typedef T value_type;
    typedef typename segment::reference reference;

    typedef flattened_iterator<segment_list> iterator;
    typedef flattened_iterator<segment_list const> const_iterator;

    segmented_vector(const segment_list& nodes) : nodes_(nodes)
    {}

    iterator begin()
    {
      typename iterator::segment_iterator nodes_begin = nodes_.begin();
      return iterator(nodes_begin, end().get_segment_iterator(),
        boost::empty(nodes_) ? boost::optional<typename iterator::local_iterator>() : nodes_begin->begin());
    }

    const_iterator begin() const
    {
      typename iterator::segment_iterator nodes_begin = nodes_.begin();
      return const_iterator(nodes_begin, end().get_segment_iterator(),
        boost::empty(nodes_) ? boost::none : nodes_begin->begin());
    }
    
    //Per Austern's paper: "It must be possible to obtain
    // a valid segment iterator even from a past-the-end
    // segmented iterator."  This will not work on an empty range,
    // so unfortunately we must check here, and all algorithms
    // on segmented iterators must also do so.
    iterator end()
    {
      typename segment_list::iterator end = nodes_.end();
      if(!nodes_.empty())
      {
        --end;
        return iterator(end, end, end->end());
      }
      return iterator(end, end, boost::none);
    }

    const_iterator end() const
    {
      typename segment_list::const_iterator end = nodes_.end();
      if(!nodes_.empty())
      {
        --end;
        return const_iterator(end, end, end->end());
      }
      return const_iterator(end, end, boost::none);
    }

  private:
     segment_list nodes_;

  };//end segmented_vector
}//end segmented

#endif

