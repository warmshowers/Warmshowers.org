function populateElement(selector, defvalue) {
    if($.trim($(selector).val()) == "") {
        $(selector).val(defvalue);
    }
    $(selector).focus(function() {
        if($(selector).val() == defvalue) {
            $(selector).val("");
        }
    });
    $(selector).blur(function() {
        if($.trim($(selector).val()) == "") {
            $(selector).val(defvalue);
        }
    });
 }

$(document).ready(function(){
	populateElement('#search-box input.form-text, #search-block-form input.form-text', 'search...');

});