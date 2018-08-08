<?php
  define('SPIDER_ROOT', getcwd());
  require_once SPIDER_ROOT . '/bootstrap.inc';
  response(requested_path());
