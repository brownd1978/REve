#ifndef _DataCollections_hh
#define _DataCollections_hh

#include "Offline/RecoDataProducts/inc/CaloCluster.hh"
#include "Offline/RecoDataProducts/inc/ComboHit.hh"
#include "Offline/RecoDataProducts/inc/KalSeed.hh"
#include "Offline/RecoDataProducts/inc/CosmicTrackSeed.hh"
#include "Offline/MCDataProducts/inc/MCTrajectoryPoint.hh"
#include "Offline/MCDataProducts/inc/MCTrajectoryCollection.hh"
//Art/FCL:
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Run.h"
#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Sequence.h"
#include "fhiclcpp/types/Table.h"

#include <TObject.h>
#include <TROOT.h>
#include <TGComboBox.h>
#include <TGListBox.h>
#include <iostream>
#include <vector>
#include <tuple>
#include <string>

namespace mu2e{

	class DataCollections
	{
    public:
      explicit DataCollections(){};
      DataCollections(const DataCollections &){};
      DataCollections& operator=(const DataCollections &);

      //DataProducts:
      const mu2e::ComboHitCollection* chcol = 0;    
      const mu2e::CaloClusterCollection* clustercol = 0;
      const mu2e::KalSeedCollection* kalSeedcol = 0;
      const mu2e::CosmicTrackSeedCollection* CosmicTrackSeedcol = 0;
      const mu2e::MCTrajectoryCollection *mctrajcol = 0;
      std::vector<const mu2e::KalSeedCollection*> track_list;
      std::vector<const mu2e::MCTrajectoryCollection*> mctrack_list;
      //Input Tag Labels:
      std::vector<std::string> track_labels;
      std::vector<std::string> mctrack_labels;
      //Link Labels and Lists:
      std::tuple<std::vector<std::string>, std::vector<const mu2e::KalSeedCollection*>> track_tuple;
      std::tuple<std::vector<std::string>, std::vector<const mu2e::MCTrajectoryCollection*>> mctrack_tuple;
      virtual ~DataCollections(){};

	};
}
#endif 
