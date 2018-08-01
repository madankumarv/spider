<?php
  define('SPIDER_ROOT', getcwd());
  require_once SPIDER_ROOT . '/bootstrap.inc';
  response_to(request_path());
