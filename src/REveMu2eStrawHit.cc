#include "REve/inc/REveMu2eStrawHit.hh"

using namespace mu2e;
namespace REX = ROOT::Experimental;

  /*------------Function to build title:-------------*/
  std::string REveMu2eStrawHit::DataTitle(const std::string &pstr, int n){
        std::string dstr=" hit#" + std::to_string(n) + "\nLayer: ";
        std::string strlab=pstr+dstr;
        return (strlab);
  }


/*------------Function to display straws which are hit-------*/
  std::tuple<CLHEP::Hep3Vector, CLHEP::Hep3Vector, std::string, int> REveMu2eStrawHit::DrawStraw(){
        mu2e::GeomHandle<mu2e::Tracker> tracker;
        const auto& allStraws = tracker->getStraws();
        CLHEP::Hep3Vector sposi(0.0,0.0,0.0), sposf(0.0,0.0,0.0);
        std::string strawtitle;
        int colorid = 0;
        for (size_t i = 0; i<tracker->nStraws(); i++){
          const mu2e::Straw& s = allStraws[i];
          if(s.id().asUint16()==fComboHit_._sid.asUint16())
          {
          const CLHEP::Hep3Vector& p = s.getMidPoint();
          const CLHEP::Hep3Vector& d = s.getDirection();
          int idStraw =  s.id().getStraw();
          int idPanel =  s.id().getPanel();
          int idPlane =  s.id().getPlane();
          colorid = idPlane + idPanel;
          strawtitle =Form("Straw %i Panel %i  Plane %i",idStraw,idPanel,idPlane);
          double x = p.x();
          double y = p.y();
          double z = p.z();
          double theta = d.theta();
          double phi = d.phi();
          double l = s.halfLength();
          double st=sin(theta);
          double ct=cos(theta);
          double sp=sin(phi+TMath::Pi()/2.0);
          double cp=cos(phi+TMath::Pi()/2.0);
          double x1=x+l*st*sp;
          double y1=y-l*st*cp;
          double z1=z+l*ct;
          double x2=x-l*st*sp;
          double y2=y+l*st*cp;
          double z2=z-l*ct;
          sposi.set(x1, y1, z1);
          sposf.set(x2, y2, z2);
          }
        }
        return {sposi, sposf, strawtitle, colorid};
  }

