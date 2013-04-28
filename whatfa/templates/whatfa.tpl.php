<?php
/**
 * @author      Marc Sven Kleinböhl (aka Hroudtwolf)
 * @copyright   Marc Sven Kleinböhl 2013(c)
 *
 * @var array $hits    An array of hits.
 */
?>
<div id="whatfa-console">
  <div id="whatfa-console-header"><span>&#9650;&#9660;&nbsp;<?php print t('WhatFA'); ?></span></div>
  <div id="whatfa-console-content">
      <?php foreach ($hits as $hit) : ?>
      <p><?php print $hit->form_id . ': '; ?>
      <?php if (! empty($hit->hooks)) : ?>
      <br />
      <?php foreach ($hit->hooks as $hook) : ?>
      <span class="hooked">-&nbsp;<?php print $hook; ?></span><br /> 
      <?php endforeach; ?>
      <?php else : ?>
      <span class="not-hooked"><?php print t('No hooking modules.'); ?></span>
      <?php endif; ?></p>
      <?php endforeach; ?>
  </div>
</div>
