#include "REve/inc/REveMu2eDataInterface.hh"
using namespace mu2e;
namespace REX = ROOT::Experimental;

void REveMu2eDataInterface::AddCaloClusters(REX::REveManager *&eveMng, bool firstLoop_, const mu2e::CaloClusterCollection *clustercol, REX::REveElement* &scene){
    std::cout<<"[REveMu2eDataInterface] AddCaloClusters "<<std::endl;
    if(clustercol != 0){    
        
        if(!firstLoop_){
            scene->DestroyElements();;
        }
        mu2e::Calorimeter const &cal = *(mu2e::GeomHandle<mu2e::Calorimeter>());
        GeomHandle<DetectorSystem> det;
        for(unsigned int i=0; i< clustercol->size(); i++){
            mu2e::CaloCluster const  &cluster= (*clustercol)[i];
            std::string cluster_energy = std::to_string(cluster.energyDep());
            std::string cluster_time = std::to_string(cluster.time());
            std::string cluster_x = std::to_string(cluster.cog3Vector().x());
            std::string cluster_y = std::to_string(cluster.cog3Vector().y());
            
            CLHEP::Hep3Vector COG(cluster.cog3Vector().x(),cluster.cog3Vector().y(), cluster.cog3Vector().z());
            CLHEP::Hep3Vector crystalPos   = cal.geomUtil().mu2eToDiskFF(cluster.diskID(),COG);
            CLHEP::Hep3Vector pointInMu2e = det->toMu2e(crystalPos);
            
            std::string cluster_z = std::to_string(abs(pointInMu2e.z()));
            
            std::string label =  "Energy Dep. = "+cluster_energy+" MeV "+", Time = "+cluster_time+" ns " +" Pos =  ("+cluster_x+","+cluster_y+","+cluster_z+") mm";
            auto ps1 = new REX::REvePointSet("disk1", "CaloClusters Disk 1: "+label,0);
            auto ps2 = new REX::REvePointSet("disk2", "CaloClusters Disk 2: "+label,0);
            
            if(cluster.diskID() == 0) ps1->SetNextPoint(COG.x()/10, COG.y()/10 +100, abs(pointInMu2e.z())/10); 
            if(cluster.diskID() == 1) ps2->SetNextPoint(COG.x()/10, COG.y()/10 +100, abs(pointInMu2e.z())/10); 

            ps1->SetMarkerColor(kRed);
            ps1->SetMarkerStyle(4);
            ps1->SetMarkerSize(4);

            ps2->SetMarkerColor(kRed);
            ps2->SetMarkerStyle(4);
            ps2->SetMarkerSize(4);
        
            if(ps1->GetSize() !=0 ) scene->AddElement(ps1); 
            if(ps2->GetSize() !=0 ) scene->AddElement(ps2); 
        }
    }
}

void REveMu2eDataInterface::AddComboHits(REX::REveManager *&eveMng, bool firstLoop_, const mu2e::ComboHitCollection *chcol, REX::REveElement* &scene){
    std::cout<<"[REveMu2eDataInterface] AddComboHits "<<std::endl;
    if(chcol!=0){
        auto ps1 = new REX::REvePointSet("ComboHits", "",0); // TODO - add in descriptive label
        if(!firstLoop_){
            scene->DestroyElements();;
        }

        for(unsigned int i=0; i< chcol->size(); i++){
            mu2e::ComboHit const  &hit= (*chcol)[i];
            CLHEP::Hep3Vector HitPos(hit.pos().x(), hit.pos().y(), hit.pos().z());
            ps1->SetNextPoint(HitPos.x()/10, HitPos.y()/10 +100, HitPos.z()/10); 
        }
  
        ps1->SetMarkerColor(kBlue);
        ps1->SetMarkerStyle(4);
        ps1->SetMarkerSize(6);
        if(ps1->GetSize() !=0 ) scene->AddElement(ps1); 
    }
}

/*------------Function to add TimeCluster Collection in 3D and 2D displays:-------------*/
  void REveMu2eDataInterface::AddTimeClusters(REX::REveManager *&eveMng, bool firstLoop_, const mu2e::TimeClusterCollection *tccol, REX::REveElement* &scene){
      std::cout<<"[REveMu2eDataInterface] AddTimeClusters "<<std::endl;
      if(tccol!=0){
        auto ps1 = new REX::REvePointSet("TimeClusters", "TimeCluster", 0); // TODO - add in descriptive label
        if(!firstLoop_){
          scene->DestroyElements();;
        }
        for(size_t i=0; i<tccol->size();i++){
          mu2e::TimeCluster const  &tclust= (*tccol)[i];
          CLHEP::Hep3Vector HitPos(tclust._pos.x(), tclust._pos.y(), tclust._pos.z());
          ps1->SetNextPoint(HitPos.x()/10, HitPos.y()/10 +100, HitPos.z()/10); 
        }
        ps1->SetMarkerColor(kGreen);
        ps1->SetMarkerStyle(4);
        ps1->SetMarkerSize(6);
        if(ps1->GetSize() !=0 ) scene->AddElement(ps1); 
      }
  }
	
/*------------Function to color code the Tracker hits in 3D and 2D displays:-------------*/
  void REveMu2eDataInterface::AddTrkHits(REX::REveManager *&eveMng, bool firstLoop_, const mu2e::ComboHitCollection *chcol,
					 std::tuple<std::vector<std::string>, std::vector<const KalSeedCollection*>> track_tuple, REX::REveElement* &scene){
      std::vector<const KalSeedCollection*> track_list = std::get<1>(track_tuple);
      std::cout<<"[REveMu2eDataInterface] AddTrkHits  "<<std::endl;
      GeomHandle<DetectorSystem> det;
      StrawId trksid[70];
      unsigned int trkhitsize=0;
      //Save the hit straw IDs of the KalSeed hits  
      for(unsigned int j=0; j< track_list.size(); j++){
        const KalSeedCollection* seedcol = track_list[j];
        if(seedcol!=0){
          for(unsigned int k = 0; k < seedcol->size(); k++){
            KalSeed kseed = (*seedcol)[k];
            const std::vector<mu2e::TrkStrawHitSeed> &hits = kseed.hits();
            trkhitsize = hits.size();
            for(unsigned int i=0; i <trkhitsize; i++){
              const mu2e::TrkStrawHitSeed &hit = hits.at(i);
              trksid[i] = hit._sid; 
            }
          }
        }
      }        
      StrawId usedtrksid[trkhitsize];
      unsigned int usedid[trkhitsize];
      //Compare the straw IDs of the Kal seed hits with the hits in the ComboHit Collection
      if(chcol!=0){
        for(unsigned int i=0; i<chcol->size();i++){
          ComboHit hit = (*chcol)[i];
          for(unsigned int q=0; q<trkhitsize; q++){
            if(hit._sid == trksid[q]){
              usedid[q]=q;
              usedtrksid[q]=hit._sid;//Save the Straw ID if the KalSeed and Combo hit ID matches
              CLHEP::Hep3Vector HitPos(hit.pos().x(), hit.pos().y(), hit.pos().z());
	      // CLHEP::Hep3Vector HitPos = det->toMu2e(HitPosition);
              auto trkhit = new REX::REvePointSet("TrkHits", "trkhit",0); 
              trkhit ->SetMarkerStyle(9);
	      trkhit ->SetMarkerSize(1);
              trkhit ->SetMarkerColor(kGreen-4);
              trkhit ->SetNextPoint(HitPos.x()/10,HitPos.y()/10 + 100,HitPos.z()/10);
              if(trkhit->GetSize() !=0 ) scene->AddElement(trkhit); 
	    }
          }
        }
        //Hits which are not in the ComboHit Collection of the helix    
        for(unsigned int i = 0;i < trkhitsize;i++){
          if(i != usedid[i] && i<chcol->size()){
            ComboHit chhit = (*chcol)[i];
            CLHEP::Hep3Vector HitPos(chhit.pos().x(), chhit.pos().y(), chhit.pos().z());
	    // CLHEP::Hep3Vector HitPos = det->toMu2e(HitPosition);
            auto notusedtrkhit = new REX::REvePointSet("NotTrkHits", "nottrkhit",0); 
            notusedtrkhit ->SetMarkerStyle(9);
            notusedtrkhit ->SetMarkerSize(1);
            notusedtrkhit ->SetNextPoint(HitPos.x()/10,HitPos.y()/10 + 100,HitPos.z()/10);
            notusedtrkhit ->SetMarkerColor(kRed-4);
	    if(notusedtrkhit->GetSize() !=0 ) scene->AddElement(notusedtrkhit); 
	  }
        }
      }
  }

void REveMu2eDataInterface::AddKalSeedCollection(REX::REveManager *&eveMng,bool firstloop,  std::tuple<std::vector<std::string>, std::vector<const KalSeedCollection*>> track_tuple, REX::REveElement* &scene){
    std::vector<const KalSeedCollection*> track_list = std::get<1>(track_tuple);
    std::vector<std::string> names = std::get<0>(track_tuple);
    std::vector<int> colour;
    std::cout<<"[REveMu2eDataInterface] AddTracks  "<<std::endl;
    for(unsigned int j=0; j< track_list.size(); j++){
      const KalSeedCollection* seedcol = track_list[j];
      colour.push_back(j+3);
      
      if(seedcol!=0){  
        for(unsigned int k = 0; k < seedcol->size(); k = k + 20){ 
          mu2e::KalSeed kseed = (*seedcol)[k];
          const std::vector<mu2e::KalSegment> &segments = kseed.segments();
          unsigned int nSegments=segments.size();
          if(nSegments==0) continue;
          const mu2e::KalSegment &segmentFirst = kseed.segments().front();
          const mu2e::KalSegment &segmentLast = kseed.segments().back();
          double fltLMin=segmentFirst.fmin();
          double fltLMax=segmentLast.fmax();
          auto line = new REX::REveLine(names[j], names[j],nSegments); 
          
          for(unsigned int m=0; m<nSegments; m++){
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
              XYZVectorF pos;
              segment.helix().position(fltL,pos);
              CLHEP::Hep3Vector p =  GenVector::Hep3Vec(pos);
              line->SetNextPoint((p.x())/10, (p.y())/10 +100, (p.z())/10);
            }
          }
        line->SetLineColor(kBlack);
        line->SetLineWidth(5);
        scene->AddElement(line); 
    }
   }
  }
  
}

void REveMu2eDataInterface::AddCosmicTrackFit(REX::REveManager *&eveMng, bool firstLoop_, const mu2e::CosmicTrackSeedCollection *cosmiccol, REX::REveElement* &scene){
      std::cout<<"[REveMu2eDataInterface] AddCosmicTrackSeed "<<std::endl;
      if(cosmiccol!=0){
          auto line = new REX::REveLine("Cosmic","Cosmic",2); 
          if(!firstLoop_){
              scene->DestroyElements();;
          }
          for(unsigned int i=0; i< cosmiccol->size(); i++){
              mu2e::CosmicTrackSeed const  &sts= (*cosmiccol)[i];      
              mu2e::CosmicTrack st = sts._track;
              double ty1 = sts._straw_chits[0].pos().y();
              double ty2 = sts._straw_chits[sts._straw_chits.size()-1].pos().y();
              double tx1 = st.MinuitParams.A0  - st.MinuitParams.A1*ty1;
              double tx2 = st.MinuitParams.A0  - st.MinuitParams.A1*ty2;
              double tz1 = st.MinuitParams.B0  - st.MinuitParams.B1*ty1;
              double tz2 = st.MinuitParams.B0  - st.MinuitParams.B1*ty2; 	
              line->SetNextPoint((tx1)/10, (ty1)/10 + 100, (tz1)/10);
              line->SetNextPoint((tx2)/10, (ty2)/10 + 100, (tz2)/10);
          }
    
          line->SetLineColor(kGreen);
          line->SetLineWidth(5);
          scene->AddElement(line);
      }

}

