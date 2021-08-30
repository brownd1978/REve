#include "REve/inc/REveMainWindow.hh"

namespace REX = ROOT::Experimental;
using namespace std;
using namespace mu2e;


void REveMainWindow::makeEveGeoShape(TGeoNode* n, REX::REveTrans& trans, REX::REveElement* holder, int val)
 {

    auto gss = n->GetVolume()->GetShape();
    auto b1s = new REX::REveGeoShape(n->GetName());
    b1s->InitMainTrans();
    b1s->RefMainTrans().SetFrom(trans.Array());
    b1s->SetShape(gss);
    //b1s->SetMainColor(kGray-10);
    b1s->SetMainColor(kCyan);
    b1s->SetMainTransparency(100);
    holder->AddElement(b1s);
    if(val == 41612){
        mngRhoPhi->ImportElements(b1s, rPhiGeomScene); //shows only one plane for simplicity
    }
        mngRhoZ  ->ImportElements(b1s, rhoZGeomScene);
    
 }
int fp = 0;
int j = 0;
void REveMainWindow::showNodesByName(TGeoNode* n, const std::string& str, bool onOff, int _diagLevel, REX::REveTrans& trans,  REX::REveElement* holder, int maxlevel, int level, bool caloshift) {
    ++level;
    if (level > maxlevel){
       return;
    }
    std::string name(n->GetName());
    fp++;
j++;
    int ndau = n->GetNdaughters();
    for ( int i=0; i<ndau; ++i ){
        TGeoNode * pn = n->GetDaughter(i);
        
        if (name.find(str)!= std::string::npos and i==0 ){ //To make the geometry translucant we only add i==0

            //if( j==41612) {
            std::cout<<j<<" "<<name<<std::endl;
            
            //TGeoMaterial* material = n->GetVolume()->GetMaterial();
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
                    t(1,4) = tv[0]/10; t(2,4) = tv[1]/10 + 100; t(3,4) = tv[2]/10+2360/10; //dz = CaloCenter - TrackerCenter = 2360 mm
                } else {
                    t(1,4) = tv[0]/10; t(2,4) = tv[1]/10 + 100; t(3,4) = tv[2]/10;
                }
                ctrans *= t;
            }
            n->ls();
            makeEveGeoShape(n, ctrans, holder, j);
            //}
        }
       
       showNodesByName( pn, str, onOff, _diagLevel, trans,  holder, maxlevel, level, caloshift);
       }
    
  } 


 //Function to hide all elements which are not PS,TS, DS:
  void REveMainWindow::SolenoidsOnly(TGeoNode* node, REX::REveTrans& trans,  REX::REveElement* holder, int maxlevel, int level) {
    static std::vector <std::string> substrings  {"caloDisk"}; 
    for(auto& i: substrings){
     showNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level, true);
    }
    static std::vector <std::string> substringst  {  "TrackerPlaneEnvelope"};//"TrackerSupportServiceSectionEnvelope",
    for(auto& i: substringst){
     showNodesByName(node,i,kFALSE, 0, trans, holder, maxlevel, level, false);
    }
}


  void REveMainWindow::projectScenes(REX::REveManager *eveMng,bool geomp, bool eventp)
 {
    if (geomp)
    {
        
       for (auto &ie : eveMng->GetGlobalScene()->RefChildren())
       {
          //ie->SetMainColor(kCyan);
          ie->SetMainTransparency(100);
          mngRhoPhi->ImportElements(ie, rPhiGeomScene);
          mngRhoZ  ->ImportElements(ie, rhoZGeomScene);
       }
    }
    if (eventp)
    {
       for (auto &ie : eveMng->GetEventScene()->RefChildren())
       {
          
          mngRhoPhi->ImportElements(ie, rPhiEventScene);
          mngRhoZ  ->ImportElements(ie, rhoZEventScene);
       }
    }

 }

 void REveMainWindow::projectEvents(REX::REveManager *eveMng)
 {
    
       for (auto &ie : eveMng->GetEventScene()->RefChildren())
       {
          mngRhoPhi->ImportElements(ie, rPhiEventScene);
          mngRhoZ  ->ImportElements(ie, rhoZEventScene);
       }
    

 }

 void REveMainWindow::createProjectionStuff(REX::REveManager *eveMng)
 {

    // project RhoPhi
    rPhiGeomScene  = eveMng->SpawnNewScene("RPhi Geometry","RPhi");
    rPhiEventScene = eveMng->SpawnNewScene("RPhi Event Data","RPhi");
  
    mngRhoPhi = new REX::REveProjectionManager(REX::REveProjection::kPT_RPhi);
  
    rphiView = eveMng->SpawnNewViewer("RPhi View", "");
    rphiView->AddScene(rPhiGeomScene);
    rphiView->AddScene(rPhiEventScene);
  
    // ----------------------------------------------------------------
  
    rhoZGeomScene  = eveMng->SpawnNewScene("RhoZ Geometry", "RhoZ");
    rhoZEventScene = eveMng->SpawnNewScene("RhoZ Event Data","RhoZ");
  
    mngRhoZ = new REX::REveProjectionManager(REX::REveProjection::kPT_RhoZ);
  
    rhoZView = eveMng->SpawnNewViewer("RhoZ View", "");
    rhoZView->AddScene(rhoZGeomScene);
    rhoZView->AddScene(rhoZEventScene);
 }


 void REveMainWindow::showEvents(REX::REveManager *eveMng, REX::REveElement* &eventScene, bool firstLoop, DataCollections &data){
    if(data.clustercol->size() !=0) pass_data->AddCaloClusters(eveMng, firstLoop, data.clustercol, eventScene, mngRhoPhi, mngRhoZ, rPhiEventScene, rhoZEventScene );
    if(data.chcol->size() !=0)pass_data->AddComboHits(eveMng, firstLoop, data.chcol, eventScene, mngRhoPhi, mngRhoZ, rPhiEventScene, rhoZEventScene );
    std::vector<const KalSeedCollection*> track_list = std::get<1>(data.track_tuple);
    if(track_list.size() !=0) pass_data->AddKalSeedCollection(eveMng, firstLoop, data.track_tuple, eventScene, mngRhoPhi, mngRhoZ, rPhiEventScene, rhoZEventScene );
    //projectScenes(eveMng,true, true);
    projectEvents(eveMng);
 }

 void REveMainWindow::makeGeometryScene(REX::REveManager *eveMng)
 {

    TGeoManager *geom = TGeoManager::Import("REve/src/fix.gdml");

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
        SolenoidsOnly(topnode, trans, holder,8,0);
       
        
    }
    //projectScenes(eveMng,true, true);
   try {
     
	 eveMng->Show();
     
    } catch (...) {

       std::cout<< "Blocking window" << std::endl;
    }
   
 }
 







