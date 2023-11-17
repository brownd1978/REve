#include "REve/inc/EventDisplayManager.hh"


namespace mu2e {

  EventDisplayManager::EventDisplayManager(
    ROOT::Experimental::REveManager* eveMgr,
    std::condition_variable& cv,
    std::mutex& m,
    REveMu2eGUI *fGui,
    REveMu2eTextSelect *fText)
    : REveElement{"EventManager"}, eveMng_{eveMgr}, cv_{&cv}, m_{&m}, fGui_(fGui), fText_(fText)
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
    std::cout<<"Exit Signal 15, leaving REve Display "<<std::endl;
    exit(15);
  }
  void EventDisplayManager::autoplay(int  x)
   {
      std::cout << "EventManger autoplay() ....... " << x << std::endl;
   }
   void EventDisplayManager::goToRunEvent(int runId, int eventId)
   {
      std::cout<<" FEATURE STILL UNDER DEVELOPMENT "<<std::endl;
      //std::cout << "EventManger gotoEvent() ....... " << runId<<" "<<eventId << std::endl;
      //std::cout<<"setting"<<std::endl;
      //fText_->set(runId,eventId);
      //std::cout<<"finished setting"<<std::endl;
   }

}
