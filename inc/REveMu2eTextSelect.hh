#ifndef _REveMu2eTextSelect_hh
#define _REveMu2eTextSelect_hh

#include <ROOT/REveElement.hxx>

namespace REX = ROOT::Experimental;
using namespace ROOT::Experimental;

namespace mu2e {
class REveMu2eTextSelect : public ROOT::Experimental::REveElement
{
  public:
    /*REveMu2eTextSelect() = default; // ROOT needs a dictionary
    REveMu2eTextSelect(){};

    explicit REveMu2eTextSelect(
        int runn,
        int eventn)
        : REveElement{"REveMu2eTextSelect"}, runN(runn), eventN(eventn)
      {}*/
       void set(int run, int event);
       int get();
       int setRun(int run);
   private:
     int runN = 0;
     int eventN = 0;
     
};
}

#endif
