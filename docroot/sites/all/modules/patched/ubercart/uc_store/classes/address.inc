<?php

/**
 * @file
 * UcAddress utility class definition.
 */

/**
 * Defines an object to hold Ubercart mailing address information.
 */
class UcAddress {

  /** Given name. */
  public $first_name = '';

  /** Surname. */
  public $last_name = '';

  /** Company or organization. */
  public $company = '';

  /** First line of street address. */
  public $street1 = '';

  /** Second line of street address. */
  public $street2 = '';

  /** City name. */
  public $city = '';

  /** State, provence, or region id. */
  public $zone = 0;

  /** ISO 3166-1 3-digit numeric country code. */
  public $country = 0;

  /** Postal code. */
  public $postal_code = '';

  /** Telephone number. */
  public $phone = '';

  /** Email address. */
  public $email = '';


  /**
   * Constructor.
   *
   * @param $country
   *   ISO 3166-1 3-digit numeric country code.  Defaults to the value of the
   *   uc_store_country system variable if that variable is set, or 840
   *   (United States of America) if it is not set.
   */
  public function __construct($country = NULL) {
    if (!$this->country) {
      $this->country = isset($country) ? $country : variable_get('uc_store_country', 840);
    }
  }

  /**
   * Compares two UcAddress objects to determine if they represent the same
   * physical address.
   *
   * Address properties such as first_name, phone, and email aren't considered
   * in this comparison because they don't contain information about the
   * physical location.
   *
   * @param $address
   *   An object of type UcAddress.
   *
   * @return
   *   TRUE if the two addresses are the same physical location, else FALSE.
   */
  public function isSamePhysicalLocation(UcAddress $address) {
    $physicalProperty = array(
      'street1', 'street2', 'city', 'zone', 'country', 'postal_code'
    );

    foreach ($physicalProperty as $property) {
      // Canonicalize properties before comparing.
      if (UcAddress::makeCanonical($this->$property)   !=
          UcAddress::makeCanonical($address->$property)  ) {
        return FALSE;
      }
    }
    return TRUE;
  }

  /**
   * Utility function to simplify comparison of address properties.
   *
   * For the purpose of this function, the canonical form is stripped of all
   * whitespace and has been converted to upper case.  This ensures that we
   * don't get false inequalities when comparing address properties that a
   * human would consider identical, but may be capitalized differently or
   * have different whitespace.
   *
   * @param $string
   *   String to make canonical.
   *
   * @return
   *   Canonical form of input string.
   */
  public static function makeCanonical($string = '') {
    // Remove all whitespace.
    $string = preg_replace('/\s+/', '', $string);
    // Make all characters upper case.
    $string = drupal_strtoupper($string);

    return $string;
  }

  /**
   * Formats the address for display based on the country's address format.
   *
   * @return
   *   A formatted string containing the address.
   */
  public function __toString() {
    $result = db_query('SELECT * FROM {uc_zones} WHERE zone_id = :id', array(':id' => $this->zone));
    if (!($zone_data = $result->fetchAssoc())) {
      $zone_data = array('zone_code' => t('N/A'), 'zone_name' => t('Unknown'));
    }
    $result = db_query('SELECT * FROM {uc_countries} WHERE country_id = :id', array(':id' => $this->country));
    if (!($country_data = $result->fetchAssoc())) {
      $country_data = array(
        'country_name' => t('Unknown'),
        'country_iso_code_2' => t('N/A'),
        'country_iso_code_3' => t('N/A'),
      );
    }

    $variables = array(
      "\r\n" => '<br />',
      '!company' => check_plain($this->company),
      '!first_name' => check_plain($this->first_name),
      '!last_name' => check_plain($this->last_name),
      '!street1' => check_plain($this->street1),
      '!street2' => check_plain($this->street2),
      '!city' => check_plain($this->city),
      '!zone_code' => $zone_data['zone_code'],
      '!zone_name' => $zone_data['zone_name'],
      '!postal_code' => check_plain($this->postal_code),
      '!country_name' => t($country_data['country_name']),
      '!country_code2' => $country_data['country_iso_code_2'],
      '!country_code3' => $country_data['country_iso_code_3'],
    );

    if (uc_store_default_country() != $this->country) {
      $variables['!country_name_if'] = t($country_data['country_name']);
      $variables['!country_code2_if'] = $country_data['country_iso_code_2'];
      $variables['!country_code3_if'] = $country_data['country_iso_code_3'];
    }
    else {
      $variables['!country_name_if']  = '';
      $variables['!country_code2_if'] = '';
      $variables['!country_code3_if'] = '';
    }

    $format = variable_get('uc_address_format_' . $this->country, '');
    if (empty($format)) {
      $format = "!company\r\n!first_name !last_name\r\n!street1\r\n!street2\r\n!city, !zone_code !postal_code\r\n!country_name_if";
    }
    $address = strtr($format, $variables);
    $address = strtr($address, array("\n" => '<br />'));

    $match = array('`^<br( /)?>`', '`<br( /)?>$`', '`<br( /)?>(\s*|[\s*<br( /)?>\s*]+)<br( /)?>`', '`<br( /)?><br( /)?>`', '`<br( /)?>, N/A`');
    $replace = array('', '', '<br />', '<br />', '', '');
    $address = preg_replace($match, $replace, $address);

    return $address;
  }

  /**
   * PHP magic method to use in relation with var_export().
   *
   * Created for strongarm compatibility.
   *
   * @param array $data
   *   Data to import
   */
  public static function __set_state($data) {
    $obj = new self;

    foreach ($data as $key => $val) {
      $obj->$key = $val;
    }

    return $obj;
  }

}
