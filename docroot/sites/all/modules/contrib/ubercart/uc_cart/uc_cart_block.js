/**
 * @file
 * Adds effects and behaviors to the cart block.
 */

/**
 * Sets the behavior to (un)collapse the cart block on a click
 */
Drupal.behaviors.ucCollapseBlock = {
  attach: function(context) {
    jQuery('.cart-block-title-bar:not(.ucCollapseBlock-processed)', context).addClass('ucCollapseBlock-processed').click(
      function() {
        var $items = jQuery('.cart-block-items').toggleClass('collapsed');
        jQuery('.cart-block-arrow').toggleClass('arrow-down', $items.hasClass('collapsed'));
      }
    );
  }
}
