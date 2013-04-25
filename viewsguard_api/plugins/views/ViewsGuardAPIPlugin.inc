<?php
/**
 * @copyright	2013+ by Marc Sven Kleinböhl (aka Hroudtwolf)
 * @author		Marc Sven Kleinböhl (aka Hroudtwolf)
 * 
 * The API connector plugin.
 */

/**
 * Access plugin that provides access control based hook_viewsguard_api_access($view, $account).
 *
 * @ingroup viewsguard_api
 */
class ViewsGuardAPIPlugin extends views_plugin_access {

  /**
   * Implements views_plugin_access#summary_title().
   */
  function summary_title() {
    
    return t('Views Guard API connector');
  }

  /**
   * Implements views_plugin_access#get_access_callback().
   */
  function get_access_callback() {

    return array(
      '_viewsguard_api_access_controll', 
      array(
        $this->view, 
        $GLOBALS['user']
      )
    );
  }
}
