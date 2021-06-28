#include "REve/inc/EventDisplayManager.hh"


namespace mu2e {

  EventDisplayManager::EventDisplayManager(
    ROOT::Experimental::REveManager* eveMgr,
    std::condition_variable& cv,
    std::mutex& m)
    : REveElement{"EventManager"}, eveMng_{eveMgr}, cv_{&cv}, m_{&m}
  {}

  void
  EventDisplayManager::NextEvent()
  {
    std::unique_lock lock{*m_};
    cv_->notify_all();
  }

  void
  EventDisplayManager::QuitRoot()
  {
    // This will wreak havoc if not thrown from the main thread.
    std::unique_lock<std::mutex> lock{*m_};
    cv_->notify_all();
    throw std::runtime_error("EventDisplay");
  }

}


