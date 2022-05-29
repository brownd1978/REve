#include "REve/inc/REveMainWindow.hh"

namespace REX = ROOT::Experimental;
using namespace std;
using namespace mu2e;

double disk1_center = 0;
double disk2_center = 0;
double nCrystals = 674; 
//TODO the following numbers should be placed in a Util somewhere and extracted from the GeometryService
double dz = 2360; //= CaloCenter-TrackerCenter = 2360 mm
double STz = 5871-1635.1;//stoppingTarget.z0InMu2e-tracker.mother.halfLength;//stoppingTarget.z0InMu2e from CD3_34foils
double crvheight = 2*3083;//shift of 2 *  maximum height of crv module taken from crv_counters07
double psts_x = 4818; //From GDML
double psts_y = 5917; //From the GDML
double psts_z = -5953; //From the GDML

void REveMainWindow::makeEveGeoShape(TGeoNode* n, REX::REveTrans& trans, REX::REveElement* holder, int val, bool crystal1, bool crystal2)
 {
    auto gss = n->GetVolume()->GetShape();
    auto b1s = new REX::REveGeoShape(n->GetName());
    b1s->InitMainTrans();
    b1s->RefMainTrans().SetFrom(trans.Array());
    b1s->SetShape(gss);
    b1s->SetMainColor(kCyan);
    b1s->SetMainTransparency(100);
    holder->AddElement(b1s);
    if(crystal1){ 
        mngXYCaloDisk1->ImportElements(b1s, XYCaloDisk1GeomScene);
    }if(crystal2){
        mngXYCaloDisk2->ImportElements(b1s, XYCaloDisk2GeomScene);
    }
    if(val == 41612){ //FIXME - hardcoded number
        mngTrackerXY->ImportElements(b1s, TrackerXYGeomScene); //shows only one plane for simplicity
    }
    mngRhoZ  ->ImportElements(b1s, rhoZGeomScene); 
 }
 
int j = 0;
int fp =0;

void REveMainWindow::showNodesByName(TGeoNode* n, const std::string& str, bool onOff, int _diagLevel, REX::REveTrans& trans,  REX::REveElement* holder, int maxlevel, int level, bool caloshift, bool crystal,  std::vector<double> shift, bool print) { //FIXME this function needs rewrting
    ++level;
    if (level > maxlevel){
       return;
    }
    std::string name(n->GetName());
    
    j++;
    bool cry1 = false; // these help us know which disk and to draw crystals
    bool cry2 = false;
    int ndau = n->GetNdaughters();
    for ( int i=0; i<ndau; ++i ){
        TGeoNode * pn = n->GetDaughter(i);
        if (name.find(str)!= std::string::npos and i==0 ){ //To make the geometry translucant we only add i==0
            //std::cout<<j<<" "<<name<<std::endl;
            REX::REveTrans ctrans;
            ctrans.SetFrom(trans.Array());
            {
                TGeoMatrix     *gm = n->GetMatrix();
                const Double_t *rm = gm->GetRotationMatrix();
                const Double_t *tv = gm->GetTranslation();
                REX::REveTrans t;
                t(1,1) = rm[0]; t(1,2) = rm[1]; t(1,3) = rm[2];
                t(2,1) = rm[3]; t(2,2) = rm[4]; t(2,3) = rm[5];
                t(3,1) = rm[6]; t(3,2) = rm[7]; t(3,3) = rm[8];
                t(1,4) = tv[0] + shift[0]; t(2,4) = tv[1]  + shift[1]; t(3,4) = tv[2] + shift[2];
                if(print) std::cout<<j<<" "<<name<<" "<<tv[0]<<" "<<tv[1]<<" "<<tv[2]<<std::endl;
                if(name == "caloDisk_00x3d71700") {
                  disk1_center = tv[2];
                 }
                if(name == "caloDisk_10x3e1ec70") {
                  disk2_center = tv[2];
                 }
                if(caloshift){
                    fp++;
                    double d = 0;
                    if(fp < nCrystals) { d = disk1_center; }
                    else { d = disk2_center; }
                    if(crystal) { t(1,4) = tv[0]; t(2,4) = tv[1] ; t(3,4) = tv[2] + dz/10 + d; } 
                    else { t(1,4) = tv[0]; t(2,4) = tv[1]; t(3,4) = tv[2] + dz/10; } 
                    if (fp < nCrystals and crystal) cry1 = true; 
                    if (fp >= nCrystals and crystal) cry2 = true; 
                } 
                ctrans *= t;
               }
            n->ls();
            makeEveGeoShape(n, ctrans, holder, j, cry1, cry2);  
        }
       showNodesByName( pn, str, onOff, _diagLevel, trans,  holder, maxlevel, level, caloshift, crystal,shift, print);
       }
    } 
  
  
  /* function to hide all elements which are not PS,TS, DS */
 void REveMainWindow::GeomDrawer(TGeoNode* node, REX::REveTrans& trans,  REX::REveElement* holder, int maxlevel, int level, bool addCRV, bool addPS, bool addTS, bool addDS){
    std::vector<double> shift;
    shift.push_back(0);
    shift.push_back(0);
    shift.push_back(0); 

    if(addTS){
      static std::vector <std::string> substrings_ts  {"TS"};  
      shift.at(0) = psts_x/10;  
      shift.at(1) = psts_y/10;
      shift.at(2) = psts_z/10;
      for(auto& i: substrings_ts){
        showNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level,  false, false, shift, true);
      }
    }
    if(addPS){
      static std::vector <std::string> substrings_ps  {"PSVacuum"};  
      shift.at(0) = psts_x/10;  
      shift.at(1) = psts_y/10;
      shift.at(2) = psts_z/10;
      for(auto& i: substrings_ps){
        showNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level,  false, false, shift, true);
      }
    }
    if(addDS){
      static std::vector <std::string> substrings_ds {"DS1Vacuum","DS2Vacuum","DS3Vacuum"}; 
      for(auto& i: substrings_ds){
        shift.at(0) = psts_x/10;  
        shift.at(1) = psts_y/10;
        shift.at(2) = psts_z/10;
        showNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level, false, false, shift, true);
      }
     }
    if(addCRV){
      static std::vector <std::string> substrings_crv  {"CRS"};
      shift.at(0) = 0;  
      shift.at(1) = crvheight; 
      shift.at(2) = 0;
      for(auto& i: substrings_crv){
        showNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level,  false, false, shift, false);
      }
    }
    static std::vector <std::string> substrings_disk  {"caloDisk"}; 
    for(auto& i: substrings_disk){
      shift.at(0) = 0;  
      shift.at(1) = 0;
      shift.at(2) = 0;
      showNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level, true, false, shift, true);
    }
    static std::vector <std::string> substring_tracker  {"TrackerPlaneEnvelope"};
    for(auto& i: substring_tracker){
      shift.at(0) = 0;  
      shift.at(1) = 0;
      shift.at(2) = 0;
      showNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level, false, false, shift, true);
    }
    static std::vector <std::string> substrings_stoppingtarget  {"StoppingTarget","Foil"};
    shift.at(0) = 0;  
    shift.at(1) = 0;
    shift.at(2) = -1*STz/10;
    for(auto& i: substrings_stoppingtarget){
      showNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level, false, false, shift, true);
    }
    static std::vector <std::string> substrings_crystals  {"caloCrystal"};  
    for(auto& i: substrings_crystals){
      shift.at(0) = 0;  
      shift.at(1) = 0;
      shift.at(2) = 0;
      showNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level, true, true, shift, false);
    }
}

  

 void REveMainWindow::projectEvents(REX::REveManager *eveMng)
 {
       for (auto &ie : eveMng->GetEventScene()->RefChildren())
       {
          mngTrackerXY->ImportElements(ie, TrackerXYEventScene);
          mngRhoZ  ->ImportElements(ie, rhoZEventScene);
          if(ie->GetName() == "disk1") mngXYCaloDisk1->ImportElements(ie, XYCaloDisk1EventScene);
          if(ie->GetName() == "disk2") mngXYCaloDisk2->ImportElements(ie, XYCaloDisk2EventScene);
       }
 }

 void REveMainWindow::createProjectionStuff(REX::REveManager *eveMng)
 {
    // -------------------Tracker XY View ----------------------------------
    TrackerXYGeomScene  = eveMng->SpawnNewScene("TrackerXY Geometry","TrackerXY");
    TrackerXYEventScene = eveMng->SpawnNewScene("TrackerXY Event Data","TrackerXY");
  
    mngTrackerXY = new REX::REveProjectionManager(REX::REveProjection::kPT_RPhi);
  
    TrackerXYView = eveMng->SpawnNewViewer("TrackerXY View", "");
    TrackerXYView->AddScene(TrackerXYGeomScene);
    TrackerXYView->AddScene(TrackerXYEventScene);
  
    // --------------------Tracker + Calo YZ View ------------------------------
  
    rhoZGeomScene  = eveMng->SpawnNewScene("ZY Detector Geometry", "ZY");
    rhoZEventScene = eveMng->SpawnNewScene("ZY Event Data","YZ");
  
    mngRhoZ = new REX::REveProjectionManager(REX::REveProjection::kPT_ZY );
  
    rhoZView = eveMng->SpawnNewViewer("ZY Detector View", "");
    rhoZView->AddScene(rhoZGeomScene);
    rhoZView->AddScene(rhoZEventScene);
    
     // ---------------------Calo Disk 1 XY View ----------------------------
    
    XYCaloDisk1GeomScene  = eveMng->SpawnNewScene("XYCaloDisk1 Geometry", "XYCaloDisk1");
    XYCaloDisk1EventScene = eveMng->SpawnNewScene("XYCaloDisk1 Event Data","XYCaloDisk1");
  
    mngXYCaloDisk1 = new REX::REveProjectionManager(REX::REveProjection::kPT_RPhi);
  
    XYCaloDisk1View = eveMng->SpawnNewViewer("XYCaloDisk1 View", "");
    XYCaloDisk1View->AddScene(XYCaloDisk1GeomScene);
    XYCaloDisk1View->AddScene(XYCaloDisk1EventScene);
    
    // -------------------- Calo Disk 2 XY View ------------------------
    
    XYCaloDisk2GeomScene  = eveMng->SpawnNewScene("XYCaloDisk2 Geometry", "XYCaloDisk2");
    XYCaloDisk2EventScene = eveMng->SpawnNewScene("XYCaloDisk2 Event Data","XYCaloDisk2");
  
    mngXYCaloDisk2 = new REX::REveProjectionManager(REX::REveProjection::kPT_RPhi);
  
    XYCaloDisk2View = eveMng->SpawnNewViewer("XYCaloDisk2 View", "");
    XYCaloDisk2View->AddScene(XYCaloDisk2GeomScene);
    XYCaloDisk2View->AddScene(XYCaloDisk2EventScene); 
 }


 void REveMainWindow::showEvents(REX::REveManager *eveMng, REX::REveElement* &eventScene, bool firstLoop, DataCollections &data, DrawOptions drawOpts, std::vector<int> particleIds, bool strawdisplay){
    if(!firstLoop){
      eventScene->DestroyElements();;
    }
    //...addReco:
    if(drawOpts.addComboHits) {
      std::vector<const ComboHitCollection*> combohit_list = std::get<1>(data.combohit_tuple);
      if(combohit_list.size() !=0 ) pass_data->AddComboHits(eveMng, firstLoop, data.combohit_tuple, eventScene, strawdisplay);
    }
    if(drawOpts.addCRVInfo){
       std::vector<const CrvRecoPulseCollection*> crvpulse_list = std::get<1>(data.crvpulse_tuple);
       if(crvpulse_list.size() !=0) pass_data->AddCRVInfo(eveMng, firstLoop, data.crvpulse_tuple, eventScene);
    }
    if(drawOpts.addClusters){
      std::vector<const CaloClusterCollection*> calocluster_list = std::get<1>(data.calocluster_tuple);
      if(calocluster_list.size() !=0 ) pass_data->AddCaloClusters(eveMng, firstLoop, data.calocluster_tuple, eventScene);
    }
    std::vector<const KalSeedCollection*> track_list = std::get<1>(data.track_tuple);
    if(drawOpts.addTracks and track_list.size() !=0) {
      pass_data->AddKalSeedCollection(eveMng, firstLoop, data.track_tuple, eventScene);
      if(drawOpts.addTrkHits) {
        std::vector<const ComboHitCollection*> combohit_list = std::get<1>(data.combohit_tuple);
        pass_data->AddTrkHits(eveMng, firstLoop, data.combohit_tuple,data.track_tuple, eventScene);
        }
    }
    if(drawOpts.addCosmicTracks){
      pass_data->AddCosmicTrackFit(eveMng, firstLoop, data.CosmicTrackSeedcol, eventScene);
    }
    if(drawOpts.addTimeClusters){
      std::vector<const TimeClusterCollection*> timecluster_list = std::get<1>(data.timecluster_tuple);
      if(timecluster_list.size() !=0) pass_data->AddTimeClusters(eveMng, firstLoop, data.timecluster_tuple, eventScene);
    }

    //... add MC:
    std::vector<const MCTrajectoryCollection*> mctrack_list = std::get<1>(data.mctrack_tuple);
    if(drawOpts.addMCTrajectories and mctrack_list.size() !=0){
      pass_mc->AddMCTrajectoryCollection(eveMng, firstLoop,  data.mctrack_tuple, eventScene, particleIds);
    } 
    
    // ... project these events onto 2D geometry:
    projectEvents(eveMng);
 }
 
 void REveMainWindow::makeGeometryScene(REX::REveManager *eveMng, bool addCRV, bool addPS, bool addTS, bool addDS, std::string gdmlname)
 {
    TGeoManager *geom = TGeoManager::Import(gdmlname.c_str()); 
    TGeoVolume* topvol = geom->GetTopVolume();
    gGeoManager->SetTopVolume(topvol);
    gGeoManager->SetTopVisible(kFALSE);
    TGeoNode* topnode = gGeoManager->GetTopNode(); 
    createProjectionStuff(eveMng);
    std::string name(topnode->GetName());
    {
        auto holder = new REX::REveElement("Mu2e World");
        eveMng->GetGlobalScene()->AddElement(holder);
        REX::REveTrans trans;
        GeomDrawer(topnode, trans, holder,8,0, addCRV, addPS, addTS, addDS);
    }

    try {
      eveMng->Show();
    } catch (...) {
       std::cout<< "Blocking window" << std::endl;
    }
   
 }
