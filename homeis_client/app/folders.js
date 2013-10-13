(function (App) {

	App.ViewModels.FolderModel = function(psocket,list)
	{
		var self = this;
		self.socket = psocket;
		self.list = list;
		if (list!=null)
			self.path = list.GetPath();
		self.type = ko.observable(null);
		self.name = ko.observable();
		self.id = ko.observable();
		self.parentId = ko.observable();
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
			 var dto = self.CreateDto(this);
			if (dto.id==null || dto.id=='')
				self.socket.create("folder",dto,function(data){
					alert('Error saving');
				});
			else
				self.socket.update("folder",self.id(),dto,function(data){
					alert('Error saving');
				});
				
			//alert(self.name());
			//self.list.Load(self.parentId());
			App.Helpers.LoadPage('#demopage','onewirelist.html',App.Instance.GetOneWireList(),function(){
				App.Instance.GetOneWireList().Load(self.parentId());				 
				App.Instance.GetOneWireList().Refresh();		
			});
			return true;
		}
		self.folderEdit = function(event,ui) {
			if (event.name()=='..') return false;
			App.Helpers.LoadPage('#demopage','folderdetail.html', self, function(){
				//$.mobile.loadPage('#folderDetail', { showLoadMsg: false } );
				var isLogged = App.Instance.GetLogInPage().logged();
				var element = $('#demopagecontent').get(0);
				var item = ko.dataFor(element);
				if (item == null)
				{
					//ko.toJS(event)
					item = new App.ViewModels.FolderModel(self.socket,self.list);				
					ko.applyBindings(item,element);											
				}
				item = ko.dataFor(element);			
				item.CopyDataFrom(ko.toJS(event));			
			});			
			
			return true;
		}
		self.folderSelect = function(event,ui) {
			if (event.name()=='..')
				self.list.Load(event.parentId());
			else
				self.list.Load(event.id());
			self.list.Refresh();
			return true;
		}
		self.btnDelete= function() {
			var dto = self.CreateDto(this);
			if (dto.id==null || dto.id=='')
				alert('Neulozenou slozku nelze smazat')
			else
				if (confirm('Opravdu si pøejete smazat složku: ' + self.name() + '?'))
				{
					self.socket.destroy("folder",self.id(),function(data){
						alert('Error delete');
					});
				}
				
			//alert(self.name());
			LoadPage('#demopage','onewirelist.html',App.Instance.GetOneWireList(),function(){
				App.Instance.GetOneWireList().Load(self.parentId());				 
				App.Instance.GetOneWireList().Refresh();		
			});
			return true;
		}
		self.btnStorno = function() {
			LoadPage('#demopage','onewirelist.html',App.Instance.GetOneWireList(),function(){
				App.Instance.GetOneWireList().Load(self.parentId());				 
				App.Instance.GetOneWireList().Refresh();		
			});
			return true;
		}
		App.ViewModels.FolderModel.prototype.CreateParent = function()
		{
		}
		App.ViewModels.FolderModel.prototype.CopyDataFrom = function(dataObject)
		{
			var self = this;			
			self.name(dataObject.name);
			self.id(dataObject.id);
			self.parentId(dataObject.parentId);
		}
		App.ViewModels.FolderModel.prototype.CreateDto = function(src)
		{
			var dto = {};
			dto.id = src.id();
			dto.name = src.name();
			dto.parentId = src.parentId();
			return dto;
		}
		App.ViewModels.FolderModel.prototype.CreateEmptyDto = function(src)
		{
			var dto = {};
			dto.id = '';
			dto.name = '';
			dto.parentId = '';
			return dto;
		}
	}
	
	//model for select folder
	App.ViewModels.FoldersModel = function(psocket,list)
	{		
		var self = this;		
		self.folders = ko.observableArray();
		self.socket = psocket;				
        self.isLogged = ko.observable(false); 
		self.SelectedItem = ko.observable();
		self.path = 'folders';
		self.GetPath = function() { return self.path; }
		self.Refresh = function() {			
			$('#foldersId').listview('refresh');
		}
		
		//load function
		self.Load = function(folderId)
		{
			self.parentId = folderId;
			
			self.parent = null;			
			self.socket.read(App.ViewModels.FoldersModel.GetPath(),folderId,function(data){			
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
				self.sort();
				self.Refresh();
			});																				
		}
		
		self.sort = function()
		{
			self.oneWireList.sort(function(left,right){
				if (left.name()=='...') return -1;				
				if (left.name()>right.name()) return 1;
				if (left.name()<right.name()) return -1;
				return 0;
			});
		}
	}
	
	App.ViewModels.FoldersModel.GetPath = function() { return 'folders'; }

})(App)