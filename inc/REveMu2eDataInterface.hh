#ifndef REveMu2eDataInterface_hh
#define REveMu2eDataInterface_hh
#include <ROOT/REveElement.hxx>
#include <ROOT/REvePointSet.hxx>
#include <ROOT/REveManager.hxx>
#include <ROOT/REveLine.hxx>
#include <ROOT/REveScene.hxx>
#include <ROOT/REveTrackPropagator.hxx>
#include "RecoDataProducts/inc/CaloCluster.hh"
#include "RecoDataProducts/inc/ComboHit.hh"
#include "RecoDataProducts/inc/KalSeed.hh"
#include "RecoDataProducts/inc/ComboHit.hh"
#include "GeometryService/inc/GeomHandle.hh"
#include "CalorimeterGeom/inc/CaloGeomUtil.hh"
#include "CalorimeterGeom/inc/Calorimeter.hh"
#include "GeometryService/inc/DetectorSystem.hh"
#include "Mu2eInterfaces/inc/Detector.hh"
#include "DataProducts/inc/XYZVec.hh"
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
    class REveMu2eDataInterface {
        public:
         
          explicit REveMu2eDataInterface(){};
          explicit REveMu2eDataInterface(const REveMu2eDataInterface &);
          REveMu2eDataInterface& operator=(const REveMu2eDataInterface &);
          virtual ~REveMu2eDataInterface() = default;
          #ifndef __CINT__
          REX::REveElement *fClusters;
          void AddComboHits(REX::REveManager *&eveMng,bool firstloop, const mu2e::ComboHitCollection *clustercol,REX::REveElement* &hitlist, REX::REveProjectionManager *mngRhoPhi, REX::REveProjectionManager *mngRhoZ, REX::REveScene  *rPhiGeomScene, REX::REveScene  *rhoZGeomScene);
          void AddCaloClusters(REX::REveManager *&eveMng,bool firstloop, const mu2e::CaloClusterCollection *hitcol, REX::REveElement* &clusterlist, REX::REveProjectionManager *mngRhoPhi, REX::REveProjectionManager *mngRhoZ, REX::REveScene  *rPhiGeomScene, REX::REveScene  *rhoZGeomScene);
          void AddKalSeedCollection(REX::REveManager *&eveMng,bool firstloop,  std::tuple<std::vector<std::string>, std::vector<const KalSeedCollection*>> track_tuple,REX::REveElement* &tracklist, REX::REveProjectionManager *mngRhoPhi, REX::REveProjectionManager *mngRhoZ, REX::REveScene  *rPhiGeomScene, REX::REveScene  *rhoZGeomScene);
          #endif
          ClassDef(REveMu2eDataInterface, 0);
      }; 
}


#endif 


