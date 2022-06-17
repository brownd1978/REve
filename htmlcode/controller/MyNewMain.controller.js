sap.ui.define(['rootui5/eve7/controller/Main.controller',
               'rootui5/eve7/lib/EveManager',
               'rootui5/browser/controller/FileDialog.controller',
               "sap/ui/core/mvc/XMLView",
               'sap/ui/core/Fragment'
], function(MainController, EveManager, FileDialogController, XMLView, Fragment) {
   "use strict";

   return MainController.extend("custom.MyNewMain", {

      onWebsocketClosed : function() {
         var elem = this.byId("centerTitle");
         elem.setHtmlText("<strong style=\"color: red;\">Client Disconnected: Check code for errors !</strong>");
      },

      onInit: function() {
         MainController.prototype.onInit.apply(this, arguments);
         this.mgr.handle.setReceiver(this);
         this.mgr.RegisterController(this);
      },

      onEveManagerInit: function() {
         MainController.prototype.onEveManagerInit.apply(this, arguments);
         var world = this.mgr.childs[0].childs;

         world.forEach((item) => {
            if (item._typename == "mu2e::REveMu2eGUI") {
               this.fw2gui = item;
               var pthis = this;
               this.mgr.UT_refresh_event_info = function () {
                  pthis.showEventInfo();
                  
               }
               this.showEventInfo();
               return;
            }
         });
      },

      onWebsocketMsg : function(handle, msg, offset)
      {
         this.mgr.onWebsocketMsg(handle, msg, offset);
      },

      showEventInfo : function() {
         let tinfo = "Event : " + this.fw2gui.eventid + " Run :" + this.fw2gui.runid;
         document.title = tinfo;
         let infoLabel = this.getView().byId("infoLabel");
         console.log(infoLabel);
         infoLabel.setText(tinfo);
      },

      nextEvent : function(oEvent) {
          this.mgr.SendMIR("NextEvent()", this.fw2gui.fElementId, "EventDisplayManager");
      }
   });
});
