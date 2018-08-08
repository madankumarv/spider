<?php
  define('SPIDER_ROOT', getcwd());
  require_once SPIDER_ROOT . '/includes/bootstrap.inc';
  response(requested_path());
