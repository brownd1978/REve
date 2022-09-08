#ifndef REveComboHit_HH_
#define REveComboHit_HH_

#include "Offline/RecoDataProducts/inc/ComboHit.hh"
#include "REve/inc/REveDataProduct.hh"
#include <string>

namespace mu2e {

  class REveComboHit : REveDataProduct {
   public:
    REveComboHit(std::string name) : _name(name) {};
    REveComboHit(){};
    std::string& name() { return _name; }

   private:
    std::string _name;

  };
}  // namespace mu2e

#endif
