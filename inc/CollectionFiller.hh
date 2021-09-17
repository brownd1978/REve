#ifndef CollectionFiller_hh
#define CollectionFiller_hh

//Art:
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Run.h"
#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/Sequence.h"
#include "fhiclcpp/types/Table.h"

#include <TObject.h>
#include <TROOT.h>
#include <TGComboBox.h>
#include <TGListBox.h>

#include "REve/inc/DataCollections.hh"

namespace mu2e{

  enum RecoDataProductName {ComboHits, CaloClusters, KalSeeds, CosmicTrackSeeds};
  
	class CollectionFiller
	{
      public:
        struct Config{
          using Name=fhicl::Name;
          using Comment=fhicl::Comment;
          fhicl::Atom<int> diagLevel{Name("diagLevel"), Comment("for info"),0};
          fhicl::Atom<art::InputTag>chTag{Name("ComboHitCollection"),Comment("chTag"), "makePH"};
          fhicl::Atom<art::InputTag>cluTag{Name("CaloClusterCollection"),Comment("cluTag")};
          fhicl::Sequence<art::InputTag>kalSeedTag{Name("KalSeedCollection"),Comment("kalseedTag")};
          fhicl::Atom<art::InputTag>cosmicTrackSeedTag{Name("CosmicTrackSeedCollection"),Comment("cosmicTrackSeedTag")};
          fhicl::Atom<bool> addHits{Name("addHits"), Comment("set to add the hits"),false};
          fhicl::Atom<bool> addClusters{Name("addClusters"), Comment("set to add caloclusters"),false};
          fhicl::Atom<bool> addKalSeeds{Name("addKalSeeds"), Comment("set to add kalseeds"),false};
          fhicl::Atom<bool> addCosmicTrackSeeds{Name("addCosmicTrackSeeds"), Comment("set to add cosmic track seeds"),false};
          fhicl::Atom<bool> FillAll{Name("FillAll"), Comment("to see all available products"), false};
        };

        explicit CollectionFiller(const Config& conf);
        CollectionFiller(const CollectionFiller &);
        CollectionFiller& operator=(const CollectionFiller &);

        art::InputTag chTag_;
        art::InputTag cluTag_;
        std::vector<art::InputTag> kalSeedTag_;
        art::InputTag cosmicTrackSeedTag_;
        art::Event *_event;
        art::Run *_run;
        bool addHits_,  addClusters_, addKalSeeds_, addCosmicTrackSeeds_, FillAll_;
        void FillRecoCollections(const art::Event& evt, DataCollections &data, RecoDataProductName code);
        virtual ~CollectionFiller(){};

      private:
        Config _conf;
      
};

}

#endif 
