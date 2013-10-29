
  
   
	
  
 
  
  /////////////////Page Live/////////////////////////////////////////////////////////
 
	 LoadPanel = function (panelHolder,panelFile)
	 {	
		var _menuLoaded = false;
		$(panelHolder).load(panelFile, function () {
			$(this).find(":jqmData(role=listview)").listview();
			$(this).trigger("updatelayout");
			$.mobile.loading("hide");
			_menuLoaded = true;			
		});
			
		$(panelHolder).on("panelopen", function (event, ui) {
			if (!_menuLoaded) {
				$.mobile.loading("show");
				return;
			}
			$(panelHolder).off("panelopen");
		});
	 }
	 
	 LoadPanelExt = function (page,panelHolder,panelFile)
	 {	
		var holder = $(page).find(":jqmData(role=panel)");
		//var holder = $(page).children(panelHolder);
		var _menuLoaded = false;
		$(holder).load(panelFile, function () {
			$(this).find(":jqmData(role=listview)").listview();
			$(this).trigger("updatelayout");
			$.mobile.loading("hide");
			ko.applyBindings(App.Instance.GetMenu(),this);
			_menuLoaded = true;			
		});
			
		$(holder).on("panelopen", function (event, ui) {
			if (!_menuLoaded) {
				$.mobile.loading("show");
				return;
			}
			$(holder).off("panelopen");
		});
	 }
	 
	 LoadHeader = function (pageid,headerFile)
	 {		
		var header = $(pageid).find(":jqmData(role=header)");
		if (header)
		{
			//$(headerHolder).load(headerFile, function () {			
			$(header).load(headerFile, function () {			
				$(pageid).trigger("pagecreate");
				ko.applyBindings(App.Instance.GetHeader(),this);
			//$.mobile.loading("hide");			
			});	
		}		
	 }
	 
	 // LoadPage = function (holderId,pageFile,model,callback)
	 // {		
		// var holder = $(holderId).find(":jqmData(role=content)");
		// if (holder)
		// {
			// //$.mobile.loadPage(pageFile);			
			// $(holder).load(pageFile, function () {											
				// $(holderId).trigger("pagecreate");								
				// ko.applyBindings(model,this);
				// callback();				
			// });	
		// }		
	 // }
	 
	$("#demopage").on( "pageinit", function( event ) {				
		LoadHeader(this,"header.html");
		LoadPanelExt(this,'#panel_menu',"menu.html");
		App.Helpers.LoadPage(this,'onewirelist.html',App.Instance.GetOneWireList(),function(){
			App.Instance.GetOneWireList().Mode(App.Enums.OneWireListMode.Folders);
			App.Instance.GetOneWireList().Load();				 
			App.Instance.GetOneWireList().Refresh();		
		});
		//$.mobile.loadPage('#demopage', { showLoadMsg: false } );		
		
	});	
	
	 $("#folderSelect").on( "pageinit", function( event ) {				
		// alert('test');
		 //LoadHeader(this,"header.html");
		 //LoadPanelExt(this,"#panel_menu","menu.html");
	 });	
	
	// $("#onewireDetail").on( "pageinit", function( event ) {				
		// LoadHeader(this,"header.html");
		// LoadPanelExt(this,"#panel_menu","menu.html");
	// });	
	
	// $("#onewirelist").on( "pageinit", function( event ) {				
		// LoadHeader(this,"header.html");
		// LoadPanelExt(this,"#panel_menu","menu.html");		
		// ko.applyBindings(App.Instance.GetOneWireList(), this);  		
		// App.Instance.GetOneWireList().Load();				 
		// App.Instance.GetOneWireList().Refresh();		
	// });
	// var interval;
	// $("#onewirelist").on( "pageshow", function( event ) {			
		// App.Instance.GetOneWireList().Refresh();		
		// // interval = setInterval(function(){
			// // App.Instance.GetOneWireList().Load();				 
			// // App.Instance.GetOneWireList().Refresh();		
		// // },5000);		
	// });
	// $("#onewirelist").on( "pagebeforehide", function( event ) {							
			// clearInterval(interval);
	// });
	
	
	
	// $("#LogInDialogPage").on( "pageinit", function( event ) {					
		// //LoadPanel("#panelmenuLogin","menu.html");
		// LoadPanelExt(this,"#panel_menu","menu.html");		
		// LoadHeader(this,"header.html");
	// });
	// $("#tagdef").on( "pageinit", function( event ) {		
		// //LoadPanel("#panelmenutagdef","menu.html");
		// LoadPanelExt(this,"#panel_menu","menu.html");		
		// LoadHeader(this,"header.html");		
	// });
			
	// $("#LogInDialogPage").on( "swiperight", function( event ) {				
		// $("#panelmenuLogin").panel( "open" );
	// });	
	// $("#LogInDialogPage").on( "swipeleft", function( event ) {				
		// $("#panelmenuLogin").panel( "close" );		
	// });		
   
	//App.Instance = new App.ViewModels.AppModel();	  					
						  
	// $('#main').on('pageinit', function (event, ui) {
		// //alternativa pro binding na urcite id	  			
		// //ko.applyBindings(App.Instance.GetMainPage(), document.getElementById('main'));
	// });
	
	

	// $('#onewireDetail').on('pageinit', function (event, ui) {	 							
		// ko.applyBindings(App.Instance.GetOneWireList().SelectedItem,this);
	// });			
				
	// $('#LogInDialogPage').on('pageinit', function (event, ui) {	 							
		// ko.applyBindings(App.Instance.GetLogInPage(), this);
	// });						
  
  $(document).ready(function () {
	//App.Instance = new App.ViewModels.AppModel();	  					
  });
  
  
    