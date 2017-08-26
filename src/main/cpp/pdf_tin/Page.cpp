#include "Page.hpp"
#include <pdf_tin/TextStyleSegment.hpp>
#include <pdf_tin/detail/GListPtr.hpp>
#include <poppler/glib/poppler.h>

using namespace pdf_tin;

const char* Page::label() const {
  return poppler_page_get_label(page_.get());
}

int Page::index() const {
  return poppler_page_get_index(page_.get());
}

double Page::width() const {
  double w, h;
  poppler_page_get_size(page_.get(), &w, &h);
  return w;
}

double Page::height() const {
  double w, h;
  poppler_page_get_size(page_.get(), &w, &h);
  return h;
}

double Page::duration() const {
  return poppler_page_get_duration(page_.get());
}


std::vector<Text> Page::text() const {
  std::vector<Text> textBlocks;
  PopplerRectangle* regions = nullptr;
  uint32_t numRegions = 0;

  try {
    poppler_page_get_text_layout(page_.get(), &regions, &numRegions);
    for (uint32_t i = 0; i < numRegions; ++i) {
      PopplerRectangle* region = &regions[i];
      if (((region->x2 - region->x1) > 1e-03) &&
	  ((region->y2 - region->y1) > 1e-03)) {
	const char* text = poppler_page_get_text_for_area(page_.get(), region);
	if (text && text[0]) {
	  detail::GListPtr attributes(
	      poppler_page_get_text_attributes_for_area(page_.get(), region),
	      std::function<void (GList*)>(poppler_page_free_text_attributes)
	  );
	  std::vector<TextStyleSegment> styles;
	  for (GList* p = attributes.get(); p; p = p->next) {
	    styles.push_back(
	      TextStyleSegment::convert((_PopplerTextAttributes*)p->data)
	    );
	  }
	  textBlocks.emplace_back(std::string(text), BoundingBox(*region),
				  styles.size(), styles.begin());
	}
      }
    }						  
    g_free(regions);
    return std::move(textBlocks);  
  } catch(...) {
    g_free(regions);
    throw;
  }
}


std::vector<Image> Page::images() const {
  std::vector<Image> imageList;
  detail::GListPtr imageMapping(
      poppler_page_get_image_mapping(page_.get()),
      poppler_page_free_image_mapping
  );
  for (auto p = imageMapping.get(); p; ++p) {
    PopplerImageMapping* image = (PopplerImageMapping*)p->data;
    imageList.emplace_back(image->image_id, BoundingBox(image->area));
  }

  return std::move(imageList);
}

