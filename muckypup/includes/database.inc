<?php
/*
 * @author By Marc Sven Kleinboehl aka Hroudtwolf.
 * 
 * The database layer of the muckypup module.
 */


/*
 * Loads an array of filtered node IDs.
 * @param array  $content_types	An array of content type machine names.
 * @param string $content_stati The status of nodes. ('published', 'unpublished', 'all')
 * @return array								An array which contains node IDs.
 */
function _muckypup_database_get_node_ids ($content_types, $content_stati) {
  
  $node_ids = array ();
  $where    = '';

  foreach ($content_types as  $content_type) {
  	if ($content_type != 0) {
  		$type_conditions[] = $content_type;
  	}
  }
  
  if (empty ($type_conditions)) {
    return array ();
  }
  
  $where .= 'n.type IN ("' . implode ('", "', $type_conditions) . '")';

  
  if ($content_stati != MUCKYPUP_STATUS_ALL) {
  	$where .= ' AND n.status=' . ($content_stati == MUCKYPUP_STATUS_PUBLISHED ? 1 : 0);
  }
  
  $results = db_query ('SELECT n.nid AS nid FROM {node} n WHERE ' . $where);
  
  foreach ($results as $result) {
    $node_ids[] = $result->nid;
  }
  
  return $node_ids;
}

/*
 * Loads an array of filtered user IDs.
 * @param array 	$user_roles		An array of user roles.
 * @param string  $user_stati		The user status. 'active', 'inactive', 'all'.
 * @return array								An array which contains user IDs.
 */
function _muckypup_database_get_user_ids ($user_roles, $user_stati) {
 
  $role_ids = array ();
  
  foreach ($user_roles as  $user_role) {
  	if ($user_role != 0) {
  		$role_ids[] = (integer)$user_role;
  	}
  }
  
  if (empty ($role_ids)) {
  	return array ();
  }
  
  $query_string = 'SELECT u.uid AS uid FROM {users} u';
  
  if (! empty ($role_ids)) {
    $query_string .= ' INNER JOIN {users_roles} r ON (u.uid=r.uid AND (r.rid=' . implode (' OR r.rid=', $role_ids) . '))';
  }
  
  if ($user_stati != MUCKYPUP_STATUS_ALL) {
    $query_string .= ' WHERE u.uid>1 AND u.status=' . ($user_stati == MUCKYPUP_STATUS_ACTIVE ? 1 : 0);
  }
  
  $results = db_query ($query_string);
  
  foreach ($results as $result) {
    $role_ids[] = $result->uid;
  }
  
	return $role_ids;
}

/*
 * Loads an array of terms of different vocabularies.
 * @param array	$vocabularies	An array of vocabulary IDs.
 */
function _muckypup_database_get_term_ids ($vocabularies) {

  $terms = array ();
  
  foreach ($vocabularies as $vocabulary) {
    if ($vocabulary != 0) {
      $term_tree = taxonomy_get_tree($vocabulary);
      
      foreach ($term_tree as $term_branch) {
        $terms[] = $term_branch->tid;
      }
    }
  }

	return $terms;
}

/*
* Retrieves an array of available node types, prepared for using with form-select.
* @return array	An array which contains key/value pairs of node types. (machine_name=>name)
*/
function _muckypup_database_get_node_types () {

	$types         = array ();
	$content_types = _node_types_build()->types;

	foreach ($content_types as $content_type) {
		$types[$content_type->type] = $content_type->name;
	}

	return $types;
}

/*
 * Retrieves an array of available roles, prepared for using with form-select.
* @return array	An array which contains key/value pairs of roles. (rid=>name)
*/
function _muckypup_database_get_roles () {

	$roles      = array ();
	$user_roles = user_roles();
	
	unset ($user_roles[1]); // Deleting anonymous role. By default at index 1.
	unset ($user_roles[2]); // Deleting authenticated role. By default at index 2.
	
	foreach ($user_roles as $rid=>$user_role) {
		$roles[$rid] = $user_role;
	}

	return $roles;
}

/*
 *
*/
function _muckypup_database_get_vocabularies() {

	$vocabularies       = array ();
	$saved_vocabularies = taxonomy_get_vocabularies();

	foreach ($saved_vocabularies as $saved_vocabulary) {
		$vocabularies[$saved_vocabulary->vid] = $saved_vocabulary->name;
	}

	return $vocabularies;
}