#include "REve/inc/REveMu2eGUI.hh"

using namespace mu2e;


int REveMu2eGUI::WriteCoreJson(nlohmann::json &j, int rnr_offset)
{
  /*j["path"] = "data.root";
  j["count"] = fCount;
  j["total"] = fTotal;
  j["UT_PostStream"] = "UT_refresh_event_info";*/
  return ROOT::Experimental::REveElement::WriteCoreJson(j, 0);
}
