#ifndef REveCaloCluster_HH_
#define REveCaloCluster_HH_

#include "Offline/RecoDataProducts/inc/CaloCluster.hh"
#include "REve/inc/REveDataProduct.hh"
#include <string>

namespace mu2e {

  class REveCaloCluster : REveDataProduct {
   public:
    REveCaloCluster(std::string name) : _name(name) {};
    REveCaloCluster(){};
    std::string& name() { return _name; }

   private:
    std::string _name;

  };
}  // namespace mu2e

#endif
