(function (App) {

	App.ViewModels.FolderModel = function(psocket,data)
	{
		var self = this;
		self.socket = psocket;	
		self.data = {};		
		self.data.type = ko.observable(null);
		self.data.name = ko.observable();
		self.data.id = ko.observable();		
		self.data.parentId = ko.observable();
		
		
		self.prevPage = null;
		self.IsNew = ko.observable(data==null);
		self.NameEditEnabled = ko.observable(true);
		self.SaveEnabled = ko.observable(true);
		self.DeleteEnabled = ko.observable(true);
		App.ViewModels.FolderModel.GetPath = function()
		{
			return "folder";
		};
		self.image = ko.computed(function() {
			return 'app/icons/Folder_Icon.png'			
		}, this);
		self.btnSave = function(){			 
			if (self.IsNew())
			{
				self.socket.create("folder",self.data,function(response){
					if (response.success)
					{
						if (self.data.parentId()!=null)
						{
							var parent = new App.ViewModels.FolderModel(self.socket,{});
							parent.Load(self.data.parentId(),function() {
								App.Instance.GetFolders().Load(parent);	
								App.Instance.GetOneWireList().LoadPage();
							});
						}
						else
						{
							App.Instance.GetFolders().Load();	
							App.Instance.GetOneWireList().LoadPage();
						}
					}
					else alert('Chyba při ukaládání');					
				});
			}
			else
			{
				self.socket.update("folder",self.data.id(),self.data,function(response){
					if (response.success)
					{
						if (self.data.parentId()!=null)
						{
							var parent = new App.ViewModels.FolderModel(self.socket,{});
							parent.Load(self.data.parentId(),function(){
								App.Instance.GetFolders().Load(parent);	
								App.Instance.GetOneWireList().LoadPage();
							});
						}
						else
						{
							App.Instance.GetFolders().Load();	
							App.Instance.GetOneWireList().LoadPage();
						}
					}
					else alert('Chyba při ukaládání');
				});
			}
				
			// //alert(self.name());
			// //self.list.Load(self.parentId());
			// App.Helpers.LoadPage('#demopage','onewirelist.html',App.Instance.GetOneWireList(),function(){
				// App.Instance.GetOneWireList().Load(self.parentId());				 
				// App.Instance.GetOneWireList().Refresh();		
			// });
			return true;
		}
		self.folderEdit = function(event,ui) {
			if (self.data.name()=='..') return false;
			self.LoadPage();
			return true;
		}
		self.folderSelect = function(event,ui) {
			if (self.data.name()=='..')
			{
				if (self.data.parentId()!=null)
				{
					var parent = new App.ViewModels.FolderModel(self.socket,{});
					parent.Load(self.data.parentId(),function(){
						App.Instance.GetFolders().Load(parent);	
					});
				}
				else
				{
					App.Instance.GetFolders().Load();
				}
			}
			else
				App.Instance.GetFolders().Load(self);
			return true;
		}
		self.btnDelete = function() {
			
			if (self.IsNew())
				alert('Neuloženou složku nelze smazat')
			else if (confirm('Opravdu si přejete smazat složku: ' + self.data.name() + '?'))
			{				
					self.socket.destroy("folder",self.data.id(),function(response){
						if (response.success) 			
						{
							if (self.data.parentId()!=null)
							{
								var parent = new App.ViewModels.FolderModel(self.socket,{});
								parent.Load(self.data.parentId(),function(){
									App.Instance.GetFolders().Load(parent);	
									App.Instance.GetOneWireList().LoadPage();
								});
							}
							else
							{
								App.Instance.GetFolders().Load();	
								App.Instance.GetOneWireList().LoadPage();
							}
						}
						else 
							alert('Chyba pri mazani');			
					 });
			}										
			return true;
		}
		self.btnStorno = function() {
			App.Instance.GetOneWireList().LoadPage();
			return true;
		}
		self.LoadPage = function() {
			App.Instance.SetPageContainerBeforeShowFunc( function( pevent, ui ) {
					var element = ui.toPage;	
					var item = ko.dataFor(ui.prevPage[0]);
					self.prevPage = item;					
					ko.applyBindings(self,element[0]);										
			} );
			$.mobile.pageContainer.pagecontainer("change","folderdetail.html",{changeHash:false,reload:true});
		};
		self.Mapping = function(pdata)
		{						
			ko.mapping.fromJS(pdata, {}, self.data);
			delete (self.data.__ko_mapping__);
		};
		self.Load = function(pid,feedback) {					
			self.socket.read("folder",pid,function(response){			
				if (response.success)
				{							
					self.Mapping(response.message[0]);
					if (feedback!=null) feedback();
				}
				else
				{
					alert('Chyba při ční dat');
				}
			});						
		};
		//load v konstruktoru
		self.Mapping(data);
	}
	
	//model for select folder
	App.ViewModels.FoldersModel = function(psocket)
	{		
		var self = this;					
		self.folders = ko.observableArray();
		self.socket = psocket;				
        self.isLogged = ko.observable(false); 
		self.SelectedItem = ko.observable();		
		self.ActualFolder = null;
		self.Refresh = function() {			
			try
			{
				$('ul[data-role="listview"]').listview('refresh');									
			}
			catch(x)
			{					
				$('ul[data-role="listview"]').listview().listview('refresh');					
			}	
		}
		
		self.Reload = function()
		{
			self.Load(self.ActualFolder);
		}
		
		//load function
		self.Load = function(folder,callback)
		{	
			var backDir = null;				
			if (self.ActualFolder!=folder)
				self.folders.removeAll();
			self.ActualFolder = folder;			
			self.socket.read("folder/allitems",folder!=null?folder.data.id():null,function(data){			
				//odstranim ty co nejsou v doslych datech
				self.folders().forEach(function(x){
					var match = ko.utils.arrayFirst(data.message, function(item) {
						return item.id === x.data.id();
					});
					if (match==null && x.data.name()!='..') {				
						self.folders.remove(x);
					}
				});	
				data.message.forEach(function (x) {					
					//zjistim, jestli jiz neni v seznamu
					var match = ko.utils.arrayFirst(self.folders(), function(item) {
						return item.data.id() == x.id;
					});
					//kdyz ne pridam
					if (match==null) {
						switch(x.NodeName)
						{
							case "folder":
								match = new App.ViewModels.FolderModel(self.socket,x);						
							break;
							case "value":
								match = new App.ViewModels.TagModel(self.socket,x);
							break;
							case "expression":
								match = new App.ViewModels.ExpressionModel(self.socket,x);
							break;
						}
						
						match.parent = folder;
						self.folders.push(match);
					} //jinak provedu update
					else					
					{
						match.parent = folder;
						match.Mapping(x);
					}					
				});
									
				var upfolder = ko.utils.arrayFirst(self.folders(), function(item) {
						return item.data.name() === "..";
				});					
				
				//odstranim nebo pridam .. upfolder
				if ( folder != null )					
				{
					if (upfolder==null)
					{
						var upfolder = new App.ViewModels.FolderModel(self.socket,ko.toJS(folder.data));
						upfolder.data.name('..');							
						self.folders.push(upfolder);
					}
				}
				else
				{
					self.folders.remove(upfolder);
				}
				
				self.sort();
				self.Refresh();
				if (callback!=null) callback();
			});																				
		}
		
		self.sort = function()
		{
			self.folders.sort(function(left,right){
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
		
		// self.sort = function()
		// {
			// self.folders.sort(function(left,right){
				// if (left.data.name()=='...') return -1;				
				// if (left.data.name()>right.data.name()) return 1;
				// if (left.data.name()<right.data.name()) return -1;
				// return 0;
			// });
		// }
	}		

})(App)