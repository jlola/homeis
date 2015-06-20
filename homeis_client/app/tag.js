(function (App) {
	
App.ViewModels.TagModel = function(psocket,data)
{
	var self = this;
	self.socket = psocket;
	self.parent = null;
	self.data = {};	
	self.data.id = ko.observable();
	self.data.parentid = ko.observable();	
	self.data.name = ko.observable();
	self.data.type = ko.observable(App.Enums.EDataType.Unknown);
	self.data.logged = ko.observable(true);
	self.data.internal = ko.observable(true);		
	self.data.unit = ko.observable(" °C");
	self.data.force = ko.observable(false);
	self.data.direction = ko.observable(App.Enums.EDirection.Read);						
	self.data.IsEnabled = ko.observable(self.data.logged());		
	self.data.value = ko.observable();		
	self.data.address = ko.observable();		
	self.data.oldvalue = null;
	self.data.setOutputState = ko.observable(0);			
	self.data.error = ko.observable(true);	
	self.IsNew = ko.observable(data==null ? true : false);
	self.types = ko.observableArray(App.Helpers.CreateArray(App.Enums.EDataType));				
	self.selectedValue = ko.observable(3);		
	self.SaveEnabled = ko.observable(self.data.logged());
	self.itemSelect = function() {
		if (self.parent.itemSelect!=null && self.parent.itemSelect(self)==true)
			self.LoadPage();				
	};
	self.prevPage = null;
	self.myPostProcessingLogic = function(elements) {
	// "elements" is an array of DOM nodes just rendered by the template
	// You can add custom post-processing logic here
		//$("div[id$='divalue']").trigger('create');			
	}
		
	self.name = ko.computed(function(){
		return self.data.name();
	},this);
	self.valueTemplate = function() {
		if (self.data.type()==4)	return "valueBooleanTemplate";
		return "valueTemplateText";
	}
	self.directionLabel = ko.computed(function() {
		if (self.data.direction()==App.Enums.EDirection.Read)
			return 'Čtení';
		return 'Čtení/Zápis';
	}, this);		
	self.isForceVisible = ko.computed(function() {
		if (((self.data.direction()==App.Enums.EDirection.Write || self.data.direction()==App.Enums.EDirection.ReadWrite) && !self.data.error())||
			self.data.internal())
			return true;
		else
			return false;
		
	}, this);
	self.valueunit = ko.computed(function(){
		return self.data.value() + ' ' + self.data.unit();
	},this);
	
	self.errorUI = ko.computed(function(){
		if (self.data.error()==false) return "Připojeno";
		else return "Odpojeno";
	},this);
		
	self.ValueEnabled = ko.computed(function(){
		return self.data.internal() || self.data.force()//|| self.direction()==App.Enums.EDirection.Write || self.direction()==App.Enums.EDirection.ReadWrite;
	},this);
	// self.image = ko.computed(function(){
		
		// if (self.data.error() == false)
		// {
			// if (self.data.force()==true)
				// return 'app/icons/Circle_blau.png';
			// else
				// return 'app/icons/Circle_Green.png';
		// }
		// else
		// {
			// return 'app/icons/Circle_Red.png';
		// }
	// });
	self.forceChanged = function(object,event)
	{
		self.data.force(!self.data.force())
		
		self.Save(function(){});
		
	};		
	self.typeFormated = ko.computed(function() {
		switch(self.data.type())
		{
			case App.Enums.EDataType.Int:
				return 'Int';
			case App.Enums.EDataType.UInt:
				return 'UInt';
			case App.Enums.EDataType.Double:
				return 'Double';
			case App.Enums.EDataType.String:
				return 'String';
			case App.Enums.EDataType.Bool:
				return 'Bool';
		}		
	}, this);
	
	self.image = ko.computed(function() {
		if (self.data.type()==null) return 'app/icons/Folder_Icon.png'			
		if (self.data.internal()) 
		{
			if (self.data.force()==false)			
				return 'app/icons/Circle_Green_Internal.png';
			else
				return 'app/icons/Circle_blau _Internal2.png';
		}
		if (self.data.force()) return 'app/icons/Circle_blau.png';
		if (self.data.error() == false)
			return 'app/icons/Circle_Green.png';
		else
			return 'app/icons/Circle_Red.png';
	}, this);				       
	
	self.IsValueValid = function(pvalue,ptype)
	{
		try	
		{
			switch(ptype)
			 {
				 case App.Enums.EDataType.String:
				 return true;
				 case App.Enums.EDataType.Bool:
					 App.Helpers.BooleanParse(pvalue);
					 return true;
				 break;
				 case App.Enums.EDataType.UInt:
				 case App.Enums.EDataType.Int:
					if (!isNaN(parseInt(pvalue))) return true;						
				 case App.Enums.EDataType.Double:
					if (!isNaN(parseFloat(pvalue))) return true;						
				 break;				
			}
		}
		catch(ex)
		{
			console.log(ex);				
		}
		return false;
	}
	
	self.GetFormatedValue = function(pvalue,ptype) {
		 switch(ptype)
		 {
			 case App.Enums.EDataType.UInt:
			 case App.Enums.EDataType.Int:
			 case App.Enums.EDataType.Double:
			 case App.Enums.EDataType.String:
				return pvalue;
			 break;				 
			 case App.Enums.EDataType.Bool:
				 if (pvalue==1) return '1';
				 else return '0';
			 break;				 							
		}
		return "";//.toFixed(2);		
	 };
	self.btnDelete = function() {						
		if (confirm('Opravdu si přejete smazat tag: ' + self.data.name() + '?'))
		{
			self.socket.destroy("onewiredevices/devvalue",self.data.id(),function(response){
				if (response.success) 	
				{					
					self.prevPage.Load();
					self.prevPage.LoadPage();
				}
				else 
					alert('Chyba pri mazani');			
			});
		}
		return true;												
	}
	
	self.btnSave = function() {									
		self.Save();
		return true;
	}
	self.btnStorno = function() {						
		self.prevPage.LoadPage();
		return true;
	}
	self.LoadPage = function()
	{
		App.Instance.SetPageContainerBeforeShowFunc( function( pevent, ui ) {
					var element = ui.toPage;	
					var item = ko.dataFor(ui.prevPage[0]);										
					self.prevPage = item;
					if (self.prevPage.UnloadPage!=undefined)
						self.prevPage.UnloadPage();
					if (ko.dataFor(element[0])==null)
						ko.applyBindings(self,element[0]);	
					$('#selectionType').selectmenu('refresh', true);
				} );
		$.mobile.pageContainer.pagecontainer("change","onewiredetail.html",{changeHash:false,reload:true});
	};
	self.Save = function(callback)
	{				
		var dto = ko.toJS(self.data);	
		
		if (self.IsNew())
		{
			 self.socket.create("onewiredevices/devvalue",self.data,function(response){
				if (response.success) 
				{	
					//var obj = JSON.parse(response.message);					
					self.prevPage.Load();
					self.prevPage.LoadPage();
				}
				else 
					alert('Chyba pri ukaldani');
				if (callback!=null) 
					callback();
			 });				 
		}
		else {
			 self.socket.update("onewiredevices/devvalue",self.data.id(),dto,function(response){
				 if (response.success) 
				 {	
					self.prevPage.LoadPage();
					//App.Instance.GetDevicesController().LoadPage();
				 }
				 else 
					 alert('Chyba pri ukaldani');
				 if (callback!=null) callback();
			 });
			 
		}		
	};
	self.Mapping = function(pdata)
	{
		ko.mapping.fromJS(pdata, {}, self.data);
	};
	
	self.Mapping(data);	
};

})(App)
