
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
			var host = "http://192.168.1.10:81";
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
					service.read().done(function(data) {
					  //'client.foo.read' is now cached for 5 seconds
					  if (callback) callback(data);
					});
				else
					service.read(par).done(function(data) {
					  //'client.foo.read' is now cached for 5 seconds
					  if (callback) callback(data);
					});
			}
			self.update = function(serviceId,id,data,callback)
			{				
				self.client[serviceId]=null;
				if (self.client[serviceId]==null)
					self.client.add(serviceId);				
				var service = self.client[serviceId];
				try
				{					
					service.update(id,ko.toJS(data)).done(function(data) {
						//alert('Update error');
						if (callback) callback(data);
					});
				}
				catch(ex)				
				{
					alert(ex);
				}				
			}
			
			self.destroy = function(serviceId,id,callback)
			{				
				self.client[serviceId]=null;
				if (self.client[serviceId]==null)
					self.client.add(serviceId);				
				var service = self.client[serviceId];
				try
				{					
					service.del(id).done(function(data) {
						alert(data.message);
					});
					if (callback!=null) callback();
				}
				catch(ex)				
				{
					alert(ex);
				}				
			}
			
			self.create = function(serviceId,data,callback)
			{				
				self.client[serviceId]=null;
				if (self.client[serviceId]==null)
					self.client.add(serviceId);				
				var service = self.client[serviceId];
				try
				{					
					service.create(ko.toJS(data)).done(function(data) {
						alert('Saving error');
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