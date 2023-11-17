#include "REve/inc/REveMu2eTextSelect.hh"

using namespace mu2e;

/*REveMu2eTextSelect::REveMu2eTextSelect(){};

REveMu2eTextSelect::REveMu2eTextSelect(
    int runn,
    int eventn)
    : REveElement{"REveMu2eTextSelect"}, runN(runn), eventN(eventn)
  {};*/
  
  
int REveMu2eTextSelect::get(){
  return runN;
}

int REveMu2eTextSelect::setRun(int run){
        int N = run;
        std::cout<<"set run "<<N<<std::endl;
        return N;
      }

void REveMu2eTextSelect::set(int run, int event){
  int N = setRun(run);
  runN = N;
  std::cout<<" final N "<<N<<std::endl;
  std::cout<<" final N "<<runN<<std::endl;
}
   

  
