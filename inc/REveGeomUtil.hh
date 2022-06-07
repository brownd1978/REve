#ifndef _REveGeomUtil_hh
#define _REveGeomUtil_hh

#include <ROOT/REveElement.hxx>
#include <ROOT/REveManager.hxx>

#include "Offline/GeometryService/inc/GeomHandle.hh"
#include "Offline/StoppingTargetGeom/inc/StoppingTarget.hh"
#include "Offline/GeometryService/inc/DetectorSystem.hh"

namespace REX = ROOT::Experimental;
 
namespace mu2e {
    class REveGeomUtil  : public REX::REveElement {

        public :
            explicit REveGeomUtil() { SetErrorHandler(DefaultErrorHandler); }
            virtual ~REveGeomUtil() {}
            #ifndef __CINT__
            double FindStoppingTarget_z();
            double GetStoppingTarget_z(){ return StoppingTarget_z; }
            double StoppingTarget_z;
  
            #endif
            ClassDef(REveGeomUtil, 0);


    
      };

}
#endif


