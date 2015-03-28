(function (App) {
	
App.ViewModels.TagModel = function(data)
{
	var self = this;
	self.SaveEnabled = ko.observable(false);
	self.types = ko.observableArray(App.Helpers.CreateArray(App.Enums.EDataType));				
	self.id = ko.observable();
	self.name = ko.observable();
	self.type = ko.observable(App.Enums.EDataType.Unknown);
	self.logged = ko.observable(false);
	self.internal = ko.observable(true);		
	self.unit = ko.observable(" °C");
	self.force = ko.observable(false);
	self.direction = ko.observable(App.Enums.EDirection.Read);						
	self.IsEnabled = ko.observable(false);		
	self.value = ko.observable();
	self.valueunit = ko.computed(function(){
		return self.value() + ' ' + self.unit();
	},this);
	self.error = ko.observable(true);
	
	self.ValueEnabled = ko.computed(function(){
		return self.internal() || self.force()//|| self.direction()==App.Enums.EDirection.Write || self.direction()==App.Enums.EDirection.ReadWrite;
	},this);
	self.selectedValue = ko.observable(3);		
	self.address = ko.observable();		
	self.oldvalue = null;
	self.setOutputState = ko.observable(0);		
	self.image = ko.computed(function(){
		if (self.error() == false)
				return 'app/icons/Circle_Green.png';
			else
				return 'app/icons/Circle_Red.png';
	});
	ko.mapping.fromJS(data, {}, self);
};

App.ViewModels.DeviceModel = function (data) {
	var self = this;
	self.isnew = ko.observable(false);
	self.id = ko.observable();
	self.name = ko.observable();
	self.Address = ko.observable();
	self.Tags = ko.observableArray();
	self.isLogged = ko.observable(true);
	self.NameEditEnabled = ko.observable(true);
	
	self.SettingsClick = function(e,event) {
		event.stopPropagation();
		event.stopImmediatePropagation();          
		alert(e.Address());
		return false;
	};
	
	self.SaveEnabled = ko.computed(function(){
		
	},this);
	self.btnSaveClick = function(e,event) {
	};
	self.btnStornoClick = function(e,event) {
		App.Instance.GetDevicesController().LoadPage();
	};
	self.DeleteEnabled = ko.computed(function(){
		
	},this);
	self.btnDeleteClick = function(e,event) {
	};
	self.OneWireTemplate = function() {
		return "ds18b20Template";
	}
	var mappingOptions = {
		'Tags': {
			create: function(options) {
				return new App.ViewModels.TagModel(options.data);
			}
		}
	};
	if (data!=null) ko.mapping.fromJS(data, mappingOptions, self);
};

App.ViewModels.DevicesModel = function () {
	var self = this;	
	/*vytvori nove virtualni zarizeni s tagem*/
	self.newDeviceClick = function(e,event) {
			App.Helpers.LoadPage('#demopage','deviceDetail.html', new App.ViewModels.DeviceModel(), function(){																		
				
			});
			
			// $.mobile.pageContainer.pagecontainer('change', 'deviceDetail.html', {
				// transition: 'none',
				// changeHash: false,
				// reverse: false,
				// showLoadMsg: true
			// });			
			$('input[type="button"]').button().button('refresh');
		return true;
	};	
	self.isLogged = ko.observable(true);
	self.Devices = ko.observableArray();
};

App.Controllers.DeviceController = function(psocket)
{
	
}

App.Controllers.DevicesController = function(psocket)
{
	var self = this;
	self.model = new App.ViewModels.DevicesModel();
	self.socket = psocket;		
	App.Controllers.DevicesController.prototype.GetModel = function() {
		return self.model;
	};
	App.Controllers.DevicesController.prototype.LoadPage = function()
	{
		$.mobile.pageContainer.pagecontainer("change", "devicelist.html");		
		
		
		// App.Helpers.LoadPage($("#demopage"),'devicelist.html',self.GetModel(),function(){
			 // self.Load(function(){										
				// $('div[data-role="collapsibleset"]').collapsibleset('refresh');					
				// $('ul[data-role="listview"]').listview().listview('refresh');					
				// $('input[type="button"]').button().button('refresh');
			// });				
		// });
	};	
	App.Controllers.DevicesController.prototype.Load = function(callback) {
		self.socket.read("onewiredevices",null,function(response){			
					if (response.success)
					{
						var jsdata = {};
						jsdata.Devices = response.message;						
						var mapping = {
							'Devices': {
								create: function(options) {
									return new App.ViewModels.DeviceModel(options.data);
								}
							}
						}
						ko.mapping.fromJS(jsdata,mapping, self.model); 
						
						if (callback!=null) callback();
						//self.sort();
						//self.Refresh();								
					}
				});
	};
};

})(App)