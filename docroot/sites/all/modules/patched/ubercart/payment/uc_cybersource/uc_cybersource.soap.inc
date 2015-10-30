<?php

/**
 * @file
 * Defines a class used for communicating with CyberSource via SOAP.
 *
 * Provided by Acquia, Commercially supported Drupal - http://acquia.com
 */

class CyberSourceSoapClient extends SoapClient {
  function __construct($wsdl, $options = NULL) {
    parent::__construct($wsdl, $options);
  }

  /**
   * Inserts the UsernameToken information in the outgoing request.
   */
  function __doRequest($request, $location, $action, $version) {
    $login = _uc_cybersource_soap_login_data();

    $soapHeader = '<SOAP-ENV:Header xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/" xmlns:wsse="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd"><wsse:Security SOAP-ENV:mustUnderstand="1"><wsse:UsernameToken><wsse:Username>' . $login['merchant_id'] . '</wsse:Username><wsse:Password Type="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordText">' . $login['transaction_key'] . '</wsse:Password></wsse:UsernameToken></wsse:Security></SOAP-ENV:Header>';

    $requestDOM = new DOMDocument('1.0');
    $soapHeaderDOM = new DOMDocument('1.0');

    try {
      $requestDOM->loadXML($request);
      $soapHeaderDOM->loadXML($soapHeader);
      $node = $requestDOM->importNode($soapHeaderDOM->firstChild, TRUE);
      $requestDOM->firstChild->insertBefore($node, $requestDOM->firstChild->firstChild);
      $request = $requestDOM->saveXML();
    }
    catch (DOMException $e) {
      die('Error adding UsernameToken: ' . $e->code);
    }

    return parent::__doRequest($request, $location, $action, $version);
  }
}
