/**
 * @author      Marc Sven Kleinböhl (aka Hroudtwolf)
 * @copyright   Marc Sven Kleinböhl 2013(c)
 *
 * This script enables to toggling the WhatFa console.
 */

(function ($) {
  
  Drupal.behaviors.whatfa = { 
    attach: function(context) {
      
      $('#whatfa-console-header').click (function () {
        $('#whatfa-console-content').toggle();
      });
      
    }
  }
  
})(jQuery);