(function ($) {
  
  Drupal.behaviors.whatfa = { 
    attach: function(context) {
      
      $('#whatfa-console-header').click (function () {
        $('#whatfa-console-content').toggle();
      });
      
    }
  }
  
})(jQuery);