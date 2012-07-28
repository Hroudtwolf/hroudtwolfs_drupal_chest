<?php
/*
 * Copyright (c) 2011-2012, Marc Sven Kleinböhl (Hroudtwolf) <coding4food@hroudtwolf.de>.
 * All rights reserved.
 */

/*
 * The control center form callback.
 */
function _muckypup_control_center_form ($form, $form_state) {
  
  drupal_add_css (drupal_get_path ('module', 'muckypup') . '/css/muckypup_control_center.css');
  
  module_load_include ('inc', 'muckypup', 'includes/database');
  
  $types        = _muckypup_database_get_node_types ();
  $roles        = _muckypup_database_get_roles ();
  $vocabularies = _muckypup_database_get_vocabularies();
  
  $form         = array ();
  $form['#id']  = 'muckypup-control-center-form';
  
  $form['nodes'] = array (
    '#type'  				=> 'fieldset',
    '#title'	      => t('Nodes'),
    '#tree'		      => TRUE,
  	'#description'	=> t('This filter take only affect, if you select a content type.')
  );
  
  if (! empty ($types)) {
    $form['nodes']['content_types'] = array (
      '#type'     => 'checkboxes',
      '#title'	  => t('Node types'),
  		'#options'	=> $types
    );
    
    $form['nodes']['content_stati'] = array (
        '#type'     => 'select',
        '#title'	  => t('Status'),
    		'#options'	=> array (
    		  MUCKYPUP_STATUS_ALL         => t('All'),
    		  MUCKYPUP_STATUS_PUBLISHED		=> t('Only published'),
    		  MUCKYPUP_STATUS_UNPUBLISHED	=> t('Only unpublished')
    		)
    );
    }else{
      $form['nodes']['content_types'] = array (
        '#type'     => 'item',
        '#markup'	  => t('No content types available.'),
      );
  }
    
  $form['users'] = array (
    '#type'         => 'fieldset',
    '#title'	      => t('Users'),
    '#tree'		      => TRUE,
    '#description'	=> t('This filter take only affect, if you select a user role.')
  );
  
  if (! empty ($roles)) {
    $form['users']['roles'] = array (
      '#type'     => 'checkboxes',
      '#title'	  => t('User roles'),
  		'#options'	=> $roles
    );
    
    $form['users']['active'] = array (
      '#type'     => 'select',
      '#title'	  => t('Status'),
      '#options'	=> array (
        MUCKYPUP_STATUS_ALL       => t('All'),
        MUCKYPUP_STATUS_ACTIVE    => t('Active'),
        MUCKYPUP_STATUS_INACTIVE	=> t('Inactive')
     )
   );
    
    $form['users']['item'] = array (
      '#type'     => 'item',
      '#title'	  => t('ATTENTION'),
      '#markup'		=> '<div id="muckypup-control-center-founder-ignore">' . t('The account of the site founder will be untouched.') .'</div>'
    );
  }else{
    $form['users']['roles'] = array (
      '#type'     => 'item',
      '#markup'	  => t('No roles available.'),
    );
  }
  
  $form['terms'] = array (
    '#type'         => 'fieldset',
    '#title'	      => t('Terms'),
    '#tree'		      => TRUE,
  	'#description'	=> t('Select a vocabulary from which you want to remove the terms.')
  );
  
  if (! empty ($vocabularies)) {
    $form['terms']['vocabularies'] = array (
      '#type'     => 'checkboxes',
      '#title'	  => t('Vocabularies'),
  		'#options'	=> $vocabularies
    );
    }else{
      $form['terms']['vocabularies'] = array (
        '#type'     => 'item',
        '#markup'	  => t('No roles available.'),
      );
  }
  
  $form['control_elements'] = array (
      '#type'   => 'item',
      '#prefix' => '<div style="clear: both;"></div>',
      '#id'			=> 'muckypup-control-elements'
  );
  
  $form['control_elements']['sure'] = array (
    '#type'           => 'checkbox',
    '#title'	        => t('Are you sure?'),
    '#default_value'  => FALSE
  );
  
 $form['control_elements']['delete'] = array (
    '#type'           => 'submit',
    '#value'	        => t('Delete'),
  );
  
 $form['control_elements']['clear'] = array (
       '#type'   => 'item',
       '#prefix' => '<div style="clear: both;"></div>',
 );
 
  return $form;
}

/*
 * The validate callback of the _muckypup_control_center_form.
 */
function _muckypup_control_center_form_validate ($form, &$form_state) {

  extract ($form_state['values']);
  
  $content_type_selected = array_count_values($nodes['content_types']);
  $content_type_selected = $content_type_selected[0]  < count ($nodes['content_types']);

  $role_selected = array_count_values($users['roles']);
  $role_selected = $role_selected[0]  < count ($users['roles']);
  
  $vocabulary_selected = array_count_values($terms['vocabularies']);
  $vocabulary_selected = isset ($vocabulary_selected[0]) ? ($vocabulary_selected[0] < count ($terms['vocabularies'])) : TRUE;
  
  if ($sure == 0) {
    form_set_error('sure', t('You must confirm the selection by checking the checkbox.'));
  }

  if (! $content_type_selected && ! $role_selected && ! $vocabulary_selected) {
    form_set_error('', t('Selection was empty. Please select a note type, a user role or a vocabulary.')); 
  }
  
	return;
}

/*
 * The submit callback of the _muckypup_control_center_form.
 */
function _muckypup_control_center_form_submit ($form, &$form_state) {
  
  extract ($form_state['values']);
 
  module_load_include ('inc', 'muckypup', 'includes/batch');
  
  _muckypup_batch_create ($nodes['content_types'], $nodes['content_stati'], $users['roles'], $users['active'], $terms['vocabularies']);
  
  return;
}
