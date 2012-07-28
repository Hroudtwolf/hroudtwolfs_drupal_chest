<?php
/*
 * Copyright (c) 2011-2012, Marc Sven Kleinböhl (Hroudtwolf) <coding4food@hroudtwolf.de>.
 * All rights reserved.
 */

/*
 * Loads redirection data from database.
 * @return 	array		An array of redirection data.
 */
function _domainhome_data_load_redirections () {
  
  $redirection = NULL;
  
  $results = $query = db_select ('domainhome_register', 'dr')
  ->fields ('dr', array ('did', 'domain', 'home'))
  ->execute ();
  
  $homes = array ();
  
  foreach ($results as $result) {
    $homes[$result->did] = array (
      'domain'  => $result->domain,
      'home'    => $result->home
    );
  }
  
  return $homes;
}

/*
 * Loads redirection data from database.
 * @param 	string $domain 	The domain.
 * @return string						The redirection page path.
 */
function _domainhome_data_load_redirection_destination ($domain) {

  $redirection = NULL;

  $result = db_select ('domainhome_register', 'dr')
  ->fields ('dr', array ('home'))
  ->condition ('dr.domain', $domain, '=')
  ->range(0, 1)
  ->execute ();

  $redirection = $result->fetchObject();
  if ($redirection === FALSE) {
    return FALSE;
  }

  return $redirection->home;
}

/*
 * Saves a new domain redirection to the database.
 * @param 	string $domain 	The domain.
 * @param		string $home		The home path.
 */
function _domainhome_data_add_redirection ($domain, $home_path) {
 
  db_insert ('domainhome_register')
  ->fields(array(
    'domain' => $domain,
    'home'   => $home_path,
  ))
  ->execute ();
  
  return;
}

/*
 * Updates a new domain redirection to the database.
 * @param 	string $domain 	The domain.
 * @param		string $home		The home path.
 */
function _domainhome_data_update_redirection ($domain, $home_path) {

  db_update ('domainhome_register')
  ->fields(array(
    'home'   => $home_path,
  ))
  ->condition ('domain', $domain, '=')
  ->execute ();

  return;
}

/*
 * Deletes a domain redirection from the database.
 * @param	integer $did 	Specifies the redirection data.
 */
function _domainhome_data_delete_redirection ($did) {
  
  db_delete('domainhome_register')
  ->condition ('did', $did, '=')
  ->execute ();
  
  return;
}
