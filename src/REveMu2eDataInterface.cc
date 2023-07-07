#include "REve/inc/REveMu2eDataInterface.hh"
using namespace mu2e;
namespace REX = ROOT::Experimental;

std::string drawfilename("REve/config/drawutils.txt");
SimpleConfig drawconfig(drawfilename);

void REveMu2eDataInterface::AddCaloClusters(REX::REveManager *&eveMng, bool firstLoop_, std::tuple<std::vector<std::string>, std::vector<const CaloClusterCollection*>> calocluster_tuple, REX::REveElement* &scene){

    std::cout<<"[REveMu2eDataInterface] AddCaloClusters "<<std::endl;
    std::vector<const CaloClusterCollection*> calocluster_list = std::get<1>(calocluster_tuple);
    std::vector<std::string> names = std::get<0>(calocluster_tuple);
    //std::cout<<"size of list in drawing "<<calocluster_list.size()<<std::endl;
    for(unsigned int j = 0; j< calocluster_list.size(); j++){
      // Extract cluster list
      const CaloClusterCollection* clustercol = calocluster_list[j];
      if(clustercol->size() != 0){
          if(!firstLoop_){
              scene->DestroyElements();;
          }
          mu2e::Calorimeter const &cal = *(mu2e::GeomHandle<mu2e::Calorimeter>());
          GeomHandle<DetectorSystem> det;
          for(unsigned int i = 0; i < clustercol->size(); i++){
            mu2e::CaloCluster const  &cluster= (*clustercol)[i];
            // Info for label:
            std::string cluster_energy = std::to_string(cluster.energyDep());
            std::string cluster_time = std::to_string(cluster.time());
            std::string cluster_x = std::to_string(cluster.cog3Vector().x());
            std::string cluster_y = std::to_string(cluster.cog3Vector().y());
            // Extract center of gravity, convert to coord sys
            CLHEP::Hep3Vector COG(cluster.cog3Vector().x(),cluster.cog3Vector().y(), cluster.cog3Vector().z());
            CLHEP::Hep3Vector crystalPos   = cal.geomUtil().mu2eToDiskFF(cluster.diskID(),COG);
            CLHEP::Hep3Vector pointInMu2e = det->toMu2e(crystalPos);

            // Info for label
            std::string cluster_z = std::to_string(abs(pointInMu2e.z()));
            // Make label and REve objects
            std::string label =  "Instance = " + names[0] +  '\n'
             + " Energy Dep. = "+cluster_energy+" MeV "+   '\n'
             + " Time = "+cluster_time+" ns " +  '\n'
             + " Pos =  ("+cluster_x+","+cluster_y+","+cluster_z+") mm";
            auto ps1 = new REX::REvePointSet("disk1", "CaloClusters Disk 1: "+label,0);
            auto ps2 = new REX::REvePointSet("disk2", "CaloClusters Disk 2: "+label,0);
            
            // Set positions
            if(cluster.diskID() == 0) ps1->SetNextPoint(pointmmTocm(COG.x()), pointmmTocm(COG.y()) , abs(pointmmTocm(pointInMu2e.z()))); 
            if(cluster.diskID() == 1) ps2->SetNextPoint(pointmmTocm(COG.x()), pointmmTocm(COG.y()) , abs(pointmmTocm(pointInMu2e.z()))); 
            // Set draw options 
            Color_t color = kRed;//TODO - this needs improving
            if(cluster_energy < 10)  color = kRed - 10; 
            if(cluster_energy >= 10 and cluster_energy < 20) color = kRed - 7 ; 
            if(cluster_energy >=20  and cluster_energy < 30) color = kRed - 5 ; 
            if(cluster_energy >=30  and cluster_energy < 40) color = kRed - 3; 
            if(cluster_energy >=40  and cluster_energy < 50) color = kRed - 1; 
            if(cluster_energy >= 50) color = kRed; 
            
            ps1->SetMarkerColor(color); 
            ps1->SetMarkerStyle(REveMu2eDataInterface::mstyle);
            ps1->SetMarkerSize(REveMu2eDataInterface::mstyle);

            ps2->SetMarkerColor(color);
            ps2->SetMarkerStyle(REveMu2eDataInterface::mstyle);
            ps2->SetMarkerSize(REveMu2eDataInterface::mstyle);

            // Add to REve world
            if(ps1->GetSize() !=0 ) scene->AddElement(ps1); 
            if(ps2->GetSize() !=0 ) scene->AddElement(ps2); 

          }
      }
    }
  }

void REveMu2eDataInterface::AddComboHits(REX::REveManager *&eveMng, bool firstLoop_, std::tuple<std::vector<std::string>, std::vector<const ComboHitCollection*>> combohit_tuple, REX::REveElement* &scene, bool strawdisplay){

  std::vector<const ComboHitCollection*> combohit_list = std::get<1>(combohit_tuple);
  std::vector<std::string> names = std::get<0>(combohit_tuple);
  
  // Loop over hit lists
  for(unsigned int j = 0; j < combohit_list.size(); j++){
    const ComboHitCollection* chcol = combohit_list[j];
    int colour = (j+3);
    if(chcol->size() !=0 ){
      // Loop over hits
      for(unsigned int i=0; i< chcol->size(); i++){
        mu2e::ComboHit const  &hit= (*chcol)[i];
        // Display hit straws if selected too in FCL file
        if(strawdisplay){
          mu2e::GeomHandle<mu2e::Tracker> tracker;
          const auto& allStraws = tracker->getStraws();
          int sid = hit._sid.asUint16();
          CLHEP::Hep3Vector sposi(0.0,0.0,0.0), sposf(0.0,0.0,0.0);
          const mu2e::Straw& s = allStraws[sid];
          const CLHEP::Hep3Vector& p = s.getMidPoint();
          const CLHEP::Hep3Vector& d = s.getDirection();
          double x = p.x();
          double y = p.y();
          double z = p.z();
          double l = s.halfLength();
          double st=sin(d.theta());
          double ct=cos(d.theta());
          double sp=sin(d.phi()+TMath::Pi()/2.0);
          double cp=cos(d.phi()+TMath::Pi()/2.0);
          if(sid < drawconfig.getInt("maxSID")){   
            double x1=x+l*st*sp;
            double y1=y-l*st*cp;
            double z1=z+l*ct;
            double x2=x-l*st*sp;
            double y2=y+l*st*cp;
            double z2=z-l*ct;
            std::string strawtitle;
            int idStraw =  s.id().getStraw();
            int idPanel =  s.id().getPanel();
            int idPlane =  s.id().getPlane();
            int colorid = idPlane + idPanel;
            strawtitle =Form("Straw %i Panel %i  Plane %i",idStraw,idPanel,idPlane);
            sposi.set(x1, y1, z1);
            sposf.set(x2, y2, z2);
            if(sposi.x()!=0){ 
              auto strawline = new REX::REveLine("StrawHit ",strawtitle,1); 
              strawline->SetPoint(0,pointmmTocm(sposi.x()),pointmmTocm(sposi.y()),pointmmTocm(sposi.z()));
              strawline->SetNextPoint(pointmmTocm(sposf.x()),pointmmTocm(sposf.y()),pointmmTocm(sposf.z()));            
              strawline->SetLineWidth(1);
              strawline->SetLineColor(colorid);
              if(strawline->GetSize() !=0 ) scene->AddElement(strawline);  
            }
          }
        }
        bool AddErrorBar_ = true; //TODO turn off when mixed
        if(AddErrorBar_){
          //XY
          auto error = new REX::REveLine("errors","error title",1); 
          auto const& p = hit.pos();
          auto const& w = hit.wdir();
          auto const& s = hit.wireRes();
          double x1 = (p.x()+s*w.x());
          double x2 = (p.x()-s*w.x());
          double z1 = (p.z()+s*w.z());
          double z2 = (p.z()-s*w.z());
          double y1 = (p.y()+s*w.y());
          double y2 = (p.y()-s*w.y());

          std::string errorbar = "ErrorBar Length: %d, %d, %d";
          
          error->SetPoint(0, pointmmTocm(x1),pointmmTocm(y1),pointmmTocm(z1));
          error->SetNextPoint(pointmmTocm(x2),pointmmTocm(y2),pointmmTocm(z2));
          
          error->SetLineColor(colour);
          scene->AddElement(error);
          //TODO - Z - drift cricle, might have to be series of lines in a circle (?)
        }
        CLHEP::Hep3Vector HitPos(pointmmTocm(hit.pos().x()), pointmmTocm(hit.pos().y()), pointmmTocm(hit.pos().z()));
        std::string chtitle = "ComboHits tag = "
        + (names[j])  +  '\n'
        + " position : x "  + std::to_string(hit.pos().x())  +  '\n'
        + " y " + std::to_string(hit.pos().y())  +  '\n'
        + " z " + std::to_string(hit.pos().z())  +  '\n'
        + " time :" + std::to_string(hit.time()) +  '\n'
        + " energy dep : "
        + std::to_string(hit.energyDep())  + 
        + "MeV";
        auto ps1 = new REX::REvePointSet("ComboHit", chtitle,0); 
        ps1->SetNextPoint(HitPos.x(), HitPos.y() , HitPos.z()); 
        ps1->SetMarkerColor(colour);
        ps1->SetMarkerStyle(REveMu2eDataInterface::mstyle);
        ps1->SetMarkerSize(REveMu2eDataInterface::msize);
        if(ps1->GetSize() !=0 ) scene->AddElement(ps1);  
      }
    }
  }
}


/*------------Function to add CRV information to the display:-------------*/
  void REveMu2eDataInterface::AddCRVInfo(REX::REveManager *&eveMng, bool firstLoop_, std::tuple<std::vector<std::string>, std::vector<const CrvRecoPulseCollection*>>  crvpulse_tuple, REX::REveElement* &scene){
    
    std::vector<const CrvRecoPulseCollection*> crvpulse_list = std::get<1>(crvpulse_tuple);
    std::vector<std::string> names = std::get<0>(crvpulse_tuple); 
    GeomHandle<CosmicRayShield> CRS;
    GeomHandle<DetectorSystem> det;
    if(crvpulse_list.size() !=0){
      for(unsigned int i=0; i < crvpulse_list.size(); i++){
      const CrvRecoPulseCollection* crvRecoPulse = crvpulse_list[i];
      if(crvRecoPulse->size() !=0){
        std::string crvtitle = " tag : " + names[i];
        auto ps1 = new REX::REvePointSet("CRVRecoPulse", crvtitle,0);
        for(unsigned int j=0; j< crvRecoPulse->size(); j++){
          mu2e::CrvRecoPulse const &crvpulse = (*crvRecoPulse)[j];
          const CRSScintillatorBarIndex &crvBarIndex = crvpulse.GetScintillatorBarIndex();
          const CRSScintillatorBar &crvCounter = CRS->getBar(crvBarIndex);
          const CRSScintillatorBarDetail &barDetail = crvCounter.getBarDetail();
          CLHEP::Hep3Vector crvCounterPos = crvCounter.getPosition();
          CLHEP::Hep3Vector HitPos(crvCounterPos.x(), crvCounterPos.y(), crvCounterPos.z());
          CLHEP::Hep3Vector pointInMu2e = det-> toDetector(crvCounterPos);
          CLHEP::Hep3Vector sibardetails(barDetail.getHalfLengths()[0],barDetail.getHalfLengths()[1],barDetail.getHalfLengths()[2]);
          CLHEP::Hep3Vector sposi(0.0,0.0,0.0), sposf(0.0,0.0,0.0);
          sposi.set(pointInMu2e.x()-sibardetails.x(), pointInMu2e.y()-sibardetails.y(), pointInMu2e.z()-sibardetails.z());
          sposf.set(pointInMu2e.x()+sibardetails.x(), pointInMu2e.y()+sibardetails.y(), pointInMu2e.z()+sibardetails.z());
          // CRV hit scintillation bars highlighted
          std::string bartitle = " bar : ";// + std::to_string(crvBarIndex):
          auto scibar = new REX::REveLine("scintillationBar",bartitle,1); 
          scibar->SetPoint(0,pointmmTocm(sposi.x()),pointmmTocm(sposi.y()),pointmmTocm(sposi.z()));
          scibar->SetNextPoint(pointmmTocm(sposf.x()),pointmmTocm(sposf.y()),pointmmTocm(sposf.z()));            
          scibar->SetLineWidth(1);
          scibar->SetLineColor(i+3);
          if(scibar->GetSize() !=0 ) scene->AddElement(scibar);
          // CRV hits
          ps1->SetNextPoint(pointmmTocm(pointInMu2e.x()), pointmmTocm(pointInMu2e.y()) , pointmmTocm(pointInMu2e.z()));
        }
        
        ps1->SetMarkerColor(i+3);
        ps1->SetMarkerStyle(REveMu2eDataInterface::mstyle);
        ps1->SetMarkerSize(REveMu2eDataInterface::msize);
        if(ps1->GetSize() !=0 ) scene->AddElement(ps1); 
      }
      }
    }
    
  }


/*------------Function to add TimeCluster Collection in 3D and 2D displays:-------------*/
  void REveMu2eDataInterface::AddTimeClusters(REX::REveManager *&eveMng, bool firstLoop_, std::tuple<std::vector<std::string>, std::vector<const TimeClusterCollection*>>  timecluster_tuple, REX::REveElement* &scene){
  
    std::vector<const TimeClusterCollection*> timecluster_list = std::get<1>(timecluster_tuple);
    std::vector<std::string> names = std::get<0>(timecluster_tuple);
    
    if(timecluster_list.size() !=0){
      for(unsigned int i = 0; i < timecluster_list.size(); i++){
        const TimeClusterCollection* tccol = timecluster_list[i];
        if(tccol->size() != 0){    
          if(!firstLoop_){
            scene->DestroyElements();;
          }
          std::string tctitle = names[i];
          auto ps1 = new REX::REvePointSet("TimeClusters", tctitle, 0); 
          for(size_t j=0; j<tccol->size();j++){
            mu2e::TimeCluster const  &tclust= (*tccol)[j];
            CLHEP::Hep3Vector HitPos(tclust._pos.x(), tclust._pos.y(), tclust._pos.z());
            ps1->SetNextPoint(pointmmTocm(HitPos.x()), pointmmTocm(HitPos.y()) , pointmmTocm(HitPos.z())); 
          }
          ps1->SetMarkerColor(i+6);
          ps1->SetMarkerStyle(kOpenCircle);
          ps1->SetMarkerSize(REveMu2eDataInterface::msize);
          if(ps1->GetSize() !=0 ) scene->AddElement(ps1); 
        }
      }
    }
  }

/*------------Function to color code the Tracker hits -------------*/
void REveMu2eDataInterface::AddTrkHits(REX::REveManager *&eveMng, bool firstLoop_, std::tuple<std::vector<std::string>, std::vector<const ComboHitCollection*>> combohit_tuple,std::tuple<std::vector<std::string>, std::vector<const KalSeedCollection*>> track_tuple, REX::REveElement* &scene){
  std::cout<<"[REveMu2eDataInterface] AddTrkHits  "<<std::endl;
  std::vector<const ComboHitCollection*> combohit_list = std::get<1>(combohit_tuple);
  std::vector<const KalSeedCollection*> track_list = std::get<1>(track_tuple);
    
  GeomHandle<DetectorSystem> det;
  StrawId trksid[drawconfig.getInt("maxStrawID")]; 
  unsigned int trkhitsize=0;
  //Save the hit straw IDs of the KalSeed hits  
  for(unsigned int j = 0; j< track_list.size(); j++){
    const KalSeedCollection* seedcol = track_list[j];
    if(seedcol!=0){
      for(unsigned int k = 0; k < seedcol->size(); k++){
        KalSeed kseed = (*seedcol)[k];
        const std::vector<mu2e::TrkStrawHitSeed> &hits = kseed.hits();
        trkhitsize = hits.size();
        for(unsigned int i = 0; i <trkhitsize; i++){
          const mu2e::TrkStrawHitSeed &hit = hits.at(i);
          trksid[i] = hit._sid; 
        }
        StrawId usedtrksid[trkhitsize];
        unsigned int usedid[trkhitsize];
        //Compare the straw IDs of the Kal seed hits with the hits in the ComboHit Collection
        for(unsigned int j=0; j< combohit_list.size(); j++){
          const ComboHitCollection* chcol = combohit_list[j];
          if(chcol!=0){
            for(unsigned int i=0; i<chcol->size();i++){
              ComboHit hit = (*chcol)[i];
              for(unsigned int q=0; q<trkhitsize; q++){
                if(hit._sid == trksid[q]){
                  usedtrksid[q]=hit._sid;//Save the Straw ID if the KalSeed and Combo hit ID matches
                  usedid[q]=q;
                  CLHEP::Hep3Vector HitPos(hit.pos().x(), hit.pos().y(), hit.pos().z());
                  auto trkhit = new REX::REvePointSet("TrkHits", "trkhit",0); 
                  trkhit ->SetMarkerStyle(REveMu2eDataInterface::mstyle);
                  trkhit ->SetMarkerSize(REveMu2eDataInterface::msize);
                  // trkhit ->SetMarkerColor(drawconfig.getInt("RecoTrackColor")-4); //TODO
                  trkhit ->SetMarkerColor(kRed); //TODO
                  trkhit ->SetNextPoint(pointmmTocm(HitPos.x()),pointmmTocm(HitPos.y()) ,pointmmTocm(HitPos.z()));
                  if(trkhit->GetSize() !=0 ) scene->AddElement(trkhit); 
                  // std::cout<<"TrkHit = "<<HitPos.x()<<"  "<<HitPos.y()<<" "<<HitPos.z()<<std::endl;
                }
              }
            }
            for(unsigned int i = 0;i < trkhitsize;i++){ 
              if(i != usedid[i] and i<chcol->size()){
                ComboHit chhit = (*chcol)[i];
                CLHEP::Hep3Vector HitPos(chhit.pos().x(), chhit.pos().y(), chhit.pos().z());
                auto notusedtrkhit = new REX::REvePointSet("NotTrkHits", "nottrkhit",0); 
                notusedtrkhit ->SetMarkerStyle(REveMu2eDataInterface::mstyle);
                notusedtrkhit ->SetMarkerSize(REveMu2eDataInterface::msize);
                notusedtrkhit ->SetNextPoint(pointmmTocm(HitPos.x()),pointmmTocm(HitPos.y()) ,pointmmTocm(HitPos.z()));
                notusedtrkhit ->SetMarkerColor(kRed);
                if(notusedtrkhit->GetSize() !=0 ) scene->AddElement(notusedtrkhit); 
              }
            }
          }
        }
      }
    }
  }
}

using LHPT = KalSeed::LHPT;
using CHPT = KalSeed::CHPT;
using KLPT = KalSeed::KLPT;
template<class KTRAJ> void REveMu2eDataInterface::AddKinKalTrajectory( std::unique_ptr<KTRAJ> &trajectory, REX::REveLine* line){
  double t1=trajectory->range().begin();
  double t2=trajectory->range().end();

  double x1=trajectory->position3(t1).x();
  double y1=trajectory->position3(t1).y();
  double z1=trajectory->position3(t1).z();
  
  line->SetPoint(0,pointmmTocm(x1), pointmmTocm(y1) , pointmmTocm(z1));
  for(double t=t1; t<=t2; t+=0.1)
  {
    const auto &p = trajectory->position3(t);
    double xt=p.x();
    double yt=p.y();
    double zt=p.z();
    line->SetNextPoint(pointmmTocm(xt), pointmmTocm(yt) , pointmmTocm(zt));
  }
}

void REveMu2eDataInterface::FillKinKalTrajectory(REX::REveManager *&eveMng, bool firstloop, REX::REveElement* &scene, std::tuple<std::vector<std::string>, std::vector<const KalSeedCollection*>> track_tuple){

  std::vector<const KalSeedCollection*> track_list = std::get<1>(track_tuple);
  std::vector<std::string> names = std::get<0>(track_tuple);

  for(unsigned int j=0; j< track_list.size(); j++){
    const KalSeedCollection* seedcol = track_list[j];
    if(seedcol!=0){  
     for(unsigned int k = 0; k < seedcol->size(); k = k + 20){ 
        mu2e::KalSeed kseed = (*seedcol)[k];
        auto line = new REX::REveLine(names[j], names[j], 100);
        if(kseed.loopHelixFit())
        {
          auto trajectory=kseed.loopHelixFitTrajectory();
          AddKinKalTrajectory<LHPT>(trajectory,line);
        }
        else if(kseed.centralHelixFit())
        {
          auto trajectory=kseed.centralHelixFitTrajectory();
          AddKinKalTrajectory<CHPT>(trajectory,line);
        }
        else if(kseed.kinematicLineFit())
        {
          auto trajectory=kseed.kinematicLineFitTrajectory();
          AddKinKalTrajectory<KLPT>(trajectory,line);
        }
        line->SetLineColor(j+6);
        line->SetLineWidth(drawconfig.getInt("TrackLineWidth"));
        scene->AddElement(line); 
      }
    }
  }
}

void REveMu2eDataInterface::AddHelixSeedCollection(REX::REveManager *&eveMng,bool firstloop,  std::tuple<std::vector<std::string>, std::vector<const HelixSeedCollection*>> helix_tuple, REX::REveElement* &scene){
    std::cout<<"[REveMu2eDataInterface] AddHelices "<<std::endl;
    std::vector<const HelixSeedCollection*> helix_list = std::get<1>(helix_tuple);
    std::vector<std::string> names = std::get<0>(helix_tuple);
    for(unsigned int j=0; j< helix_list.size(); j++){
      const HelixSeedCollection* seedcol = helix_list[j];
      if(seedcol!=0){  
          for(unsigned int k = 0; k < seedcol->size(); k++){
          mu2e::HelixSeed hseed = (*seedcol)[k];
          const ComboHitCollection& hhits = hseed.hits();
          unsigned int nhhits = hhits.size();
          auto line = new REX::REveLine(names[j], names[j],nhhits);
          // std::cout<<"Helix hits = "<<nhhits<<" radius = "<<hseed.helix()._radius<<" x0 = "<<hseed.helix()._rcent*cos(hseed.helix()._fcent)<<std::endl;
          float helrad = hseed.helix()._radius;
          float circphi = 0.0;
          float xc = hseed.helix()._rcent*cos(hseed.helix()._fcent);
          float yc = hseed.helix()._rcent*sin(hseed.helix()._fcent);
          if(hhits.size() !=0 ){
              for(int i=-1500; i < 1500; i +=100){ //TODO Remove hard-coded numbers
              float z = i;
              if(hseed.helix()._lambda !=0.0) circphi = hseed.helix()._fz0 + z/hseed.helix()._lambda;
	          float x= xc + helrad*cos(circphi);
              float y = yc + helrad*sin(circphi);
	          //std::cout<<"x = "<<x<<" y = "<<y<<" z = "<<z<<std::endl;
	          CLHEP::Hep3Vector HelPos(x, y, z);
              if(circphi !=0.0)line->SetNextPoint(pointmmTocm(HelPos.x()),pointmmTocm(HelPos.y()) ,pointmmTocm(HelPos.z()));                  
	    } 
	  }
      line->SetLineColor(kBlue);
      line->SetLineWidth(drawconfig.getInt("TrackLineWidth"));
      scene->AddElement(line); 
      }
    }
  }   
}

void REveMu2eDataInterface::AddKalSeedCollection(REX::REveManager *&eveMng,bool firstloop,  std::tuple<std::vector<std::string>, std::vector<const KalSeedCollection*>> track_tuple, REX::REveElement* &scene){

    std::vector<const KalSeedCollection*> track_list = std::get<1>(track_tuple);
    std::vector<std::string> names = std::get<0>(track_tuple);
    
    for(unsigned int j=0; j< track_list.size(); j++){
      const KalSeedCollection* seedcol = track_list[j];
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
          
          for(unsigned int m = 0; m<nSegments; m++){
            
            const mu2e::KalSegment &segment = segments.at(m);
            fltLMin=segment.fmin();
            fltLMax=segment.fmax();
            std::string kaltitle = " tag : " + names[j] ;
            
            if(kseed.loopHelixFit()){
              kaltitle = "KalSeed tag : " + names[j] +  '\n'
              + " mom " + std::to_string(segment.mom()) +  '\n'
              + " MeV/c t0 " + std::to_string(segment.loopHelix().t0()) +  '\n'
              + " ns tandip " + std::to_string(segment.loopHelix().tanDip() ) +  '\n'
              + " lam "  + std::to_string(segment.loopHelix().lam() ) +  '\n'
              + " rad "  + std::to_string(segment.loopHelix().rad() ) +  '\n'
              + " cx "  + std::to_string(segment.loopHelix().cx() ) +  '\n'
              + " cy "  + std::to_string(segment.loopHelix().cy() ) +  '\n'
              + " phi0 "  + std::to_string(segment.loopHelix().phi0() );
            }
            if(kseed.centralHelixFit() ){
              kaltitle = "KalSeed tag : " + names[j] +  '\n'
              + " mom " + std::to_string(segment.mom()) +  '\n'
              + " MeV/c tandip " + std::to_string(segment.centralHelix().tanDip() ) +  '\n'
              + " d0 " + std::to_string(segment.centralHelix().d0() ) +  '\n'
              + " z0 " + std::to_string(segment.centralHelix().z0() ) +  '\n'
              + " phi0 " + std::to_string(segment.centralHelix().phi0() ) +  '\n'
              + " omega " + std::to_string(segment.centralHelix().omega() );
            }
            if(kseed.kinematicLineFit() ){
              kaltitle = "KalSeed tag : " + names[j]
              + " mom " + std::to_string(segment.mom());
            }
            auto line = new REX::REveLine(kaltitle,kaltitle,1);
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
              line->SetNextPoint(pointmmTocm(p.x()), pointmmTocm(p.y()) , pointmmTocm(p.z()));
            }
            line->SetLineColor(j+3);
            line->SetLineWidth(drawconfig.getInt("TrackLineWidth"));
            scene->AddElement(line); 
          }
        
    }
   }
  }
}

void REveMu2eDataInterface::AddCosmicTrackFit(REX::REveManager *&eveMng, bool firstLoop_, const mu2e::CosmicTrackSeedCollection *cosmiccol, REX::REveElement* &scene){

      std::cout<<"[REveMu2eDataInterface] AddCosmicTrackSeed "<<std::endl;
      if(cosmiccol!=0){
          auto line = new REX::REveLine("Cosmic","Cosmic",2);  
          for(unsigned int i=0; i< cosmiccol->size(); i++){
              mu2e::CosmicTrackSeed const  &sts= (*cosmiccol)[i];      
              mu2e::CosmicTrack st = sts._track;
              double ty1 = sts._straw_chits[0].pos().y();
              double ty2 = sts._straw_chits[sts._straw_chits.size()-1].pos().y();
              double tx1 = st.MinuitParams.A0  - st.MinuitParams.A1*ty1;
              double tx2 = st.MinuitParams.A0  - st.MinuitParams.A1*ty2;
              double tz1 = st.MinuitParams.B0  - st.MinuitParams.B1*ty1;
              double tz2 = st.MinuitParams.B0  - st.MinuitParams.B1*ty2; 	
              line->SetNextPoint(pointmmTocm(tx1), pointmmTocm(ty1) , pointmmTocm(tz1));
              line->SetNextPoint(pointmmTocm(tx2), pointmmTocm(ty2) , pointmmTocm(tz2));
          }
    
          line->SetLineColor(drawconfig.getInt("RecoTrackColor"));
          line->SetLineWidth(drawconfig.getInt("TrackLineWidth"));
          scene->AddElement(line);
      }

}

