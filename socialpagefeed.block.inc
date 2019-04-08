<?php

/**
 * @file
 * File include for Social Page Feed module.
 */

/**
 * Return HTML with posts.
 *
 * @param string $provider
 *   Accepts string as value.
 *
 * @return string
 *   Returns string.
 *
 * @throws \Exception
 */
function socialpagefeed_posts_render($provider) {
  $fn = 'socialpagefeed_' . $provider . '_posts';
  $message_feed = call_user_func($fn);
  return theme('socialpagefeed_' . $provider, array($provider => $message_feed));
}

/**
 * Rendering values from the Facebook feed.
 *
 * @param int $i
 *   Accepts int as value.
 * @param string $facebook_entry
 *   Accepts string as value.
 * @param bool $display_all_posts
 *   Accepts bool as value.
 * @param bool $display_time
 *   Accepts bool as value.
 * @param bool $display_pic
 *   Accepts bool as value.
 * @param bool $display_video
 *   Accepts bool as value.
 * @param string $teaser_text
 *   Accepts string as value.
 * @param string $facebook_hash_tag
 *   Accepts string as value.
 *
 * @return mixed
 *   Returns mixed.
 */
function socialpagefeed_facebook_data($i, $facebook_entry, $display_all_posts, $display_time, $display_pic, $display_video, $teaser_text, $facebook_hash_tag) {
  $trim_length = variable_get('socialpagefeed_facebook_trim_length');
  if (array_key_exists('message', $facebook_entry)) {
    if (isset($facebook_entry['full_picture']) && $display_all_posts == 0) {
      if ($display_pic == 1) {
        $message_feed['full_picture'] = $facebook_entry['full_picture'];
      }
    }
    if (isset($facebook_entry['source']) && $display_all_posts == 0) {
      if ($display_video == 1) {
        $message_feed['video'] = $facebook_entry['source'];
      }
    }
    if (isset($facebook_entry['message'])) {
      if (isset($trim_length) && !empty($trim_length)) {
        $trimmed_message = substr($facebook_entry['message'], 0, $trim_length);
        $message_feed['message'] = $trimmed_message;
      }
      else {
        $message_feed['message'] = substr($facebook_entry['message'], 0, 200);
      }
    }
    if (isset($teaser_text) && !empty($teaser_text) && isset($facebook_entry['permalink_url'])) {
      $message_feed['full_feed_link'] = l(t('@teaser_text', array('@teaser_text' => $teaser_text)), $facebook_entry['permalink_url'], array('attributes' => array('target' => '_blank')));
    }
    else {
      $message_feed['full_feed_link'] = t('@teaser_text', array('@teaser_text' => $teaser_text));
    }

    if ($facebook_hash_tag == 1) {
      $message_feed['message'] = preg_replace_callback('/#(\\w+)/', function ($hash) {
        return l($hash[0], 'https:facebook.com/hashtag/' . $hash[1], array('attributes' => array('target' => '_blank')));
      }, $message_feed['message']);
    }
    if ($display_time == 1) {
      $formatted_date = new DateTime($facebook_entry['created_time']);
      $message_feed['created_stamp'] = $formatted_date->format(variable_get('socialpagefeed_facebook_time_format'));
    }
  }
  else {
    if (isset($facebook_entry['full_picture']) && $display_all_posts == 0) {
      if ($display_pic == 1) {
        $message_feed['full_picture'] = $facebook_entry['full_picture'];
      }
    }
    if (isset($facebook_entry['source']) && $display_all_posts == 0) {
      if ($display_video == 1) {
        $message_feed['video'] = $facebook_entry['source'];
      }
    }

    if (isset($facebook_entry['message']) && !empty($facebook_entry['message'])) {
      $message_feed['message'] = substr($facebook_entry['message'], 0, variable_get('socialpagefeed_facebook_trim_length'));
    }
    if (isset($teaser_text) && !empty($teaser_text) && isset($facebook_entry['permalink_url'])) {
      $message_feed['full_feed_link'] = l(t('@teaser_text', array('@teaser_text' => $teaser_text)), $facebook_entry['permalink_url'], array('attributes' => array('target' => '_blank')));
    }
    else {
      $message_feed['full_feed_link'] = t('@teaser_text', array('@teaser_text' => $teaser_text));
    }
  }

  return $message_feed;
}

/**
 * Returns HTML with feeds in required format.
 *
 * @param string $page_name
 *   Accepts string as value.
 *
 * @return string
 *   Returns string.
 *
 * @throws \Exception
 */
function socialpagefeed_facebook_feeds($page_name = '') {
  create_advertiser_ads();
}

/**
 * Uses socialpagefeed_facebook_posts() for fetching Facebook feeds.
 */
function socialpagefeed_facebook_posts() {
  $facebook_latest = socialpagefeed_facebook_feeds(variable_get('socialpagefeed_facebook_page_name'));

  return $facebook_latest;
}

/**
 * Uses socialpagefeed_twitter_posts() for fetching Twitter tweets.
 *
 * @return string
 *   Returns string.
 *
 * @throws \Exception
 */
function socialpagefeed_twitter_posts() {
  $twitter_tweets = array();
  $tweets_count = variable_get('socialpagefeed_twitter_tweets_count');
  $twitter_username = variable_get('socialpagefeed_twitter_username');
  $display_time = variable_get('socialpagefeed_twitter_time_stamp');
  $display_date_twitter_style = variable_get('socialpagefeed_twitter_time_ago');
  $twitter_hash_tag = variable_get('socialpagefeed_twitter_hashtag');
  $teaser_text = variable_get('socialpagefeed_twitter_teaser_text');
  $twitter_consumer_key = variable_get('socialpagefeed_twitter_consumer_key');
  $twitter_consumer_secrete = variable_get('socialpagefeed_twitter_consumer_secret');

  // Authentication Parameters.
  $api_key = urlencode($twitter_consumer_key);
  $api_secret = urlencode($twitter_consumer_secrete);
  $auth_url = 'https://api.twitter.com/oauth2/token';

  if (!empty($api_key) && !empty($api_secret)) {
    // What we want?
    $data_username = $twitter_username;
    $data_count = $tweets_count;
    $data_url = 'https://api.twitter.com/1.1/statuses/user_timeline.json';

    // Get API Access Token.
    $api_credentials = base64_encode($api_key . ':' . $api_secret);
    $auth_headers = 'Authorization: Basic ' . $api_credentials . "\r\n" . 'Content-Type: application/x-www-form-urlencoded;charset=UTF-8' . "\r\n";
    $auth_context = stream_context_create(array(
      'http' => array(
        'header' => $auth_headers,
        'method' => 'POST',
        'content' => http_build_query(array(
          'grant_type' => 'client_credentials',
        )),
      ),
    ));
    $auth_response = json_decode(file_get_contents($auth_url, 0, $auth_context), TRUE);
    $auth_token = $auth_response['access_token'];

    // Get Tweets.
    $data_context = stream_context_create(array('http' => array('header' => 'Authorization: Bearer ' . $auth_token . "\r\n")));
    $twitter_values = json_decode(file_get_contents($data_url . '?count=' . $data_count . '&tweet_mode=extended&screen_name=' . urlencode($data_username), 0, $data_context), TRUE);

    // Results - Do what you want!
    foreach ($twitter_values as $key => $twitter_value) {
      $twitter_tweets[$key]['username'] = $twitter_value['user']['screen_name'];
      $twitter_tweets[$key]['full_username'] = 'http://twitter.com/' . $twitter_value['user']['screen_name'];
      preg_match_all('#\bhttps?://[^\s()<>]+(?:\([\w\d]+\)|([^[:punct:]\s]|/))#', $twitter_value['full_text'], $extra_links);
      foreach ($extra_links[0] as $extra_link) {
        $twitter_tweets[$key]['extra_links'][] = $extra_link;
      }
      if (isset($twitter_value['full_text'])) {
        if (!empty($extra_link)) {
          $twitter_tweets[$key]['tweet'] = substr(rtrim($twitter_value['full_text'], $extra_link), 0, variable_get('socialpagefeed_twitter_trim_length'));
        }
        else {
          $twitter_tweets[$key]['tweet'] = substr(rtrim($twitter_value['full_text']), 0, variable_get('socialpagefeed_twitter_trim_length'));
        }
      }
      if (isset($teaser_text) && !empty($teaser_text)) {
        if (array_key_exists('media', $twitter_value['entities'])) {
          $twitter_tweets[$key]['tweet_url'] = l(t('@teaser_text', array('@teaser_text' => $teaser_text)), $twitter_value['entities']['media'][0]['url'], array('attributes' => array('target' => '_blank')));
        }
      }
      if (!empty($twitter_value['entities']['media'])) {
        $twitter_tweets[$key]['media'] = $twitter_value['entities']['media'];
      }
      if ($display_time == 1) {
        $formatted_twitter_date = new DateTime($twitter_value['created_at']);
        $reflection_object = new ReflectionObject($formatted_twitter_date->setTimezone(new DateTimeZone(drupal_get_user_timezone())));
        $get_property = $reflection_object->getProperty('date');
        $date = $get_property->getValue($formatted_twitter_date);
        if ($display_date_twitter_style == 1) {
          $twitter_tweets[$key]['twitter_date'] = socialpagefeed_time_elapsed_string($date);
        }
        else {
          $twitter_tweets[$key]['twitter_date'] = $formatted_twitter_date->format(variable_get('socialpagefeed_twitter_time_format'));
        }
      }
      if ($twitter_hash_tag == 1) {
        $twitter_tweets[$key]['tweet'] = preg_replace_callback('/#(\\w+)|@(\\w+)/', function ($hash) {
          if ($hash[0][0] == '#') {
            return l($hash[0], '//twitter.com/hashtag/' . $hash[1], array('attributes' => array('target' => '_blank')));
          }
          if ($hash[0][0] == '@') {
            return l($hash[0], '//twitter.com/' . $hash[2], array('attributes' => array('target' => '_blank')));
          }
        }, $twitter_tweets[$key]['tweet']);
      }
    }
    return $twitter_tweets;
  }
  drupal_set_message(t('Please provide your Twitter credentials <a href="@twitter">here</a>.', array('@twitter' => url('admin/config/services/socialpagefeed/twitter'))), 'warning');

  return array();
}

/**
 * Displays date in Twitter format.
 *
 * @param string $datetime
 *   Accepts string as value.
 * @param bool $full
 *   Accepts bool as value.
 *
 * @return string
 *   Returns string.
 */
function socialpagefeed_time_elapsed_string($datetime, $full = FALSE) {
  $now = new DateTime();
  $ago = new DateTime($datetime);
  $diff = $now->diff($ago);

  $diff->w = floor($diff->d / 7);
  $diff->d -= $diff->w * 7;

  $string = array(
    'y' => 'year',
    'm' => 'month',
    'w' => 'week',
    'd' => 'day',
    'h' => 'hour',
    'i' => 'minute',
    's' => 'second',
  );
  foreach ($string as $k => &$v) {
    if ($diff->$k) {
      $v = $diff->$k . ' ' . t($v . ($diff->$k > 1 ? 's' : ''));
    }
    else {
      unset($string[$k]);
    }
  }

  if (!$full) {
    $string = array_slice($string, 0, 1);
  }
  return $string ? t("@time ago", array('@time' => implode(', ', $string))) : t('just now');
}

/**
 * Uses socialpagefeed_instagram_posts() for fetching instagram pictures.
 *
 * @return string
 *   Returns string.
 *
 * @throws \Exception
 */
function socialpagefeed_instagram_posts() {
  $i = 0;
  $images = $pic = array();
  $instagram_post_link = variable_get('socialpagefeed_instagram_post_link');
  $resolution = variable_get('socialpagefeed_instagram_picture_resolution');
  $access_token = variable_get('socialpagefeed_instagram_access_token');
  $url = "https://api.instagram.com/v1/users/self/media/recent?access_token=" . variable_get('socialpagefeed_instagram_access_token') . '&count=' . variable_get('socialpagefeed_instagram_picture_count');
  $request = drupal_http_request($url);
  $status = strtoupper($request->status_message);

  if ($status == 'OK') {
    if (isset($access_token) && !empty($access_token)) {
      $json_response = drupal_json_decode($request->data);
      foreach ($json_response['data'] as $key => $response_data) {
        if ($instagram_post_link == 1) {
          $images[$key]['post_url'] = $response_data['link'];
        }
        $pic[] = $response_data['images'];
        $i++;
        if ($i == variable_get('socialpagefeed_instagram_picture_count')) {
          break;
        }
      }

      foreach ($pic as $key => $image) {
        $images[$key]['image_url'] = $image[$resolution]['url'];
      }
      return $images;
    }
    else {
      drupal_set_message(t('The access_token provided is invalid.'), 'error');
    }
  }
  else {
    drupal_set_message(t('Please provide your Instagram credentials <a href="@instagram">here</a>.', array('@instagram' => url('admin/config/services/socialpagefeed/instagram'))), 'warning');
  }
}
