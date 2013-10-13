(function (App) {

    App.DataObjects.LoginDto = function (login) {
        var self = this;
        self.Command = login ? 'Login' : 'Logout';
        self.UserName = 'user';
        self.Password = 'password';
        return self;
    }

    App.ViewModels.LogInModel = function (psocket) {
        self = this;
        self.socket = psocket;
        self.resultfunction = null;
		self.onLoginChanged = null;
        self.login = ko.observable();
        self.password = ko.observable();
		self.logged = ko.observable(true);
		self.btnlogintext = ko.computed(function(){
		    return self.logged() ? 'Odhlásit' : 'Přihlásit';			
		},this);			
		
        self.btnLoginClick = function () {																
			self = this;
            if (this.login()==null || this.password()==null || this.login()=='' || this.password()=='')
			{
				alert('Není vyplněné jméno a heslo');				
				return;
			}			
			else
			{
				if (!self.logged())
				{
					this.Login(this.login(),this.password(),function(LogResult){						
						self.logged(LogResult);
						if (self.logged())
						{
							$('#login').textinput('disable');
							$('#password').textinput('disable');
							$('#btnLogin .ui-btn-text').text('Odhlásit');					
							var	strlogin = self.login();
						}
						else
						{
							alert('Neplatné heslo nebo jméno');
						}
						if (self.onLoginChanged!=null) self.onLoginChanged(strlogin);
					});
				}
				else
				{
					this.Logout(function(LogResult){						
						self.logged(LogResult);
						self.login(null);
						self.password(null);
						$('#login').textinput('enable');
						$('#password').textinput('enable');
						$('#btnLogin .ui-btn-text').text('Přihlásit');	
						var	strlogin = self.login();
						if (self.onLoginChanged!=null) self.onLoginChanged(strlogin);
					});
				}				
			}					
        }							

        // App.ViewModels.LogInModel.prototype.onmessage = function (msg) {
            // console.log(msg);
            // // Editable data
            // self.socket.setOnMessage(null);

            
        // };

        App.ViewModels.LogInModel.prototype.Login = function (UserName, Password, resultfunction) {
            var self = this; 
				
            //set method for result of function
            //self.resultfunction = resultfunction;
            //create new dto
            //var data = new App.DataObjects.LoginDto(true);
            //set command
            //data.Command = 'Login';
            //data.UserName = UserName;
            //data.Password = Password;
            //var stringdata = ko.toJSON(data);
            //send data
            //self.socket.send(App.Enums.EServices.LoginService, stringdata);
			
			//validace hesla a uzivatele
			if (UserName=='a' && Password=='a')
			{
				if (resultfunction!=null) resultfunction(true);
			}
			else
			{
				if (resultfunction!=null) resultfunction(false);
			}
        }	

        App.ViewModels.LogInModel.prototype.Logout = function (resultfunction) {
            var self = this;
            //self.resultfunction = resultfunction;
            //self.socket.setOnMessage(self.onmessage);
            //var data = new App.DataObjects.LoginDto(false);            
            //var stringdata = ko.toJSON(data);
            //self.socket.send(App.Enums.EServices.LoginService, stringdata);
			if (resultfunction!=null) resultfunction(false);
        }
    }



})(App)