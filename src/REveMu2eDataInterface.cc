
#include "REve/inc/REveMu2eDataInterface.hh"
using namespace mu2e;
namespace REX = ROOT::Experimental;

void REveMu2eDataInterface::AddCaloClusters(REX::REveManager *&eveMng, bool firstLoop_, const mu2e::CaloClusterCollection *clustercol, REX::REveElement* &scene, REX::REveProjectionManager *mngRhoPhi, REX::REveProjectionManager *mngRhoZ, REX::REveScene  *rPhiEveScene, REX::REveScene  *rhoZEveScene){
    if(clustercol != 0){    
        mu2e::Calorimeter const &cal = *(mu2e::GeomHandle<mu2e::Calorimeter>());
        GeomHandle<DetectorSystem> det;
        double dz = cal.geomUtil().origin().z() - cal.geomUtil().trackerCenter().z();
        if(!firstLoop_){
            scene->DestroyElements();;
        }

        auto ps1 = new REX::REvePointSet("clusters", "",1);

        for(unsigned int i=0; i< clustercol->size(); i++){
            mu2e::CaloCluster const  &cluster= (*clustercol)[i];
            CLHEP::Hep3Vector COG(cluster.cog3Vector().x(),cluster.cog3Vector().y(), cluster.cog3Vector().z());
            //CLHEP::Hep3Vector crystalPos   = cal.geomUtil().mu2eToDiskFF(cluster.diskID(),COG);
            //CLHEP::Hep3Vector pointInMu2e = det->toMu2e(crystalPos);
            ps1->SetNextPoint(COG.x()/10, COG.y()/10 + 100, COG.z()/10 + dz/10 - 70); //70cm = separation between disks TODO - extract from GeomService
        }

        ps1->SetMarkerColor(kRed);
        ps1->SetMarkerStyle(4);
        ps1->SetMarkerSize(4);

        scene->AddElement(ps1); 
    }
}

void REveMu2eDataInterface::AddComboHits(REX::REveManager *&eveMng, bool firstLoop_, const mu2e::ComboHitCollection *chcol, REX::REveElement* &scene, REX::REveProjectionManager *mngRhoPhi, REX::REveProjectionManager *mngRhoZ, REX::REveScene  *rPhiEveScene, REX::REveScene  *rhoZEveScene){
    if(chcol!=0){
        auto ps1 = new REX::REvePointSet("combohits", "",1);
        if(!firstLoop_){
            scene->DestroyElements();;
        }
        for(unsigned int i=0; i< chcol->size(); i++){
            mu2e::ComboHit const  &hit= (*chcol)[i];
            CLHEP::Hep3Vector HitPos(hit.pos().x(), hit.pos().y(), hit.pos().z());
            ps1->SetNextPoint(HitPos.x()/10, HitPos.y()/10 + 100, HitPos.z()/10); 
        }


        ps1->SetMarkerColor(kBlue);
        ps1->SetMarkerStyle(4);
        ps1->SetMarkerSize(6);
        scene->AddElement(ps1); 
    }
}

void REveMu2eDataInterface::AddKalSeedCollection(REX::REveManager *&eveMng,bool firstloop,  std::tuple<std::vector<std::string>, std::vector<const KalSeedCollection*>> track_tuple, REX::REveElement* &scene, REX::REveProjectionManager *mngRhoPhi, REX::REveProjectionManager *mngRhoZ, REX::REveScene  *rPhiEveScene, REX::REveScene  *rhoZEveScene){
    std::vector<const KalSeedCollection*> track_list = std::get<1>(track_tuple);
    std::vector<std::string> names = std::get<0>(track_tuple);
    std::vector<int> colour;

    for(unsigned int j=0; j< track_list.size(); j++){
      const KalSeedCollection* seedcol = track_list[j];
      colour.push_back(j+3);
      
      if(seedcol!=0){  
        
        for(unsigned int k = 0; k < seedcol->size(); k = k + 20){ 
          mu2e::KalSeed kseed = (*seedcol)[k];
          const std::vector<mu2e::KalSegment> &segments = kseed.segments();
          size_t nSegments=segments.size();
          if(nSegments==0) continue;
          const mu2e::KalSegment &segmentFirst = kseed.segments().front();
          const mu2e::KalSegment &segmentLast = kseed.segments().back();
          double fltLMin=segmentFirst.fmin();
          double fltLMax=segmentLast.fmax();
          auto line = new REX::REveLine(names[j], "",nSegments);
  
          for(size_t m=0; m<nSegments; m++){
            const mu2e::KalSegment &segment = segments.at(m);
            fltLMin=segment.fmin();
            fltLMax=segment.fmax();
            if(m>0){
              double fltLMaxPrev=segments.at(m-1).fmax();
              fltLMin=(fltLMin+fltLMaxPrev)/2.0;
            }
            if(m+1<nSegments){
              double fltLMinNext=segments.at(m+1).fmin();
              fltLMax=(fltLMax+fltLMinNext)/2.0;
            }
            for(double fltL=fltLMin; fltL<=fltLMax; fltL+=1.0){
              XYZVec pos;
              segment.helix().position(fltL,pos);
              CLHEP::Hep3Vector p = Geom::Hep3Vec(pos);
              line->SetNextPoint((p.x())/10, (p.y())/10 + 100, (p.z())/10);
            }
          }
        
        line->SetLineColor(kBlack);
        line->SetLineWidth(5);
        scene->AddElement(line); 
    }
   }
  }
}



