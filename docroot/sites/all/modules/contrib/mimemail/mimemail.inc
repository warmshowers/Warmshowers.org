<?php

/**
 * @file
 * Common mail functions for sending e-mail. Originally written by Gerhard.
 *
 *   Allie Micka < allie at pajunas dot com >
 */

/**
 * Attempts to RFC822-compliant headers for the mail message or its MIME parts.
 *
 * @todo Could use some enhancement and stress testing.
 *
 * @param $headers
 *   An array of headers.
 *
 * @return
 *   A string containing the header.
 */
function mimemail_rfc_headers($headers) {
  $header = '';
  $crlf = variable_get('mimemail_crlf', "\n");

  foreach ($headers as $key => $value) {
    $key = trim($key);
    // Collapse spaces and get rid of newline characters.
    $value = preg_replace('/(\s+|\n|\r|^\s|\s$)/', ' ', $value);
    // Fold headers if they're too long.
    if (drupal_strlen($value) > 60) {
      // If there's a semicolon, use that to separate.
      if (count($array = preg_split('/;\s*/', $value)) > 1) {
        $value = trim(join(";$crlf    ", $array));
      }
      else {
        $value = wordwrap($value, 50, "$crlf    ", FALSE);
      }
    }
    $header .= "$key: $value$crlf";
  }

  return trim($header);
}

/**
 * Gives useful defaults for standard email headers.
 *
 * @param $headers
 *   An array of headers.
 *
 * @return
 *   An array containing the encoded headers with the default values.
 */
function mimemail_headers($headers, $from = NULL) {
  $default_from = variable_get('site_mail', ini_get('sendmail_from'));

  // Overwrite standard headers.
  if ($from) {
    if (!isset($headers['From']) || $headers['From'] == $default_from) {
      $headers['From'] = $from;
    }
    if (!isset($headers['Sender']) || $headers['Sender'] == $default_from) {
      $headers['Sender'] = $from;
    }
    // This may not work. The MTA may rewrite the Return-Path.
    if (!isset($headers['Return-Path']) || $headers['Return-Path'] == $default_from) {
      preg_match('/[a-z0-9\-\.\+_]+@{1}[a-z0-9\-\.]+/i', $from, $matches);
      $headers['Return-Path'] = "<$matches[0]>";
    }
  }

  // Convert From header if it is an array.
  if (is_array($headers['From'])) {
    $headers['From'] = mimemail_address($headers['From']);
  }

  // Run all headers through mime_header_encode() to convert non-ascii
  // characters to an rfc compliant string, similar to drupal_mail().
  foreach ($headers as $key => $value) {
    $headers[$key] = mime_header_encode($value);
  }

  return $headers;
}

/**
 * Helper function to extracts links to local images from HTML documents.
 *
 * @param $html
 *   A string containing the HTML document.
 * @param $name
 *   A string containing the document's name.
 *
 * @return
 *   An array of arrays containing the extraced files.
 */
function mimemail_extract_files($html) {
  $pattern = '/(<link[^>]+href=[\'"]?|<object[^>]+codebase=[\'"]?|@import |[\s]src=[\'"]?)([^\'>"]+)([\'"]?)/mis';
  $content = preg_replace_callback($pattern, '_mimemail_replace_files', $html);

  $encoding = '8Bit';
  $body = split("\n", $content);
  foreach ($body as $line) {
    if (strlen($line) > 998) {
      $encoding = 'base64';
      break;
    }
  }
  if ($encoding == 'base64') {
    $content = rtrim(chunk_split(base64_encode($content)));
  }

  $document = array(array(
    'Content-Type' => "text/html; charset=utf-8",
    'Content-Transfer-Encoding' => $encoding,
    'content' => $content,
  ));

  $files = _mimemail_file();

  return array_merge($document, $files);
}

/**
 * Callback function for preg_replace_callback().
 */
function _mimemail_replace_files($matches) {
  return stripslashes($matches[1]) . _mimemail_file($matches[2]) . stripslashes($matches[3]);
}

/**
 * Helper function to extract local files.
 *
 * @param $url
 *   The URL of the file.
 * @param $content
 *   The actual file content.
 *
 * @return
 *   The Content-ID and/or an array of the files on success or the URL on failure. 
 */
function _mimemail_file($url = NULL, $content = NULL, $name = '', $type = '', $disposition = 'related') {
  static $files = array();
  static $filenames = array();

  if ($url) {
    // The file exists on the server as-is. Allows for non-web-accessible files.
    if (@is_file($url)) {
      $file = $url;
    }
    else {
      $url = _mimemail_url($url, 'TRUE');
      // If the url is absolute, we're done here.
      if (strpos($url, '://') !== FALSE || preg_match('!mailto:!', $url)) {
        return $url;
      }
      // The url is a relative file path, continue processing.
      else {
         // Make sure ImageCache images are existing before trying to attach.
        if (module_exists('imagecache') && strpos($url, 'imagecache') !== FALSE) {
            _mimemail_imagecache($url);
        }
        // Download method is private, and the URL needs conversion.
        if (variable_get('file_downloads', FILE_DOWNLOADS_PUBLIC) == FILE_DOWNLOADS_PRIVATE && strpos($url, 'system/files/') !== FALSE) {
          $file = file_create_path(drupal_substr($url, (strpos($url, 'system/files/') + drupal_strlen('system/files/'))));
        }
        // Download method is public.
        else {
          $file = $url;
        }
      }
    }
  }
  // We have the actual content.
  elseif ($content) {
    $file = $content;
  }

  if (isset($file) && (@is_file($file) || $content)) {
    $public_path = file_directory_path();
    $no_access = !user_access('send arbitrary files - Warning: has security implications!');
    $not_in_public_path = strpos(realpath($file), $public_path) === FALSE;
    if (@is_file($file) && $not_in_public_path && $no_access) {
      return $url;
    }

    if (!$name) $name = (@is_file($file)) ? basename($file) : 'attachment.dat';
    if (!$type) $type = ($name) ? file_get_mimetype($name) : file_get_mimetype($file);

    // Prevent duplicate items.
    if (isset($filenames[$name])) return 'cid:'. $filenames[$name];
    $content_id = md5($name) .'@'. $_SERVER['HTTP_HOST'];

    $new_file = array(
      'name' => $name,
      'file' => $file,
      'Content-ID' => $content_id,
      'Content-Disposition' => $disposition,
      'Content-Type' => $type,
    );

    $files[] = $new_file;
    $filenames[$name] = $content_id;

    return 'cid:'. $content_id;
  }
  // The file does not exist and no content, return the URL if possible.
  elseif ($url) {
    return $url;
  }

  $ret = $files;
  $files = array();
  $filenames = array();
  return $ret;
}

/**
 * Helper function to ensure that ImageCache files are created before attaching.
 */
function _mimemail_imagecache($url) {
  static $urls = array();

  // Prevent rechecking.
  if (isset($urls[$url])) {
    return;
  }

  $segments = explode('/', $url);

  // Extract the preset from the path.
  $key = array_search('imagecache', $segments);
  $preset = $segments[$key+1];

  // Remove unnecessary segments to get the original path.
  unset($segments[$key], $segments[$key+1]);
  $path = implode('/', $segments);

  $urls[$url] = TRUE;
  return imagecache_generate_image($preset, $path);
}

/**
 * Helper function to build multipart messages.
 *
 * @param $parts
 *   An array of arrays of parts to be included:
 *   - name: The name of the attachment.
 *   - content: Textual content.
 *   - file: A file.
 *   - Content-Type: Content type of either file or content. Mandatory for content, optional for file.
 *      If not present, it will be derived from the file if mime_content_type is available. If not,
 *      application/octet-stream is used.
 *   - Content-Disposition: Inline is assumed (optional).
 *   - Content-Transfer-Encoding: Base64 is assumed for files, 8bit for other content (optional).
 *   - Content-ID: ID for in-mail references to attachements (optional).
 *   Name is mandatory, one of content and file is required, they are mutually exclusive.
 * @param $content_type
 *   A string containing the content-type for the combined message (optional).
 *
 * @return
 *   An array containing the following elements:
 *   - headers: An array that includes some headers for the mail to be sent.
 *   - body: A string containing the mime encoded multipart body of a mail.
 */
function mimemail_multipart_body($parts, $content_type = 'multipart/mixed; charset=utf-8', $sub_part = FALSE) {
  $boundary = md5(uniqid(time()));
  $body = '';

  $headers = array(
    'Content-Type' => "$content_type; boundary=\"$boundary\"",
  );

  if (!$sub_part) {
    $headers['MIME-Version'] = '1.0';
    $body = "This is a multi-part message in MIME format.\n";
  }

  foreach ($parts as $part) {
    $part_headers = array();

    if (isset($part['Content-ID'])) {
      $part_headers['Content-ID'] = '<'. $part['Content-ID'] .'>';
    }

    if (isset($part['Content-Type'])) {
      $part_headers['Content-Type'] = $part['Content-Type'];
    }

    if (isset($part['Content-Disposition'])) {
      $part_headers['Content-Disposition'] = $part['Content-Disposition'];
    }
    elseif (strpos($part['Content-Type'], 'multipart/alternative') === FALSE) {
      $part_headers['Content-Disposition'] = 'inline';
    }

    if (isset($part['Content-Transfer-Encoding'])) {
      $part_headers['Content-Transfer-Encoding'] = $part['Content-Transfer-Encoding'];
    }

    // Mail content provided as a string.
    if (isset($part['content']) && $part['content']) {
      if (!isset($part['Content-Transfer-Encoding'])) {
        $part_headers['Content-Transfer-Encoding'] = '8bit';
      }
      $part_body = $part['content'];
      if (isset($part['name'])) {
        $part_headers['Content-Type'] .= '; name="'. $part['name'] .'"';
        $part_headers['Content-Disposition'] .= '; filename="'. $part['name'] .'"';
      }

    // Mail content references in a filename.
    }
    else {
      if (!isset($part['Content-Transfer-Encoding'])) {
        $part_headers['Content-Transfer-Encoding'] = 'base64';
      }

      if (!isset($part['Content-Type'])) {
        $part['Content-Type'] = file_get_mimetype($part['file']);
      }

      if (isset($part['name'])) {
        $part_headers['Content-Type'] .= '; name="'. $part['name'] .'"';
        $part_headers['Content-Disposition'] .= '; filename="'. $part['name'] .'"';
      }

      if (isset($part['file'])) {
        $file = (is_file($part['file'])) ? file_get_contents($part['file']) : $part['file'];
        $part_body = chunk_split(base64_encode($file), 76, variable_get('mimemail_crlf', "\n"));
      }
    }

    $body .= "\n--$boundary\n";
    $body .= mimemail_rfc_headers($part_headers) ."\n\n";
    $body .= $part_body;
  }
  $body .= "\n--$boundary--\n";

  return array('headers' => $headers, 'body' => $body);
}

/**
 * Callback for preg_replace_callback()
 */
function _mimemail_expand_links($matches) {
  return $matches[1] . _mimemail_url($matches[2]);
}

/**
 * Generate a multipart message body with a text alternative for some HTML text.
 *
 * The first MIME part is a multipart/alternative containing MIME-encoded
 * sub-parts for HTML and plaintext. Each subsequent part is the required
 * image/attachment.
 *
 * @param $body
 *   An HTML message body
 * @param $subject
 *   The message subject
 * @param $plaintext
 *   TRUE if the recipient prefers plaintext-only messages. Defaults to FALSE.
 *
 * @return
 *   An array containing the headers and the body of the message.
 */
function mimemail_html_body($body, $subject, $plaintext = FALSE, $text = NULL, $attachments = array()) {
  if (empty($text)) {
    // @todo Remove once filter_xss() can handle direct descendant selectors in inline CSS.
    // @see http://drupal.org/node/1116930, http://drupal.org/node/370903
    // Pull out the message body.
    preg_match('|<body.*?</body>|mis', $body, $matches);
    $text = drupal_html_to_text($matches[0]);
  }
  if ($plaintext) {
    // Plain mail without attachment.
    if (empty($attachments)) {
      $content_type = 'text/plain';
      return array(
        'body' => $text,
        'headers' => array('Content-Type' => 'text/plain; charset=utf-8'),
      );
    }
    // Plain mail has attachement.
    else {
      $content_type = 'multipart/mixed';
      $parts = array(array(
      'content' => $text,
        'Content-Type' => 'text/plain; charset=utf-8',
      ));
    }
  }
  else {
    $content_type = 'multipart/mixed';

    $text_part = array('Content-Type' => 'text/plain; charset=utf-8', 'content' => $text);

    // Expand all local links.
    $pattern = '/(<a[^>]+href=")([^"]*)/mi';
    $body = preg_replace_callback($pattern, '_mimemail_expand_links', $body);

    $mime_parts = mimemail_extract_files($body);

    $content = array($text_part, array_shift($mime_parts));
    $content = mimemail_multipart_body($content, 'multipart/alternative', TRUE);
    $parts = array(array('Content-Type' => $content['headers']['Content-Type'], 'content' => $content['body']));

    if ($mime_parts) {
      $parts = array_merge($parts, $mime_parts);
      $content = mimemail_multipart_body($parts, 'multipart/related; type="multipart/alternative"', TRUE);
      $parts = array(array('Content-Type' => $content['headers']['Content-Type'], 'content' => $content['body']));
    }
  }

  if (is_array($attachments) && !empty($attachments)) {
    foreach ($attachments as $a) {
      $a = (object) $a;
      // Check the list parameter if its set or ignore it (Upload module support).
      if (!isset($a->list) || $a->list) {
        _mimemail_file($a->filepath, $a->filecontent, $a->filename, $a->filemime, 'attachment');
        $parts = array_merge($parts, _mimemail_file());
      }
    }
  }

  return mimemail_multipart_body($parts, $content_type);
}

/*
 * Parse an incoming message. 
 */
function mimemail_parse($message) {
  // Provides a "headers", "content-type" and "body" element.
  $mail = mimemail_parse_headers($message);
  // Get an address-only version of "From" (useful for user_load() and such).
  $mail['from'] = preg_replace('/.*\b([a-z0-9._%+-]+@[a-z0-9.-]+\.[a-z]{2,4})\b.*/i', '\1', drupal_strtolower($mail['headers']['From']));
  // Get a subject line, which may be cleaned up/modified later.
  $mail['subject'] = $mail['headers']['Subject'];
  // Make an array to hold any non-content attachments.
  $mail['attachments'] = array();
  // We're dealing with a multi-part message.
  $mail['parts'] = mimemail_parse_boundary($mail);

  foreach ($mail['parts'] as $i => $part_body) {
    $part = mimemail_parse_headers($part_body);
    $sub_parts = mimemail_parse_boundary($part);
    // Content is encoded in a multipart/alternative section
    if (count($sub_parts) > 1) {
      foreach ($sub_parts as $j => $sub_part_body) {
        $sub_part = mimemail_parse_headers($sub_part_body);
        if ($sub_part['content-type'] == 'text/plain') {
          $mail['text'] = mimemail_parse_content($sub_part);
        }
        if ($sub_part['content-type'] == 'text/html') {
          $mail['html'] = mimemail_parse_content($sub_part);
        }
        else {
          $mail['attachments'][] = mimemail_parse_attachment($sub_part);
        }
      }
    }

    if (($part['content-type'] == 'text/plain') && !isset($mail['text'])) {
      $mail['text'] = mimemail_parse_content($part);
    }
    elseif (($part['content-type'] == 'text/html') && !isset($mail['html'])) {
      $mail['html'] = mimemail_parse_content($part);
    }
    else {
      $mail['attachments'][] = mimemail_parse_attachment($part);
    }
  }

  // Make sure our text and HTML parts are accounted for.
  if (isset($mail['html']) && !isset($mail['text'])) {
    $mail['text'] = preg_replace('|<style.*</style>|mis', '', $mail['html']);
    $mail['text'] = drupal_html_to_text($mail['text']);
  }
  elseif (isset($mail['text']) && !isset($mail['html'])) {
    $format = variable_get('mimemail_format', FILTER_FORMAT_DEFAULT);
    $mail['html'] = check_markup($mail['text'], $format, FALSE);
  }

  // Last ditch attempt, use the body as-is.
  if (!isset($mail['text'])) {
    $mail['text'] = mimemail_parse_content($mail);
    $format = variable_get('mimemail_format', FILTER_FORMAT_DEFAULT);
    $mail['html'] = check_markup($mail['text'], $format, FALSE);
  }

  return $mail;
}

/*
 * Split a multi-part message using MIME boundaries.
 */
function mimemail_parse_boundary($part) {
  $m = array();
  if (preg_match('/.*boundary="?([^";]+)"?.*/', $part['headers']['Content-Type'], $m)) {
    $boundary = "\n--". $m[1];
    $body = str_replace("$boundary--", '', $part['body']);
    return array_slice(explode($boundary, $body), 1);
  }
  return array($part['body']);
}

/*
 * Split a message (or message part) into its headers and body section.
 */
function mimemail_parse_headers($message) {
  // Split out body and headers.
  if (preg_match("/^(.*?)\r?\n\r?\n(.*)/s", $message, $match)) {
    list($hdr, $body) = array($match[1], $match[2]);
  }

  // Un-fold the headers.
  $hdr = preg_replace(array("/\r/", "/\n(\t| )+/"), array('', ' '), $hdr);

  $headers = array();
  foreach (explode("\n", trim($hdr)) as $row) {
    $split = strpos( $row, ':' );
    $name = trim(drupal_substr($row, 0, $split));
    $val  = trim(drupal_substr($row, $split+1));
    $headers[$name] = $val;
  }

  $type = (preg_replace('/\s*([^;]+).*/', '\1', $headers['Content-Type']));

  return array('headers' => $headers, 'body' => $body, 'content-type' => $type);
}

/*
 * Return a decoded MIME part in UTF-8.
 */
function mimemail_parse_content($part) {
  $content = $part['body'];

  // Decode this part.
  if ($encoding = drupal_strtolower($part['headers']['Content-Transfer-Encoding'])) {
    switch ($encoding) {
      case 'base64':
        $content = base64_decode($content);
        break;
      case 'quoted-printable':
        $content = quoted_printable_decode($content);
        break;
      case '7bit':  // 7bit is the RFC default
        break;
    }
  }

  // Try to convert character set to UTF-8.
  if (preg_match('/.*charset="?([^";]+)"?.*/', $part['headers']['Content-Type'], $m)) {
    $content = drupal_convert_to_utf8($content, $m[1]);
  }

  return $content;
}

/*
 * Convert a MIME part into a file array.
 */
function mimemail_parse_attachment($part) {
  $m = array();
  if (preg_match('/.*filename="?([^";])"?.*/', $part['headers']['Content-Disposition'], $m)) {
    $name = $m[1];
  }
  elseif (preg_match('/.*name="?([^";])"?.*/', $part['headers']['Content-Type'], $m)) {
    $name = $m[1];
  }

  return array(
    'filename' => $name,
    'filemime' => $part['content-type'],
    'content'  => mimemail_parse_content($part),
  );
}

/**
 * Helper function to format URLs.
 *
 * @param $url
 *   A string containing an URL.
  * @param $embed_file
 *   TRUE if the file should be embedded in the message. Defaults to FALSE.
 *
 * @return
 *   A string containing an absolute URL or mailto.
 */
function _mimemail_url($url, $embed_file = FALSE) {
  global $base_url;
  $url = urldecode($url);

  // If the URL is absolute or a mailto, return it as-is.
  if (strpos($url, '://') !== FALSE || preg_match('!mailto:!', $url)) {
    $url = str_replace(' ', '%20', $url);
    return $url;
  }
  // If the image embedding is disabled, return the absolute URL for the image.
  elseif (variable_get('mimemail_linkonly', 0) && preg_match('!\.(png|gif|jpg|jpeg)$!i', $url)) {
    $url = $base_url . $url;
    $url = str_replace(' ', '%20', $url);
    return $url;
  }

  $url = preg_replace( '!^'. base_path() .'!', '', $url, 1);

  // If we're processing to embed the file, we're done here so return.
  if ($embed_file) return $url;

  if (!preg_match('!^\?q=*!', $url)) {
    $strip_clean = TRUE;
  }

  $url = str_replace('?q=', '', $url);
  @list($url, $fragment) = explode('#', $url, 2);
  @list($path, $query) = explode('?', $url, 2);

  // If we're dealing with an intra-document reference, return it.
  if (empty($path) && !empty($fragment)) {
    return '#'. $fragment;
  }

  // Get a list of enabled languages.
  $languages = language_list('enabled');
  $languages = $languages[1];

  // Default language settings.
  $prefix = '';
  $language = language_default();

  // Check for language prefix.
  $args = explode('/', $path);
  foreach (array_keys($languages) as $lang) {
    if ($args[0] == $lang) {
      $prefix = array_shift($args) . '/';
      $language = $languages[$lang];
      $path = implode('/', $args);
      break;
    }
  }

  $options = array(
    'query' => $query,
    'fragment' => $fragment,
    'absolute' => TRUE,
    'language' => $language,
    'prefix' => $prefix,
  );

  $url = url($path, $options);

  // If url() added a ?q= where there should not be one, remove it.
  if ($strip_clean) $url = preg_replace('!\?q=!', '', $url);

  $url = str_replace('+', '%2B', $url);
  return $url;
}

/**
 * Helper function to format an address string.
 *
 * @todo Could use some enhancement and stress testing.
 *
 * @param $address
 *   A user object, a text email address or an array containing name, mail.
 *
 * @return
 *   A formatted address string or FALSE.
 */
function mimemail_address($address) {
  $simple_address = variable_get('mimemail_simple_address', 0);

  if (is_array($address)) {

    // It's an array containing mail and/or name.
    if (isset($address['mail'])) {
      $output = '';
      if (empty($address['name']) || $simple_address) {
        return $address['mail'];
      }
      else {
        return '"'. addslashes(mime_header_encode($address['name'])) .'" <'. $address['mail'] .'>';
      }
    }

    // It's an array of address items.
    $addresses = array();
    foreach ($address as $a) {
      $addresses[] = mimemail_address($a);
    }
    return $addresses;
  }

  // It's a user object.
  if (is_object($address) && isset($address->mail)) {
    if (empty($address->name) || $simple_address) {
      return $address->mail;
    }
    else {
      return '"'. addslashes(mime_header_encode($address->name)) .'" <'. $address->mail .'>';
    }
  }

  // Its a formatted or an unformatted string.
  // @todo Shouldn't assume it's valid, should try to re-parse.
  if (is_string($address)) {
    return $address;
  }

  // It's null, return the site default address.
  if (is_null($address)) {
    return array(
      'name' => mime_header_encode(variable_get('site_name', 'Drupal')),
      'mail' => variable_get('site_mail', ini_get('sendmail_from')),
    );
  }

  return FALSE;
}
