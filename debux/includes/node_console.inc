<?php
/*
 * Copyright (c) 2011-2012, Marc Sven Kleinböhl (Hroudtwolf) <coding4food@hroudtwolf.de>.
 * All rights reserved.
 */
 
function _debux_node_console ($nid) {
  
  $node = node_load ($nid);
  
  if ($node === FALSE) {    
    return;
  }

  module_load_include ('inc', 'debux', 'includes/Debux.class');
  
  return Debux::renderDebux(array (
  	(object)array ('level' => Debux::DEBUGLEVEL_NOTICE, 'output' => $node)
  ), TRUE);
}