<?php

/**
 * @file
 * Social Page Feed module admin inc file.
 *
 * Contains admin configuration page callback functions.
 */

/**
 * Form builder; Configure social feeds settings for this site.
 *
 * @ingroup forms
 *
 * @see system_settings_form()
 */
function socialpagefeed_facebook_settings($form, &$form_state) {
  $form['socialpagefeed_facebook_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook App ID'),
    '#default_value' => variable_get('socialpagefeed_facebook_app_id'),
    '#description' => t('Create your Facebook app and visit <a href="@this" target="@blank">this link</a> to get App ID.', array('@this' => 'https://developers.facebook.com/apps', '@blank' => '_blank')),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );

  $form['socialpagefeed_facebook_secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Secret Key'),
    '#default_value' => variable_get('socialpagefeed_facebook_secret_key'),
    '#description' => t('<a href="@this" target="@blank"> This link</a> will help you to find out your App Secret Key.', array('@this' => 'https://developers.facebook.com/apps', '@blank' => '_blank')),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );

  $form['socialpagefeed_facebook_user_access_token'] = array(
    '#type' => 'textarea',
    '#title' => t('Facebook User Access Token'),
    '#default_value' => variable_get('socialpagefeed_facebook_user_access_token'),
    '#description' => t('<a href="@this" target="@blank"> This link</a> will help you to generate your User Access Token.', array('@this' => 'https://developers.facebook.com/tools/explorer/', '@blank' => '_blank')),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Form builder; Configure social feeds settings for this site.
 *
 * @ingroup forms
 *
 * @see system_settings_form()
 */
function socialpagefeed_twitter_settings($form, &$form_state) {
  $form['socialpagefeed_twitter_consumer_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Consumer Key'),
    '#default_value' => variable_get('socialpagefeed_twitter_consumer_key'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );

  $form['socialpagefeed_twitter_consumer_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Consumer Secret'),
    '#default_value' => variable_get('socialpagefeed_twitter_consumer_secret'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );

  $form['socialpagefeed_twitter_oauth_access_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter Oauth Access Token'),
    '#default_value' => variable_get('socialpagefeed_twitter_oauth_access_token'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );

  $form['socialpagefeed_twitter_oauth_access_token_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Twitter User Name'),
    '#default_value' => variable_get('socialpagefeed_twitter_oauth_access_token_secret'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );

  /*$form['socialpagefeed_twitter_username'] = array(
  '#type' => 'textfield',
  '#title' => t('Twitter User Name'),
  '#default_value' => variable_get('socialpagefeed_twitter_username'),
  '#size' => 60,
  '#maxlength' => 100,
  '#required' => TRUE,
  );*/

  return system_settings_form($form);
}

/**
 * Form builder; Configure social feeds settings for this site.
 *
 * @ingroup forms
 *
 * @see system_settings_form()
 */
function socialpagefeed_instagram_settings($form, &$form_state) {
  $form['socialpagefeed_instagram_header']['#markup'] = t('To get Client ID you need to manage clients from your instagram account detailed information <a href="@admin" target="@blank">here</a>.', array('@admin' => url('admin/help/socialpagefeed'), '@blank' => '_blank'));
  $form['socialpagefeed_instagram_client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#description' => t('Client ID from your Instagram Account, for more details click <a href="@here"  target="@blank">here</a>.', array('@here' => 'https://gist.github.com/hemangigokhale/156db13c70840f076d766cd2b3976276#instagram', '@blank' => '_blank')),
    '#default_value' => variable_get('socialpagefeed_instagram_client_id'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['socialpagefeed_instagram_redirect_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect URI'),
    '#description' => t('Redirect URI from your Instagram Account.'),
    '#default_value' => variable_get('socialpagefeed_instagram_redirect_uri'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['socialpagefeed_instagram_auth_link'] = array(
    '#type' => 'item',
    '#title' => t('Generate Instagram Access Token'),
    '#description' => t('Access this URL in your browser https://instagram.com/oauth/authorize/?client_id=&lt;your_client_id&gt;&redirect_uri=&lt;your_redirect_uri&gt;&response_type=token, you will get the access token.'),
    '#markup' => t('Click <a href="@here" target="@blank">here</a> for more details.', array('@here' => url('https://gist.github.com/hemangigokhale/156db13c70840f076d766cd2b3976276#generate-an-instagram-access-token'), '@blank' => '_blank')),
  );
  $form['socialpagefeed_instagram_access_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Access Token'),
    '#default_value' => variable_get('socialpagefeed_instagram_access_token'),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );
  $form['socialpagefeed_instagram_picture_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Picture Count'),
    '#default_value' => variable_get('socialpagefeed_instagram_picture_count', 1),
    '#size' => 60,
    '#maxlength' => 100,
  );
  if (variable_get('socialpagefeed_instagram_access_token')) {
    $form['socialpagefeed_instagram_feed'] = array(
      '#type' => 'item',
      '#title' => t('Feed URL'),
      '#markup' => t('https://api.instagram.com/v1/users/self/media/recent?access_token=@socialpagefeed_instagram_access_token&count=@socialpagefeed_instagram_picture_count',
        array(
          '@socialpagefeed_instagram_access_token' => variable_get('socialpagefeed_instagram_access_token'),
          '@socialpagefeed_instagram_picture_count' => variable_get('socialpagefeed_instagram_picture_count', 1),
        )
      ),
    );
  }
  $form['socialpagefeed_instagram_picture_resolution'] = array(
    '#type' => 'select',
    '#title' => t('Picture Resolution'),
    '#default_value' => variable_get('socialpagefeed_instagram_picture_resolution', 'thumbnail'),
    '#options' => array(
      'thumbnail' => t('Thumbnail'),
      'low_resolution' => t('Low Resolution'),
      'standard_resolution' => t('Standard Resolution'),
    ),
  );
  $form['socialpagefeed_instagram_post_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show post URL'),
    '#default_value' => variable_get('socialpagefeed_instagram_post_link', FALSE),
  );

  return system_settings_form($form);
}
