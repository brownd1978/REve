#include "REve/inc/REveMu2eGUI.hh"

using namespace mu2e;


int REveMu2eGUI::WriteCoreJson(nlohmann::json &j, int rnr_offset)
{
  j["path"] = "Event/SubRun/Run"; 
  j["count"] = fCount;
  j["total"] = fTotal;
  j["eventid"] = feventid;
  j["runid"] = frunid;
  j["UT_PostStream"] = "UT_refresh_event_info";
  return ROOT::Experimental::REveElement::WriteCoreJson(j, 0);
}

void REveMu2eGUI::PrintEventInfo(){
  std::cout<<"Event : "<<feventid<<" Sub Run : "<<" Run : "<<frunid<<std::endl;
}

