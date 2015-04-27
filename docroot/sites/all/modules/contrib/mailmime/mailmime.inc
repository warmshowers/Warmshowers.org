<?php

/**
 * @file
 * Provides the MailMIME class for creating MIME-formatted email messages.
 */

// The Mail_mime line-ending constant.
define('MAIL_MIME_CRLF', variable_get('mail_line_endings', MAIL_LINE_ENDINGS));

/**
 * The PEAR folder of the local files directory has been added to the include
 * path by mailmime_init(), and the requisite files have been downloaded by
 * mailmime_requirements(), so the following require_once commands should
 * succeed.
 *
 * @see mailmime_init(), mailmime_requirements()
 */
require_once 'Mail/mime.php';
require_once 'Mail/mimeDecode.php';

if (!function_exists('url_to_path')) {
  require_once dirname(__FILE__) . '/url_to_path.inc';
}

/**
 * The MailMIME class is used to create MIME email messages.
 *
 * The MailMIME class extends the PEAR Mail_Mime class as follows:
 * - All errors are routed to watchdog().
 * - Content-IDs are assigned based on filename, not the current timestamp.
 * - Only the first call to MailMIME::addHTMLImage() for a given filename will
 *   attach the file.  Subsequent calls with the same filename will return
 *   TRUE for success but will not attach additional copies.
 * - Image references within the HTML part are auto-detected and converted
 *   to inline attachments, as long as their URLs can be resolved to files
 *   within the current site.
 * - Public methods are named and documented according to Drupal standards.
 *
 * @see http://pear.php.net/package/Mail_mime
 */
class MailMIME extends Mail_mime {
  /**
   * Holds attached content-ids to to avoid attaching the same file twice.
   *
   * @var array
   */
  protected $cids = array();

  /**
   * Holds parameters used for building the formatted message.
   *
   * @var array
   *   An associative array of parameters containing the following:
   *   - head_encoding: The encoding to use for headers.  May be:
   *     - base64:
   *     - quoted-printable: (default)
   *   - text_encoding: The encoding to use for the text/plain part.  May be:
   *     - 7bit:
   *     - 8bit:
   *     - base64:
   *     - quoted-printable: (default)
   *   - html_encoding: The encoding to use for the text/html part.  May be:
   *     - 7bit:
   *     - 8bit:
   *     - base64:
   *     - quoted-printable: (default)
   *   - html_charset: The character set to use for the text/html part.
   *     Defaults to 'UTF-8'.
   *   - text_charset: The character set to use for the text/plain part.
   *     Defaults to 'UTF-8'.
   *   - head_charset: The character set to use for the header values.
   *     Defaults to 'UTF-8'.
   *   - eol: The end-of-line or line-ending sequence.  Defaults to an auto-
   *     detected value depending on the server operating system.  May be
   *     overridden by setting $config['mail_line_endings'].
   *   - delay_file_io: FALSE if attached files should be read immediately,
   *     rather than when the message is built.  Defaults to TRUE.
   */
  public $_build_params = array(
    'head_encoding' => 'quoted-printable',
    'text_encoding' => '8bit',
    'html_encoding' => '8bit',
    'html_charset' => 'UTF-8',
    'text_charset' => 'UTF-8',
    'head_charset' => 'UTF-8',
    'eol' => MAIL_MIME_CRLF,
    'delay_file_io' => TRUE,
  );

  /**
   * Routes PEAR_Error objects to watchdog().
   *
   * Passes PEAR_Error objects to watchdog(), and returns FALSE.
   *
   * @param $data
   *   The result of another function that may return a PEAR_Error object.
   *
   * @return
   *   FALSE if $data is a PEAR_Error object; otherwise $data.
   */
  protected static function &successful(&$data) {
    if (PEAR::isError($data)) {
      watchdog('mailmime',
        '<a href="!pear_error">PEAR error: @error',
        array(
          '!pear_error' => url('http://pear.php.net/manual/core.pear.pear.iserror.php'),
          '@error' => $data->toString()
        )
      );
      $data = FALSE;
    }
    return $data;
  }

  /**
   * Constructs and returns a new object instance.
   *
   * @param $params
   *   (optional) A list of parameters that change the way the message is
   *   formatted.  May contain any of the following:
   *   - head_encoding: The encoding to use for headers.  May be:
   *     - base64:
   *     - quoted-printable:
   *   - text_encoding: The encoding to use for the text/plain part.  May be:
   *     - 7bit:
   *     - 8bit:
   *     - base64:
   *     - quoted-printable:
   *   - html_encoding: The encoding to use for the text/html part.  May be:
   *     - 7bit:
   *     - 8bit:
   *     - base64:
   *     - quoted-printable:
   *   - html_charset: The character set to use for the text/html part.
   *   - text_charset: The character set to use for the text/plain part.
   *   - head_charset: The character set to use for the header values.
   *   - eol: The end-of-line or line-ending sequence.
   *   - delay_file_io: FALSE if attached files should be read immediately;
   *     TRUE if file I/O should be delayed until the message is built.
   *
   * @return
   *   A newly-constructed MailMIME object.
   */
  // public function __construct($params = array());

  /**
   * Set a specific build parameter value.
   *
   * @param $name
   *   The parameter name.
   * @param $value
   *   The parameter value.
   *
   * @see __construct()
   */
  // public function setParam($name, $value);

  /**
   * Get a build parameter value.
   *
   * @param $name
   *   The name of the parameter.
   *
   * @return
   *   The value of the parameter, if it has been set, or else NULL.
   */
  // public function getParam($name);

  /**
   * Set the text/plain part of the message.
   *
   * @param $data
   *   Either the text/plain data or the name of a file containing data.
   * @param $is_file
   *   (optional) TRUE if $data is a filename.  Defaults to FALSE.
   * @param $append
   *   (optional) TRUE to append the data to the exiting text/plain part, or
   *   FALSE to overwrite existing data.  Defaults to FALSE.
   *
   * @return
   *   TRUE if successful; otherwise FALSE.
   */
  public function setTXTBody($data, $is_file = FALSE, $append = FALSE) {
    return self::successful(parent::setTXTBody($data, $is_file, $append));
  }

  /**
   * Returns the text/plain message part.
   *
   * @return
   *   The text/plain message part, or NULL if it has not been set.
   */
  // public function getTXTBody();

  /**
   * Sets the text/html part of the message.
   *
   * @param $data
   *   Either the text/html data or the name of a file containing the data.
   * @param $is_file
   *   (optional) TRUE if $data is a filename.  Defaults to FALSE.
   *
   * @return
   *   TRUE if successful; otherwise FALSE.
   */
  public function setHTMLBody($data, $is_file = FALSE) {
    return self::successful(parent::setHTMLBody($data, $is_file));
  }

  /**
   * Returns the text/html message part.
   *
   * @return
   *   The text/html message part, or NULL if it has not been set.
   */
  // public function getHTMLBody();

  /**
   * Adds an image to the list of embedded images.
   *
   * @param $file
   *   The image file name OR image data itself.
   * @param $content_type
   *   (optional) The content-type of the image, such as "image/gif".
   * @param $name
   *   (optional) The filename of the image, if $is_file is FALSE.
   * @param bool $is_file
   *   (optional) FALSE if $file contains the actual image data, rather than
   *   a filename.  Defaults to TRUE.
   * @param $content_id
   *   (optional) The desired Content-ID for this MIME part.
   *
   * @return
   *   TRUE if the file was successfully attached, and FALSE if it wasn't.
   */
  public function addHTMLImage(
    $file,
    $content_type = NULL,
    $name = '',
    $is_file = TRUE,
    $content_id = NULL
  ) {
    $filename = $is_file ? $file : $name;
    if (empty($content_id)) {
      $content_id = md5($filename);
    }
    if (empty($content_type)) {
      $content_type = self::guess_mimetype($filename, $is_file ? NULL : $file);
    }
    if (!isset($this->cids[$content_id])) {
      $this->cids[$content_id] =
        self::successful(
          parent::addHTMLImage($file, $content_type, $name, $is_file, $content_id)
        );
    }
    return $this->cids[$content_id];
  }

  /**
   * Guess the content type of a file or inline data stream.
   *
   * Uses the Mime Detect module if available; otherwise uses the
   * file_get_mimetype() function.  Provides a smaller-than-default
   * mime-type mapping to improve performance a bit.
   *
   * @param $filename
   *   The name of the file, used for guessing the mime type based on the
   *   filename extension.
   * @param $data
   *   (optional) The contents of the file, used by the Mime Detect module if it
   *   is available.  If omitted or blank, the Mime Detect module will try to
   *   open the file referenced by the $filename parameter.
   *
   * @return
   *   The MIME content-type matching the file contents or filename extension,
   *   or "application/octet-stream" if no match could be found.
   */
  static function guess_mimetype($filename, $data = NULL) {
    static $mapping;
    // @todo: Use Mime Detect module if available.
    if (!isset($mapping)) {
      $mapping = array(
        'pdf' => 'application/pdf',
        'ps|ai|eps' => 'application/postscript',
        'km(l|z)' => 'application/vnd.google-earth',
        'pp(a|s|t)(m|x|)' => 'application/vnd.ms-powerpoint',
        'swf(l|)' => 'application/x-shockwave-flash',
        'gif' => 'image/gif',
        'jp(e|eg|g)' => 'image/jpeg',
        'pcx' => 'image/pcx',
        'png' => 'image/png',
        'svg(z|)' => 'image/svg+xml',
        'tif(f|)' => 'image/tiff',
        'wbmp' => 'image/vnd.wap.wbmp',
        'ico' => 'image/x-icon',
        'art' => 'image/x-jg',
        'bmp' => 'image/x-ms-bmp',
        'eml' => 'message/rfc822',
        'ics|icz' => 'text/calendar',
        'css' => 'text/css',
        'vrm(l|)|wrl' => 'x-world/x-vrml',
      );
      $mapping = variable_get('mime_extension_mapping', $mapping);
    }
    return file_get_mimetype($filename, $mapping);
  }

  /**
   * Adds a file to the list of attachments.
   *
   * @param $file
   *   The filename to attach, or the file contents itself.
   * @param $content_type
   *   (optional) The content-type, such as 'application/x-pdf'.
   * @param $name
   *   (optional) The filename of the attachment, if $is_file is FALSE.
   * @param $is_file
   *   (optional) FALSE if $file contains file data rather than a filename.
   *   Defaults to TRUE.
   * @param $encoding
   *   (optional) The encoding to use for the file data. May be one of:
   *   - 7bit:
   *   - 8bit:
   *   - base64: (default)
   *   - quoted-printable
   * @param $disposition
   *   (optional) The content-disposition of this file.  May be one of:
   *   - attachment: (default)
   *   - inline:
   * @param $charset
   *   (optional) The character set of the attachment's content.
   * @param $language
   *   (optional) The language of the attachment.
   * @param $location
   *   (optional) The RFC 2557.4 location of the attachment.
   * @param $name_encoding
   *   (optional) The encoding to use for the attachment name, instead of the
   *   default RFC2231 encoding.  May be one of:
   *   - base64
   *   - quoted-printable
   * @param $filename_encoding
   *   (optional) The encoding to use for the attachment filename, instead of
   *   the default RFC2231 encoding.  May be one of:
   *   - base64
   *   - quoted-printable
   * @param $description
   *   (optional) The value to use for the Content-Description header.
   * @param $header_charset
   *   (optional) The character set to use for this part's MIME headers.
   *
   * @return
   *   TRUE if successful; otherwise FALSE.
   */
  public function addAttachment($file,
    $content_type = 'application/octet-stream', $name = '', $is_file = TRUE,
    $encoding = 'base64', $disposition = 'attachment', $charset = '',
    $language = '', $location = '', $name_encoding = NULL,
    $filename_encoding = NULL, $description = '', $header_encoding = NULL) {
    // @todo Set content_type with mimedetect if possible.
    return self::successful(
      parent::addAttachment($file, $content_type, $name, $is_file, $encoding,
        $disposition, $charset, $language, $location, $name_encoding,
        $filename_encoding, $description, $header_encoding)
    );
  }

  /**
   * Returns the complete e-mail, ready to send.
   *
   * @param $separation
   *   (optional) The string used to separate header and body parts.
   * @param $params
   *   (optional) Build parameters for the MailMimeInterface::get() method.
   * @param $headers
   *   (optional) The extra headers that should be passed to the
   *   self::headers() method.
   * @param $overwrite
   *   TRUE if $headers parameter should overwrite previous data.
   *
   * @return
   *   The complete message as a string if successful; otherwise FALSE.
   */
  public function getMessage($separation = NULL, $params = NULL,
    $headers = NULL, $overwrite = FALSE) {
    return self::successful(
      parent::getMessage($separation, $params, $headers, $overwrite)
    );
  }

  /**
   * Appends the complete e-mail to a file.
   *
   * @param $filename
   *   The output file location.
   * @param $params
   *   (optional) Build parameters for the MailMimeInterface::get() method.
   * @param $headers
   *   (optional) The extra headers that should be passed to the
   *   MailMimeInterface::headers() method.
   * @param $overwrite
   *   TRUE if $headers parameter should overwrite previous data.
   *
   * @return
   *   TRUE if successful; otherwise FALSE.
   */
  public function saveMessage($filename, $params = NULL, $headers = NULL,
    $overwrite = FALSE) {
    return self::successful(
      parent::saveMessage($filename, $params, $headers, $overwrite)
    );
  }

  /**
   * Appends the complete e-mail body to a file.
   *
   * @param $filename
   *   The output file location.
   * @param $params
   *   (optional) Build parameters for the MailMimeInterface::get() method.
   *
   * @return
   *   TRUE if successful; otherwise FALSE.
   */
  public function saveMessageBody($filename, $params = NULL) {
    return self::successful(
      parent::saveMessageBody($filename, $params)
    );
  }

  /**
   * A preg_replace_callback used to attach local files, if possible.
   *
   * @see get()
   */
  protected function attachRegex($matches) {
    if ( ($url = drupal_strip_dangerous_protocols(urldecode($matches[4])))
      && ($path = url_to_realpath($url))
      && is_file($path)
      // Add the image and include any query string ($matches[5]) in the 3rd
      // "name" parameter. This enables compatibility with the token DoS fix
      // added in Drupal 7.20.
      && $this->addHTMLImage($path, NULL, $path . $matches[5])
    ) {
      // The parent method will replace this with the actual cid: string.
      $matches[4] = $path;
    }
    return implode('', array_slice($matches, 1));
  }

  /**
   * Builds and returns the full multipart message with all its parts.
   *
   * Searches for inline file references and attaches local files, if possible.
   *
   * @param array $params
   *   (optional) An associative array used to override the
   *   HTMLMailMime::_build_params values for building this message.
   * @param $filename
   *   (optional) The filename where the message data should be written. The
   *   default is to return the message data as a string.
   * @param $skip_head
   *   (optional) TRUE if only the message body should be generated.  Defaults
   *   to FALSE: return both headers and body together.
   *
   * @return mixed
   *   - FALSE: If an error occurred.
   *   - NULL: If $filename is set and no error occurred.
   *   - string: The formatted message if $filename is not set and no error
   *     occurred.
   */
  public function &get($params = NULL, $filename = NULL, $skip_head = FALSE) {
    if (isset($this->_htmlbody)) {
      $this->_htmlbody = preg_replace_callback(
        array(
          '#(?<!\S)(src|background|href)\s*(=)\s*(["\'])(?!cid:)([^?]*?)(?<!\.css)(?<!\.js)(\?.*?)?(\3)(?=[ >])#i',
          '#(?<!\S)(url)\s*(\()\s*(["\'])(?!cid:)([^?]*?)(?<!\.css)(?<!\.js)(\?.*?)?(\3)(?=[ )])#i'
        ),
        array(&$this, 'attachRegex'),
        $this->_htmlbody
      );
    }
    return self::successful(
      parent::get($params, $filename, $skip_head)
    );
  }

  /**
   * Returns an array of the headers needed to prepend to the message, including
   * 'MIME-Version:' and 'Content-Type:'.
   *
   * @param $extra_headers
   *   (optional) An associative array of extra headers to add.  The format is
   *   array('Header-Name' => 'Header-Value').  Don't set the Content-Type for
   *   multipart messages here!
   * @param $overwrite
   *    (optional) TRUE if $extra_headers should overwrite existing data.
   *    Defaults to FALSE.
   * @param $skip_content
   *    (optional) TRUE if the following headers should not be returned:
   *    - Content-Type:
   *    - Content-Disposition:
   *    - Content-Transfer-Encoding:
   *    Defaults to FALSE.
   *
   * @return array
   *   An associative array of ('Header-Name' => 'Header-Value') header items.
   */
  // public function &headers($extra_headers = NULL, $overwrite = FALSE,
  //   $skip_content = FALSE);

  /**
   * Get the text version of the headers, which can be used in the PHP Mail() function.
   *
   * @param $extra_headers
   *   (optional) An associative array of extra headers to add.  The format is
   *   array('Header-Name' => 'Header-Value').  Don't set the Content-Type for
   *   multipart messages here!
   * @param $overwrite
   *    (optional) TRUE if $extra_headers should overwrite existing data.
   *    Defaults to FALSE.
   * @param $skip_content
   *    (optional) TRUE if the following headers should not be returned:
   *    - Content-Type:
   *    - Content-Disposition:
   *    - Content-Transfer-Encoding:
   *    Defaults to FALSE.
   *
   * @return string
   *   The headers as a string.
   */
  // public function txtHeaders($extra_headers = NULL, $overwrite = FALSE,
  //   $skip_content = false);

  /**
   * Sets the message Content-Type header.
   *
   * This can be used to build messages with various multipart content-types
   * not supported by the Mail_mime::_contentHeaders() method.
   *
   * @param $type
   *   The content-type name, such as 'multipart/report'.
   * @param $params
   *   An associative array of ('name' => 'value') parameters to be added.
   *
   * @see http://www.apps.ietf.org/rfc/rfc2046.html
   * @see http://www.apps.ietf.org/rfc/rfc1892.html
   */
  // public function setContentType($type, $params = array());

  /**
   * Sets the message 'Subject:' header value.
   *
   * There can be only one 'Subject:' value, so subsequent calls to this method
   * will overwrite the previous value, rather than adding to it.
   *
   * @param $subject
   *   The text string to be inserted into the 'Subject:' header line.
   */
  // public function setSubject($subject);

  /**
   * Sets the message 'From:' header value.
   *
   * There can be only one 'From:' value, so subsequent calls to this method
   * will overwrite the previous value, rather than  adding to it.
   *
   * @param $email
   *   The email address to be inserted into the 'From:' header line.
   */
  // public function setFrom($email);

  /**
   * Adds a 'To:' value to the message.
   *
   * There can be more than one 'To:' value, so subsequent calls to this method
   * will add to the previous value rather than overwriting it.
   *
   * @param $email
   *   The email address to be added to the 'To:' header line.
   */
  // public function addTo($email);

  /**
   * Adds a 'Cc:' (carbon copy) value to the message.
   *
   * There can be more than one 'Cc:' value, so subsequent calls to this method
   * will add to the previous value rather than overwriting it.
   *
   * @param $email
   *   The email address to be added to the 'Cc:' header line.
   */
  // public function addCc($email);

  /**
   * Adds a 'Bcc:' (blind carbon copy) value to the message.
   *
   * There can be more than one 'Bcc:' value, so subsequent calls to this method
   * will add to the previous value rather than overwriting it.
   *
   * @param $email
   *   The email address to be added to the 'Bcc:' header line.
   */
  // public function addBcc($email);

  /**
   * Encodes a list of recipients for use in the PHP mail() function.
   *
   * Since the PHP mail() function requires you to specify recipients separately
   * from the other headers, the resulting 'To:' header may not be properly
   * encoded. To fix this, you may use this public method to encode your
   * recipient list before sending via the PHP mail() function.
   *
   * @param $recipients
   *   A comma-delimited list of recipients.
   *
   * @return
   *   The encoded data, for use by the PHP mail() function.
   */
  // public function encodeRecipients($recipients);

  /**
   * Encodes a header value as per RFC2047.
   *
   * @param $name
   *   The header name.
   * @param $value
   *   The header value to be encoded.
   * @param $charset
   *   The character set name to be used, such as 'UTF-8' or 'ISO-8859-1'.
   * @param $encoding
   *   The encoding name. Must be be one of:
   *   - base64:
   *   - quoted-printable:
   *
   * @return
   *   The encoded header value (without a name)
   *
   * @see http://www.apps.ietf.org/rfc/rfc2047.html
   */
  public function encodeHeader($name, $value,
    $charset = 'UTF-8', $encoding = 'quoted-printable') {
    return parent::encodeHeader($name, $value, $charset, $encoding);
  }

  /**
   * Parse a complete message and return a MailMIME object.
   *
   * @param $message
   *   The complete message, including headers and body.
   *
   * @return
   *   FALSE if an error occured; otherwise a new MailMIME object containing
   *   the parsed message and its attachments, if any.
   */
  public static function &parse($message) {
    $decoder = new Mail_mimeDecode($message);
    $decoded = $decoder->decode(
      array(
        'decode_headers' => TRUE,
        'decode_bodies' => TRUE,
        'include_bodies' => TRUE,
        'rfc822_bodies' => TRUE,
      )
    );
    if (!self::successful($decoded)) {
      return FALSE;
    }
    $parsed = new MailMIME();
    self::parseDecoded($parsed, $decoded);
    return $parsed;
  }

  /**
   * Return a (headers, body) pair for sending.
   *
   * Merge the $headers parameter with the MIME headers
   * and return it with the fully-encoded message body.
   *
   * @param $headers
   *   The original message headers array.
   *
   * @return array
   *   An array containing two elements, the merged headers and the fully-
   *   encoded message body, both ready to send.
   */
  public function toEmail($headers) {
    $headers = self::toHeaders($headers);
    $mime_headers = $this->headers();
    return array(
      array_diff_key($headers, $mime_headers) + $mime_headers,
      $this->get(NULL, NULL, TRUE),
    );
  }

  /**
   * Recursively copies message parts into a MailMIME object.
   *
   * Copies the MIME parts from an object returned by Mail_mimeDecode->decode()
   * into a MailMIME object, including subparts of any 'multipart' parts.
   *
   * @param $parsed
   *   The target MailMIME object.
   * @param $decoded
   *   The object returned by Mail_mimeDecode->decode() whose MIME parts
   *   are being copied.
   * @param $parent_subtype
   *   The content-type subtype of the parent multipart MIME part.  This should
   *   be either 'mixed', 'related', or 'alternative'.  Defaults to an empty
   *   string, signifying the root of the MIME tree.
   */
  protected static function parseDecoded(MailMIME &$parsed, stdClass &$decoded,
    $parent_subtype = '') {
    if ($decoded->ctype_primary == 'multipart') {
      if (!empty($decoded->parts)) {
        foreach (array_keys($decoded->parts) as $key) {
          self::parseDecoded($parsed, $decoded->parts[$key], $decoded->ctype_secondary);
        }
      }
      return;
    }
    if (empty($decoded->body)) {
      return;
    }
    switch ($decoded->ctype_primary) {
      case 'text':
        if ( $parent_subtype == ''
          || $parent_subtype == 'alternative'
          || $parent_subtype == 'related') {
          if ($decoded->ctype_secondary == 'plain') {
            $parsed->setTXTBody($decoded->body);
            return;
          }
          elseif ($decoded->ctype_secondary == 'html') {
            $parsed->setHTMLBody($decoded->body);
            return;
          }
        }
      case 'image':
        if ($parent_subtype == 'related') {
          $cid = isset($decoded->headers['content-id'])
            ? $decoded->headers['content-id'] : NULL;
          return;
        }
      default:
        $type = $decoded->ctype_primary . '/' . $decoded->ctype_secondary;
        $name = isset($decoded->d_parameters['name'])
          ? $decoded->d_parameters['name'] :
          ( isset($decoded->d_parameters['filename'])
            ? $decoded->d_parameters['filename']
            : ''
          );
        if (!empty($name) && !empty($cid)) {
          $parsed->addHTMLImage($decoded->body, $type, $name, FALSE, $cid);
          return;
        }
        $parsed->addAttachment($decoded->body, $type, $name, FALSE);
    }
  }
  /**
   * Returns an array with keys changed to match the case of email headers.
   *
   * @param $input
   *   The headers to be changed, either as a MAIL_MIME_CRLF-delimited string
   *   or as an associative array of (name => value) pairs.
   *
   * @return
   *   An associative array of (name => value) pairs, with the case changed to
   *   match normal email headers.
   */
  public static function toHeaders($input) {
    $headers = array();
    if (!is_array($input)) {
      $input = mail_mimeDecode::decode(
        array(
          'decode_headers' => TRUE,
          'input' => $input,
        )
      )->headers;
    }
    foreach ($input as $name => $value) {
      $name = preg_replace(
        array(
          '/([[:alpha:]])([[:alpha:]]+)/e',
          '/^Mime-/',
          '/-Id$/',
        ),
        array(
          'strtoupper("\1") . strtolower("\2")',
          'MIME-',
          '-ID',
        ),
        $name
      );
      $headers[$name] = $value;
    }
    return $headers;
  }

  /**
   * Collapses a message array into a single string and standardizes the
   * line-ending character.
   *
   * @param $data
   *   The original message array or string.
   *
   * @return
   *   The collapsed message string.
   */
  public static function concat($data) {
    $data = preg_replace('/(\r|\r\n|\n)/', MAIL_MIME_CRLF, $data);
    if (is_array($data)) {
      $data = implode(MAIL_MIME_CRLF, $data);
    }
    return $data;
  }

  /**
   * Convert message headers and body into an encoded string.
   *
   * @param $headers
   *   The message headers as a string or an array.
   * @param $body
   *   The message body as a stringi or an array.
   *
   * @return
   *   The fully-encoded email message as a string.
   */
  public static function encodeEmail($headers, $body) {
    // Standardize capitalization of header names.
    $headers = self::toHeaders($headers);
    $output = '';
    foreach ($headers as $name => $value) {
      $output .= $name . ': ' . Mail_mimePart::encodeHeader(
        $name, $value, 'UTF-8', 'quoted-printable', MAIL_MIME_CRLF
      ) . MAIL_MIME_CRLF;
    }
    $output .= MAIL_MIME_CRLF . self::concat($body);
    return $output;
  }
}
