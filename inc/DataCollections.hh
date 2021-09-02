#ifndef _DataCollections_hh
#define _DataCollections_hh

#include "RecoDataProducts/inc/CaloCluster.hh"
#include "RecoDataProducts/inc/ComboHit.hh"
#include "RecoDataProducts/inc/KalSeed.hh"
#include "RecoDataProducts/inc/CosmicTrackSeed.hh"
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

      //RecoDataProducts:
      const mu2e::ComboHitCollection *chcol = 0;    
      const mu2e::CaloClusterCollection* clustercol = 0;
      const mu2e::KalSeedCollection* kalSeedcol = 0;
      const mu2e::CosmicTrackSeedCollection* CosmicTrackSeedcol = 0;
      std::vector<const mu2e::KalSeedCollection*> track_list;
      std::vector<std::string> track_labels;
      std::tuple<std::vector<std::string>, std::vector<const mu2e::KalSeedCollection*>> track_tuple;
      virtual ~DataCollections(){};

	};
}
#endif 
