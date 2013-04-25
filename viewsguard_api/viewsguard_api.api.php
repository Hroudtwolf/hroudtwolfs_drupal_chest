<?php
/**
 * @copyright	2013+ by Marc Sven Kleinböhl (aka Hroudtwolf)
 * @author		Marc Sven Kleinböhl (aka Hroudtwolf)
 * 
 * API example.
 */

/**
 * Implements hook_viewsguard_api_access_HOOK.
 * 
 * @param 	View 	$view			The Views object.
 * @param 	User 	$account		The User object of the currently requesting user.
 * @return	boolean					TRUE on access granted.
 * 									FALSE on access denied.
 */
function mymodule_viewsguard_api_access_THE_NAME_OF_THE_VIEW($view, $account) {
  
  // Guests not allowed.
  if (! isset ($account->uid)) {
    return FALSE;
  }
  
  // View with the URL SEGMENT 1 "private" is only accessible for the
  // viewer (user) that has the same ID like in segment 2 was given. 
  if (arg(1) == 'private' && arg(2) != $account->uid) {
    return FALSE;
  }
  
  return TRUE;
}