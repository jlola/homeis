(function (App) {

    App.DataObjects.HeaderDto = function () {
        var self = this;        
        self.UserName = '';        
        return self;
    }
	
    App.ViewModels.HeaderModel = function (psocket) {
        self = this;
        self.socket = psocket;        	
        self.login = ko.observable('Hlavička');		
		return self;
	}
	
	App.ViewModels.HeaderModel.prototype.Load = function()	{
		ko.applyBindings(this,$("#header")[0]);
	}
	
	App.ViewModels.HeaderModel.prototype.Update = function (loginName) {
		var self = this;		
		//create new dto
		var data = new App.DataObjects.HeaderDto();		
		self.login(loginName);		
	}        
})(App)