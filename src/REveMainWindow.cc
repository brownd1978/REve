#include "REve/inc/REveMainWindow.hh"

namespace REX = ROOT::Experimental;
using namespace std;
using namespace mu2e;

//TODO the following numbers should be placed in a Util somewhere and extracted from the GeometryService
double disk1_center = -49.4705;
double disk2_center = 20.95295;
double nCrystals = 674; 
double dz = 2360; //=CaloCenter-TrackerCenter= 2360 mm
double crvheight = 2*3083;//shift of 2 *  maximum height of crv module taken from crv_counters07
double detector_x = 3904; 

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
 
int fp = 0;
int j = 0;
void REveMainWindow::showNodesByName(TGeoNode* n, const std::string& str, bool onOff, int _diagLevel, REX::REveTrans& trans,  REX::REveElement* holder, int maxlevel, int level, bool caloshift, bool crystal, bool crvshift) {
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
                t(1,1) = rm[0]/10; t(1,2) = rm[1]/10; t(1,3) = rm[2]/10;
                t(2,1) = rm[3]/10; t(2,2) = rm[4]/10; t(2,3) = rm[5]/10;
                t(3,1) = rm[6]/10; t(3,2) = rm[7]/10; t(3,3) = rm[8]/10;
                
                if(caloshift){
                    fp++;
                    double d = 0;
                    if(fp < nCrystals) { d = disk1_center; }
                    else { d = disk2_center; }
                    if(crystal) { t(1,4) = tv[0]/10; t(2,4) = tv[1]/10 + 1000/10; t(3,4) = tv[2]/10 + dz/10 + d*10/10; } 
                    else { t(1,4) = tv[0]/10; t(2,4) = tv[1]/10 + 1000/10; t(3,4) = tv[2]/10 + dz/10; } 
                    if (fp < nCrystals and crystal) cry1 = true; 
                    if (fp >= nCrystals and crystal) cry2 = true; 
                } else if( !crvshift){
                    t(1,4) = tv[0]/10; t(2,4) = tv[1]/10 + 1000/10 ; t(3,4) = tv[2]/10;
                } else if (crvshift){
                    t(1,4) = tv[0]/10 + detector_x/10; t(2,4) = tv[1]/10 + 1000/10 + crvheight/10 ; t(3,4) = tv[2]/10; 
                }
                ctrans *= t;
            }
            n->ls();
            makeEveGeoShape(n, ctrans, holder, j, cry1, cry2);  
        }
       showNodesByName( pn, str, onOff, _diagLevel, trans,  holder, maxlevel, level, caloshift, crystal,crvshift);
       }
  } 

  /* function to hide all elements which are not PS,TS, DS */
  void REveMainWindow::SolenoidsOnly(TGeoNode* node, REX::REveTrans& trans,  REX::REveElement* holder, int maxlevel, int level, bool addCRV) {
    if(addCRV){
      static std::vector <std::string> substrings_crv  {"CRS"};  
        for(auto& i: substrings_crv){
          showNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level,  false, false, true); //TODO remove these arguements and make it easier
        }
    }
    static std::vector <std::string> substrings_disk  {"caloDisk"}; 
    for(auto& i: substrings_disk){
      showNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level, true, false, false);
    }
    static std::vector <std::string> substringst  {"TrackerPlaneEnvelope"};
    for(auto& i: substringst){
      showNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level, false, false, false);
    }
    static std::vector <std::string> substrings_crystal  {"caloCrystal"};  
    for(auto& i: substrings_crystal){
      showNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level, true, true, false);
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
  
    // --------------------Tracker + Calo XZ View ------------------------------
  
    rhoZGeomScene  = eveMng->SpawnNewScene("RhoZ Geometry", "RhoZ");
    rhoZEventScene = eveMng->SpawnNewScene("RhoZ Event Data","RhoZ");
  
    mngRhoZ = new REX::REveProjectionManager(REX::REveProjection::kPT_RhoZ );
  
    rhoZView = eveMng->SpawnNewViewer("RhoZ View", "");
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
 
 void REveMainWindow::makeGeometryScene(REX::REveManager *eveMng, bool addCRV, std::string gdmlname)
 {
    TGeoManager *geom = TGeoManager::Import(gdmlname.c_str()); 
    TGeoVolume* topvol = geom->GetTopVolume();
    gGeoManager->SetTopVolume(topvol);
    gGeoManager->SetTopVisible(kFALSE);
    TGeoNode* topnode = gGeoManager->GetTopNode(); 
    createProjectionStuff(eveMng);
    std::string name(topnode->GetName());
    {
        auto holder = new REX::REveElement("Inside DS");
        eveMng->GetGlobalScene()->AddElement(holder);
        REX::REveTrans trans;
        SolenoidsOnly(topnode, trans, holder,8,0, addCRV);
    }

    try {
      eveMng->Show();
    } catch (...) {
       std::cout<< "Blocking window" << std::endl;
    }
   
 }
