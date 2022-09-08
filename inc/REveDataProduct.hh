#ifndef REveDataProduct_HH_
#define REveDataProduct_HH_

#include <string>

namespace mu2e {

  class REveDataProduct {
   public:
    REveDataProduct(std::string name) : _name(name) {};
    REveDataProduct(){};
    std::string& name() { return _name; }

   private:
    std::string _name;

  };
}  // namespace mu2e

#endif
