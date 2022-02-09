#ifndef REveMu2eStrawHit_hh
#define REveMu2eStrawHit_hh
#include <ROOT/REveElement.hxx>
#include <ROOT/REvePointSet.hxx>
#include <ROOT/REveManager.hxx>
#include <ROOT/REveLine.hxx>
#include <ROOT/REveScene.hxx>
#include <ROOT/REveTrackPropagator.hxx>
#include "Offline/RecoDataProducts/inc/ComboHit.hh"
#include "Offline/TrackerGeom/inc/Tracker.hh"
#include "Offline/GeometryService/inc/GeomHandle.hh"
#include "Offline/CalorimeterGeom/inc/CaloGeomUtil.hh"
#include "Offline/Mu2eInterfaces/inc/Detector.hh"
#include "Offline/GeometryService/inc/DetectorSystem.hh"
#include "Offline/DataProducts/inc/GenVector.hh"
#include <TApplication.h>
#include <TEvePad.h>
#include <TObject.h>
#include <TSystem.h>
#include <limits>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace mu2e;
namespace REX = ROOT::Experimental;
namespace mu2e{
    class REveMu2eStrawHit {
     public:
      explicit REveMu2eHit();
      explicit REveMu2eStrawHit(const REveMu2eStrawHit &);
          REveMu2eStrawHit& operator=(const REveMu2eStrawhit &);
          virtual ~REveMu2eStrawHit() = default;
          #ifndef __CINT__
      std::tuple<CLHEP::Hep3Vector, CLHEP::Hep3Vector, std::string, int> DrawStraw();
      std::string DataTitle(const std::string &pstr, int n);
      #endif
  
      ClassDef(REveMu2eStrawHit, 0);
      }; 
}


#endif 
