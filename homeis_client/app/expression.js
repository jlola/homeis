 (function (App) 
 {
	 App.ViewModels.ExpressionModel = function (psocket,data) {	
		self = this;			
		self.socket = psocket;				
		self.parent = null;
		self.data = {
			parentId: ko.observable(),
			id:  ko.observable(),
			expression: ko.observable(),
			running: ko.observable(),
			description: ko.observable(),
			name: ko.observable(),
			errormessage: ko.observable()
		};		
		self.nametext = ko.computed(function(){
			var runningtext = self.data.running() ? 'zapnuto' : 'vypnuto';
			return self.data.name() + ' (' + runningtext + ')';
		});
		self.IsNew = ko.observable(data==null ? true : false);
		self.IsEnabled = ko.observable(false);
		self.EditEnabled = ko.observable(true);		
		self.DeleteEnabled = ko.computed(function(){
			return self.data.running()=='0';
		});
		self.SaveEnabled = ko.observable(true);		
		self.disabled = ko.observable(false);		
		self.image = ko.computed(function() {
			return 'app/icons/cog.png'
		}, this);
				
		self.runningChanged = function(newValue) {			
			alert(self.data.running());
			//console.log(newValue);			
		};
		
		//self.data.running.subscribe(self.runningChanged);			
				
		self.expressionSelect = function(event,ui) {
		}
		self.expressionDelete = function(event,ui) {
		}		
		
		self.btnRun = function(event,ui) {
			var dto = ko.toJS(self.data);
			self.socket.read('expression/run',dto.id,function(response){							
				if (!response.success)
				{
					//alert(response.message);
					var obj = JSON.parse(response.message);
					event.data.errormessage(obj.message);
				}
				else
				{
					event.data.errormessage('');
					alert("Script byl úspěšně spuštěn");
				}
			});
		}
		
		self.btnStorno = function(event,ui) {						
			App.Instance.GetOneWireList().LoadPage();
			return true;
		}				
		
		self.btnSave = function(event,ui) {			
			var dto = ko.toJS(self.data);
			dto.expression = self.editor.getValue();
			if (self.IsNew())
			{
				self.socket.create("expression",dto,function(response){
					if (response.success)
					{
						var obj = JSON.parse(response.message);
						self.Mapping(obj);
						alert('Uloženo');						
					}
					else
						alert('Chyba při ukaládání \n'+response.message);
				});
			}
			else
			{
				self.socket.update("expression",self.data.id(),dto,function(response){
					if (response.success)
						alert('Ulozeno');
					else
						alert('Chyba pri ukaladani'+ response.message);
				});
			}					
						
			return true;
		}
		
		self.btnDelete = function(event,ui) {
			if (confirm('Opravdu si přejete smazat výraz: ' + self.data.name() + '?'))
			{
				self.socket.destroy('expression',self.data.id(),function(result){
					if (result.success)
					{
						App.Instance.GetOneWireList().LoadPage();
					}
					else
					{
						alert(result.message);
					}
				});
			}
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
						
					} );
			App.Instance.SetPageContainerShowFunc( function( event, ui ) {
				self.editor = CodeMirror.fromTextArea(document.getElementById("expression"), {
					lineNumbers: true,							
					mode: "lua",
					extraKeys: {"Ctrl-Space": "autocomplete"},
				});
				//self.editor.text = {'tag1','tag2','tag3'};
				//self.editor.showHint = true;
			} );
			$.mobile.pageContainer.pagecontainer("change","expressiondetail.html",{changeHash:false,reload:true});
		};
		
		self.Mapping = function(pdata)
		{
			ko.mapping.fromJS(pdata, {}, self.data);
		};
		
		self.Mapping(data);	
	 }
})(App)