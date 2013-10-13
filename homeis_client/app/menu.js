(function (App) {
	 App.ViewModels.MenuModel = function (psocket) {	
		self = this;				
		
		self.btnFolders = function() {
			App.Helpers.LoadPage($("#demopage"),'onewirelist.html',App.Instance.GetOneWireList(),function(){
				App.Instance.GetOneWireList().Mode(App.Enums.OneWireListMode.Folders);
				App.Instance.GetOneWireList().Load();				 
				App.Instance.GetOneWireList().Refresh();						
			});
			$( "#panel_menu" ).panel( "close" );
		}
		
		self.btnonewirelist = function() {
			App.Helpers.LoadPage($("#demopage"),'onewirelist.html',App.Instance.GetOneWireList(),function(){
				App.Instance.GetOneWireList().Mode(App.Enums.OneWireListMode.Devices);
				App.Instance.GetOneWireList().Load();				 
				App.Instance.GetOneWireList().Refresh();						
			});
			$( "#panel_menu" ).panel( "close" );
			
			return true;
		}
		
		self.btnlogindialog = function() {
			App.Helpers.LoadPage($("#demopage"),'logindialog.html',App.Instance.GetLogInPage());
			$( "#panel_menu" ).panel( "close" );
			return false;
		}
	 }
})(App)