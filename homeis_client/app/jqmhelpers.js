(function (App) {
	App.Helpers = {}
	//vytvori pole klic hodnota z pole enumeraci
	App.Helpers.CreateArray = function(obj)
	{
		var arr = [];
		for (var key in obj) {
			arr.push({id:obj[key],name:key})
		}
		return arr;
	};
	
	//vytvori property objektu
	App.Helpers.CreatePropertiesFromData = function(obj) {
		for (var key in obj.data) {
			obj[key] = ko.observable(obj.data[key]);
		}
	}
	
	App.Helpers.CreateDtoFromProperties = function(obj) {
		for (var key in obj.data) {			
			obj.data[key] = obj[key]();
		}
		return obj.data;
	}
	
	//zkopiruje data dvou objektu
	App.Helpers.CopyData = function(src,dst) {
		for (var key in src.data) {
			dst[key](src[key]());
		}
	}
	
	//zkopiruje data z dto do objektu
	App.Helpers.CopyFromDto = function(src,dst) {
		dst.isLoading = true;
		for (var key in dst.data) {
			dst[key](src[key]);
		}
		dst.isLoading = false;
	}
				
	ko.bindingHandlers.inputEnable = {		
		update: function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {
			$(element).textinput();
			var valueProp = valueAccessor();
			if (valueProp())
			{
				$(element).textinput('enable');						
			}
			else
			{
				$(element).textinput('disable');								
				//$(element).addClass('ui-disabled');
			}				
			$(element).height( $(element)[0].scrollHeight );				
		}
	};
	
	ko.bindingHandlers.sliderEnable = {		
		update: function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {
			$(element).slider();
			var valueProp = valueAccessor();
			if (valueProp())
				$(element).slider('enable');
			else
				$(element).slider('disable');
		}
	};
	
	ko.bindingHandlers.ButtonEnable = {		
		update: function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {
			//$(element).button().button("refresh");
			var valueProp = valueAccessor();
			if (valueProp())
				//$(element).removeClass('ui-disabled');
				$( element ).button( "enable" );
			else
				//$(element).addClass('ui-disabled');						
				$( element ).button( "disable" );
		}
	};
	
	ko.bindingHandlers.SelectEnable = {		
		init: function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {
			// $(element).selectmenu();
		},
		update: function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {						
			
			var value = valueAccessor();

			var valueUnwrapped = ko.utils.unwrapObservable(value); 
			var trufalse = valueUnwrapped ? "true" : "false";
			 
			 try
			 {
				//$(element).selectmenu();				
    			if (valueUnwrapped)
    				$(element).selectmenu('enable');
    			else
    				$(element).selectmenu('disable');
			 }
			 catch(ex)
			 {
				console.log(ex.toString());
			 }
			 //$(element).attr("disabled",trufalse);	
				
		}
	};
		
	ko.bindingHandlers.checkbox = {
		update: function(element, valueAccessor, allBindingsAccessor, viewModel, bindingContext) {

			var value = valueAccessor();

			var valueUnwrapped = ko.utils.unwrapObservable(value); 

			//$(element).attr("checked", valueUnwrapped).checkboxradio("refresh");
			$(element).checkboxradio();
			$(element).prop( "checked", valueUnwrapped ).checkboxradio( "refresh" );
		}		
	};
	
	ko.bindingHandlers.jqmFlip = {
    init: function (element, valueAccessor) {
        var result = ko.bindingHandlers.value.init.apply(this, arguments);		       
        try {
            $(element).slider("refresh");
        } catch (x) {
			$(element).slider();
			$(element).slider("refresh");
			//console.log(x.message);
		}		
        return result;
    },
    update: function (element, valueAccessor) {
        ko.bindingHandlers.value.update.apply(this, arguments);
        var value = valueAccessor();
        var valueUnwrapped = ko.utils.unwrapObservable(value);
        try {
            $(element).slider("refresh");
        } catch (x) {
			console.log(x.message);
		}		
    }
};
	
	
	// ko.bindingHandlers.jqmOptions = {
            // init: function (element, valueAccessor, allBindingsAccessor, viewModel) {
                // if (typeof ko.bindingHandlers.options.init != 'undefined') {
                    // ko.bindingHandlers.options.init(element, valueAccessor, allBindingsAccessor, viewModel);
                // }
            // },

            // update: function (element, valueAccessor, allBindingsAccessor, viewModel) {
                // if (typeof ko.bindingHandlers.options.update != 'undefined') {
                    // ko.bindingHandlers.options.update(element, valueAccessor, allBindingsAccessor, viewModel);
                // }
				
				// $(element).selectmenu();
                // var instance = $.data(element, 'selectmenu');
                // if (instance) {
                    // $(element).selectmenu('refresh', true);
                // }
            // }
        // };
	App.Helpers.BooleanParse = function (str) {
	  switch (str.toLowerCase ()) {
		case "true":
		case '1':
		  return true;
		case "false":
		case "0":
		  return false;
		default:
		  throw new Error ("Boolean.parse: Cannot convert string to boolean.");
	  }
	};
		
	App.Helpers.ensureDropdownSelectionIsConsistentWithModelValue = function(element, modelValue, preferModelValue) {
		if (element!=null)
		{
		//if (preferModelValue) {
			if (modelValue != ko.selectExtensions.readValue(element))
				ko.selectExtensions.writeValue(element, modelValue);
				// DWH added this to correct problem with the change not firing on initial population
				ko.utils.triggerEvent(element, "change");
	 
		//} else {
			// No matter which direction we're syncing in, we want the end result to be equality between dropdown value and model value.
			// If they aren't equal, either we prefer the dropdown value, or the model value couldn't be represented, so either way,
			// change the model value to match the dropdown.
			//if (modelValue != ko.selectExtensions.readValue(element))
			//	ko.utils.triggerEvent(element, "change");
		//}
		}
	};
	
	
	
	App.Helpers.SetDropDownListValue = function(elementid,value) {
		//$('#selectionType').get(0),self.type(),self.type()
		App.Helpers.ensureDropdownSelectionIsConsistentWithModelValue($('#'+elementid).get(0),value,value);
	};
	
	App.Helpers.LoadPage = function (holderId,pageFile,model,callback)
	{		
		var holder = $(holderId).find(":jqmData(role=content)");
		if (holder)
		{
			var element = $(holder)[0];
			var item = ko.dataFor(element);
			if (item!=null && item.AutoRefresh!=null) item.AutoRefresh(false);
			
			ko.cleanNode($(holder)[0]);
			$(holder).load(pageFile, function () {											
				$(holderId).trigger("create");								
				//$('[data-role="content"]').trigger('create');
				//$.mobile.loadPage(holderId, { showLoadMsg: false } );
				//ko.cleanNode(this);				
				if (model!=null) ko.applyBindings(model,this);
				
				if (model.AutoRefresh!=null) model.AutoRefresh(true);
				
				if (callback!=null) callback();				
			});	
		}		
	};

})(App)