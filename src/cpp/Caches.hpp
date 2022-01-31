#ifndef __PYEXT__PDF_TIN__CACHES_HPP__
#define __PYEXT__PDF_TIN__CACHES_HPP__

#include <pdf_tin/Color.hpp>
#include <pdf_tin/TextStyle.hpp>

#include <Color.hpp>
#include <PyObjectPtr.hpp>
#include <TextStyle.hpp>
#include <functional>
#include <memory>
#include <string>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

template <typename KeyType, typename ValueType>
class PyObjectCache {
public:
  PyObjectCache(const std::function<ValueType* (const KeyType&)>& valueFactory)
      : cache_(), createValue_(valueFactory) {
  }
  PyObjectCache(const PyObjectCache&) = delete;
  PyObjectCache(PyObjectCache&&) = default;

  ValueType* get(const KeyType& key) {
    auto i = cache_.find(key);
    ValueType* value = nullptr;

    if (i != cache_.end()) {
      value = (ValueType*)i->second.get();
    } else {
      value = createValue_(key);
      cache_.insert(std::make_pair(key, PyObjectPtr((PyObject*)value)));
    }
    
    Py_INCREF(value);
    return value;
  }

  void clear() { cache_.clear(); }

  PyObjectCache& operator=(const PyObjectCache&) = delete;
  PyObjectCache& operator=(PyObjectCache&&) = default;

  ValueType* operator[](const KeyType& key) { return get(key); }

private:
  std::unordered_map<KeyType, PyObjectPtr> cache_;
  std::function<ValueType* (const KeyType&)> createValue_;
};

class StringCache : public PyObjectCache<std::string, PyObject> {
public:
  StringCache() : PyObjectCache([](const std::string& s) {
      return PyUnicode_DecodeUTF8(s.c_str(), s.size(), nullptr);
  }) {
    // Intentionally left blank
  }
};

class ColorCache : public PyObjectCache<pdf_tin::Color, PdfTin_Color> {
public:
  ColorCache() : PyObjectCache([](const pdf_tin::Color& c) {
      PyObjectPtr args(Py_BuildValue("HHH", c.red(), c.green(), c.blue()));
      return (PdfTin_Color*)PyObject_Call((PyObject*)pdfTin_ColorType(),
					  args.get(), nullptr);
  }) {
    // Intentionally left blank
  }
};

class TextStyleCache
    : public PyObjectCache<pdf_tin::TextStyle, PdfTin_TextStyle> {
public:
  TextStyleCache(const std::shared_ptr<StringCache>& stringCache,
		 const std::shared_ptr<ColorCache>& colorCache)
      : PyObjectCache([this](const pdf_tin::TextStyle& s) {
            PyObjectPtr styleCapsule(
		PyCapsule_New((void*)&s, nullptr, nullptr)
	    );
	    PyObjectPtr stringCacheCapsule(
	        PyCapsule_New(this->stringCache_.get(), nullptr, nullptr)
	    );
	    PyObjectPtr colorCacheCapsule(
	      PyCapsule_New(this->colorCache_.get(), nullptr, nullptr)
	    );
	    PyObjectPtr args(
		Py_BuildValue("OOO", styleCapsule.get(),
			      stringCacheCapsule.get(),
			      colorCacheCapsule.get())
	    );
	    if (!args) {
	      return (PdfTin_TextStyle*)0;
	    }
            return (PdfTin_TextStyle*)PyObject_Call(
		(PyObject*)pdfTin_TextStyleType(), args.get(), nullptr
	    );
        }), stringCache_(stringCache), colorCache_(colorCache) {
    // Intentionally left blank
  }

  const std::shared_ptr<StringCache> stringCache() const {
    return stringCache_;
  }
  
  const std::shared_ptr<ColorCache> colorCache() const {
    return colorCache_;
  }
  
private:
  std::shared_ptr<StringCache> stringCache_;
  std::shared_ptr<ColorCache> colorCache_;
};

#endif
