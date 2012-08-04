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
  
    $backtrace = debug_backtrace();
    
    $_SESSION[self::SESSION_IDENTIFIER][] = (object)array (
      'output'     => $output, 
      'level'      => $level,
      'backtrace'  => (object)$backtrace[1]
    ); 
 
    return;
  }
  
  public static function render () {

    $messages                           = $_SESSION[self::SESSION_IDENTIFIER];
    $_SESSION[self::SESSION_IDENTIFIER] = array ();
    
    return self::renderDebux ($messages);
  }

  public static function renderDebux ($outputArray, $noBacktrace = FALSE) {
    
    $output = '';

    if (empty ($outputArray)) {
      return FALSE;
    }
    
    foreach ($outputArray as $outputField) {
      
      $output .= self::varToHtml($outputField->output, $outputField->output, 0, $outputField->level);
      
      if (! $noBacktrace) {
        $output .= t('Line @line in file "@file".', array (
          '@file'  => self::getRelativePathOf ($outputField->backtrace->file),
          '@line'  => $outputField->backtrace->line
        ));
      }
    }
    
    return theme ('debux', array ('output'=>$output));
  }
  
  private static function getLevelCSSClass ($level) {

    switch ($level) {
      case self::DEBUGLEVEL_NOTICE:
        return 'notice';
        
       case self::DEBUGLEVEL_ERROR:
        return 'error';
    }
    
    return 'unknown';
  }
  
  private static function varToHtml($var='', $key='', $scopeLevel = 0, $debugLevel = self::DEBUGLEVEL_NOTICE) {
    
    $type = gettype($var);
    $result = '';
  
    if (in_array($type, array('object','array'))) {
      $result .=  '
            <ul ' . ($scopeLevel == 0 ? 'class="first-level"' : '') . '>
              <li class="array ' . self::getLevelCSSClass($debugLevel) . '">'. $type . '('. count($var).') "' . (is_string($key) ? $key : '') . '" =></li><ul class="array-branch" style="display: none;">';
  
      foreach ($var as $akey => $val) {
        
        $scopeLevel++;
        $result .= self::varToHtml($val, $akey, $scopeLevel, $debugLevel);
        
      }
      $result .= '</ul></ul>';
    } else {
      $result .= '<li class="' . self::getLevelCSSClass($debugLevel) . '">'. $type . '::' . $key . ' ('. count ($var) .') = '.$var.'</li>';
    }
    
    $scopeLevel++;
  
    return $result;
  }
  
  private static function getRelativePathOf ($path) {
    
    $relevantPath = strpos ($path, '/sites/all/');
   
    if ($relevantPath === FALSE) {
      $relevantPath = strpos ($path, '\\sites\\all\\');
    }

    if ($relevantPath === FALSE) {
      return $path;
    } 
    
    return substr ($path, $relevantPath + 1);
  }
  
  public static function isOutputAvailable () {
    
    return ! empty ($_SESSION[self::SESSION_IDENTIFIER]);
  }
}
