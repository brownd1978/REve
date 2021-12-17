#ifndef REveMu2eMCInterface_hh
#define REveMu2eMCInterface_hh
#include <ROOT/REveElement.hxx>
#include <ROOT/REvePointSet.hxx>
#include <ROOT/REveManager.hxx>
#include <ROOT/REveLine.hxx>
#include <ROOT/REveScene.hxx>
#include "Offline/GeometryService/inc/GeomHandle.hh"
#include "Offline/GeometryService/inc/DetectorSystem.hh"
#include "Offline/Mu2eInterfaces/inc/Detector.hh"
#include "Offline/DataProducts/inc/GenVector.hh"
#include "Offline/MCDataProducts/inc/MCTrajectoryPoint.hh"
#include "Offline/MCDataProducts/inc/MCTrajectoryCollection.hh"
#include <TApplication.h>
#include <TEvePad.h>
#include <TObject.h>
#include <TSystem.h>
#include <vector>
#include <tuple>
#include <string>


using namespace mu2e;
namespace REX = ROOT::Experimental;
namespace mu2e{
    class REveMu2eMCInterface {
        public:
          explicit REveMu2eMCInterface(){};
          explicit REveMu2eMCInterface(const REveMu2eMCInterface &);
          REveMu2eMCInterface& operator=(const REveMu2eMCInterface &);
          virtual ~REveMu2eMCInterface() = default;
          #ifndef __CINT__
          void AddMCTrajectoryCollection(REX::REveManager *&eveMng, bool firstLoop, const MCTrajectoryCollection *trajcol, REX::REveElement* &scene);
          #endif
          ClassDef(REveMu2eMCInterface, 0);
      }; 
}


#endif 
