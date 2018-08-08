<?php

function spider_fread($filename) {
  $handle = fopen($filename, "r");
  if($handle) {
    $content = fread($handle, filesize($filename));
    fclose($handle);
    return $content;
  }
  return $handle;
}

