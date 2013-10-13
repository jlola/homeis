

(function (App) {

	// App.DataObjects.OneWireDeviceListDto = function() {
		// var self = this;
		// self.Command = 'OneWireDeviceList';
		// self.OneWireList = ko.observableArray();
		// return self;
	// }
	
	App.DataObjects.OneWireDeviceDto = function() {
		var self = this;
		self.id = '';
		self.name = '';		
		return self;
	}
	
	App.Enums.EDirection = {
		Read: 0,
		Write: 1,
		ReadWrite: 2
	}

    App.Enums.EOneWireType = {
		Unknown: 0,
        DS18B20: 2,
		Folder: 3,
        DS2413: 4,
		Virtual :5
    }

	App.Enums.EDataType = {
		Unknown: -1,
		Int: 0,
		Uint: 1,
		Double: 2,
		String: 3,
		Bool: 4
	}
	
'[]'
	
///////////////////////////////////////////////////OneWireDevice//////////////////////////////////////////////////////////////////////		
    App.ViewModels.OneWireDevice = function (psocket,list) {
        var self = this;
		self.IsNew = ko.observable(true);
		self.socket = psocket;
		if (list!=null)
			self.path = list.GetPath();			
		self.SaveEnabled = ko.observable(false);
		self.types = ko.observableArray(App.Helpers.CreateArray(App.Enums.EDataType));				
		self.id = ko.observable();
		self.name = ko.observable();
		self.type = ko.observable(App.Enums.EDataType.Unknown);
		self.logged = ko.observable(false);
		self.internal = ko.observable(true);		
		self.unit = ko.observable(" °C");
		self.ValueEnabled = ko.computed(function(){
			return self.internal() || self.direction()==App.Enums.EDirection.Write || self.direction()==App.Enums.EDirection.ReadWrite;
		},this);
		self.selectedValue = ko.observable(3);
		//self.selTypeIndex = ko.observable(2);
		self.address = ko.observable();		
		self.setOutputState = ko.observable(0);		
		self.setOutputStateChanged = function(newValue) {			
			//alert(self.setOutputState());
			//console.log(newValue);			
		};
		
		self.setOutputState.subscribe(self.setOutputStateChanged);		
		self.force = ko.observable(false);
		self.direction = ko.observable(App.Enums.EDirection.Read);						
		self.IsEnabled = ko.observable(false);		
		self.value = ko.observable();
		self.error = ko.observable(true);
		//self.CopyDataFrom(dataObject);
		
		self.directionLabel = ko.computed(function() {
			if (self.direction()==App.Enums.EDirection.Read)
				return 'Čtení';
			return 'Čtení/Zápis';
		}, this);		
		self.isForceVisible = ko.computed(function() {
			if (((self.direction()==App.Enums.EDirection.Write || self.direction()==App.Enums.EDirection.ReadWrite) && !self.error())||
				self.internal())
				return true;
			else
				return false;
			
		}, this);
		
		App.ViewModels.OneWireDevice.prototype.CopyDataFrom = function(pdataObject)
		{
			if (pdataObject==null) return;
			var self = this;			
			dataObject = ko.toJS(pdataObject);			
			self.id(dataObject.id);			
			self.name(dataObject.name);			
			self.address(dataObject.address);
			self.list = list;			
			self.type(dataObject.type);						
			self.direction(dataObject.direction);			
			self.value(dataObject.value);			
			self.error(dataObject.error);			
			self.force(dataObject.force);			
			self.unit(dataObject.unit);
			//self.setOutputState(dataObject.value);			
			self.internal(dataObject.internal==null?false:dataObject.internal);
			self.selectedValue(self.type());					
			self.formatedValue(self.GetFormatedValue(self.value(),self.type()));			
			self.DirValueId = dataObject.DirValueId;
			App.Helpers.SetDropDownListValue('selectionType',self.type());
		};
		
		self.checkBoxChanged = function(object,event)
		{
			self.force(!self.force())
			
			self.Save();
			
		};		
		self.typeFormated = ko.computed(function() {
			switch(self.type())
			{
				case App.Enums.EDataType.Int:
					return 'Int';
				case App.Enums.EDataType.Uint:
					return 'Uint';
				case App.Enums.EDataType.Double:
					return 'Double';
				case App.Enums.EDataType.String:
					return 'String';
				case App.Enums.EDataType.Bool:
					return 'Bool';
			}		
		}, this);
		
		self.image = ko.computed(function() {
			if (self.type()==null) return 'app/icons/Folder_Icon.png'
			if (self.internal()) return 'app/icons/Circle_Green_Internal.png';
			if (self.error() == false)
				return 'app/icons/Circle_Green.png';
			else
				return 'app/icons/Circle_Red.png';
		}, this);
		
		
        self.formatedValue = ko.observable();
		
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
				 case App.Enums.EDataType.Uint:
				 case App.Enums.EDataType.Int:
				 case App.Enums.EDataType.Double:
				 case App.Enums.EDataType.String:
					return pvalue;
				 break;				 
				 case App.Enums.EDataType.Bool:
					 if (pvalue==1) return 'True';
					 else return 'False';
				 break;				 							
			}
			return "";//.toFixed(2);		
		 };
				
		
		self.CreateDto = function(src)
		{
			var dto = new App.DataObjects.OneWireDeviceDto();
			dto.id = src.address();
			dto.name = src.name();
			dto.force = src.force();
			dto.type = src.type();
			dto.value = src.formatedValue();
			dto.unit = src.unit();
			//if (src.force()) dto.value = src.setOutputState();
			return dto;
		}
						
		self.CreateEmptyDto = function()
		{
			var dto = new App.DataObjects.OneWireDeviceDto();
			dto.id = 0;
			dto.name = '';
			dto.force = true;			
			dto.value = '';
			dto.type = -1;
			dto.unit = '';
			dto.internal = true;
			dto.error = false;
			return dto;
		}
		
		// self.itemSelect = function(event,ui) {						 
			 // //$.mobile.loadPage('#onewireDetail', { showLoadMsg: false } );				
											
			// return true;
		// }
		
		self.btnMove = function() {
			
			return true;
		}	

		self.btnDelete = function() {
			
				self.Remove(function(){
					LoadPage('#demopage','onewirelist.html',App.Instance.GetOneWireList(),function(){
						App.Instance.GetOneWireList().Load();				 
						App.Instance.GetOneWireList().Refresh();		
					});
				});
				
				return true;												
		}
		
		self.btnSave = function() {									
			if (!self.internal())
			{
				self.Save(function(){
					LoadPage('#demopage','onewirelist.html',App.Instance.GetOneWireList(),function(){
						App.Instance.GetOneWireList().Load();				 
						App.Instance.GetOneWireList().Refresh();		
					});
				});
				
				return true;
			}
			else
			{
				var val = self.formatedValue()				
				if (!self.IsValueValid(val,self.type()))
				{
					alert('Zadaná hodnota neodpovídá datovému typu');
					return false;
				}
				self.Save(function(){
					LoadPage('#demopage','onewirelist.html',App.Instance.GetOneWireList(),function(){
						App.Instance.GetOneWireList().Load();				 
						App.Instance.GetOneWireList().Refresh();		
					});
				});
			}			
			return true;
		}
		self.btnStorno = function() {			
			LoadPage('#demopage','onewirelist.html',App.Instance.GetOneWireList(),function(){
				//App.Instance.GetOneWireList().Load();				 
				App.Instance.GetOneWireList().Refresh();		
			});
			return true;
		}
		
        self.typetext = ko.computed(function () {
            switch (self.type) {
                case App.Enums.EOneWireType.Temperature:
                    return 'Teplota';
                case App.Enums.EOneWireType.IO2:
                    return 'IO';
            }
        }, this);
		
		self.Remove = function(callback) {			
			if (confirm('Opravdu si přejete odstranit zařízení: ' + self.name() + '?'))
			{								
				self.socket.destroy('onewiredevices/',self.id(),function(data){
					if (callback!=null) callback();
				});
				
			}
		}
		
		self.Save = function(callback)
		{
			var dto = self.CreateDto(this);
			if (self.IsNew())
			{
				 self.socket.create("onewiredevices",dto,function(data){
					 alert('data');
				 });
				 if (callback!=null) callback();
			}
			else {
				 self.socket.update("onewiredevices",self.address(),dto,function(data){
					 alert('data');
				 });
				 if (callback!=null) callback();
			}
		};
		
		self.Load = function()
		{
			
		};
		return self;
    };
////////////////////////////////////////////////////OneWireListModel////////////////////////////////////////////////////////////////////////////
	App.Enums.OneWireListMode = {	
		Devices: 0,
		Folders: 1,
		DeviceSelect: 2		
	}


    // Overall viewmodel for this screen, along with initial state
    App.ViewModels.OneWireListModel = function (psocket) {
        var self = this;				
		self.OnItemSelected = null;
		self.Mode = ko.observable(App.Enums.OneWireListMode.Devices);
		self.oneWireList = ko.observableArray();
		self.socket = psocket;				
        self.isLogged = ko.observable(false); 
		self.SelectedItem = ko.observable();
		self.parentFolderId = ko.observable();
		self.path = 'onewiredevices';
		self.folderDevicesPath = 'onewiredevices/folder';
		self.GetPath = function() { return self.path; }
		self.Refresh = function(holderId) {		
			if (holderId==null)
			{
				$('#oneWireListid').listview('refresh');
			}
			else
			{
				//holder = $(holderId).find("#oneWireListid");
				$(holderId + ' #oneWireListid').listview('refresh');
				//$(holderId).triger('create');
			}
		}
		
		self.expressionSelect = function(event,ui) {						 
			if (self.Mode()!=App.Enums.OneWireListMode.DeviceSelect)
			{
				App.Helpers.LoadPage('#demopage','expressiondetail.html', event, function(){
					var isLogged = App.Instance.GetLogInPage().logged();
					var element = $('#demopagecontent').get(0);
					var item = ko.dataFor(element);
					if (item == null)
					{				
						item = new App.ViewModels.ExpressionModel(self.socket,self.list);										
					}
					else item = ko.dataFor(element);							
					//item.CopyDataFrom(ko.toJS(event));
					App.Helpers.CopyData(event,item);
					item.IsNew(false);
					item.IsEnabled(isLogged);
					item.SaveEnabled(isLogged);				
				});
			}
		}
		
		self.itemSelect = function(event,ui) {						 
			
				if (self.Mode()!=App.Enums.OneWireListMode.DeviceSelect)
				{
					App.Helpers.LoadPage('#demopage','onewiredetail.html', event, function(){
						var isLogged = App.Instance.GetLogInPage().logged();
						var element = $('#demopagecontent').get(0);
						var item = ko.dataFor(element);
						if (item == null)
						{				
							item = new App.ViewModels.OneWireDevice(self.socket,self.list);																			
						}
						else item = ko.dataFor(element);							
						item.CopyDataFrom(ko.toJS(event));
						item.IsNew(false);
						item.IsEnabled(isLogged);
						item.SaveEnabled(isLogged);				
					});
				} else
				{
					if (confirm('Opravdu si přejete vložit: '+ event.name() + '?')) {						
						$('#folderSelect').dialog('close');
						// self.socket.create("onewiredevices/folder",dto,function(data){
							// alert('chyba pri ukladani');
						// });
						var dto = { DevValueAddress: event.address() };
						self.socket.update("onewiredevices/folder",self.parentFolderId,dto,function(data){
							 alert('data');
						});
						if (self.OnItemSelected!=null) self.OnItemSelected(event);
					}
				}			
		}
		
		self.removeValueId = function(event,ui) {			
			if (confirm('Opravdu si přejete odstranit datový bod ' + event.name() + '?'))
			{								
				self.socket.destroy('onewiredevices/folder',event.DirValueId,function(data){
					self.Load(self.parentFolderId);
				});
			}
		}
		// Int: 0,
		// Uint: 1,
		// Double: 2,
		// String: 3,
		// Bool: 4
		
		self.OneWireTemplate = function(oneWireDevice) {
			if (oneWireDevice instanceof App.ViewModels.ExpressionModel)
				return 'expressionTemplate';
			if (oneWireDevice.type()==App.Enums.EDataType.Int ||
				oneWireDevice.type()==App.Enums.EDataType.Uint ||
				oneWireDevice.type()==App.Enums.EDataType.Double ||
				oneWireDevice.type()==App.Enums.EDataType.String)
				return 'ds18b20Template';
			if (oneWireDevice.type()==4)
				return 'ds2413Template';						
			return 'folderTemplate';
		}
		
		self.btnNewFolder = function() {
			App.Helpers.LoadPage('#demopage','folderdetail.html', new App.ViewModels.FolderModel(self.socket,this), function(){									
				var item = ko.dataFor($('#demopagecontent').get(0));
				if (item == null)						
				{				
					item = new App.ViewModels.FolderModel(self.socket,this);
					ko.applyBindings(item,$('#demopagecontent').get(0));
				}
				else
				{
					item.CopyDataFrom(item.CreateEmptyDto());
				}
				item.parentId(self.parentFolderId);
			});
			return true;
		}
		
		self.btnInsertExpression = function() {
			App.Helpers.LoadPage('#demopage','expressiondetail.html', new App.ViewModels.ExpressionModel(self.socket,this), function(){
				var item = ko.dataFor($('#demopagecontent').get(0));
				if (item == null)						
				{				
					item = new App.ViewModels.ExpressionModel(self.socket,this);
					ko.applyBindings(item,$('#demopagecontent').get(0));
				}
				else
				{
					//item.CopyDataFrom(item.CreateEmptyDto());
				}
				item.parentId(self.parentFolderId);
			});
			return true;
		}		
		
		self.btnInsertDevice = function() {			
			$.mobile.changePage( '#folderSelect', { 
					role: "dialog",
					transition: "pop",				
					changeHash: true			
			});	
			var list = new App.ViewModels.OneWireListModel(self.socket)			
			list.Mode(App.Enums.OneWireListMode.DeviceSelect);
			list.OnItemSelected = function(event) {
				self.Load(self.parentFolderId);
			};
			list.Load(null,function(){
				App.Helpers.LoadPage("#folderSelect" ,'onewirelist.html',list,function(){														
					list.Refresh('#folderSelect');		
				});
				list.parentFolderId = self.parentFolderId;
			});				 			 																		
			return true;						
		}
		
		self.btnNewDevice = function() {
			App.Helpers.LoadPage('#demopage','onewiredetail.html', new App.ViewModels.OneWireDevice(self.socket,null,this), function(){
				var item = ko.dataFor($('#demopagecontent').get(0));
				if (item == null)						
				{
					item = new App.ViewModels.OneWireDevice(self.socket,null,this);								
					ko.applyBindings(item,$('#demopagecontent').get(0));							
				}
				else
				{
					item.CopyDataFrom(item.CreateEmptyDto());
				}
				item.IsNew(true);
				item.internal(true);
				item.IsEnabled(self.isLogged);
				item.SaveEnabled(self.isLogged);
			});
			return true;
		}
		
		//accept data
		self.onmessage = function(msg)
		{
			console.log(msg);			
			// Editable data
			var parsedlist = JSON.parse(msg);			
			
			
			self.socket.setOnMessage(null);
			
			 self.Refresh();
		};
		
		self.LoadFolders = function(folderId,callback) {			
			//seznam adresaru v adresari
			self.socket.read(App.ViewModels.FoldersModel.GetPath(),self.parentFolderId,function(data){			
				data.forEach(function (x) {
					//zjistim, jestli jiz neni v seznamu
					var match = ko.utils.arrayFirst(self.oneWireList(), function(item) {
						return item.id() == x.id;
					});
					if (match==null) {						
						match = new App.ViewModels.FolderModel(self.socket,self);
						match.CopyDataFrom(x);
						self.oneWireList.push(match);						
					} else
					{
						match.CopyDataFrom(x);
					}
				});				
				//self.sort();
				//self.Refresh();
				if (callback!=null) callback();
			});
		}
		
		self.LoadExpressions = function(folderId,callback) {
			if (self.parentFolderId!=null) {				
				self.socket.read('expression/folder',self.parentFolderId,function(data){			
					data.forEach(function (x) {
						//zjistim, jestli jiz neni v seznamu
						var match = ko.utils.arrayFirst(self.oneWireList(), function(item) {
							return item.id() == x.id;
						});
						if (match==null) {						
							match = new App.ViewModels.ExpressionModel(self.socket,self);
							//match.CopyDataFrom(x);
							App.Helpers.CopyFromDto(x,match);
							self.oneWireList.push(match);						
						} else
						{
							match.CopyDataFrom(x);
						}
					});				
					//self.sort();
					//self.Refresh();
					if (callback!=null) callback();
				});
			} else if (callback!=null) callback();						;
		}
		
		self.LoadDevicesInFolder = function(folderId,callback) {
			if (self.parentFolderId!=null)
			{
				self.socket.read(self.folderDevicesPath,self.parentFolderId,function(data){			
					data.forEach(function (x) {
						//zjistim, jestli jiz neni v seznamu
						var match = ko.utils.arrayFirst(self.oneWireList(), function(item) {
							return item.id() === x.id;
						});
						if (match==null) {						
							match = new App.ViewModels.OneWireDevice(self.socket,self)						
							match.CopyDataFrom(x);
							self.oneWireList.push(match);						
						} else
						{
							match.CopyDataFrom(x);
						}																
					});				
					//self.sort();
					//self.Refresh();	
					if (callback!=null) callback();					
				});
			} else if (callback!=null) callback();						
		}
		
		self.LoadParentFolder = function(folderId,callback) {
			if (self.parentFolderId!=null)
			{					
				self.socket.read(App.ViewModels.FolderModel.GetPath(),self.parentFolderId,function(data){			
					self.parent = new App.ViewModels.FolderModel(self.socket,self);
					self.parent.CopyDataFrom(data[0]);
					self.parent.name('..');					
					self.oneWireList.push(self.parent);
					//self.sort();
					//self.Refresh();	
					if (callback!=null) callback();						
				});					
			}
			else if (callback!=null) callback();						
		}
		
		//load function
		self.Load = function(folderId,callback)
		{
				self.parentFolderId = folderId;
			self.isLogged(App.Instance.GetLogInPage().logged());
			self.parent = null;	
			//nacte data pro adresar
			if (self.Mode()==App.Enums.OneWireListMode.Folders)
			{
				self.oneWireList.removeAll();				
				self.LoadFolders(folderId,function() {
					self.Refresh();
					self.LoadExpressions(folderId,function() {
						self.Refresh();
						self.LoadDevicesInFolder(folderId,function() {
							self.Refresh();
							self.LoadParentFolder(folderId,function() {
								self.sort();
								self.Refresh();
							});
						});
					});
				});								
			} 
			//nacte data pro seznam zarizeni nebo vyber zarizeni
			else if (self.Mode()==App.Enums.OneWireListMode.Devices||
					 self.Mode()==App.Enums.OneWireListMode.DeviceSelect)
			{
				self.oneWireList.removeAll();
				self.socket.read(self.path,null,function(data){			
					data.forEach(function (x) {
						//zjistim, jestli jiz neni v seznamu
						var match = ko.utils.arrayFirst(self.oneWireList(), function(item) {
							return item.id() === x.id;
						});
						if (match==null) {						
							match = new App.ViewModels.OneWireDevice(self.socket,self)						
							match.CopyDataFrom(x);
							self.oneWireList.push(match);						
						} else
						{
							match.CopyDataFrom(x);
						}																
					});				
					self.sort();
					self.Refresh();		
					if (callback!=null) callback();
				});
			}								
		}
		
		//sort
		self.sort = function()
		{
			self.oneWireList.sort(function(left,right){
				if (left.name()=='..') return -1;
				if (left instanceof App.ViewModels.ExpressionModel)
					return 1;
				if (left instanceof App.ViewModels.FolderModel && right instanceof App.ViewModels.OneWireDevice)
					return -1;
				if (right instanceof App.ViewModels.FolderModel && left instanceof App.ViewModels.OneWireDevice)
					return 1;
				if (left.name()>right.name()) return 1;
				if (left.name()<right.name()) return -1;
				return 0;
			});
		}
		
		//save
		self.Save = function()
		{
			self.socket.setOnMessage(self.onmessage);
		}	
        
        return self;
    };

})(App)

   

    

    

    //alert('testc');

     