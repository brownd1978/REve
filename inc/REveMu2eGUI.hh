#ifndef _REveMu2eGUI_hh
#define _REveMu2eGUI_hh

#include <ROOT/REveElement.hxx>
#include "nlohmann/json.hpp"
namespace ROOT::Experimental {
  class REveManager;
}

namespace mu2e {
class REveMu2eGUI : public ROOT::Experimental::REveElement
{
  public:
     int fCount{0};
     int fTotal{10};
     int fLumi{1111};

     using ROOT::Experimental::REveElement::WriteCoreJson;
     int WriteCoreJson(nlohmann::json &j, int rnr_offset) override
     {
        j["path"] = "data.root";
        j["count"] = fCount;
        j["total"] = fTotal;
        j["UT_PostStream"] = "UT_refresh_event_info";
        return ROOT::Experimental::REveElement::WriteCoreJson(j, 0);
     }
};
}

#endif
