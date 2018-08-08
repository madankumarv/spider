<?php
require_once "utils.inc";
require_once "spider.inc";
/**
 * Returns the requested URL path of the page being viewed.
 *
 * @return
 *   The requested URL path.
 *
 * @see current_path()
 */
function requested_path() {
  static $path;

  if (isset($path)) {
    return $path;
  }

  if (isset($_SERVER['REQUEST_URI'])) {
    // This request is either a clean URL, or 'index.php', or nonsense.
    // Extract the path from REQUEST_URI.
    $request_path = strtok($_SERVER['REQUEST_URI'], '?');
    $base_path_len = strlen(rtrim(dirname($_SERVER['SCRIPT_NAME']), '\/'));
    // Unescape and strip $base_path prefix, leaving q without a leading slash.
    $path = substr(urldecode($request_path), $base_path_len + 1);
    // If the path equals the script filename, either because 'index.php' was
    // explicitly provided in the URL, or because the server added it to
    // $_SERVER['REQUEST_URI'] even when it wasn't provided in the URL (some
    // versions of Microsoft IIS do this), the front page should be served.
    if ($path == basename($_SERVER['PHP_SELF'])) {
      $path = '';
    }
  }
  else {
    // This is the front page.
    $path = '';
  }

  // Under certain conditions Apache's RewriteRule directive prepends the value
  // assigned to $_GET['q'] with a slash. Moreover we can always have a trailing
  // slash in place, hence we need to normalize $_GET['q'].
  $path = trim($path, '/');

  return $path;
}

//Renders a page
function render_page($variables, $theme='default') {
  ob_start();
  extract($variables);
  if(isset($_SERVER['HTTP_X_REQUESTED_WITH'])) {
    echo $variables['content'];
  } else {
    require_once SPIDER_ROOT.'/themes/'.$theme.'/page.tpl';
  }
  $html = ob_get_flush();
}

function response($url_path) {
  $file_name = "paths.json";
  $pathsJson = spider_fread($file_name);
  if($pathsJson) {
    $paths = json_decode($pathsJson, true);
    if(isset($paths[$url_path])) {
      $path = $paths[$url_path];
      if(!isset($path['content'])) {
        $path['content'] = call_user_func($path['callback']);
      }
      $html = render_page($path);
    } else {
      render_page(
        array(
          "title" => "404",
          "content" => "Requested page not found. Please verify the path"
        )
      ); 
    }
  } else {
    echo "<b>paths.json</b> not found!";
  }
}
