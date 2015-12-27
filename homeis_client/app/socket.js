
(function (App) {
		App.Sockets.SocketStatus = {
				Offline: 0,				
				Online: 1,
				Closing: 2,
				Closed: 3,
				Authenticate: 4
			}
		App.Sockets.SocketChangeEventArgs = function(SocketStatus,IsConnected)
		{
			var self = this;
			self.SocketStatus = SocketStatus;			
			self.IsConnected = IsConnected;
		}
/////////////////Socket/////////////////////////////////////////////////////////							
		App.Sockets.Socket = function(host) {					
			var host = "http://" + location.host;
			if (host=="http://")
			{
				host = host + "192.168.88.226:81";
			}
			var self = this;						
			//internal storage of socket
			self.socket = null;					
			self.onStatusChanged = null;
			self.IsConnected = false;
			self.client = new $.RestClient(host+'/api/',{
			  username: 'a',
			  password: 'a',
			  stringifyData: true
			});
			
			self.read = function(serviceId,par,callback)
			{											
				if (self.client[serviceId]==null)
					self.client.add(serviceId);
				var service = self.client[serviceId];
				if (par==null || par=='') 
					response = service.read();					
				else
					response = service.read(par);
				response.error(function(data)
				{
					var result = {};
					result.success = false;
					result.message = data.responseText;
					result.statusText = data.statusText;
					result.status = data.status;
					if (response.status==200) 
					{
						result.success = true;
					}
					
					if (callback!=null) callback(result);
				});
				response.done(function(data) {
					var result = {};
					result.success = false;
					result.message = data;
					result.statusText = response.statusText;
					result.status = response.status;
					if (response.status==200) 
					{
						result.success = true;
					}
					
					if (callback!=null) callback(result);
				});								
			}
			self.update = function(serviceId,id,data,callback)
			{				
				$.mobile.loading( "show", {
						text: "Loading",
						textVisible: false,						
						textonly: false
				});
				
				self.client[serviceId]=null;
				if (self.client[serviceId]==null)
					self.client.add(serviceId);				
				var service = self.client[serviceId];
				try
				{					
					response = service.update(id,ko.toJS(data));//.done(function(data) {
						// //alert('Update error');
						// if (callback) callback(data);
					// });
					response.complete(function(response){
						var result = {};
						result.success = false;
						result.message = data;
						result.statusText = response.statusText;
						result.status = response.status;
						if (response.status==200) 
						{
							result.success = true;
						}
						$.mobile.loading( "hide" );
						if (callback!=null) callback(result);
					});				
				}
				catch(ex)				
				{
					alert(ex);
				}				
			}
			
			self.destroy = function(serviceId,id,callback)
			{			
				$.mobile.loading( "show", {
						text: "Loading",
						textVisible: false,						
						textonly: false
				});
				
				self.client[serviceId]=null;
				if (self.client[serviceId]==null)
					self.client.add(serviceId);				
				var service = self.client[serviceId];
				try
				{					
					response = service.del(id);	
					
					response.complete(function(resp){
						var result = {};
						result.success = false;
						result.message = response.responseText;
						result.statusText = response.statusText;
						result.status = response.status;
						if (response.status==200) 
						{
							result.success = true;
						}
						$.mobile.loading( "hide" );
						if (callback!=null) callback(result);
					});					
				}
				catch(ex)				
				{
					alert(ex);
				}				
			}
			
			self.create = function(serviceId,data,callback)
			{				
				$.mobile.loading( "show", {
						text: "Loading",
						textVisible: false,						
						textonly: false
				});
				
				self.client[serviceId]=null;
				if (self.client[serviceId]==null)
					self.client.add(serviceId);				
				var service = self.client[serviceId];
				try
				{					
					response = service.create(ko.toJS(data)).done(function(data) {						
					});
					
					response.complete(function(response){
						var result = {};
						result.success = false;
						result.message = response.responseText;
						result.statusText = response.statusText;
						result.status = response.status;
						if (response.status==200) 
						{
							result.success = true;
						}
						$.mobile.loading( "hide" );
						if (callback!=null) callback(result);
					});
				}
				catch(ex)				
				{
					alert(ex);
				}				
			}
			
			//serviceId - id of service that understand reqests data
			//data - data with request			
			//
			self.send = function(serviceId,data)
			{
				var mainDataObject = new App.DataObjects.MainDataObject(data,serviceId);						
				var jsondata = ko.toJSON(mainDataObject);						
				self.socket.send(jsondata);						
			}					
			
			self.close = function()
			{
				self.socket.close();
			}
			//connects to server
			self.connect = function(onconnected)
			{
				try {														
					console.log('connecting..');					
					self.socket = new WebSocket(host);
					
					if (self.onStatusChanged!=null) self.onStatusChanged(new App.Sockets.SocketChangeEventArgs(self.socket.readyState, self.IsConnected)/*'Connecting..'*/);
					
					self.socket.onopen = function(){
						console.log('Connected. Socket status: ' + self.socket.readyState);											
						//send clientid
						self.send(App.Enums.EServices.ConnectService,'connect');						
						
						//if (self.onStatusChanged!=null) self.onStatusChanged('Authenticate...');																																
						if (self.onStatusChanged!=null) self.onStatusChanged(new App.Sockets.SocketChangeEventArgs(App.Sockets.SocketStatus.Authenticate, self.IsConnected)/*'Online'*/);
					}
					
					self.socket.onmessage = function(msg){					
						console.log('Received message: ' + msg + ' Status: ' + self.socket.readyState);
						self.IsConnected = true;
						if (self.onStatusChanged!=null) self.onStatusChanged(new App.Sockets.SocketChangeEventArgs(self.socket.readyState, self.IsConnected)/*'Online'*/);
						try
						{							
							var jsonParse = JSON.parse(msg.data);
							if (self.onMessage!=null) self.onMessage(jsonParse.Data);
							if (onconnected!=null) {
								onconnected();
								onconnected = null;
							}
						}
						catch(ex)
						{
							console.log('Received message error: ' + ex);
						}						
					}
					
					self.socket.onclose = function(){
						console.log('Socket Status: ' + self.socket.readyState + ' (Closed)');					
						if (self.onStatusChanged!=null) self.onStatusChanged(new App.Sockets.SocketChangeEventArgs(self.socket.readyState, self.IsConnected)/*'Offline'*/);
					}
				} catch(exception){
					
					self.IsConnected = false;
					console.log('Error'+exception);					
				}				
			};		
			
			self.setOnMessage = function(method)
			{
				self.onMessage = method;
			}
			
			self.setOnStatusChanged = function(method)
			{
				self.onStatusChanged = method;
			}
			
			return self;
		}
		
})(App)