

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
					 App.Instance.GetFolders().Reload();
				 },200);	
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
				$('ul[data-role="listview"]').listview();//listview('refresh');					
			}							
		}
		
		self.expressionSelect = function(expression, event) {						 
			expression.LoadPage();
		}
		
		self.itemSelecting = false;
		
		self.itemSelect = function(event,ui) {						 
			
				if (!self.itemSelecting)
				{						
					event.LoadPage();
					
				} 
				else
				{
					self.itemSelecting = false;
					if (confirm('Opravdu si přejete vložit: '+ event.name() + '?')) {						
						//$('#folderSelect').dialog('close');
						// self.socket.create("onewiredevices/folder",dto,function(data){
							// alert('chyba pri ukladani');
						// });
						var dto = { DevValueId: event.data.id() };
						self.socket.update("folder/valueid",App.Instance.GetFolders().ActualFolder.data.id(),dto,function(response){
							if (response.success)
							{
								App.Instance.GetFolders().Reload();
							}
							else alert('Chyba při ukaládání');
						});
						if (self.OnItemSelected!=null) self.OnItemSelected(event);
					}
				}			
		}
		
		self.removeValueId = function(event,ui) {			
			if (confirm('Opravdu si přejete odstranit datový bod ' + event.name() + '?'))
			{								
				self.socket.destroy('onewiredevices/folder',event.data.DirValueId(),function(response){
					if (response.success)
					{
						App.Instance.GetFolders().Reload();
					}
					else alert('Chyba při ukaládání');					
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
			newfolder.data.parentId(foldersInstance.ActualFolder !=null?foldersInstance.ActualFolder.data.id():null);
			newfolder.LoadPage();			
			return true;
		}
		
		self.btnInsertExpression = function() {
			var foldersInstance = App.Instance.GetFolders();
			var newexpression = new App.ViewModels.ExpressionModel(self.socket,null);
			newexpression.data.parentId(foldersInstance.ActualFolder !=null?foldersInstance.ActualFolder.data.id():null);
			newexpression.LoadPage();
			return true;
		}		
		
		self.btnInsertDevice = function(event) {			
			self.itemSelecting = true;
			App.Instance.GetDevicesController().LoadDialog();			 			 																	
			return true;						
		}
		
		self.btnNewDevice = function() {			
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
			foldersInstance.Load(null,function(){
				self.oneWireList = foldersInstance.folders;
				if (callback!=null) callback();
			});
		}							
		
		self.UnloadPage = function() {
			self.AutoRefresh(false);
		};
		
		self.LoadPage = function() {			
			
			var activePage = $.mobile.pageContainer.pagecontainer( "getActivePage" );
			if (activePage!=null && activePage[0].id=='onewirelistpage') return false;
			App.Instance.SetPageContainerShowFunc(function(event,ui){								
				if (ui.prevPage.UnloadPage!=undefined)
					ui.prevPage.UnloadPage();
				var element = ui.toPage;
				ko.applyBindings(self,element[0]);												
				self.Refresh();
			});
			App.Instance.SetPageContainerBeforeShowFunc( function( event, ui ) {
				self.AutoRefresh(true);
				self.Refresh();
			} );				
			//self.Load(null,function(){										
			$.mobile.pageContainer.pagecontainer("change","onewirelist.html",{changeHash:false,reload:true});							
			//});					
			
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
						// self.LoadDevicesInFolder(folderId,function() {							
							// self.Refresh();
							 self.sort();								
							// if (callback!=null) callback();														
						// });
						if (callback!=null) callback();																			
				});								
			} 
			// //nacte data pro seznam zarizeni nebo vyber zarizeni
			// else if (self.Mode()==App.Enums.OneWireListMode.Devices||
					 // self.Mode()==App.Enums.OneWireListMode.DeviceSelect)
			// {
				// //self.oneWireList.removeAll();
				// self.socket.read(self.path,null,function(response){			
					// if (response.success)
					// {
						// data = response.message;
						// data.forEach(function (x) {
							// x = x.Tags[0];
							// //zjistim, jestli jiz neni v seznamu
							// var match = ko.utils.arrayFirst(self.oneWireList(), function(item) {
								// return item.id() === x.id;
							// });
							// if (match==null) {						
								// match = new App.ViewModels.OneWireDevice(self.socket,self)						
								// match.CopyDataFrom(x);
								// self.oneWireList.push(match);						
							// } else
							// {
								// match.CopyDataFrom(x);
							// }																
						// });				
						// //odstranim ty co nejsou v doslych datech
						// self.oneWireList().forEach(function(x){							
							// var match = ko.utils.arrayFirst(data, function(item) {
								// item = item.Tags[0];
								// return item.id === x.id();
							// });
							// if (match==null) {				
								// self.oneWireList.remove(x);
							// }
						// });
						
						// self.sort();
						// self.Refresh();		
						// if (callback!=null) callback();
					// }
				// });
			//}								
		}
		
		//sort
		self.sort = function()
		{
			self.oneWireList.sort(function(left,right){
				if (left.data.name()=='..') return -1;
				if (left instanceof App.ViewModels.ExpressionModel && !(right instanceof App.ViewModels.ExpressionModel))
					return 1;
				if (right instanceof App.ViewModels.ExpressionModel && !(left instanceof App.ViewModels.ExpressionModel))
					return -1;
				if (left instanceof App.ViewModels.FolderModel && right instanceof App.ViewModels.TagModel)
					return -1;
				if (right instanceof App.ViewModels.FolderModel && left instanceof App.ViewModels.TagModel)
					return 1;
				if (left.data.name()>right.data.name()) return 1;
				if (left.data.name()<right.data.name()) return -1;
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

     