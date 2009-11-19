// $Id: devel.js 505 2009-05-24 18:55:09Z rfay $

    
/**
  *  @name    jQuery Logging plugin
  *  @author  Dominic Mitchell
  *  @url     http://happygiraffe.net/blog/archives/2007/09/26/jquery-logging
  */
jQuery.fn.log = function (msg) {
    console.log("%s: %o", msg, this);
    return this;
};