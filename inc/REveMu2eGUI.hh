#ifndef _REveMu2eGUI_hh
#define _REveMu2eGUI_hh

#include <ROOT/REveElement.hxx>
#include "nlohmann/json.hpp"
namespace REX = ROOT::Experimental;
using namespace ROOT::Experimental;

namespace mu2e {
class REveMu2eGUI : public ROOT::Experimental::REveElement
{
  public:
     int fCount{0};
     int fTotal{10};

     int WriteCoreJson(nlohmann::json &j, int rnr_offset) override;
};
}

#endif
