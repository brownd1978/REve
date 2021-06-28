#ifndef _EventDisplayManager_hh
#define _EventDisplayManager_hh

#include <ROOT/REveElement.hxx>
#include <ROOT/REveScene.hxx>
#include <condition_variable>
#include <limits>
#include <mutex>
#include <stdexcept>
#include <iostream>
namespace ROOT::Experimental {
  class REveManager;
}

namespace mu2e {

  constexpr auto invalid_event = std::numeric_limits<unsigned>::max();

  class EventDisplayManager : public ROOT::Experimental::REveElement {
    public:
      EventDisplayManager() = default; // ROOT needs a dictionary

      explicit EventDisplayManager(ROOT::Experimental::REveManager* eveMgr,
                                       std::condition_variable& cv,
                                       std::mutex& m);

      void NextEvent();
      void QuitRoot();

    private:
      ROOT::Experimental::REveManager* eveMng_{nullptr};
      std::condition_variable* cv_{nullptr};
      std::mutex* m_{nullptr};
      bool doneProcessingEvents_{false};
    };
}

#endif /* EventDisplayManager_h */
