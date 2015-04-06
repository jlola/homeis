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
		};
		self.Load = function(pid,feedback) {					
			this.socket.read("folder",pid,function(response){			
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
		self.actual = null;
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
		
		//load function
		self.Load = function(folder)
		{	
			var backDir = null;				
			self.actual = folder;
			self.folders.removeAll();
			self.socket.read("folders",folder!=null?folder.data.id():null,function(data){			
				data.message.forEach(function (x) {
					//zjistim, jestli jiz neni v seznamu
					var match = ko.utils.arrayFirst(self.folders(), function(item) {
						return item.data.id() == x.id;
					});
					if (match==null) {
						match = new App.ViewModels.FolderModel(self.socket,x);						
						self.folders.push(match);						
					} 
					else					
					{
						match.Mapping(x);
					}
					match.parent = folder;
				});
				
				if (folder!=null) {
					var upfolder = new App.ViewModels.FolderModel(self.socket,ko.toJS(folder.data));
					upfolder.data.name('..');							
					self.folders.push(upfolder);
				}
				self.sort();
				self.Refresh();
			});																				
		}
		
		self.sort = function()
		{
			self.folders.sort(function(left,right){
				if (left.data.name()=='...') return -1;				
				if (left.data.name()>right.data.name()) return 1;
				if (left.data.name()<right.data.name()) return -1;
				return 0;
			});
		}
	}		

})(App)