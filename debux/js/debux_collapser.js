(function ($) {
	
	Drupal.behaviors.debux_collapser = { attach: function(context) {
	 
			$('#debux-console .header').click(function (event){
				
				$('#debux-console .content').toggle ();
				
				return;
			});
		
			$('#debux-console .array').click(function (event){
				
				$('.array-branch', this).toggle ();
				
				return;
			});
			
		
			return;
		}
	};
})(jQuery)