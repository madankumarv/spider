<?php
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
function _render_page($variables, $tpl_file = 'default') {
  ob_start();
  extract($variables);
  require_once 'themes/'.$tpl_file.'.tpl';
  ob_end_flush();
}

//Parses the URL
function _parse_url($url) {
  global $urls;
  $args = explode('/', $url);
  $callback = array_shift($args);
  if (isset($urls[$callback])) {
    $content = call_user_func_array($urls[$callback]['callback'], $args);
  }
  $variables = array(
    'title' => isset($url) ? $url : 'SMS',
    'content' => isset($content) ? $content : '<i>No content assigned/returned.</i>',
  );
  $html = _render_page($variables);
  
  return $html;
}

function response($url_path) {
  $file_name = "paths.json";
  $paths_handle = @fopen($file_name, "r");
  if($paths_handle) {
    $pathsJson = fread($paths_handle, filesize($file_name));
    $paths = json_decode($pathsJson, true);
    fclose($paths_handle);
    if(isset($paths[$url_path])) {
      _render_page($paths[$url_path]);
    } else {
      _render_page(
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
