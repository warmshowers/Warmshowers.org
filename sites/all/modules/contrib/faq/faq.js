/* $Id: faq.js,v 1.1.2.6.2.20 2009/11/20 02:44:40 snpower Exp $ */

function teaser_handler(event) {
  if ($("input[name=faq_display]:checked").val() != "new_page") {
    if ($("input[name=faq_use_teaser]:checked").val() == 1) {
      $("input[name=faq_more_link]").removeAttr("disabled");
    }
    else {
      $("input[name=faq_more_link]").attr("disabled", "disabled");
    }
  }
}

function faq_display_handler(event) {
  // Enable / disable "questions_inline" and "questions_top" only settings.
  if ($("input[name=faq_display]:checked").val() == "questions_inline" || $("input[name=faq_display]:checked").val() == "questions_top") {
    $("input[name=faq_back_to_top]").removeAttr("disabled");
  }
  else {
    $("input[name=faq_back_to_top]").attr("disabled", "disabled");
  }

  // Enable / disable "hide_answer" only settings.
  if ($("input[name=faq_display]:checked").val() != "hide_answer") {
    $("input[name=faq_hide_qa_accordion]").attr("disabled", "disabled");
  }
  else {
    $("input[name=faq_hide_qa_accordion]").removeAttr("disabled");
  }

  // Enable / disable "new_page" only settings.
  if ($("input[name=faq_display]:checked").val() != "new_page") {
    $("input[name=faq_use_teaser]").removeAttr("disabled");
    $("input[name=faq_more_link]").removeAttr("disabled");
    $("input[name=faq_disable_node_links]").removeAttr("disabled");
  }
  else {
    $("input[name=faq_use_teaser]").attr("disabled", "disabled");
    $("input[name=faq_more_link]").attr("disabled", "disabled");
    $("input[name=faq_disable_node_links]").attr("disabled", "disabled");
  }
  teaser_handler(event);

  // Enable / disable "new_page" and "questions_top" only settings.
  if ($("input[name=faq_display]:checked").val() == "new_page" ||
    $("input[name=faq_display]:checked").val() == "questions_top") {
    $("select[name=faq_question_listing]").removeAttr("disabled");
  }
  else {
    $("select[name=faq_question_listing]").attr("disabled", "disabled");
  }

  // Enable / disable "questions_inline" only settings.
  if ($("input[name=faq_display]:checked").val() == "questions_inline") {
    $("input[name=faq_qa_mark]").removeAttr("disabled");
    // Enable / disable label settings according to "qa_mark" setting.
    if ($("input[name=faq_qa_mark]:checked").val() == 1) {
      $("input[name=faq_question_label]").removeAttr("disabled");
      $("input[name=faq_answer_label]").removeAttr("disabled");
    }
    else {
      $("input[name=faq_question_label]").attr("disabled", "disabled");
      $("input[name=faq_answer_label]").attr("disabled", "disabled");
    }
  }
  else {
    $("input[name=faq_qa_mark]").attr("disabled", "disabled");
    $("input[name=faq_question_label]").attr("disabled", "disabled");
    $("input[name=faq_answer_label]").attr("disabled", "disabled");
  }
}

function qa_mark_handler(event) {
  if ($("input[name=faq_display]:checked").val() == "questions_inline") {
    // Enable / disable label settings according to "qa_mark" setting.
    if ($("input[name=faq_qa_mark]:checked").val() == 1) {
      $("input[name=faq_question_label]").removeAttr("disabled");
      $("input[name=faq_answer_label]").removeAttr("disabled");
    }
    else {
      $("input[name=faq_question_label]").attr("disabled", "disabled");
      $("input[name=faq_answer_label]").attr("disabled", "disabled");
    }
  }
}

function questions_top_handler(event) {
  $("input[name=faq_display]").val() == "questions_top" ?
    $("input[name=faq_group_questions_top]").removeAttr("disabled"):
    $("input[name=faq_group_questions_top]").attr("disabled", "disabled");

  $("input[name=faq_display]").val() == "questions_top" ?
    $("input[name=faq_answer_category_name]").removeAttr("disabled"):
    $("input[name=faq_answer_category_name]").attr("disabled", "disabled");
}


function child_term_handler(event) {
  if ($("input[name=faq_hide_child_terms]:checked").val() == 1) {
    $("input[name=faq_show_term_page_children]").attr("disabled", "disabled");
  }
  else if ($("input[name=faq_category_display]:checked").val() != "categories_inline") {
    $("input[name=faq_show_term_page_children]").removeAttr("disabled");
  }
}


function categories_handler(event) {
  if ($("input[name=faq_display]").val() == "questions_top") {
    $("input[name=faq_category_display]:checked").val() == "categories_inline" ?
      $("input[name=faq_group_questions_top]").removeAttr("disabled"):
      $("input[name=faq_group_questions_top]").attr("disabled", "disabled");
    $("input[name=faq_category_display]:checked").val() == "new_page" ?
      $("input[name=faq_answer_category_name]").attr("disabled", "disabled"):
      $("input[name=faq_answer_category_name]").removeAttr("disabled");
  }
  else {
    $("input[name=faq_group_questions_top]").attr("disabled", "disabled");
  }

  // Enable / disable "hide_qa" only settings.
  if ($("input[name=faq_category_display]:checked").val() != "hide_qa") {
    $("input[name=faq_category_hide_qa_accordion]").attr("disabled", "disabled");
  }
  else {
    $("input[name=faq_category_hide_qa_accordion]").removeAttr("disabled");
  }

  $("input[name=faq_category_display]:checked").val() == "categories_inline" ?
    $("input[name=faq_hide_child_terms]").attr("disabled", "disabled"):
    $("input[name=faq_hide_child_terms]").removeAttr("disabled");
  $("input[name=faq_category_display]:checked").val() == "categories_inline" ?
    $("input[name=faq_show_term_page_children]").attr("disabled", "disabled"):
    $("input[name=faq_show_term_page_children]").removeAttr("disabled");
  $("input[name=faq_category_display]:checked").val() == "new_page" ?
    $("select[name=faq_category_listing]").removeAttr("disabled"):
    $("select[name=faq_category_listing]").attr("disabled", "disabled");

  child_term_handler();
}

Drupal.behaviors.initFaqModule = function (context) {
  // Hide/show answer for a question.
  var faq_hide_qa_accordion = Drupal.settings.faq.faq_hide_qa_accordion;
  if (faq_hide_qa_accordion) {
    $('div.faq-dd-hide-answer').addClass("collapsible collapsed");
  }
  else {
    $('div.faq-dd-hide-answer').hide();
  }
  $('div.faq-dt-hide-answer').click(function() {
    $(this).toggleClass('faq-qa-visible');
    if (faq_hide_qa_accordion) {
      $('div.faq-dd-hide-answer').not($(this).next('div.faq-dd-hide-answer')).addClass("collapsed");
      $(this).next('div.faq-dd-hide-answer').toggleClass("collapsed");
    }
    else {
      $(this).next('div.faq-dd-hide-answer').slideToggle('fast', function() {
        $(this).parent().toggleClass('expanded');
      });
    }
    return false;
  });


  // Hide/show q/a for a category.
  var faq_category_hide_qa_accordion = Drupal.settings.faq.faq_category_hide_qa_accordion;
  if (faq_category_hide_qa_accordion) {
    $('div.faq-qa-hide').addClass("collapsible collapsed");
  }
  else {
    $('div.faq-qa-hide').hide();
  }
  $('div.faq-qa-header .faq-header').click(function() {
    $(this).toggleClass('faq-category-qa-visible');
    if (faq_category_hide_qa_accordion) {
      $('div.faq-qa-hide').not($(this).parent().next('div.faq-qa-hide')).addClass("collapsed");
      $(this).parent().next('div.faq-qa-hide').toggleClass("collapsed");
    }
    else {
      $(this).parent().next('div.faq-qa-hide').slideToggle('fast', function() {
        $(this).parent().toggleClass('expanded');
      });
    }
    return false;
  });


  // Show expand all link. 
  if (!faq_hide_qa_accordion && !faq_category_hide_qa_accordion) {
    $('#faq-expand-all').show();
    $('#faq-expand-all a.faq-expand-all-link').show();

    // Add collapse link click event.
    $('#faq-expand-all a.faq-collapse-all-link').click(function () {
      $(this).hide();
      $('#faq-expand-all a.faq-expand-all-link').show();
      $('div.faq-qa-hide').slideUp('slow', function() {
        $(this).removeClass('expanded');
      });
      $('div.faq-dd-hide-answer').slideUp('slow', function() {
        $(this).removeClass('expanded');
      });
    });

    // Add expand link click event.
    $('#faq-expand-all a.faq-expand-all-link').click(function () {
      $(this).hide();
      $('#faq-expand-all a.faq-collapse-all-link').show();
      $('div.faq-qa-hide').slideDown('slow', function() {
        $(this).addClass('expanded');
      });
      $('div.faq-dd-hide-answer').slideDown('slow', function() {
        $(this).addClass('expanded');
      });
    });
  }



  // Handle faq_category_settings_form.
  faq_display_handler();
  questions_top_handler();
  categories_handler();
  teaser_handler();
  $("input[name=faq_display]").bind("click", faq_display_handler);
  $("input[name=faq_qa_mark]").bind("click", qa_mark_handler);
  $("input[name=faq_use_teaser]").bind("click", teaser_handler);
  $("input[name=faq_category_display]").bind("click", categories_handler);
  $("input[name=faq_hide_child_terms]").bind("click", child_term_handler);

}


