

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
		UInt: 1,
		Double: 2,
		String: 3,
		Bool: 4
	}
	
'[]'


	

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
		self.parentFolderId = "";
		self.path = 'onewiredevices';
		self.folderDevicesPath = 'onewiredevices/folder';
		self.GetPath = function() { return self.path; }
		self.AutoRefresh = function(enable) {
			if (enable)
			{				
				self.interval = setInterval(function(){
					self.Load(self.parentFolderId);					
					self.Refresh();
				},5000);	
			}
			else
			{
				if (self.interval != null)
				{
					clearInterval(self.interval);
					self.interval = null;
				}
			}
		}
		self.Refresh = function(holderId) {					
			try
			{
				$('ul[data-role="listview"]').listview('refresh');									
			}
			catch(x)
			{					
				$('ul[data-role="listview"]').listview().listview('refresh');					
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
					event.LoadPage();
					
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
			if (oneWireDevice instanceof App.ViewModels.FolderModel)
				return 'folderTemplate';
			if (oneWireDevice.data.type()==App.Enums.EDataType.Int ||
				oneWireDevice.data.type()==App.Enums.EDataType.UInt ||
				oneWireDevice.data.type()==App.Enums.EDataType.Double ||
				oneWireDevice.data.type()==App.Enums.EDataType.String)
				return 'ds18b20Template';
			if (oneWireDevice.data.type()==4)
				return 'ds2413Template';						
			throw "Not supported type";
		}
		
		self.btnNewFolder = function() {
			var foldersInstance = App.Instance.GetFolders();
			var newfolder = new App.ViewModels.FolderModel(self.socket,null);
			newfolder.data.parentId(foldersInstance.actual!=null?foldersInstance.actual.data.id():null);
			newfolder.LoadPage();			
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
		
		self.btnInsertDevice = function(event) {			
			$.mobile.changePage( '#folderSelect', { 
					role: "dialog",
					transition: "pop",				
					changeHash: true			
			});	
			event.AutoRefresh(false);
			var list = new App.ViewModels.OneWireListModel(self.socket);			
			list.Mode(App.Enums.OneWireListMode.DeviceSelect);
			list.OnItemSelected = function(event) {			
				event.list.AutoRefresh(false);			
				self.Load(self.parentFolderId,function() {					
				});	
			};			
			
			App.Helpers.LoadPage("#folderSelect" ,'onewirelist.html',list,function(){																		
				//vypnu autorefresh u listu dialogu po zavreni
				$('#folderSelect').on('pagehide', function(){
					list.AutoRefresh(false);
					self.AutoRefresh(true);
				});
				list.Load(null,function(){				
					list.Refresh('#folderSelect');		
					list.parentFolderId = self.parentFolderId;					
				});				
			});				 			 																		
			return true;						
		}
		
		self.btnNewDevice = function() {
			// App.Helpers.LoadPage('#demopage','onewiredetail.html', new App.ViewModels.OneWireDevice(self.socket,null,this), function(){
				// var item = ko.dataFor($('#demopagecontent').get(0));
				// if (item == null)						
				// {
					// item = new App.ViewModels.OneWireDevice(self.socket,null,this);								
					// ko.applyBindings(item,$('#demopagecontent').get(0));							
				// }
				// else
				// {
					// item.CopyDataFrom(item.CreateEmptyDto());
				// }
				// item.IsNew(true);
				// item.internal(true);
				// item.IsEnabled(self.isLogged);
				// item.SaveEnabled(self.isLogged);
			// });
			App.Instance.SetPageContainerBeforeShowFunc( function( event, ui ) {
				var element = ui.toPage;			
				ko.applyBindings(new App.ViewModels.OneWireDevice(self.socket,null,this),element[0]);				
				$('input[type="button"]').button().button('refresh');
			} );
			
			$.mobile.pageContainer.pagecontainer("change","onewiredetail.html",{changeHash:false,reload:true});
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
			var foldersInstance = App.Instance.GetFolders();
			foldersInstance.Load();
			self.oneWireList = foldersInstance.folders;
		}
		
		self.LoadExpressions = function(folderId,callback) {
			// if (self.parentFolderId!=null) {				
				// self.socket.read('expression/folder',self.parentFolderId,function(response){			
					// if (response.success)
					// {
						// data = response.message;
						// data.forEach(function (x) {
							// //zjistim, jestli jiz neni v seznamu
							// var match = ko.utils.arrayFirst(self.oneWireList(), function(item) {
								// return item.id() == x.id;
							// });
							// if (match==null) {						
								// match = new App.ViewModels.ExpressionModel(self.socket,self);
								// //match.CopyDataFrom(x);
								// App.Helpers.CopyFromDto(x,match);
								// self.oneWireList.push(match);						
							// } else
							// {
								// match.CopyDataFrom(x);
							// }
						// });
						// //odstranim ty co nejsou v doslych datech
						// self.oneWireList().forEach(function(x){
							// var match = ko.utils.arrayFirst(data, function(item) {
								// return item.id === x.id();
							// });
							// if (match==null && x.name()!='..' && x instanceof App.ViewModels.ExpressionModel) {				
								// self.oneWireList.remove(x);
							// }
						// });						
						// //self.sort();
						// //self.Refresh();
						// if (callback!=null) callback();
					// }
				// });
			// } else if (callback!=null) callback();						;
		}
		
		self.LoadDevicesInFolder = function(folderId,callback) {
			// if (self.parentFolderId!=null)
			// {
				// self.socket.read(self.folderDevicesPath,self.parentFolderId,function(response){			
					// if (response.success)
					// {
						// data = response.message;
						// data.forEach(function (x) {
							// //zjistim, jestli jiz neni v seznamu
							// var match = ko.utils.arrayFirst(self.oneWireList(), function(item) {
								// return item.data.id() === x.id;
							// });
							// if (match==null) {						
								// match = new App.ViewModels.TagModel(x);//new App.ViewModels.OneWireDevice(self.socket,self)						
								// //match.CopyDataFrom(x);
								// self.oneWireList.push(match);						
							// } else
							// {
								// match.CopyDataFrom(x);
							// }																
						// });	
						// //odstranim ty co nejsou v doslych datech
						// self.oneWireList().forEach(function(x){
							// var match = ko.utils.arrayFirst(data, function(item) {
								// return item.id === x.data.id();
							// });
							// if (match==null && x instanceof App.ViewModels.OneWireDevice) {				
								// self.oneWireList.remove(x);
							// }
						// });
						// //self.sort();
						// //self.Refresh();	
						// if (callback!=null) callback();					
					// }
				// });
			// } else if (callback!=null) callback();						
		}
		
		self.LoadParentFolder = function(folderId,callback) {
			// if (self.parentFolderId!=null)
			// {					
				// self.socket.read(App.ViewModels.FolderModel.GetPath(),self.parentFolderId,function(response){			
					// if (response.success)
					// {
						// data = response.message;
						// var match = ko.utils.arrayFirst(self.oneWireList(), function(item) {
								// return item.name() === '..';
							// });
						// if (match==null)
						// {
							// self.parent = new App.ViewModels.FolderModel(self.socket,self);						
							// self.oneWireList.push(self.parent);
							// //self.sort();
							// //self.Refresh();										
						// }
						// else self.parent = match;
						
						// if (self.parent!=null) {
							// self.parent.CopyDataFrom(data[0]);						
							// self.parent.name('..');
							// //self.parent.parentId(null);
						// }
						// if (callback!=null) callback();											
					// }
				// });					
			// }
			// else if (callback!=null) callback();						
		}
		
		self.LoadPage = function() {
			// App.Helpers.LoadPage($("#demopage"),'onewirelist.html',App.Instance.GetOneWireList(),function(){
				// App.Instance.GetOneWireList().Mode(App.Enums.OneWireListMode.Folders);
				// App.Instance.GetOneWireList().Load();				 
				// App.Instance.GetOneWireList().Refresh();						
			// });
			var activePage = $.mobile.pageContainer.pagecontainer( "getActivePage" );
			if (activePage!=null && activePage[0].id=='onewirelistpage') return false;
			
			App.Instance.SetPageContainerBeforeShowFunc( function( event, ui ) {
				var element = ui.toPage;							
				ko.applyBindings(self,element[0]);
				
				self.Refresh();						
			} );
					
			$.mobile.pageContainer.pagecontainer("change","onewirelist.html",{changeHash:false,reload:true});			
			return true;
		};
		
		self.myPostProcessingLogic = function(elements) {
		// "elements" is an array of DOM nodes just rendered by the template
		// You can add custom post-processing logic here
			//$("div[id$='listinput']").trigger('create');
		}
		
		self.Clear = function() {
			self.oneWireList.removeAll();	
		}
		
		//load function
		self.Load = function(folderId,callback)
		{
			//if (self.parentFolderId != folderId || (folderId == undefined && self.parentFolderId!=undefined)) 
			//	self.oneWireList.removeAll();				
			self.parentFolderId = folderId;
			self.isLogged(App.Instance.GetLogInPage().logged());
			self.parent = null;	
			//nacte data pro adresar
			if (self.Mode()==App.Enums.OneWireListMode.Folders)
			{				
				self.LoadFolders(folderId,function() {
					self.Refresh();
					self.LoadExpressions(folderId,function() {
						self.Refresh();
						self.LoadDevicesInFolder(folderId,function() {
							self.Refresh();
							self.LoadParentFolder(folderId,function() {
								self.sort();
								self.Refresh();
								//$('#demopage').trigger('pagecreate');								
							});
						});
					});
				});								
			} 
			//nacte data pro seznam zarizeni nebo vyber zarizeni
			else if (self.Mode()==App.Enums.OneWireListMode.Devices||
					 self.Mode()==App.Enums.OneWireListMode.DeviceSelect)
			{
				//self.oneWireList.removeAll();
				self.socket.read(self.path,null,function(response){			
					if (response.success)
					{
						data = response.message;
						data.forEach(function (x) {
							x = x.Tags[0];
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
						//odstranim ty co nejsou v doslych datech
						self.oneWireList().forEach(function(x){							
							var match = ko.utils.arrayFirst(data, function(item) {
								item = item.Tags[0];
								return item.id === x.id();
							});
							if (match==null) {				
								self.oneWireList.remove(x);
							}
						});
						
						self.sort();
						self.Refresh();		
						if (callback!=null) callback();
					}
				});
			}								
		}
		
		//sort
		self.sort = function()
		{
			self.oneWireList.sort(function(left,right){
				if (left.name()=='..') return -1;
				if (left instanceof App.ViewModels.ExpressionModel && !(right instanceof App.ViewModels.ExpressionModel))
					return 1;
				if (right instanceof App.ViewModels.ExpressionModel && !(left instanceof App.ViewModels.ExpressionModel))
					return -1;
				if (left instanceof App.ViewModels.FolderModel && right instanceof App.ViewModels.TagModel)
					return -1;
				if (right instanceof App.ViewModels.FolderModel && left instanceof App.ViewModels.TagModel)
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

     