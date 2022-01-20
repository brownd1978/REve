#include "REve/inc/REveMu2eMCInterface.hh"
using namespace mu2e;
namespace REX = ROOT::Experimental;

  int REveMu2eMCInterface::Contains(std::vector<int> v, int x)
  {
    return std::count(v.begin(), v.end(), x);
  }

  void REveMu2eMCInterface::AddMCTrajectoryCollection(REX::REveManager *&eveMng, bool firstloop,  std::tuple<std::vector<std::string>, std::vector<const MCTrajectoryCollection *>> mctrack_tuple, REX::REveElement* &scene, std::vector<int> particleIds){
    std::vector<const MCTrajectoryCollection*> track_list = std::get<1>(mctrack_tuple);
    std::vector<std::string> names = std::get<0>(mctrack_tuple);
    std::vector<int> colour;

    for(unsigned int j=0; j< track_list.size(); j++){
      const MCTrajectoryCollection* trajcol = track_list[j];
      colour.push_back(j+3);
      if(trajcol!=0){
          std::map<art::Ptr<mu2e::SimParticle>,mu2e::MCTrajectory>::const_iterator trajectoryIter;
          for(unsigned int k = 0; k < trajcol->size(); k++){ 

            for(trajectoryIter=trajcol->begin(); trajectoryIter!=trajcol->end(); trajectoryIter++)
            { 
              //check user defined list of particles to plot:
              int x = Contains(particleIds,trajectoryIter->first->pdgId()); 
              if(x == 1){
                const std::vector<MCTrajectoryPoint> &points = trajectoryIter->second.points();
                std::string energy = std::to_string(points[0].kineticEnergy());
                const std::string title = " MCTrajectory "+ energy + " Creation code = " + std::to_string(trajectoryIter->first->creationCode()) + "Stopping code = " + std::to_string(trajectoryIter->first->stoppingCode()) + " End Global Time = " + std::to_string(trajectoryIter->first->endGlobalTime())  ;
                auto line = new REX::REveLine(Form(title.c_str()), Form(title.c_str()),1); //TODO : add InputTag to name
                for(unsigned int i=0; i<points.size();i++){
                  CLHEP::Hep3Vector Pos(points[i].x(), points[i].y(), points[i].z());
                  GeomHandle<DetectorSystem> det;
                  CLHEP::Hep3Vector HitPos2D = det->toDetector(Pos);
                  line->SetNextPoint((Pos.x()),(Pos.y()),(Pos.z()));

                }

                line->SetLineColor(kRed);
                line->SetLineWidth(5);
                scene->AddElement(line); 
              } else std::cout<<"Warning: No Particles of User-Specified Type In File "<<std::endl;
            }
          }
        }
      }
     }



