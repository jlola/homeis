(function (App) {
	
// App.ViewModels.TagModel = function(psocket,data)
// {
	// var self = this;
	// self.socket = psocket;
	// self.parent = null;
	// self.data = {};	
	// self.data.id = ko.observable();
	// self.data.parentid = ko.observable();	
	// self.data.name = ko.observable();
	// self.data.type = ko.observable(App.Enums.EDataType.Unknown);
	// self.data.logged = ko.observable(true);
	// self.data.internal = ko.observable(true);		
	// self.data.unit = ko.observable(" °C");
	// self.data.force = ko.observable(false);
	// self.data.direction = ko.observable(App.Enums.EDirection.Read);						
	// self.data.IsEnabled = ko.observable(self.data.logged());		
	// self.data.value = ko.observable();		
	// self.data.address = ko.observable();		
	// self.data.oldvalue = null;
	// self.data.setOutputState = ko.observable(0);			
	// self.data.error = ko.observable(true);	
	// self.IsNew = ko.observable(data==null ? true : false);
	// self.types = ko.observableArray(App.Helpers.CreateArray(App.Enums.EDataType));				
	// self.selectedValue = ko.observable(3);		
	// self.SaveEnabled = ko.observable(self.data.logged());
	// self.itemSelect = function() {
		// if (self.parent.itemSelect!=null && self.parent.itemSelect(self)==true)
			// self.LoadPage();				
	// };
	// self.prevPage = null;
	// self.myPostProcessingLogic = function(elements) {
	// // "elements" is an array of DOM nodes just rendered by the template
	// // You can add custom post-processing logic here
		// //$("div[id$='divalue']").trigger('create');			
	// }
		
	// self.name = ko.computed(function(){
		// return self.data.name();
	// },this);
	// self.valueTemplate = function() {
		// if (self.data.type()==4)	return "valueBooleanTemplate";
		// return "valueTemplateText";
	// }
	// self.directionLabel = ko.computed(function() {
		// if (self.data.direction()==App.Enums.EDirection.Read)
			// return 'Čtení';
		// return 'Čtení/Zápis';
	// }, this);		
	// self.isForceVisible = ko.computed(function() {
		// if (((self.data.direction()==App.Enums.EDirection.Write || self.data.direction()==App.Enums.EDirection.ReadWrite) && !self.data.error())||
			// self.data.internal())
			// return true;
		// else
			// return false;
		
	// }, this);
	// self.valueunit = ko.computed(function(){
		// return self.data.value() + ' ' + self.data.unit();
	// },this);
	
	// self.errorUI = ko.computed(function(){
		// if (self.data.error()==false) return "Připojeno";
		// else return "Odpojeno";
	// },this);
		
	// self.ValueEnabled = ko.computed(function(){
		// return self.data.internal() || self.data.force()//|| self.direction()==App.Enums.EDirection.Write || self.direction()==App.Enums.EDirection.ReadWrite;
	// },this);
	// self.image = ko.computed(function(){
		// if (self.data.error() == false)
				// return 'app/icons/Circle_Green.png';
			// else
				// return 'app/icons/Circle_Red.png';
	// });
	// self.forceChanged = function(object,event)
	// {
		// self.data.force(!self.data.force())
		
		// self.Save(function(){});
		
	// };		
	// self.typeFormated = ko.computed(function() {
		// switch(self.data.type())
		// {
			// case App.Enums.EDataType.Int:
				// return 'Int';
			// case App.Enums.EDataType.UInt:
				// return 'UInt';
			// case App.Enums.EDataType.Double:
				// return 'Double';
			// case App.Enums.EDataType.String:
				// return 'String';
			// case App.Enums.EDataType.Bool:
				// return 'Bool';
		// }		
	// }, this);
	
	// self.image = ko.computed(function() {
		// if (self.data.type()==null) return 'app/icons/Folder_Icon.png'			
		// if (self.data.internal()) return 'app/icons/Circle_Green_Internal.png';
		// if (self.data.force()) return 'app/icons/Circle_blau.png';
		// if (self.data.error() == false)
			// return 'app/icons/Circle_Green.png';
		// else
			// return 'app/icons/Circle_Red.png';
	// }, this);				       
	
	// self.IsValueValid = function(pvalue,ptype)
	// {
		// try	
		// {
			// switch(ptype)
			 // {
				 // case App.Enums.EDataType.String:
				 // return true;
				 // case App.Enums.EDataType.Bool:
					 // App.Helpers.BooleanParse(pvalue);
					 // return true;
				 // break;
				 // case App.Enums.EDataType.UInt:
				 // case App.Enums.EDataType.Int:
					// if (!isNaN(parseInt(pvalue))) return true;						
				 // case App.Enums.EDataType.Double:
					// if (!isNaN(parseFloat(pvalue))) return true;						
				 // break;				
			// }
		// }
		// catch(ex)
		// {
			// console.log(ex);				
		// }
		// return false;
	// }
	
	// self.GetFormatedValue = function(pvalue,ptype) {
		 // switch(ptype)
		 // {
			 // case App.Enums.EDataType.UInt:
			 // case App.Enums.EDataType.Int:
			 // case App.Enums.EDataType.Double:
			 // case App.Enums.EDataType.String:
				// return pvalue;
			 // break;				 
			 // case App.Enums.EDataType.Bool:
				 // if (pvalue==1) return '1';
				 // else return '0';
			 // break;				 							
		// }
		// return "";//.toFixed(2);		
	 // };
	// self.btnDelete = function() {						
		// if (confirm('Opravdu si přejete smazat tag: ' + self.data.name() + '?'))
		// {
			// self.socket.destroy("onewiredevices/devvalue",self.data.id(),function(response){
				// if (response.success) 	
				// {					
					// self.prevPage.Load();
					// self.prevPage.LoadPage();
				// }
				// else 
					// alert('Chyba pri mazani');			
			// });
		// }
		// return true;												
	// }
	
	// self.btnSave = function() {									
		// self.Save();
		// return true;
	// }
	// self.btnStorno = function() {						
		// self.prevPage.LoadPage();
		// return true;
	// }
	// self.LoadPage = function()
	// {
		// App.Instance.SetPageContainerBeforeShowFunc( function( pevent, ui ) {
					// var element = ui.toPage;	
					// var item = ko.dataFor(ui.prevPage[0]);					
					// self.prevPage = item;
					// if (ko.dataFor(element[0])==null)
						// ko.applyBindings(self,element[0]);					
				// } );
		// $.mobile.pageContainer.pagecontainer("change","onewiredetail.html",{changeHash:false,reload:true});
	// };
	// self.Save = function(callback)
	// {				
		// var dto = ko.toJS(self.data);	
		
		// if (self.IsNew())
		// {
			 // self.socket.create("onewiredevices/devvalue",self.data,function(response){
				// if (response.success) 
				// {	
					// //var obj = JSON.parse(response.message);					
					// self.prevPage.Load();
					// self.prevPage.LoadPage();
				// }
				// else 
					// alert('Chyba pri ukaldani');
				// if (callback!=null) 
					// callback();
			 // });				 
		// }
		// else {
			 // self.socket.update("onewiredevices/devvalue",self.data.id(),dto,function(response){
				 // if (response.success) 
				 // {	
					// self.prevPage.LoadPage();
					// //App.Instance.GetDevicesController().LoadPage();
				 // }
				 // else 
					 // alert('Chyba pri ukaldani');
				 // if (callback!=null) callback();
			 // });
			 
		// }		
	// };
	// ko.mapping.fromJS(data, {}, self.data);
// };

App.ViewModels.DeviceModel = function (psocket,data) {
	var self = this;
	self.socket = psocket;
	self.IsNew = ko.observable(data==null);	
	self.data = {};
	self.data.Id = ko.observable();
	self.data.Name = ko.observable();
	self.data.Address = ko.observable();
	self.data.Tags = ko.observableArray();
	self.data.Internal = ko.observable(false);	
	self.OnSelected = null;
	self.itemSelect = function(ui,e) {		
		if (self.OnSelected!=null)
			return self.OnSelected(ui);		
		return true;
	};
	self.isLogged = ko.computed(function(){
		return true;
	},this);
	self.NameEditEnabled = ko.observable(false);
	self.notisnew = ko.computed(function(){
		return !self.IsNew()
	},this);
	
	self.SettingsClick = function(e,event) {
		self = this;
		event.stopPropagation();
		event.stopImmediatePropagation();          
		self.LoadPage();
		return false;
	};
	
	self.SaveEnabled = ko.computed(function(){
		
		if (self.data.Name()!=null) 
			return self.data.Name().length>0;
		return false;
	},this);
	self.btnSaveClick = function(e,event) {

		self.Save();		
	};
	self.SerializeTags = function() {
		var stags = [];
		for	(index = 0; index < self.data.Tags().length; index++) {
			var tag = self.data.Tags()[index].data;
			var res = ko.mapping.toJS(tag,null);
			stags[stags.length] = res;
		}
		return stags;
	};
	self.Save = function(callback)
	{				
		var dto = self.data;	
		dto.Tags = self.SerializeTags();
		if (self.IsNew())
		{
			 self.socket.create("onewiredevices",dto,function(response){
				if (response.success) 
				{	
					//var obj = JSON.parse(response.message);					
					App.Instance.GetDevicesController().LoadPage();
				}
				else 
					alert('Chyba pri ukaldani');
				if (callback!=null) 
					callback();
			 });				 
		}
		else {
			 self.socket.update("onewiredevices",self.data.Id(),dto,function(response){
				 if (response.success) 
				 {	
					App.Instance.GetDevicesController().LoadPage();
				 }
				 else 
					 alert('Chyba pri ukaldani');
				 if (callback!=null) callback();
			 });
			 
		}
	};
	self.btnStornoClick = function(e,event) {
		App.Instance.GetDevicesController().LoadPage();
	};
	self.DeleteEnabled = ko.computed(function(){
		return self.notisnew();
	},this);
	self.btnDeleteClick = function(e,event) {
		if (confirm('Opravdu si přejete smazat zarizeni: ' + self.data.Name() + '?'))
		{
			self.socket.destroy("onewiredevices",self.data.Id(),function(response){
				if (response.success) 			
					App.Instance.GetDevicesController().LoadPage();			
				else 
					alert('Chyba pri mazani');			
			 });
		}
	};
	self.btnNewTagClick = function(e,event) {
		var tag = new App.ViewModels.TagModel(self.socket,null);		
		tag.data.parentid(self.data.Id());
		tag.LoadPage();
	};
	self.OneWireTemplate = function() {
		return "ds18b20Template2";
	};

	self.btnReloadClick = function(e,event) {
		self.Load();
	};
	
	self.Mapping = function(pdata) {
		var mappingOptions = {
			'Tags': {
				create: function(options) {
					var tag = new App.ViewModels.TagModel(self.socket,options.data);
					tag.parent = self;
					return tag;
				}
			}
		};
		
		if (pdata!=null) ko.mapping.fromJS(pdata, mappingOptions, self.data);
		
		$('ul[data-role="listview"]').listview().listview('refresh');								
		$('div[data-role="collapsibleset"]').collapsibleset('refresh');								
		$('input[type="button"]').button().button('refresh');
		
	};
	
	self.Load = function(pdata) {		
		if (pdata==null)
		{
			this.socket.read("onewiredevices",self.data.Id(),function(response){			
				if (response.success)
				{							
					self.Mapping(response.message[0]);					
				}
				else
				{
					alert('Chyba pri cteni dat');
				}
			});
		}
		else
		{
			self.Mapping(pdata);
		}
	};
	
	self.Mapping(data);
};

App.ViewModels.DeviceModel.prototype.LoadPage = function() {		
	var self = this;
	App.Instance.SetPageContainerBeforeShowFunc( function( event, ui ) {
		var element = ui.toPage;	
		var item = ko.dataFor(ui.prevPage[0]);					
		self.prevPage = item;
		if (ko.dataFor(element[0])==null)
			ko.applyBindings(self,element[0]);	
		
		$('ul[data-role="listview"]').listview().listview('refresh');								
		$('div[data-role="collapsibleset"]').collapsibleset('refresh');								
		$('input[type="button"]').button().button('refresh');		
	} );	
	$.mobile.pageContainer.pagecontainer("change","deviceDetail.html",{changeHash:false,reload:true});				
};

App.ViewModels.DevicesModel = function (psocket) {
	var self = this;	
	self.socket = psocket;	
	self.selectMode = ko.observable(false);
	self.isLogged = ko.observable(true);
	self.Devices = ko.observableArray();
	self.btnStornoClick = function(ui,event) {
		self.prevPage.LoadPage();
	};
	self.itemSelect = function(ui) {
		if (self.selectMode())
		{
			self.prevPage.itemSelect(ui);
			self.prevPage.LoadPage();						
		}
		else
		{
			ui.LoadPage();			
		}
		return false;
	};
	
	/*vytvori nove virtualni zarizeni s tagem*/
	self.newDeviceClick = function(e,event) {								
			//App.Instance.GetDeviceController().LoadPage();					
			new App.ViewModels.DeviceModel(self.socket,null).LoadPage();
		return true;
	};	
	self.LoadPage = function() {
		self = this;
		self.selectMode(false);	
		var activePage = $.mobile.pageContainer.pagecontainer( "getActivePage" );
		if (activePage!=null && activePage[0].id=='devicespage') return false;
		
		App.Instance.SetPageContainerBeforeShowFunc( function( event, ui ) {
			var element = ui.toPage;			
			ko.applyBindings(self,element[0]);			
			$('ul[data-role="listview"]').listview().listview('refresh');								
			$('div[data-role="collapsibleset"]').collapsibleset('refresh');								
			$('input[type="button"]').button().button('refresh');
			$( "#headerid" ).hide();
		} );
		
		self.Load(function(){
			$.mobile.pageContainer.pagecontainer("change","devicelist.html",{changeHash:false,reload:true});
		});
		return true;
	};			
};
App.ViewModels.DevicesModel.prototype.LoadDialog = function() {
		self = this;
		self.selectMode(true);			
		App.Instance.SetPageContainerBeforeShowFunc( function( event, ui ) {
			var element = ui.toPage;	
			var item = ko.dataFor(ui.prevPage[0]);					
			self.prevPage = item;
			if (ko.dataFor(element[0])==null)
				ko.applyBindings(self,element[0]);					
			$('ul[data-role="listview"]').listview().listview('refresh');								
			$('div[data-role="collapsibleset"]').collapsibleset('refresh');								
			$('input[type="button"]').button().button('refresh');
		} );
		
		self.Load(function(){
			$.mobile.pageContainer.pagecontainer("change","devicelist.html",{changeHash:false,reload:true,role:'dialog'});
		});
		return true;
};	
App.ViewModels.DevicesModel.prototype.Load = function(callback) {
	var self = this;
	this.socket.read("onewiredevices",null,function(response){			
		if (response.success)
		{
			var jsdata = {};
			jsdata.Devices = response.message;						
			var mapping = {
				'Devices': {
					create: function(options) {
						var dev = new App.ViewModels.DeviceModel(self.socket,options.data);
						dev.OnSelected = self.itemSelect;
						return dev;
					}
				}
			}
			ko.mapping.fromJS(jsdata,mapping, self); 
			
			if (callback!=null) callback();									
		}
	});
};


App.Controllers.DevicesController = function(psocket)
{
	var self = this;	
	self.socket = psocket;		
	self.model = new App.ViewModels.DevicesModel(self.socket);
	App.Controllers.DevicesController.prototype.GetModel = function() {
		return self.model;
	};
	App.Controllers.DevicesController.prototype.LoadPage = function()
	{			
		return self.model.LoadPage();
	};	
	App.Controllers.DevicesController.prototype.LoadDialog = function()
	{			
		return self.model.LoadDialog();
	};	
	
};

})(App)