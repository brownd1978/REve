#ifndef _EventDisplayManager_hh
#define _EventDisplayManager_hh

#include <ROOT/REveElement.hxx>
#include <ROOT/REveScene.hxx>
#include <condition_variable>
#include <limits>
#include <mutex>
#include <stdexcept>
#include <iostream>
#include "REve/inc/REveMu2eGUI.hh"
#include "nlohmann/json.hpp"

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
                                       std::mutex& m,
                                       REveMu2eGUI *fGui);

    void NextEvent();
    void QuitRoot();

    //void InitGuiInfo();

    private:
      ROOT::Experimental::REveManager* eveMng_{nullptr};
      std::condition_variable* cv_{nullptr};
      std::mutex* m_{nullptr};
      bool doneProcessingEvents_{false};
      REveMu2eGUI *fGui_{nullptr};
    };
}

#endif /* EventDisplayManager_h */
