
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
				//$(pageid).trigger("pagecreate");				
				ko.applyBindings(App.Instance.GetHeader(),this);
				$( "#header" ).toolbar( "refresh" );
			//$.mobile.loading("hide");			
			});	
		}		
	 }
	 	
	$("#demopage").on( "pageinit", function( event ) {				
		if (event.target.id=="demopage")
		{
						
			$.mobile.pageContainer.on( "pagecontainerbeforeshow", function( event, ui ) {
				var func = App.Instance.GetPageContainerBeforeShowFunc();
				if (func!=null)
				{
					func(event,ui);
					App.Instance.SetPageContainerBeforeShowFunc(null);
				}
			});
			$.mobile.pageContainer.on( "pagecontainertransition", function( event, ui ) {
				var func = App.Instance.GetPageContainerShowFunc();
				if (func!=null)
				{
					func(event,ui);
					App.Instance.SetPageContainerShowFunc(null);
				}
			});									
		}
	});	
	
	$(document).one('pagebeforecreate', function (e) {
			
			
	});
	
	$( document ).on( "pagecreate", function(e) {			
		
		return true;
	});
		
	$( document ).one( "pagebeforeshow", function(e) {						
	});
		 
	$(function () {
		$("[data-role=header],[data-role=footer]").toolbar().enhanceWithin();
		$("[data-role=panel]").panel().enhanceWithin();
		App.Instance.GetMenu().Load();
		App.Instance.GetHeader().Load();
		App.Instance.GetOneWireList().Mode(App.Enums.OneWireListMode.Folders);
		App.Instance.GetOneWireList().Load(null,function(){
			App.Instance.GetOneWireList().LoadPage();
		});
		
		
	});	 
		  
	$(document).ready(function () {
							
	});
  
  
    