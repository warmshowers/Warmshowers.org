<?php

/**
 * @file
 * Slack integration module API functions.
 */

/**
 * Send message to the Slack.
 *
 * @param string $message
 *   The message sent to the channel.
 * @param string $channel
 *   The channel in the Slack service to send messages.
 * @param string $username
 *   The bot name displayed in the channel.
 *
 * @return bool|object
 *   Slack response.
 */
function slack_send_message($webhook_url, $message, $channel = '', $username = '', $icon_options = array(), $attachment_options = array()) {
  if (!$webhook_url) {
    drupal_set_message('Please, enter the webhook value and try again', 'error');
    return FALSE;
  }
  if ($channel) {
    $message_options['channel'] = $channel;
  }
  else {
    $message_options['channel'] = slack_get_default_channel();
  }
  if ($username) {
    $message_options['username'] = $username;
  }
  else {
    $message_options['username'] = slack_get_default_username();
  }

  if ($icon_options['type'] == 'emoji') {
    if ($icon_options['emoji']) {
      $message_options['icon_emoji'] = $icon_options['emoji'];
    }
    else {
      $message_options['icon_emoji'] = slack_get_default_icon_emoji();
    }
  }
  elseif ($icon_options['type'] == 'image') {
    if ($icon_options['icon']) {
      $message_options['icon_url'] = $icon_options['icon'];
    }
    else {
      $message_options['icon_url'] = slack_get_default_icon_url();
    }
  }
  $result = _slack_send_message($webhook_url, $message, $message_options, $attachment_options);
  return $result;
}

/**
 * Send message to the Slack with more options.
 *
 * @param string $team_name
 *   Your team name in the Slack.
 * @param string $team_token
 *   The token from "Incoming WebHooks" integration in the Slack.
 * @param string $message
 *   The message sent to the channel.
 * @param array $message_options
 *   An associative array, it can contain:
 *     - channel: The channel in the Slack service to send messages
 *     - username: The bot name displayed in the channel
 *     - icon_emoji: The bot icon displayed in the channel
 *     - icon_url: The bot icon displayed in the channel
 * @param array $attachments
 *   An optional associative array to further style the message
 *        - title: an optinal title for the message
 *        - pretext: Text preceding the message
 *        - color: a color to emphasize the text body
 *        - title_link: a link associated with the message
 *
 * @return object
 *   Can contain:
 *                          success      fail          fail
 *     - data:                ok         No hooks      Invalid channel specified
 *     - status message:      OK         Not found     Server Error
 *     - code:                200        404           500
 *     - error:               -          Not found     Server Error
 */
function _slack_send_message($webhook_url, $message, $message_options = array(), $attachment_options = array()) {
  $headers = array(
    'Content-Type' => 'application/json',
  );
  $message = _slack_process_message($message);
  if(empty($attachment_options)) {
    $message_options['text'] = $message;
  }
  else {
    $attachment_options['fallback'] = $message;
    $attachment_options['text'] = $message;

    $message_options['attachments'] = array(
      $attachment_options
    );
  }
  $sending_data = json_encode($message_options, JSON_UNESCAPED_SLASHES);
  $options = array(
    'method' => 'POST',
    'data' => $sending_data,
    'headers' => $headers,
  );
  $sending_url = $webhook_url;
  $result = drupal_http_request($sending_url, $options);
  return $result;
}

/**
 * Replaces links with slack friendly tags. Strips all other html.
 *
 * @param string $message
 *   The message sent to the channel.
 *
 * @return string
 *   Replaces links with slack friendly tags. Strips all other html.
 */
function _slack_process_message($message) {
  $regexp = "<a\s[^>]*href=(\"??)([^\" >]*?)\\1[^>]*>(.*)<\/a>";
  if (preg_match_all("/$regexp/siU", $message, $matches, PREG_SET_ORDER)) {
    $i = 1;
    foreach ($matches as $match) {
      $new_link = "<$match[2] | $match[3]>";
      $links['link-' . $i] = $new_link;
      $message = str_replace($match[0], 'link-' . $i, $message);
      $i++;
      $message = strip_tags($message);
      foreach ($links as $id => $link) {
        $message = str_replace($id, $link, $message);
      }
    }
  }
  return $message;
}

/**
 * Get default Webhook URL.
 *
 * @return string
 *   Get default Webhook URL.
 */
function slack_get_default_webhook_url() {
  $channel = variable_get('slack_webhook_url');
  return $channel;
}

/**
 * Get default team channel.
 *
 * @return string
 *   Get default team channel.
 */
function slack_get_default_channel() {
  $channel = variable_get('slack_channel');
  return $channel;
}

/**
 * Get default Slack bot username.
 *
 * @return string
 *   Get default Slack bot username.
 */
function slack_get_default_username() {
  $username = variable_get('slack_username');
  return $username;
}

/**
 * Get default Slack bot icon.
 *
 * @return string
 *   Get default Slack bot icon.
 */
function slack_get_default_icon_url() {
  $icon['url'] = variable_get('slack_icon_url');
  return $icon['url'];
}

/**
 * Get default Slack bot icon emoji.
 *
 * @return string
 *   Get default Slack bot icon emoji.
 */
function slack_get_default_icon_emoji() {
  $icon['emoji'] = variable_get('slack_icon_emoji');
  return $icon['emoji'];
}

/**
 * Get default Slack bot image type radios value.
 *
 * @return string
 *   Get default Slack bot image type radios value.
 */
function slack_get_default_icon_type() {
  $select_image = variable_get('slack_icon_type', 'none');
  return $select_image;
}
