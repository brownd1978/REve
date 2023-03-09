sap.ui.define([
    "sap/ui/core/mvc/Controller",
    "sap/ui/model/json/JSONModel",
    "sap/m/Button",
    "sap/m/Input",
    "sap/m/StepInput",
    "sap/m/CheckBox",
    "sap/m/Text",
    "sap/m/ColorPalettePopover",
    "sap/ui/layout/HorizontalLayout"
 ], function (Controller, JSONModel, Button, mInput, mStepInput, mCheckBox, mText, ColorPalettePopover, HorizontalLayout) {
    "use strict";
 


   var EventFilterController = Controller.extend("custom.controller.EventFilter", {

      onInit: function() {
          console.log("filter controller");
      },

      setGUIElement: function(gui) {
          console.log("Event Filter FW2GUI ", gui);
          this.fw2gui = gui;


          let aData = [
            { id: Math.random(), name: "$Muons.@size > 4", checked: true, rating: "5", type: "Inactive" },
         ];


         var oModel = new sap.ui.model.json.JSONModel();
         oModel.setData({ modelData: aData});
         this.byId("filterDialog").setModel(oModel);

       },

       openFilterDialog: function () {
           console.log("open filter dialog");
           this.byId("filterDialog").open();
       },

       makeTables: function () {
           this.makePlainTable();

           let dialog = this.byId("filterDialog");

           // Simple RadioButtonGroup
           var oRBGroupRBG1 = new sap.m.RadioButtonGroup("RBG1");
           oRBGroupRBG1.setTooltip("Group 1");
           oRBGroupRBG1.setColumns(2);
           oRBGroupRBG1.attachSelect(this.handleModeSelect);

           var oButton = new sap.m.RadioButton("RB1-1");
           oButton.setText("AND");
           oButton.setTooltip("Tooltip 1");
           oRBGroupRBG1.addButton(oButton);

           oButton = new sap.m.RadioButton("RB1-2");
           oButton.setText("OR");
           oButton.setTooltip("Tooltip 2");
           oRBGroupRBG1.addButton(oButton);

           let pthis = this;
           let bar = new sap.m.Bar({
            contentLeft: [
               new sap.m.Label({
                  text: 'Enabled:'
               }),
               new sap.m.CheckBox({ selected : false,  select : function(e) {pthis.setFilterEnabled(e);}}),
               new sap.m.Label({
                  text: 'Mode:'
               }),
               oRBGroupRBG1
                 ]
             });
           this.byId("filterDialog").setSubHeader(bar);

           let beginButton = new sap.m.Button('simpleDialogAcceptButton', { text: "Apply", press: function () { this.publishFilters(); } });
           let endButton = new sap.m.Button('simpleDialogCancelButton', { text: "Cancel", press: function () { this.filterDialog.close(); } });
           dialog.setEndButton(endButton);
           dialog.setBeginButton(beginButton);
       },
       makePlainTable: function () {
           var aColumns = [
               new sap.m.Column({
                   header: new sap.m.Label({
                       text: "Expression"
                   })
               }),
               new sap.m.Column({
                   hAlign: "Center",
                   header: new sap.m.Label({
                       text: "Active"
                   })
               }),
               new sap.m.Column({
                   header: new sap.m.Label({
                       text: "Pass"
                   })
               })
           ];

           var oTemplate = new sap.m.ColumnListItem({
               vAlign: "Middle",
               type: "{type}",
               cells: [
                   new sap.m.Input({
                       value: "{name}",
                       wrapping: false
                   }),
                   new sap.m.CheckBox({
                       selected: "{checked}"
                   }),
                   new sap.m.Label({
                       text: "{rating}"
                   })
               ]
           });

           oTable.bindItems({
               path: "/modelData",
               template: oTemplate,
               key: "id"
           });


           var oAddButton = new sap.m.Button({
               icon: "sap-icon://sys-add",
               press: function (oEvent) {
                   let nv = { id: Math.random(), name: "", checked: true, rating: 0, type: "Inactive" }
                   
                   var aData = this.getModel().getProperty("/modelData");
                   aData.push(nv);
                   this.getModel().setProperty("/modelData", aData);
               }
           });

           this.byId("plainPanel").addContent(oTable);
           this.byId("plainPanel").addContent(oAddButton);
        },

       publishFilters: function () {
           console.log("publish Filters");

           // let fd = this.filterDialog.getModel().getData();
           //   console.log("FILTER PUBLISHED ", fd);
           //  let cont = JSON.stringify(fd);

           let to = { a: "A" };
           let cont = "\"" + JSON.stringify(to) + "\"";
           let xxx = btoa(cont);
           console.log(xxx);
           let cmd = "FilterPublished(\"" + xxx + "\")";

           this.mgr.SendMIR(cmd, this.fw2gui.fElementId, "EventManager");
       },

       setFilterEnabled: function(oEvent)
       {
           console.log("enable filter", oEvent.getParameter("selected"));
       },

       handleModeSelect: function(oEvent)
       {
           console.log("handle mode select idx =", oEvent.getParameter("selectedIndex"));
       }

});


   return EventFilterController;

});
