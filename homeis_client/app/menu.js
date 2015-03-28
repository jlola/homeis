(function (App) {
	 App.ViewModels.MenuModel = function (psocket) {	
		self = this;												
		
		self.btnFolders = function() {
			// App.Helpers.LoadPage($("#demopage"),'onewirelist.html',App.Instance.GetOneWireList(),function(){
				// App.Instance.GetOneWireList().Mode(App.Enums.OneWireListMode.Folders);
				// App.Instance.GetOneWireList().Load();				 
				// App.Instance.GetOneWireList().Refresh();						
			// });
			//$( "#panel_menu" ).panel( "close" );
			var ret = App.Instance.GetOneWireList().LoadPage();			
			if (ret==false)
			{
				$( "#panel_menu" ).panel( "close" );
			}
		}
		
		self.btnonewirelist = function() {
			//$( "#panel_menu" ).panel( "close" );	
			var ret = App.Instance.GetDevicesController().LoadPage();
			if (ret==false)
			{
				$( "#panel_menu" ).panel( "close" );
			}
			return true;
		}
		
		self.btnlogindialog = function() {
			App.Helpers.LoadPage($("#demopage"),'logindialog.html',App.Instance.GetLogInPage());
			$( "#panel_menu" ).panel( "close" );
			return false;
		}
	}
	
	
	App.ViewModels.MenuModel.prototype.Load = function () {		
		ko.applyBindings(this,$("#panel_menu")[0]);	  	 		
	}   	
})(App)