 (function (App) {
	 App.ViewModels.ExpressionModel = function (psocket,list) {	
		self = this;			
		self.socket = psocket;		
		self.list = list;
		
		self.data = {
			parentId:'',
			id:'',
			expression:'test exxpr',
			running: false,
			description:'',
			name:''
		};
				
		App.Helpers.CreatePropertiesFromData(this);
		//self.id = self.data.id;
		//self.name = self.data.name;
		self.IsNew = ko.observable(false);
		self.IsEnabled = ko.observable(false);
		self.EditEnabled = ko.observable(true);		
		self.SaveEnabled = ko.observable(true);		
		self.image = ko.computed(function() {
			return 'app/icons/cog.png'			
		}, this);
				
		self.runningChanged = function(newValue) {			
			alert(self.running());
			//console.log(newValue);			
		};
		
		//self.running.subscribe(self.runningChanged);
		
		self.CreateEmptyDto = function(src) {
			return {};
		}
		
		App.ViewModels.ExpressionModel.prototype.CopyDataFrom = function(pdataObject) {
			
		}
		
		self.expressionSelect = function(event,ui) {
		}
		self.expressionDelete = function(event,ui) {
		}
		
		self.CreateDto = function(src) {			
			return App.Helpers.CreateDtoFromProperties(src);
		}
		
		self.btnRun = function(event,ui) {
			self.socket.read('expression/folderrun',self.parentId(),function(data){							
				if (data.message!=null)
					alert(data.message);
				else
					alert("Script byl úspìšnì spuštìn");
			});
		}
		
		self.btnStorno = function(event,ui) {
			self.parentId(event.parentId());
			App.Helpers.LoadPage('#demopage','onewirelist.html',App.Instance.GetOneWireList(),function(){
				App.Instance.GetOneWireList().Load(self.parentId(),function(){
					App.Instance.GetOneWireList().Refresh();		
				});				
			});
			return true;
		}
		self.btnSave = function(event,ui) {
			var dto = self.CreateDto(self);
			if (dto.id==null || dto.id=='')
				self.socket.create("expression",dto,function(data){
					alert('Error saving'+data);
				});
			else
				self.socket.update("expression",self.id(),dto,function(data){
					alert('Error saving'+data);
				});
									
			return true;
		}
		self.btnDelete = function(event,ui) {
		}
	 }
})(App)