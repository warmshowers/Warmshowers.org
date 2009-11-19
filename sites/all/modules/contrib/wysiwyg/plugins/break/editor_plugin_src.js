// $Id: editor_plugin_src.js,v 1.1 2008/06/10 18:20:14 sun Exp $

// Import plugin language.
tinyMCE.importPluginLanguagePack('wysiwyg', 'en');

var TinyMCE_wysiwygBreakPlugin = {
  getInfo: function() {
    return {
      longname: 'Teaser break',
      author: 'Nathan Haug',
      authorurl: 'http://www.quicksketch.org',
      infourl: 'http://drupal.org/project/wysiwyg'
    };
  },

  initInstance: function(inst) {
    tinyMCE.importCSS(inst.getDoc(), this.baseURL + '/break.css');
  },

  getControlHTML: function (control_name) {
    switch (control_name) {
      case 'break':
        return tinyMCE.getButtonHTML(control_name, 'lang_break_desc', '{$pluginurl}/images/break.gif', 'break', false, 'null');
    }
    return '';
  },

  execCommand: function(editor_id, element, command, user_interface, value) {
    switch (command) {
      case 'break':
        var classValue = '';
        var template = new Array();
        var inst = tinyMCE.getInstanceById(editor_id);
        var focusElm = inst.getFocusElement();

        // Check whether selection is an image and belongs to this plugin.
        if (focusElm != null && focusElm.nodeName.toLowerCase() == 'img') {
          classValue = this.getAttrib(focusElm, 'class');
          if (classValue != 'wysiwyg-break') {
            return true;
          }
        }

        html = '<img src="' + tinyMCE.getParam('theme_href') + '/images/spacer.gif" alt="&lt;--break-&gt;" title="&lt;--break--&gt;" class="wysiwyg-break" />';
        tinyMCE.execInstanceCommand(editor_id, 'mceInsertContent', false, html);
        return true;
    }
    // Pass to next handler in chain.
    return false;
  },

  cleanup: function(type, content) {
    switch (type) {
      case 'insert_to_editor':
        // Parse all <!--break--> tags and replace them with images.
        var startPos = 0;
        while ((startPos = content.indexOf('<!--break-->', startPos)) != -1) {
          // Insert image.
          var contentAfter = content.substring(startPos + 12);
          content = content.substring(0, startPos);
          content += '<img src="' + tinyMCE.getParam('theme_href') + '/images/spacer.gif" alt="&lt;--break-&gt;" title="&lt;--break--&gt;" class="wysiwyg-break" />';
          content += contentAfter;
          startPos++;
        }
        break;

      case 'get_from_editor':
        // Parse all img tags and replace them with <!--break-->.
        var startPos = -1;
        while ((startPos = content.indexOf('<img', startPos + 1)) != -1) {
          var endPos = content.indexOf('/>', startPos);
          var attribs = parseAttributes(content.substring(startPos + 4, endPos));
          if (attribs['class'] == 'wysiwyg-break') {
            endPos += 2;
            chunkBefore = content.substring(0, startPos);
            chunkAfter = content.substring(endPos);
            content = chunkBefore + '<!--break-->' + chunkAfter;
          }
        }
        break;
    }
    // Pass through to next handler in chain
    return content;

    /**
     * Local function that parses the break image in and out.
     */
    function parseAttributes (attribute_string) {
      var attributeName = '', attributeValue = '', withInName, withInValue;
      var attributes = new Array();
      var whiteSpaceRegExp = new RegExp('^[ \n\r\t]+', 'g');

      if (attribute_string == null || attribute_string.length < 2) {
        return null;
      }
      withInName = withInValue = false;
      for (var i = 0; i < attribute_string.length; i++) {
        var chr = attribute_string.charAt(i);
        if ((chr == '"' || chr == "'") && !withInValue) {
          withInValue = true;
        }
        else if ((chr == '"' || chr == "'") && withInValue) {
          withInValue = false;
          var pos = attributeName.lastIndexOf(' ');
          if (pos != -1) {
            attributeName = attributeName.substring(pos+1);
          }
          attributes[attributeName.toLowerCase()] = attributeValue.substring(1).toLowerCase();
          attributeName = '';
          attributeValue = '';
        }
        else if (!whiteSpaceRegExp.test(chr) && !withInName && !withInValue) {
          withInName = true;
        }
        if (chr == '=' && withInName) {
          withInName = false;
        }
        if (withInName) {
          attributeName += chr;
        }
        if (withInValue) {
          attributeValue += chr;
        }
      }
      return attributes;
    }
  },

  handleNodeChange: function(editor_id, node, undo_index, undo_levels, visual_aid, any_selection) {
    tinyMCE.switchClass(editor_id + '_wysiwyg_break', 'mceButtonNormal');
    if (node == null) {
      return;
    }
    do {
      if (node.nodeName.toLowerCase() == 'img' && this.getAttrib(node, 'class').indexOf('wysiwyg-break') == 0) {
        tinyMCE.switchClass(editor_id + '_wysiwyg_break', 'mceButtonSelected');
      }
    } while ((node = node.parentNode));
    return true;
  },

  getAttrib: function(elm, name) {
    return elm.getAttribute(name) ? elm.getAttribute(name) : '';
  }
};

tinyMCE.addPlugin('wysiwyg', TinyMCE_wysiwygBreakPlugin);

