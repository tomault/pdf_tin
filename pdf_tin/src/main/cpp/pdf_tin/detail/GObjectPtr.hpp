#ifndef __PDF_TIN__DETAIL__GOJBECTPTR_HPP__
#define __PDF_TIN__DETAIL__GOJBECTPTR_HPP__

#include <glib-object.h>

namespace pdf_tin {
  namespace detail {

    template <typename T>
    class GObjectPtr {
    public:
      GObjectPtr() noexcept : p_(nullptr)  { }
      GObjectPtr(T* p) noexcept : p_(p) { }
      GObjectPtr(const GObjectPtr& other) noexcept : p_(addRef_(other.p_)) { }
      GObjectPtr(GObjectPtr&& other) noexcept : p_(other.p_) {
	other.p_ = nullptr;
      }
      ~GObjectPtr() noexcept { removeRef_(); }

      T* get() const noexcept { return p_; }

      operator bool() const noexcept { return (bool)p_; }

      GObjectPtr& operator=(const GObjectPtr& other) noexcept {
	removeRef_();
	p_ = addRef_(other.p_);
	return *this;
      }
      
      GObjectPtr& operator=(GObjectPtr&& other) noexcept {
	removeRef_();
	p_ = other.p_;
	other.p_ = nullptr;
	return *this;
      }
      
    private:
      T* p_;

      static T* addRef_(T* p) noexcept {
	return p ? (T*)g_object_ref((void*)p) : p;
      }
      
      void removeRef_() noexcept {
	if (p_) {
	  g_object_unref((void*)p_);
	}
      }
    };
    
  }
}

#endif
