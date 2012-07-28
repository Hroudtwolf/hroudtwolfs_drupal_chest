<?php
/*
 * Copyright (c) 2011-2012, Marc Sven Kleinböhl (Hroudtwolf) <coding4food@hroudtwolf.de>.
 * All rights reserved.
 */

/*
 * Creates a list of removal batch tasks.
 * @param array			$content_types		A list of content type filter settings.
 * @param array			$content_stati		A status code.
 * @param array			$user_roles				A list of role filter settings.
 * @param array			$user_stati				A status code.
 * @param array			$vocabularies			A list of vocabulary filter settings.
 */
function _muckypup_batch_create ($content_types, $content_stati, $user_roles, $user_stati, $vocabularies) {
    
  module_load_include ('inc', 'muckypup', 'includes/database');
  
  $tasks = array ();
  
  _muckypup_batch_create_node_tasks ($tasks, $content_types, $content_stati);
  _muckypup_batch_create_user_tasks ($tasks, $user_roles, $user_stati);
  _muckypup_batch_create_term_tasks ($tasks, $vocabularies);
  
  batch_set (array(
      'title'        => t('Content removal'),
      'operations'   => $tasks,
      'finished'     => '_muckypup_batch_finish_task',
      'file'         => drupal_get_path ('module', 'muckypup') . '/includes/batch.inc',
    )
  );
  
  return;
}

/*
* Extends the task list for user removal tasks.
* @param array_ref	$tasks						A reference to the task list array.
* @param array			$content_types		A list of content type filter settings.
* @param array			$content_stati		A status code.
*/
function _muckypup_batch_create_node_tasks (&$tasks, $content_types, $content_stati) {
  
  $node_ids = _muckypup_database_get_node_ids ($content_types, $content_stati);
 
  foreach ($node_ids as $node_id) {
    $tasks[] = array ('_muckypup_batch_task', array ($node_id, MUCKYPUP_CONTRACT_TYPE_NODE));
  }
  
  return;
}

/*
* Extends the task list for user removal tasks.
* @param array_ref	$tasks				A reference to the task list array.
* @param array			$user_roles		A list of role filter settings.
* @param array			$user_stati		A status code.
*/
function _muckypup_batch_create_user_tasks (&$tasks, $user_roles, $user_stati) {
  
  $user_ids = _muckypup_database_get_user_ids ($user_roles, $user_stati);
  
  foreach ($user_ids as $user_id) {
  	$tasks[] = array ('_muckypup_batch_task', array ($user_id, MUCKYPUP_CONTRACT_TYPE_USER));
  }
    
  return;
}

/*
 * Extends the task list for term removal tasks.
 * @param array_ref	$tasks				A reference to the task list array.
 * @param array			$vocabularies	A list of vocabulary filter settings.
 */
function _muckypup_batch_create_term_tasks (&$tasks, $vocabularies) {
  
  $term_ids = _muckypup_database_get_term_ids ($vocabularies);
  
  foreach ($term_ids as $term_id) {
    $tasks[] = array ('_muckypup_batch_task', array ($term_id, MUCKYPUP_CONTRACT_TYPE_TERM));
  }
  
  return;
}

/*
* The callback of a regular finish task.
* Learm more about batch tasks from the "batch api manual".
*/
function _muckypup_batch_finish_task ($success, $results, $operations) {
    
  if ($success) {
    drupal_set_message (t('Content successfully removed.'));
  }else{
    // For future reserved.
    drupal_set_message (t('An error occured while removing content.'));
  }
  
  return;
}

/*
 * The callback of a regular removal task.
 * Learm more about batch tasks from the "batch api manual". 
 */
function _muckypup_batch_task ($id, $contract_type, &$context) {

  $result_message = t('Error. Corrupt task informations.');
  
  switch ($contract_type) {
    case MUCKYPUP_CONTRACT_TYPE_NODE:
      node_delete ($id);
      $result_message = t('Node with ID @id removed.', array ('@id'=>$id));
      break;
      
    case MUCKYPUP_CONTRACT_TYPE_USER:
      user_delete ($id);
      $result_message = t('Node with ID @id  removed.', array ('@id'=>$id));
      break;
      
    case MUCKYPUP_CONTRACT_TYPE_TERM:
    	taxonomy_term_delete($id);
    	$result_message = t('Node with ID @id removed.', array ('@id'=>$id));
    	break;
  }
  
  $context['results'][] = $result_message;
  $context['message']   = t('Removing ' . $contract_type . ' with ID @id.', array ('@id'=>$id));
  
	return;
}

