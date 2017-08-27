#ifndef __PDF_TIN__IMMUTABLELIST_HPP__
#define __PDF_TIN__IMMUTABLELIST_HPP__

#include <algorithm>
#include <iterator>
#include <memory>

namespace pdf_tin {

  /** @brief A list whose size and content are fixed at creation */
  template <typename T, typename Allocator = std::allocator<T> >
  class ImmutableList : Allocator {
  public:
    typedef T ValueType; ///< Type of items stored in list
    typedef Allocator AllocatorType; ///< Allocator for this container
    typedef const T* ConstIterator; ///< Iterator over items in the list
    typedef const T* Iterator;      ///< Iterator over items in the list
    
  public:
    ImmutableList(const Allocator& allocator = Allocator()):
        Allocator(allocator), items_(nullptr), end_(nullptr) {
      // Intentionally left blank
    }

    template <typename InputIterator>
    ImmutableList(uint32_t numItems, InputIterator items,
		  const Allocator& allocator = Allocator()):
        Allocator(allocator), items_(this->allocate(numItems)),
	end_(items_ + numItems) {
      T* p = items_;
      for (uint32_t i = 0; i < numItems; ++i, ++p, ++items) {
	new(p) T(*items);
      }
    }

    template <typename FwdIterator>
    ImmutableList(FwdIterator startOfItems, FwdIterator endOfItems,
		  const Allocator& allocator = Allocator()):
        ImmutableList(std::distance(startOfItems, endOfItems), startOfItems,
		      allocator) {
      // Intentionally left blank
    }

    template <typename U>
    ImmutableList(const std::initializer_list<U>& items,
		  const Allocator& allocator = Allocator()):
        ImmutableList(items.size(), items.begin(), allocator) {
      // Intentionally left blank
    }

    ImmutableList(const ImmutableList<T>& other):
        Allocator(other), items_(this->allocate(other.size())),
        end_(items_ + other.size()) {
      std::uninitialized_copy(other.begin(), other.end(), items_);
    }

    template <typename U>
    ImmutableList(const ImmutableList<U>& other,
		  const Allocator& allocator = Allocator()):
        Allocator(allocator), items_(this->allocate(other.size())),
        end_(items_ + other.size()) {
      std::uninitialized_copy(other.begin(), other.end(), items_);
    }

    ImmutableList(ImmutableList<T>&& other):
        Allocator(std::move(other)), items_(other.items_), end_(other.end_) {
      other.items_ = nullptr;
      other.end_ = nullptr;
    }

    ~ImmutableList() {
      if (items_) {
	for (auto p = items_; p != end_; ++p) {
	  p->~T();
	}
	this->deallocate(items_, end_ - items_);
      }
    }
    
    uint32_t size() const { return (uint32_t)(end_ - items_); }
    ConstIterator begin() const { return items_; }
    ConstIterator end() const { return end_; }
    ConstIterator find(const T& x) const {
      return std::find(begin(), end(), x);
    }

    const T& operator[](uint32_t i) const { return items_[i]; }
    
  private:
    T* items_;  ///< First item in list
    T* end_;    ///< End of list
  };
}

#endif
