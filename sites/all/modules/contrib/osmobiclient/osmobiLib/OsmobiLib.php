<?php
/**
 * Osmobi
 *
 * @file
 * Osmobi lib file with common functionality for PHP cms systems using osmobi
 *
 * @author Tom Deryckere (Tom.Deryckere@siruna.com)
 * @author Nico Goeminne (Nico.Goeminne@siruna.com)
 * @author Heiko Desruelle
 */

define('DEBUG', FALSE); // enable in order to make service calls without security

//Variables names
define('OSMOBICLIENT_SERVICE_ENABLED', 'osmobiclient_service_enabled');
define('OSMOBICLIENT_MOBILE_URL','osmobiclient_mobile_url');
define('OSMOBICLIENT_SECRET_KEY', 'osmobiclient_secret_key');
define('OSMOBICLIENT_VERSION', 'osmobiclient_version');
define('OSMOBICLIENT_SECURITY_TIMESTAMP', 'osmobiclient_security_timestamp');
define('OSMOBICLIENT_SERVICE_INITIALIZED', 'osmobiclient_service_initialized');
if (!defined('DS')) {
  define('DS', DIRECTORY_SEPARATOR);
}

/**
 * The Osmobi class contains all functionality needed in order to
 * create an Osmobi plugin for your php based CMS.
 *
 * Usage:
 * In your module include the OsmobiLib.php file. All methods are static
 * and can be called as OsmobiLib::methodName($args).
 *
 * In order to be compatible with the Osmobi library, your code must implement
 * following functions:
 *
 * A method intercepting the initialization of your CMS that can call following Osmobi.php
 * functions:
 *  - Osmobi::userCall() => A visitor is accessing the site. Osmobi.php will
 *    take appropriate actions
 *  - Osmobi::serviceCall() => A service call is being made. Osmobi will
 *    take the appropriate acction
 *
 * osmobiclient_redirect(){} => Redirects the user to the mobile site
 * osmobiclient_theme_switch(){} => Switchtes the current template to the osmobi_mobile template
 * osmobiclient_variable_set($name, $value) {} => sets a variable
 * osmobiclient_variable_get($name, $default) {} => gets a variable
 *
 * osmobiclient_(service method function with points replaced with underscores)
 * ($arguments = array()){}
 * service calls functions must eventually call the Osmobi::serviceMethod() counterpart
 */

class OsmobiLib {
  // Error codes (in array for PHP4 compatibility)
  static $e = array(
    'OK' => '200',
    'THEME_NOT_ENABLED'     => '411',
    'THEME_NOT_INSTALLED'   => '412',
    'SECURITY_KEY_INCORRECT'=> '413',
    'HASH_FORMAT_INCORRECT' => '414',
    'INVALID_TIMESTAMP'     => '415',
    'METHOD_NON_EXISTING'   => '416',
    'INVALID_URL'           => '417',
  );

  /**
   * This function will be called by the CMS when a user
   * visits a page. The function will switch themes if being transcoded,
   * redirect if a mobile user accesses the site or do nothing for
   * a normal desktop user.
   */
   function userCall() {
      $transcoded = OsmobiLib::isTranscoded();
      $mobile     = OsmobiLib::isMobileDevice();
      $redirect   = TRUE;
      if(OsmobiLib::getRequestParameter('redirect') == 'false') {
        $redirect   = FALSE; // no redirection if thisparameter is added
      }
      if ($transcoded) {
        osmobiclient_switch_theme();
      }
      elseif ($mobile && osmobiclient_variable_get(OSMOBICLIENT_SERVICE_ENABLED, FALSE) && $redirect) {
        osmobiclient_redirect();
      }
   }

  /**
   * Method called by the CMS code when a webservice call is being made
   * This method will check the validity of the call and dispatch back to
   * the CMS module using the following convention:
   * e.g.: osmobiclient.wcm.isValidInstall is dispatched to osmobiclient_wcm_isvalidinstall()
   *
   * @param $method
   *  $method contains the $_GET['method'] argument
   * @param $hash
   *  $hash contains the $_GET['hash'] argument
   *
   */
  function serviceCall($method, $hash) {
      // Fetch all service method definitions
    $methods = OsmobiLib::getServiceDefinitions();
    // If the hash, and the method are not null we can start handling the request
    // Skip if debugging!!
    if (DEBUG) {
      // get the method arguments
      $arguments = $methods[$method];
      // translate the method '.' to the real method prototype
      $method = str_replace(".", "_", $method);
      // execute the method
      $method($arguments);
      return;
    }
    if (isset($hash, $method, $methods[$method])) {
      // The security hash has a timestamp and an MD5 hash part separated by a '|'
      $hash_parts = explode("|", $hash, 2);
      // Assume the request is not authorized untill proven otherwhise
      $auth = FALSE;
      if (count($hash_parts) == 2) {
        // Security check & response if security check fails
         $auth = OsmobiLib::checkSecurity($method, $methods[$method], $hash_parts[1], $hash_parts[0]);
      }
      else {
        // Hash does not have a timestamp '|' sepeartor
        // Respond with a forbidden reply
        $xml = '<message>hash format incorrect</message>';
        OsmobiLib::sendResponse($xml, 'error', OsmobiLib::$e['HASH_FORMAT_INCORRECT']);
      }
      // authentication
      if ($auth) {
        // get the method arguments
        $arguments = $methods[$method];
        // translate the method '.' to the real method prototype
        $method = str_replace(".", "_", $method);
        // execute the method
        return array('method' => $method, 'arguments' => $arguments);
      }
    }
    elseif(!isset($hash) && isset($methods[$method])) {
      $xml = '<message>The Osmobi service is running. Please provide a hash code.</message>';
      $code = OsmobiLib::$e['METHOD_NON_EXISTING'];
      $type = 'error';
    }
    else {
      $xml = '<message>The Osmobi service is running. Please provide a valid method.</message>';
      $code = OsmobiLib::$e['METHOD_NON_EXISTING'];
      $type = 'error';
    }
    OsmobiLib::sendResponse($xml, $type, $code);
  }

  /**
   * Check if the site is being transcoded
   * TODO: should we cache the result?
   * @return
   *   TRUE if the site is transcoded, FALSE if the site is not transcoded
   */
  function isTranscoded() {
    $methods = OsmobiLib::getServiceDefinitions();
    $method  = OsmobiLib::getRequestParameter('method');
    if (isset($method, $methods[$method])) { // This is a service call
      return FALSE;
    }
    if (array_key_exists('HTTP_X_ADAPTATION_ENGINE', $_SERVER)) {
      if (preg_match('/Siruna/', $_SERVER['HTTP_X_ADAPTATION_ENGINE'])) {
       return TRUE;
      }
      else {
       return FALSE;
      }
    }
    else {
      return FALSE;
    }
  }

  /**
   * Method to detect if the user is surfing with a mobile device
   * TODO: Should we cache the result?
   * @return
   *   TRUE for a mobile device, otherwise FALSE
   */
  function isMobileDevice() {
    // set mobile browser as FALSE till we can prove otherwise
    $mobile = FALSE;
    // get the user agent value
    // FIXME: this should be cleaned to ensure no nefarious input gets executed
    if (array_key_exists('HTTP_USER_AGENT', $_SERVER)) {
      $user_agent = $_SERVER['HTTP_USER_AGENT'];
    }
    else {
      return FALSE;
    }
    // get the content accept value
    // FIXME: this should be cleaned to ensure no nefarious input gets executed
    if (array_key_exists('HTTP_ACCEPT', $_SERVER)) {
      $accept = $_SERVER['HTTP_ACCEPT'];
    }
    else {
      $accept = '';
    }
    // If the user agent is java the we assume it is not a mobile device
    if (eregi('java', $user_agent)) {
      return FALSE;
    }
    switch (TRUE) {
      case (eregi('ipod', $user_agent) || eregi('iphone', $user_agent) || eregi('android', $user_agent) || eregi('opera mini',  $user_agent) || eregi('blackberry', $user_agent) );
        $mobile = TRUE;
        break;
      case (preg_match('/(up.browser|up.link|mmp|symbian|smartphone|midp|wap|vodafone|o2|pocket|kindle|mobile|pda|psp|treo|foma)/i', $user_agent));
        $mobile = TRUE;
        break;
      case ((strpos($accept, 'text/vnd.wap.wml')>0) || (strpos($accept, 'application/vnd.wap.xhtml+xml')>0));
        $mobile = TRUE;
        break;
      // Is the device giving us a HTTP_X_WAP_PROFILE or HTTP_PROFILE header - only mobile devices would do this
      case (isset($_SERVER['HTTP_X_WAP_PROFILE']) || isset($_SERVER['HTTP_PROFILE']));
        $mobile = TRUE;
        break;
      case (in_array(strtolower(substr($user_agent, 0, 4)),array('1207' => '1207','3gso' => '3gso','4thp' => '4thp','501i' => '501i','502i' => '502i','503i' => '503i','504i' => '504i','505i' => '505i','506i' => '506i','6310' => '6310','6590' => '6590','770s' => '770s','802s' => '802s','a wa' => 'a wa','acer' => 'acer','acs-' => 'acs-','airn' => 'airn','alav' => 'alav','asus' => 'asus','attw' => 'attw','au-m' => 'au-m','aur ' => 'aur ','aus ' => 'aus ','abac' => 'abac','acoo' => 'acoo','aiko' => 'aiko','alco' => 'alco','alca' => 'alca','amoi' => 'amoi','anex' => 'anex','anny' => 'anny','anyw' => 'anyw','aptu' => 'aptu','arch' => 'arch','argo' => 'argo','bell' => 'bell','bird' => 'bird','bw-n' => 'bw-n','bw-u' => 'bw-u','beck' => 'beck','benq' => 'benq','bilb' => 'bilb','blac' => 'blac','c55/' => 'c55/','cdm-' => 'cdm-','chtm' => 'chtm','capi' => 'capi','comp' => 'comp','cond' => 'cond','craw' => 'craw','dall' => 'dall','dbte' => 'dbte','dc-s' => 'dc-s','dica' => 'dica','ds-d' => 'ds-d','ds12' => 'ds12','dait' => 'dait','devi' => 'devi','dmob' => 'dmob','doco' => 'doco','dopo' => 'dopo','el49' => 'el49','erk0' => 'erk0','esl8' => 'esl8','ez40' => 'ez40','ez60' => 'ez60','ez70' => 'ez70','ezos' => 'ezos','ezze' => 'ezze','elai' => 'elai','emul' => 'emul','eric' => 'eric','ezwa' => 'ezwa','fake' => 'fake','fly-' => 'fly-','fly_' => 'fly_','g-mo' => 'g-mo','g1 u' => 'g1 u','g560' => 'g560','gf-5' => 'gf-5','grun' => 'grun','gene' => 'gene','go.w' => 'go.w','good' => 'good','grad' => 'grad','hcit' => 'hcit','hd-m' => 'hd-m','hd-p' => 'hd-p','hd-t' => 'hd-t','hei-' => 'hei-','hp i' => 'hp i','hpip' => 'hpip','hs-c' => 'hs-c','htc ' => 'htc ','htc-' => 'htc-','htca' => 'htca','htcg' => 'htcg','htcp' => 'htcp','htcs' => 'htcs','htct' => 'htct','htc_' => 'htc_','haie' => 'haie','hita' => 'hita','huaw' => 'huaw','hutc' => 'hutc','i-20' => 'i-20','i-go' => 'i-go','i-ma' => 'i-ma','i230' => 'i230','iac' => 'iac','iac-' => 'iac-','iac/' => 'iac/','ig01' => 'ig01','im1k' => 'im1k','inno' => 'inno','iris' => 'iris','jata' => 'jata','java' => 'java','kddi' => 'kddi','kgt' => 'kgt','kgt/' => 'kgt/','kpt ' => 'kpt ','kwc-' => 'kwc-','klon' => 'klon','lexi' => 'lexi','lg g' => 'lg g','lg-a' => 'lg-a','lg-b' => 'lg-b','lg-c' => 'lg-c','lg-d' => 'lg-d','lg-f' => 'lg-f','lg-g' => 'lg-g','lg-k' => 'lg-k','lg-l' => 'lg-l','lg-m' => 'lg-m','lg-o' => 'lg-o','lg-p' => 'lg-p','lg-s' => 'lg-s','lg-t' => 'lg-t','lg-u' => 'lg-u','lg-w' => 'lg-w','lg/k' => 'lg/k','lg/l' => 'lg/l','lg/u' => 'lg/u','lg50' => 'lg50','lg54' => 'lg54','lge-' => 'lge-','lge/' => 'lge/','lynx' => 'lynx','leno' => 'leno','m1-w' => 'm1-w','m3ga' => 'm3ga','m50/' => 'm50/','maui' => 'maui','mc01' => 'mc01','mc21' => 'mc21','mcca' => 'mcca','medi' => 'medi','meri' => 'meri','mio8' => 'mio8','mioa' => 'mioa','mo01' => 'mo01','mo02' => 'mo02','mode' => 'mode','modo' => 'modo','mot ' => 'mot ','mot-' => 'mot-','mt50' => 'mt50','mtp1' => 'mtp1','mtv ' => 'mtv ','mate' => 'mate','maxo' => 'maxo','merc' => 'merc','mits' => 'mits','mobi' => 'mobi','motv' => 'motv','mozz' => 'mozz','n100' => 'n100','n101' => 'n101','n102' => 'n102','n202' => 'n202','n203' => 'n203','n300' => 'n300','n302' => 'n302','n500' => 'n500','n502' => 'n502','n505' => 'n505','n700' => 'n700','n701' => 'n701','n710' => 'n710','nec-' => 'nec-','nem-' => 'nem-','newg' => 'newg','neon' => 'neon','netf' => 'netf','noki' => 'noki','nzph' => 'nzph','o2 x' => 'o2 x','o2-x' => 'o2-x','opwv' => 'opwv','owg1' => 'owg1','opti' => 'opti','oran' => 'oran','p800' => 'p800','pand' => 'pand','pg-1' => 'pg-1','pg-2' => 'pg-2','pg-3' => 'pg-3','pg-6' => 'pg-6','pg-8' => 'pg-8','pg-c' => 'pg-c','pg13' => 'pg13','phil' => 'phil','pn-2' => 'pn-2','ppc;' => 'ppc;','pt-g' => 'pt-g','palm' => 'palm','pana' => 'pana','pire' => 'pire','pock' => 'pock','pose' => 'pose','psio' => 'psio','qa-a' => 'qa-a','qc-2' => 'qc-2','qc-3' => 'qc-3','qc-5' => 'qc-5','qc-7' => 'qc-7','qc07' => 'qc07','qc12' => 'qc12','qc21' => 'qc21','qc32' => 'qc32','qc60' => 'qc60','qci-' => 'qci-','qwap' => 'qwap','qtek' => 'qtek','r380' => 'r380','r600' => 'r600','raks' => 'raks','rim9' => 'rim9','rove' => 'rove','s55/' => 's55/','sage' => 'sage','sams' => 'sams','sc01' => 'sc01','sch-' => 'sch-','scp-' => 'scp-','sdk/' => 'sdk/','se47' => 'se47','sec-' => 'sec-','sec0' => 'sec0','sec1' => 'sec1','semc' => 'semc','sgh-' => 'sgh-','shar' => 'shar','sie-' => 'sie-','sk-0' => 'sk-0','sl45' => 'sl45','slid' => 'slid','smb3' => 'smb3','smt5' => 'smt5','sp01' => 'sp01','sph-' => 'sph-','spv ' => 'spv ','spv-' => 'spv-','sy01' => 'sy01','samm' => 'samm','sany' => 'sany','sava' => 'sava','scoo' => 'scoo','send' => 'send','siem' => 'siem','smar' => 'smar','smit' => 'smit','soft' => 'soft','sony' => 'sony','t-mo' => 't-mo','t218' => 't218','t250' => 't250','t600' => 't600','t610' => 't610','t618' => 't618','tcl-' => 'tcl-','tdg-' => 'tdg-','telm' => 'telm','tim-' => 'tim-','ts70' => 'ts70','tsm-' => 'tsm-','tsm3' => 'tsm3','tsm5' => 'tsm5','tx-9' => 'tx-9','tagt' => 'tagt','talk' => 'talk','teli' => 'teli','topl' => 'topl','tosh' => 'tosh','up.b' => 'up.b','upg1' => 'upg1','utst' => 'utst','v400' => 'v400','v750' => 'v750','veri' => 'veri','vk-v' => 'vk-v','vk40' => 'vk40','vk50' => 'vk50','vk52' => 'vk52','vk53' => 'vk53','vm40' => 'vm40','vx98' => 'vx98','virg' => 'virg','vite' => 'vite','voda' => 'voda','vulc' => 'vulc','w3c ' => 'w3c ','w3c-' => 'w3c-','wapj' => 'wapj','wapp' => 'wapp','wapu' => 'wapu','wapm' => 'wapm','wig ' => 'wig ','wapi' => 'wapi','wapr' => 'wapr','wapv' => 'wapv','wapy' => 'wapy','wapa' => 'wapa','waps' => 'waps','wapt' => 'wapt','winc' => 'winc','winw' => 'winw','wonu' => 'wonu','x700' => 'x700','xda2' => 'xda2','xdag' => 'xdag','yas-' => 'yas-','your' => 'your','zte-' => 'zte-','zeto' => 'zeto','acs-' => 'acs-','alav' => 'alav','alca' => 'alca','amoi' => 'amoi','aste' => 'aste','audi' => 'audi','avan' => 'avan','benq' => 'benq','bird' => 'bird','blac' => 'blac','blaz' => 'blaz','brew' => 'brew','brvw' => 'brvw','bumb' => 'bumb','ccwa' => 'ccwa','cell' => 'cell','cldc' => 'cldc','cmd-' => 'cmd-','dang' => 'dang','doco' => 'doco','eml2' => 'eml2','eric' => 'eric','fetc' => 'fetc','hipt' => 'hipt','http' => 'http','ibro' => 'ibro','idea' => 'idea','ikom' => 'ikom','inno' => 'inno','ipaq' => 'ipaq','jbro' => 'jbro','jemu' => 'jemu','java' => 'java','jigs' => 'jigs','kddi' => 'kddi','keji' => 'keji','kyoc' => 'kyoc','kyok' => 'kyok','leno' => 'leno','lg-c' => 'lg-c','lg-d' => 'lg-d','lg-g' => 'lg-g','lge-' => 'lge-','libw' => 'libw','m-cr' => 'm-cr','maui' => 'maui','maxo' => 'maxo','midp' => 'midp','mits' => 'mits','mmef' => 'mmef','mobi' => 'mobi','mot-' => 'mot-','moto' => 'moto','mwbp' => 'mwbp','mywa' => 'mywa','nec-' => 'nec-','newt' => 'newt','nok6' => 'nok6','noki' => 'noki','o2im' => 'o2im','opwv' => 'opwv','palm' => 'palm','pana' => 'pana','pant' => 'pant','pdxg' => 'pdxg','phil' => 'phil','play' => 'play','pluc' => 'pluc','port' => 'port','prox' => 'prox','qtek' => 'qtek','qwap' => 'qwap','rozo' => 'rozo','sage' => 'sage','sama' => 'sama','sams' => 'sams','sany' => 'sany','sch-' => 'sch-','sec-' => 'sec-','send' => 'send','seri' => 'seri','sgh-' => 'sgh-','shar' => 'shar','sie-' => 'sie-','siem' => 'siem','smal' => 'smal','smar' => 'smar','sony' => 'sony','sph-' => 'sph-','symb' => 'symb','t-mo' => 't-mo','teli' => 'teli','tim-' => 'tim-','tosh' => 'tosh','treo' => 'treo','tsm-' => 'tsm-','upg1' => 'upg1','upsi' => 'upsi','vk-v' => 'vk-v','voda' => 'voda','vx52' => 'vx52','vx53' => 'vx53','vx60' => 'vx60','vx61' => 'vx61','vx70' => 'vx70','vx80' => 'vx80','vx81' => 'vx81','vx83' => 'vx83','vx85' => 'vx85','wap-' => 'wap-','wapa' => 'wapa','wapi' => 'wapi','wapp' => 'wapp','wapr' => 'wapr','webc' => 'webc','whit' => 'whit','winw' => 'winw','wmlb' => 'wmlb','xda-' => 'xda-')));
       $mobile = TRUE;
        break;
    }
    return $mobile;
  }

  /**
   * Implementation called by osmobiclient_wcm_isValidInstall
   * @param $base_url
   *   $base_url is the base url of the CMS installation
   * @param $theme
   *   $theme contains an associative array array('installed' => boolean, 'enabled' => boolean)
   *  to report on the installation status of the theme
   */
  function isValidInstall($base_url, $theme) {
    $out = '';
    if ($theme['enabled']) {
      $status = osmobiclient_variable_get(OSMOBICLIENT_SERVICE_ENABLED, FALSE);
      if ($status) {
        $launched = 'true';
      }
      else {
        $launched = 'false';
      }
      $out .= '<base_url>' . $base_url . '</base_url>';
      $out .= '<launched>' . $launched . '</launched>';
      OsmobiLib::sendResponse($out, 'ok', '200');
    }
    elseif ($theme['installed']) {
      $out = '<message>Mobile theme is not enabled. Enable the Osmobi theme at admin/settings/themes</message>';
      OsmobiLib::sendResponse($out, 'error', OsmobiLib::$e['THEME_NOT_ENABLED']);
    }
    else {
      $out = '<message>Mobile theme is not installed. Please download and install the Osmobi theme before continuing</message>';
      OsmobiLib::sendResponse($out, 'error', OsmobiLib::$e['THEME_NOT_INSTALLED']);
    }
  }

  /**
   * Set the mobile url of the Osmobi site
   * @param $url
   *  $url where the user will redirected to. If $url is empty, the status of the
   *  site will be put on false.
   */
  function setMobileURL($url) {
    if (OsmobiLib::validateURL($url) || $url == '' || $url == 'false') {
      osmobiclient_variable_set(OSMOBICLIENT_MOBILE_URL, $url);
      $message = '';
      if ($url == '' || $url == 'false') {
        osmobiclient_variable_set(OSMOBICLIENT_SERVICE_ENABLED, FALSE);
        $message = 'Mobile site unlaunched';
      }
      else {
        osmobiclient_variable_set(OSMOBICLIENT_SERVICE_ENABLED, TRUE);
        $message = 'Mobile site launched';
      }
      $xml  = '';
      $xml .= '<message>' . $message . '</message>';
      OsmobiLib::sendResponse($xml, 'ok', '200');
    }
    else {
      $code = OsmobiLib::$e['INVALID_URL'];
      $xml  = '<message>Mobile site not launched, the given URL is not valid.</message>';
      $type = 'error';
      OsmobiLib::sendResponse($xml, $type, $code);
    }
    
  }

  /**
   * @param $name
   *   $name is the name of the desktop theme
   * @param $colors
   *  $colors is an array containing the colors of the theme (as retrieve by retrieveColors())
   * @param $images
   *  $images is an array containing the images of the them (as retrieved by retrieveImages())
   *
   */
  function getOriginalThemeInfo($name, $colors, $images){
     $out = '';
     $out .= '<theme>';
     $out .= '<name>' . $name .'</name>';
     $out .= OsmobiLib::wrapColors($colors);
     $out .= OsmobiLib::wrapImages($images);
     $out .= '</theme>';
     OsmobiLib::sendResponse($out, 'ok', '200');
  }

  /***
   * Method to get the service definitions
   *
   * @return
   *    $methods An array of available service methods
   */
    function getServiceDefinitions() {
     $methods = array(
      'osmobiclient.wcm.isValidInstall' => array(),
      'osmobiclient.wcm.themes.getOriginalThemeInfo' => array(),
      'osmobiclient.wcm.url.setMobileURL' => array(
        'url' => OsmobiLib::getRequestParameter('url')
      )
    );
    return $methods;
  }

  /**
   * Method to generate security key, based on current time
   *
   * @return
   *   Security key
   */
  function generateSecurityKey() {
    $length = 32;
    // sha1 hashes have a 40 characters length
    $max = ceil($length / 40);
    $random = '';
    for ($i = 0; $i < $max; $i ++) {
      $random .= sha1(microtime(true) . mt_rand(10000,90000));
    }
    return substr($random, 0, $length);
  }
  /**
   * Function to check the if a secure call is made. A call has the following structure:
   * ?method=[method name]&arg1=value1&arg2=value&...&hash=[hash key]|[timestamp]
   *
   * @param $method
   *    $method is the method of the call
   * @param $arguments
   *    $arguments contains an associative array with arg=value pairs that are given during the request
   * @param $hash
   *   $hash is the [hash key] as calculated by the calling server
   * @param $timestamp
   *  The [timestamp] value
   * @returns
   *   returns false and sends a forbidden response to the service client if the access is not authorized
   */
  function checkSecurity($method, $arguments, $hash, $timestamp) {
    // each call must have a newer timestamp (ms since 01/01/1970)
    $last_stamp = osmobiclient_variable_get(OSMOBICLIENT_SECURITY_TIMESTAMP, 0);
    if ($timestamp - $last_stamp > 0) {
      // get the secret key and the hash
      $toHash = osmobiclient_variable_get(OSMOBICLIENT_SECRET_KEY, NULL) . 'method=' . $method;
      // Add arguments to the hash
      foreach ($arguments as $key => $argument) {
        // drupal automatically urldecodes requests arguments
        // solution to be compatible with the Global Redirect bug module
        if ($method == 'osmobiclient.wcm.url.setMobileURL' && $key == 'url') {
          $value = str_replace('http%3A%2F%252F', 'http%3A%2F%2F', urlencode($argument));
        }
        else {
          $value = urlencode($argument);
        }
        $toHash .= '&' . $key . '=' . $value;
      }
      // Add timestamp to the hash
      $toHash .= '&hash=' . $timestamp;
      // Check the hash
      $auth = (md5($toHash) == $hash);
      if ($auth) {
        // authentication succesed update timestamp
        osmobiclient_variable_set(OSMOBICLIENT_SECURITY_TIMESTAMP, $timestamp);
      }
      else {
        // authentication failed
        // send client response
        $type    = 'error';
        $xml = '<message>hash does not match</message>';
        $code    = OsmobiLib::$e['SECURITY_KEY_INCORRECT'];
        OsmobiLib::sendResponse($xml, $type, $code);
      }
      return $auth;
    }
    else {
      // Invalid time stamp
      // send client response
      $type    = 'error';
      $xml = '<message>timestamp not accepted</message>';
      $code    = OsmobiLib::$e['INVALID_TIMESTAMP'];
      OsmobiLib::sendResponse($xml, $type, $code);
    }
    return FALSE;
  }

  /**
   * Method retrieving all the colors that are present in the stylesheets of the current active theme
   *
   * @param $stylesheets
   *   Array containing the paths to all the stylesheets that are present for the theme
   *
   * @return $colors
   *   Array containg all the color information. The array is an associative array containg the color values as key and value of the color
   **/
  function retrieveColors($stylesheets) {
    // Colors placeholder array
    $colors = array();
    // Accumulate all colors from all stylesheets
    foreach ($stylesheets as $name => $path) {
      if (file_exists($path)) {
        $colors = OsmobiLib::readCSS($path, $colors);
      }
    }
    return $colors;
  }

  /**
   * Method to retrieve all the images within the given paths
   *
   * @param $paths
   *  The paths of the directories where images are available
   * @param $base_url
   *   The base_url of the current CMS installation
   * @return $images
   *  Array containg information of all the images
   */
  function retrieveImages($paths, $base_url) {
    $images = array();
    // Accumulate all images from all themes
    foreach ($paths as $path) {
      $images = OsmobiLib::recursiveRetrieveImages($path, $images, $base_url, TRUE);
    }
    return $images;
  }

  /**
   * This method will return all stylesheets for the given paths
   *
   * @param $paths
   *  $paths is an array with paths that should be checked to get the stylesheets.
   * @return $stylesheets
   *  $stylesheets is an array with paths to all the available stylesheets
   */
  function retrieveStylesheets($paths) {
    $stylesheets = array();
    foreach ($paths as $path) {
      $stylesheets = OsmobiLib::recursiveRetrieveStyleSheets($path, $stylesheets, TRUE);
    }
    return $stylesheets;
  }

  /**
   * Method to be called to create a service response
   *
   * @param $type
   * The $type parameter is being used to communicate the type of response that is being generated.
   * Currently following options:
   *  - error
   *  - osmobiclient.wcm.isValidInstall
   *  - osmobiclient.wcm.themes.getOriginalThemeInfo
   *  -osmobiclient.wcm.url.setMobileURL
   * @param $xml
   *   $xml is the message body
   * @param $stat
   *  $stat is the status ('ok' or 'error')
   * @param $code
   *  $code is the response code.
   *
   * @return
   *   The function exits by returning the response
   */
  function sendResponse($xml, $stat, $code) {
    $version = osmobiclient_variable_get('osmobiclient_version', '1.6.2');
    $out  = '<?xml version="1.0" ?>';
    $out .= '<rsp stat="'. $stat . '">';
    $out .= '<version>' . $version . '</version>';
    $out .= '<responsecode>' . $code . '</responsecode>';
    $out .= $xml;
    $out .= '</rsp>';
    //header('Content-type: text/xml');
    print $out;
    exit();
  }

  /**
   * Function for geting a request parameter
   *
   * Returns null if the parameter does not exists
   */
  function getRequestParameter($name) {
    if (isset($_GET[$name])) {
      return $_GET[$name];
    }
    else {
      return NULL;
    }
  }

  /**
   * Read the CSS file and add the colors to $colors
   * FIXME: take rgb() format into account
   *
   * @param $file
   *  Path to the CSS file where to retrieve the colors
   * @param $colors
   *  Array where to append the color information
   */
  function readCSS($file, $colors) {
    // build webcolors regex
    include 'webcolors.inc';
    $colors_exp = '';
    foreach ($colorname as $name => $hex) {
      $colors_exp .= $name . '|';
    }
    // parse CSS file
    $content = file_get_contents($file);
    $parser = new OsmobiCssParser();
    $parser->ParseStr($content);
    $css = $parser->getArray();
    // search for attributes containing a color value
    foreach ($css as $key => $value) {
      foreach ($value as $attribute => $attributevalue) {
        $matches = array();
        // complete color matching regex
        $exp = "/(#([0-9A-Fa-f]{3,6})\b)|"
          . $colors_exp .
          "(rgb\(\s*\b([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\b\s*,\s*\b([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\b\s*,\s*\b([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\b\s*\))|(rgb\(\s*(\d?\d%|100%)+\s*,\s*(\d?\d%|100%)+\s*,\s*(\d?\d%|100%)+\s*\))/";
        preg_match_all($exp , $attributevalue, $matches);
        if ($matches[0]) {
          // colormatch cleanup
          $match = strtolower($matches[0][0]);
          // webcolor to hex
          if (isset($colorname[$match])) {
            $match = $colorname[$match];
          }
          $match = preg_replace('/#/', '', $match);
          // rgb to 24 bit hex
          if (preg_match('/rgb/', $match)) {
            $match = rtrim(ltrim($match, 'rgb('), ');');
            $rgb = explode(',', $match);
            $r = trim($rgb[0]);
            $g = trim($rgb[1]);
            $b = trim($rgb[2]);
            $match = OsmobiLib::rgb2html($r, $g, $b);
          }
          // 12bit to 24bit hex
          if (strlen($match) == 3) {
            $match = substr($match, 0, 1) . substr($match, 0, 1) . substr($match, 1, 1) . substr($match, 1, 1) . substr($match, 2, 1) . substr($match, 2, 1);
          }
          if ($match != '') {
            $match = preg_replace('/#/', '', $match);
            $colors[$match] = $match;
          }
        }
      }
    }
    return $colors;
  }
/**
 * Helper function to convert rgb colors to hex
 * (code from http://www.anyexample.com/programming/php/php_convert_rgb_from_to_html_hex_color.xml)
 *
 **/
  function rgb2html($r=-1, $g=-1, $b=-1) {
      $r = intval($r); 
      $g = intval($g);
      $b = intval($b);
      $r = dechex($r<0?0:($r>255?255:$r));
      $g = dechex($g<0?0:($g>255?255:$g));
      $b = dechex($b<0?0:($b>255?255:$b));
      $color = (strlen($r) < 2?'0':'').$r;
      $color .= (strlen($g) < 2?'0':'').$g;
      $color .= (strlen($b) < 2?'0':'').$b;
      return $color;
  }
  /**
   * This method retrieves images from a given path ($path). If $init = TRUE images will
   * be fetched 1 level down. The output is an array of images that can be used in the
   * getOriginalThemeInfo() method
   *
   * @param $path
   *   The path where the images must be retrieved
   * @param $images
   *  The array containing the images of a previous iteration
   * @param $base_url
   *  Base url of the CMS. This is appended to the images path
   * @param $init
   *  Indicates if images must be fetched from 1 level down
   * @return $images
   *  Array containing images data
   *
   */
  function recursiveRetrieveImages($path, $images, $base_url, $init = TRUE) {
    $handler = opendir($path);
    while ($file = readdir($handler)) {
      $ext = substr($file, strrpos($file, '.') + 1);
      $name = substr($file, 0, strrpos($file, '.'));
      if ($ext != 'info' || $init == TRUE) {
        if ($file != '.' && $file != '..' && !is_dir($path .DS. $file) && $name != 'screenshot' && ($ext == "png" || $ext == 'jpg' || $ext == 'jpeg' || $ext == 'gif' ) ) {
          $images[$base_url . '/' . $path .'/'. $file] = $name;
        }
        elseif (is_dir($path .'/'. $file) && $file != '.' && $file != '..' ) {
          $images = OsmobiLib::recursiveRetrieveImages($path .'/'. $file, $images , $base_url, FALSE);
        }
      }
      else {
        closedir($handler);
        return $images;
      }
    }
    closedir($handler);
    return $images;
  }

  /***
   * This function retrieves all stylesheets in a certain path.
   * This method will put all the stylesheets in the given directory in an array
   *
   * @param $path
   *     $path is the path of the directory where to retrieve the stylesheet
   * @param $stylesheets
   *     $stylesheets contains the array with paths to all stylesheets
   * @init
   *     shows if we are in the first level of iteration
   */
  function  recursiveRetrieveStylesheets($path, $stylesheets, $init = TRUE) {
    if (is_dir($path)) {
      $handler = opendir($path);
      // read all files in directory
      while ($file = readdir($handler)) {
        $ext = substr($file, strrpos($file, '.') + 1);
        if ($ext != 'info' || $init == TRUE) {
          if ($ext == 'css') {
            $stylesheets[] = $path .DS. $file;
          }
          else if (is_dir($path .DS. $file) && $file != '.' && $file != '..' ) {
            $stylesheets = OsmobiLib::recursiveRetrieveStylesheets($path .DS. $file, $stylesheets, FALSE);
          }
        }
        else {
          closedir($handler);
        }
      }
      closedir($handler);
    }
    return $stylesheets;
  }

  /**
   * Helper function, should not be called directly from any other php code. The function provides basic
   * wrapping to create the xml document
   * @param $colors
   *   $colors is an array containing  color data
   * @return $out
   *   XML output containing the colors wrapped in XML
   */
  function wrapColors($colors) {
    $out = '';
    $out .= '<colors>';
    foreach($colors as $color) {
      $out .= '<color value="' . $color . '" />';
    }
    $out .= '</colors>';
    return $out;
  }

  /**
   * Helper function, should not be called directly from any other php code.
   * The function provides basic wrapping to create the xml document
   * @param $images
   *   $images is an array containing  associative arrays with image data:
   *   array('id' => filename, 'src' => path_to_image)
   * @return $out
   *   $out xml output <images><image id="" src="" /><image id="" src="" /></images>
   */
  function wrapImages($images) {
    $out = '';
    $out .= '<images>';
    foreach($images as $path => $file) {
      $out .= '<image src="' . $path . '" />';
    }
    $out .= '</images>';
    return $out;
  }

  /**
   * Function to get general help texts
   */
  function getGeneralInstructions() {
    $out .= '';
    $out .= 'In order to mobilize your site complete the following steps:';
    $out .= '<ul>' ;
    $out .=   '<li>Go back to <a target="_blank" href="http://www.osmobi.com">OSMOBI</a></li>';
    $out .=   '<li>Create an <a target="_blank" href="http://www.osmobi.com/user/register">account</a> or <a target="_blank" href="http://www.osmobi.com/user">sign in</a> if you already have an OSMOBI account</li>';
    $out .=   '<li>Create a new project and copy paste your security key: <b>' . osmobiclient_variable_get(OSMOBICLIENT_SECRET_KEY, '') . '</b></li>';
    $out .=   '<li>Start mobilizing using OSMOBI</li>';
    $out .=   '<li>Preview and launch your new mobile site!</li>';
    $out .=  '</ul>';
    return $out;
  }

  /**
   * Function to get general information about the system.
   */
  function getSystemInfo() {
    $theme_info = osmobiclient_check_theme();
    return  array(
      'PHP Version'     => PHP_VERSION,
      'Osmobi Version'  => osmobiclient_variable_get(OSMOBICLIENT_VERSION, ''),
      'Server Type'      => $_SERVER['SERVER_SOFTWARE'],
      'OS'              => PHP_OS,
      'Osmbi Service launched'=> osmobiclient_variable_get(OSMOBICLIENT_SERVICE_ENABLED, 'FALSE') ? 'launched' : 'not launched',
      'Osmobi mobile theme installed'  => $theme_info['installed'] ? 'installed' : 'not installed',
      'Osmobi theme enabled'    => $theme_info['enabled'] ? 'enabled' : 'not enabled',
      'Osmobi security-key'     => osmobiclient_variable_get(OSMOBICLIENT_SECRET_KEY, ''),
      'CMS-type'        => osmobiclient_get_cms(),
    );
  }
  
  /**
   * Helper function to validate  url's. Code comes from
   *    http://www.phpcentral.com/208-url-validation-php.html
   */
  function validateURL($url) {
    // SCHEME
    $urlregex = "^(https?|ftp)\:\/\/";

    // USER AND PASS (optional)
    $urlregex .= "([a-z0-9+!*(),;?&=\$_.-]+(\:[a-z0-9+!*(),;?&=\$_.-]+)?@)?";

    // HOSTNAME OR IP
    $urlregex .= "[a-z0-9+\$_-]+(\.[a-z0-9+\$_-]+)*";  // http://x = allowed (ex. http://localhost, http://routerlogin)
    //$urlregex .= "[a-z0-9+\$_-]+(\.[a-z0-9+\$_-]+)+";  // http://x.x = minimum
    //$urlregex .= "([a-z0-9+\$_-]+\.)*[a-z0-9+\$_-]{2,3}";  // http://x.xx(x) = minimum
    //use only one of the above

    // PORT (optional)
    $urlregex .= "(\:[0-9]{2,5})?";
    // PATH  (optional)
    $urlregex .= "(\/([a-z0-9%+\$_-]\.?)+)*\/?";
    // GET Query (optional)
    $urlregex .= "(\?[a-z+&\$_.-][a-z0-9;:@/&%=+\$_.-]*)?";
    // ANCHOR (optional)
    $urlregex .= "(#[a-z_.-][a-z0-9+\$_.-]*)?\$";
    // check
    if (eregi($urlregex, $url)) {return TRUE;} else {return FALSE;}
   }
}

/**
 * OsmobiCssParser class retrieves the colors from CSS files
 **/
class OsmobiCssParser {
  var $css;
  var $html;

  function OsmobiCssParser($html = true) {
    // Register "destructor"
    register_shutdown_function(array(&$this, "finalize"));
    $this->html = ($html != false);
  }

  function finalize() {
    unset($this->css);
  }

  function SetHTML($html) {
    $this->html = ($html != false);
  }

  function Add($key, $codestr) {
    $key = strtolower($key);
    $codestr = strtolower($codestr);
    if(!isset($this->css[$key])) {
      $this->css[$key] = array();
    }
    $codes = explode(";",$codestr);
    if(count($codes) > 0) {
      foreach($codes as $code) {
        $code = trim($code);
        list($codekey, $codevalue) = explode(":",$code);
        if(strlen($codekey) > 0) {
          $this->css[$key][trim($codekey)] = trim($codevalue);
        }
      }
    }
  }

  function Get($key, $property) {
    $key = strtolower($key);
    $property = strtolower($property);

    list($tag, $subtag) = explode(":",$key);
    list($tag, $class) = explode(".",$tag);
    list($tag, $id) = explode("#",$tag);
    $result = "";
    foreach($this->css as $_tag => $value) {
      list($_tag, $_subtag) = explode(":",$_tag);
      list($_tag, $_class) = explode(".",$_tag);
      list($_tag, $_id) = explode("#",$_tag);

      $tagmatch = (strcmp($tag, $_tag) == 0) | (strlen($_tag) == 0);
      $subtagmatch = (strcmp($subtag, $_subtag) == 0) | (strlen($_subtag) == 0);
      $classmatch = (strcmp($class, $_class) == 0) | (strlen($_class) == 0);
      $idmatch = (strcmp($id, $_id) == 0);

      if($tagmatch & $subtagmatch & $classmatch & $idmatch) {
        $temp = $_tag;
        if((strlen($temp) > 0) & (strlen($_class) > 0)) {
          $temp .= ".".$_class;
        } elseif(strlen($temp) == 0) {
          $temp = ".".$_class;
        }
        if((strlen($temp) > 0) & (strlen($_subtag) > 0)) {
          $temp .= ":".$_subtag;
        } elseif(strlen($temp) == 0) {
          $temp = ":".$_subtag;
        }
        if(isset($this->css[$temp][$property])) {
          $result = $this->css[$temp][$property];
        }
      }
    }
    return $result;
  }

  function GetSection($key) {
    $key = strtolower($key);

    list($tag, $subtag) = explode(":",$key);
    list($tag, $class) = explode(".",$tag);
    list($tag, $id) = explode("#",$tag);
    $result = array();
    foreach($this->css as $_tag => $value) {
      list($_tag, $_subtag) = explode(":",$_tag);
      list($_tag, $_class) = explode(".",$_tag);
      list($_tag, $_id) = explode("#",$_tag);

      $tagmatch = (strcmp($tag, $_tag) == 0) | (strlen($_tag) == 0);
      $subtagmatch = (strcmp($subtag, $_subtag) == 0) | (strlen($_subtag) == 0);
      $classmatch = (strcmp($class, $_class) == 0) | (strlen($_class) == 0);
      $idmatch = (strcmp($id, $_id) == 0);

      if($tagmatch & $subtagmatch & $classmatch & $idmatch) {
        $temp = $_tag;
        if((strlen($temp) > 0) & (strlen($_class) > 0)) {
          $temp .= ".".$_class;
        } elseif(strlen($temp) == 0) {
          $temp = ".".$_class;
        }
        if((strlen($temp) > 0) & (strlen($_subtag) > 0)) {
          $temp .= ":".$_subtag;
        } elseif(strlen($temp) == 0) {
          $temp = ":".$_subtag;
        }
        foreach($this->css[$temp] as $property => $value) {
          $result[$property] = $value;
        }
      }
    }
    return $result;
  }

  function ParseStr($str) {
    $str = preg_replace("/\/\*(.*)?\*\//Usi", "", $str);
    $parts = explode("}",$str);
    if(count($parts) > 0) {
      foreach($parts as $part) {
        list($keystr,$codestr) = explode("{",$part);
        $keys = explode(",",trim($keystr));
        if(count($keys) > 0) {
          foreach($keys as $key) {
            if(strlen($key) > 0) {
              $key = str_replace("\n", "", $key);
              $key = str_replace("\\", "", $key);
              $this->Add($key, trim($codestr));
            }
          }
        }
      }
    }
    return (count($this->css) > 0);
  }

  function parse($filename) {
    if(file_exists($filename)) {
      return $this->ParseStr(file_get_contents($filename));
    } else {
      return false;
    }
  }

  function getArray() {
    return $this->css;
  }

  function GetCSS() {
    $result = "";
    foreach($this->css as $key => $values) {
      $result .= $key." {\n";
      foreach($values as $key => $value) {
        $result .= "  $key: $value;\n";
      }
      $result .= "}\n\n";
    }
    return $result;
  }

}