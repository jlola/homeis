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
        					
	}							

	App.ViewModels.HeaderModel.prototype.Update = function (loginName) {
		var self = this;		
		//create new dto
		var data = new App.DataObjects.HeaderDto();		
		self.login(loginName);		
	}        
})(App)