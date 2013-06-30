<?php
/**
 * @file
 * Copyright 2009 Google Inc. All Rights Reserved.
 * Modified by Skyredwang to work with any Client (Not only browser without JS)
 */

// Tracker version.
define('GAVERSION', '4.4sh');

define('COOKIE_NAME', '__utmmobile');

// The path the cookie will be available to, edit this to use a different
// cookie path.
define('COOKIE_PATH', '/');

// Two years in seconds.
define('COOKIE_USER_PERSISTENCE', 63072000);

// 1x1 transparent GIF.
$GIF_DATA = array(
  chr(0x47), chr(0x49), chr(0x46), chr(0x38), chr(0x39), chr(0x61),
  chr(0x01), chr(0x00), chr(0x01), chr(0x00), chr(0x80), chr(0xff),
  chr(0x00), chr(0xff), chr(0xff), chr(0xff), chr(0x00), chr(0x00),
  chr(0x00), chr(0x2c), chr(0x00), chr(0x00), chr(0x00), chr(0x00),
  chr(0x01), chr(0x00), chr(0x01), chr(0x00), chr(0x00), chr(0x02),
  chr(0x02), chr(0x44), chr(0x01), chr(0x00), chr(0x3b),
);

/**
 * The last octect of the IP address is removed to anonymize the user.
 */
function getIP($remote_address) {
  if (empty($remote_address)) {
    return '';
  }

  // Capture the first three octects of the IP address and replace the forth
  // with 0, e.g. 124.455.3.123 becomes 124.455.3.0
  $regex = '/^([^.]+\.[^.]+\.[^.]+\.).*/';
  if (preg_match($regex, $remote_address, $matches)) {
    return $matches[1] . '0';
  }
  else {
    return '';
  }
}

/**
 * Generate a visitor id for this hit.
 * If there is a visitor id in the cookie, use that, otherwise
 * use the guid if we have one, otherwise use a random number.
 */
function getVisitorId($guid, $account, $user_agent, $cookie) {

  // If there is a value in the cookie, don't change it.
  if (!empty($cookie)) {
    return $cookie;
  }

  $message = '';
  if (!empty($guid)) {
    // Create the visitor id using the guid.
    $message = $guid . $account;
  }
  else {
    // Otherwise this is a new user, create a new random id.
    $message = $user_agent . uniqid(getRandomNumber(), TRUE);
  }

  $md5_string = md5($message);

  return '0x' . substr($md5_string, 0, 16);
}

/**
 * Get a random number string.
 */
function getRandomNumber() {
  return rand(0, 0x7fffffff);
}

/**
 * Writes the bytes of a 1x1 transparent gif into the response.
 */
function writeGifData() {
  global $GIF_DATA;
  header('Content-Type: image/gif');
  header('Cache-Control: ' .
         'private, no-cache, no-cache=Set-Cookie, proxy-revalidate');
  header('Pragma: no-cache');
  header('Expires: Wed, 17 Sep 1975 21:32:10 GMT');
  echo join($GIF_DATA);
}

/**
 * Make a tracking request to Google Analytics from this server.
 * Copies the headers from the original request to the new one.
 */
function sendRequestToGoogleAnalytics($utm_url) {
  gaservice_request_async($utm_url, array(), 'GET');
}

/**
 * Track a page view, updates all the cookies and campaign tracker,
 * makes a server side request to Google Analytics and writes the transparent
 * gif byte data to the response.
 */
function trackPageView($utmr,$utmp,$utmac,$utmdebug) {
  $time_stamp = time();
  $domain_name = $_SERVER['SERVER_NAME'];
  if (empty($domain_name)) {
    $domain_name = '';
  }

  // Get the referrer from the utmr parameter, this is the referrer to the
  // page that contains the tracking pixel, not the referrer for tracking
  // pixel.
  $document_referer = $utmr;
  if (empty($document_referer) && $document_referer !== '0') {
    $document_referer = '-';
  }
  else {
    $document_referer = urldecode($document_referer);
  }
  $document_path = $utmp;
  if (empty($document_path)) {
    $document_path = '';
  }
  else {
    $document_path = urldecode($document_path);
  }

  $account = $utmac;
  $user_agent = $_SERVER['HTTP_USER_AGENT'];
  if (empty($user_agent)) {
    $user_agent = '';
  }

  // Try and get visitor cookie from the request.
  $cookie = $_COOKIE[COOKIE_NAME];

  $visitor_id = getVisitorId($_SERVER['HTTP_X_DCMGUID'], $account, $user_agent, $cookie);

  $utm_gif_location = 'http://www.google-analytics.com/__utm.gif';

  // Construct the gif hit url.
  $utm_url = $utm_gif_location . '?' .
    'utmwv=' . GAVERSION .
    '&utmn=' . getRandomNumber() .
    '&utmhn=' . urlencode($domain_name) .
    '&utmr=' . urlencode($document_referer) .
    '&utmp=' . urlencode($document_path) .
    '&utmac=' . $account .
    '&utmcc=__utma%3D999.999.999.999.999.1%3B' .
    '&utmvid=' . $visitor_id .
    '&utmip=' . getIP($_SERVER['REMOTE_ADDR']);

  sendRequestToGoogleAnalytics($utm_url);

  // If the debug parameter is on, add a header to the response that contains
  // the url that was used to contact Google Analytics.
  if (!empty($utmdebug)) {
    header('X-GA-MOBILE-URL:' . $utm_url);
  }
}
