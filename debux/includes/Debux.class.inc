<?php
/*
 * Copyright (c) 2011-2012, Marc Sven Kleinböhl (Hroudtwolf) <coding4food@hroudtwolf.de>.
 * All rights reserved.
 */

class Debux {
  
  const DEBUGLEVEL_NOTICE = 0;
  const DEBUGLEVEL_ERROR  = 1;
  
  const SESSION_IDENTIFIER = 'debux_output';
  
  /*
  * This function display the a debug output in the debug area.
  * @param mixed	The debug output.
  * @param
  */
  public static function out ($output, $level = self::DEBUGLEVEL_NOTICE) {
    
    if ($_SESSION[self::SESSION_IDENTIFIER] == NULL) {#
      $_SESSION[self::SESSION_IDENTIFIER] = array ();
    }
  
    $_SESSION[self::SESSION_IDENTIFIER][] = (object)array (
      'output' => $output, 
      'level'  => $level
    ); 
    drupal_set_message(123);
    return;
  }
  
  public static function render () {

    $output                             = '';
    $outputArray                        = $_SESSION[self::SESSION_IDENTIFIER];
    $_SESSION[self::SESSION_IDENTIFIER] = array ();
    
    module_load_include ('php', 'debux', 'kint/kint.class');

    foreach ($outputArray as $outputField) {
 
//       ob_start ();
//       Kint::dump($outputField->output);      
//       $output .= ob_get_contents();
//       ob_end_clean();
$output .= self::varToHtml($outputField->output, $outputField->output);
 
    }
 
    return theme ('debux', array ('output'=>$output));
  }
  
  private static function getLevelCSSClass ($level) {

    switch ($level) {
      case self::DEBUGLEVEL_NOTICE:
        return 'notice';
        
       case self::DEBUGLEVEL_NOTICE:
        return 'error';
    }
    
    return 'unknown';
  }
  
  private static function varToHtml($var='', $key='') {
    $type = gettype($var);
    $result = '';
  
    if (in_array($type, array('object','array'))) {
      $result .=  '
            <ul>
              <li class="array">'.$type . '('. count($var).') =></li><ul class="array-branch">';
  
      foreach ($var as $akey => $val) {
        $result .= self::varToHtml($val, $akey);
      }
      $result .= '</ul></ul>';
    } else {
      $result .= '<li>'. $type . '::' . $key . ' ('. count ($var) .') = '.$var.'</li>';
    }
  
    return $result;
  }
}
