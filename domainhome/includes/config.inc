<?php
/*
 * Copyright (c) 2011-2012, Marc Sven Kleinböhl (Hroudtwolf) <coding4food@hroudtwolf.de>.
 * All rights reserved.
 */

/*
* The form callback for adding and deleting new domains.
*/
function _domainhome_config_form ($form, $form_state) {
  
  $form = array ();
   
  $form['domain_datasets'] = array (
    '#type'   => 'fieldset',
    '#tree'	  => FALSE,
  );
  
  $form['domain_datasets']['table_of_domains'] = array (
      '#type'     => 'tableselect',
      '#header'   => array(
        'domain' => array('data' => t('Title'), 'field' => 'domain'),
    		'home' => array('data' => t('Type'), 'field' => 'home'),
    	),
      '#options'  => _domainhome_config_get_domains (),
      '#empty'    => t('No domain home sites available.'),
  );
  
  $form['domain_datasets']['delete'] = array (
      '#type'   => 'submit',
      '#value'  => t('Delete'),
      '#submit'	=> array ('_domainhome_config_form_delete_submit'),
  );
  
  $form['dataset'] = array (
    '#type'   => 'fieldset',
    '#tree'	  => TRUE,
  );
  
  $form['dataset']['domain'] = array (
    '#type'         => 'textfield',
    '#title'        => t('Domain'),
    '#description'	=> t('I.ex.: mydomain.org or sub.mydomain.org')
  );
  
  $form['dataset']['home_path'] = array (
    '#type'         => 'textfield',
    '#title'        => t('Path to home site'),
    '#description'	=> check_markup (t('I.ex.: node/123
You can use the %domain token for the path. 
This will be replaced by a adjusted version of the domain name.
Like this:
mysubdomain.onmydomain.tld = mysubdomain_onmydomain_tld
'))
  );
  
  $form['dataset']['add'] = array (
    '#type'   => 'submit',
    '#value'  => t('Save'),
    '#submit'	=> array ('_domainhome_config_form_add_submit'),
  );
  
  return $form;
}

/*
* The submit handler for adding domain home redirections.
*/
function _domainhome_config_form_add_submit  (&$form, &$form_state) {
  
  $values = $form_state['values']['dataset'];
  
  module_load_include  ('inc', 'domainhome', 'includes/data');
  
  _domainhome_data_add_redirection (
    $values['domain'], 
    _domainhome_replace_tokens (
      $values['domain'], 
      $values['home_path']
    )
  );
  
  drupal_set_message (t('Added a new domain home.'));
  
  return;
}


/*
 * The submit handler for deleting domain home redirections.
 */
function _domainhome_config_form_delete_submit  (&$form, &$form_state) {

  $values           = $form_state['values']['table_of_domains'];
  $deleted_domains  = 0;
  
  module_load_include  ('inc', 'domainhome', 'includes/data');

  foreach ($values as $did=>$checked) {
    
    if ($did == $checked) {
      _domainhome_data_delete_redirection($did);
      
      $deleted_domains++;;
    }
  }
  
  if ($deleted_domains > 0) {
    drupal_set_message (t('Deleted @count domain home redirections.', array (
      '@count' => $deleted_domains
    )));
  }
 
  return;
}
 
/*
* Loads redirection data from database.
* @return 	array		An array of redirection data.
*/
function _domainhome_config_get_domains () {
  
  module_load_include  ('inc', 'domainhome', 'includes/data');
 
  return _domainhome_data_load_redirections ();;
}

/*
 * Replaces the %domain tokens in a home path string.
 * @param 	string $domain 	The domain.
 * @param		string $home		The home path.
 * @return	string					The changed string.
 */
function _domainhome_replace_tokens ($domain, $home_path) {
  
  return str_replace (
    '%domain',
    preg_replace (
      '/[^a-zA-Z0-9_]/',
      '_',
      $domain
    ),
    $home_path
  );
}