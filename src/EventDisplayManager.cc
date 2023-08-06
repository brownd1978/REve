#include "REve/inc/EventDisplayManager.hh"


namespace mu2e {

  EventDisplayManager::EventDisplayManager(
    ROOT::Experimental::REveManager* eveMgr,
    std::condition_variable& cv,
    std::mutex& m,
    REveMu2eGUI *fGui)
    : REveElement{"EventManager"}, eveMng_{eveMgr}, cv_{&cv}, m_{&m}, fGui_(fGui)
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
    // FIXME This will wreak havoc if not thrown from the main thread.
    //std::unique_lock<std::mutex> lock{*m_};
    //cv_->notify_all();
    std::cout<<"Exit Signal 15, leaving REve Display "<<std::endl;
    exit(15);
    //pgid = getpgid();
    //std::kill(pgid, 15);
    //throw std::runtime_error("EventDisplay");
  }

}
