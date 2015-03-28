 (function (App) {
	 App.ViewModels.ExpressionModel = function (psocket,list) {	
		self = this;			
		self.socket = psocket;		
		self.list = list;
		
		self.data = {
			parentId:'',
			id:'',
			expression:'',
			running: false,
			description:'',
			name:'',
			errormessage:''
		};
				
		App.Helpers.CreatePropertiesFromData(this);
		//self.id = self.data.id;
		//self.name = self.data.name;
		self.IsNew = ko.observable(false);
		self.IsEnabled = ko.observable(false);
		self.EditEnabled = ko.observable(true);		
		self.SaveEnabled = ko.observable(true);		
		self.disabled = ko.observable(false);		
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
			var dto = self.CreateDto(event);
			self.socket.read('expression/run',dto.id,function(response){							
				if (!response.success)
				{
					//alert(response.message);
					var obj = JSON.parse(response.message);
					event.errormessage(obj.message);
				}
				else
				{
					event.errormessage('');
					alert("Script byl úspìšnì spuštìn");
				}
			});
		}
		
		self.btnStorno = function(event,ui) {
			self.parentId(event.parentId());
			self.GoToOnewireList();
			return true;
		}
		
		self.GoToOnewireList = function()
		{
			App.Helpers.LoadPage('#demopage','onewirelist.html',App.Instance.GetOneWireList(),function(){
				App.Instance.GetOneWireList().Load(self.parentId(),function(){
					App.Instance.GetOneWireList().Refresh();		
				});				
			});
		}
		
		self.btnSave = function(event,ui) {
			var dto = self.CreateDto(event);
			if (dto.id==null || dto.id=='')
				self.socket.create("expression",dto,function(response){
					if (response.success)
					{
						var obj = JSON.parse(response.message);
						App.Helpers.CopyFromDto(obj[0], self);
						alert('Uloženo');						
					}
					else
						alert('Chyba pøi ukaládání \n'+response.message);
				});
			else
				self.socket.update("expression",self.id(),dto,function(response){
					if (response.success)
						alert('Ulozeno');
					else
						alert('Chyba pri ukaladani'+response.message);
				});
									
			$.syntax({blockLayout: "fixed", theme: "paper"});
			
			return true;
		}
		self.btnDelete = function(event,ui) {
			self.socket.destroy('expression',self.id(),function(result){
				if (result.success)
				{
					self.GoToOnewireList();	
				}
				else
				{
					alert(result.message);
				}
			});
		}
	 }
})(App)