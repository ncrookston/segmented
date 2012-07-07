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
    template <typename outer_range>
    struct const_correct_typedefs
    {
      typedef typename boost::range_iterator<outer_range>::type outer_iterator;
      typedef typename boost::mpl::eval_if_c< boost::is_const<outer_range>::value,
        boost::add_const<typename boost::range_value<
          typename boost::remove_const<outer_range>::type >::type >,
        boost::range_value<outer_range> >::type inner_range;

      typedef typename boost::range_iterator<inner_range>::type inner_iterator;
      typedef typename boost::iterator_reference<inner_iterator>::type inner_reference;
      typedef typename boost::remove_const<inner_reference>::type inner_value;
    };//end const_correct_typedefs
  }//end aux

  template <typename outer_range>
  class flattened_iterator
    : public boost::iterator_facade<
        flattened_iterator<outer_range>,
        typename aux::const_correct_typedefs<outer_range>::inner_value,
        boost::forward_traversal_tag //TODO: More types.
      >
  {
    typedef aux::const_correct_typedefs<outer_range> typedefs;
  public:
    typedef typename typedefs::outer_iterator outer_iterator;
    typedef typename typedefs::inner_range inner_range;
    typedef typename typedefs::inner_iterator inner_iterator;
    typedef typename typedefs::inner_reference inner_reference;

    flattened_iterator() : in_(), out_(), out_end_()
    {}
    flattened_iterator(outer_iterator out, outer_iterator out_end,
                       boost::optional<inner_iterator> in)
      : in_(in), out_(out), out_end_(out_end)
    {
      if(out_ != out_end_ && in_)
        make_valid_in();
    }

    template <typename U>
    flattened_iterator(flattened_iterator<U> const& other)
      : in_(other.in_), out_(other.out_), out_end_(other.out_end_)
    {}

    outer_iterator get_outer() const
    { return out_; }

    inner_iterator get_inner() const
    { return *in_; }

  private:
    friend class boost::iterator_core_access;
    template <typename> friend class flattened_iterator;

    template <typename U>
    bool equal(flattened_iterator<U> const& other) const
    { return other.out_ == out_ && other.in_ == in_; }

    void increment()
    {
      BOOST_ASSERT(in_);
      ++*in_;
      make_valid_in();
    }

    void make_valid_in()
    {
      BOOST_ASSERT(in_);
      while(out_ != out_end_ && *in_ == out_->end())
        *in_ = (++out_)->begin();
    }

    inner_reference dereference() const
    { BOOST_ASSERT(in_ && *in_ != out_->end()); return **in_; }

    boost::optional<inner_iterator> in_;
    outer_iterator out_, out_end_;
  };//end flattened_iterator

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
      boost::optional<typename iterator::inner_iterator> optIn(boost::none);
      return iterator(nodes_.begin(), end().get_outer(),
        boost::empty(nodes_) ? optIn : nodes_.begin()->begin());
    }

    const_iterator begin() const
    {
      return const_iterator(nodes_.begin(), end().get_outer(),
        boost::empty(nodes_) ? boost::none : nodes_.begin()->begin());
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

