<?php

/**
 * @file
 * Social Page Feed module inc file.
 */

/**
 * Add new facebook social page form.
 */
function socialpagefeed_add_facebook_page($form, &$form_state) {
  $submit_text = t('Add New Facebook Page');
  $parameters = drupal_get_query_parameters();
  if (isset($parameters['page_id']) && is_numeric($parameters['page_id']) && arg(6) == 'edit') {
    $submit_text = t('Update Facebook Page');
    $result = db_query('SELECT page_name, status from {socialpagefeed_pages} WHERE id=:page_id AND type=:type', [':page_id' => $parameters['page_id'], ':type' => 'facebook'])->fetchAll();

    if (!empty($result)) {
      $pages_info = reset($result);
      $page_title = $pages_info->page_title;
      $page_name = $pages_info->page_name;
      $status = $pages_info->status;
    }
    else {
      drupal_not_found();
      drupal_exit();
    }
  }

  $form['page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Page Title'),
    '#default_value' => isset($page_title) ? $page_title : '',
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );

  $form['page_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Facebook Page Name'),
    '#default_value' => isset($page_name) ? $page_name : '',
    '#description' => t('eg. If your Facebook page URL is this http://www.facebook.com/YOUR_PAGE_NAME, <br />then you just need to add this YOUR_PAGE_NAME above OR find your Facebook Page ID <a href="@here" target="@blank">here</a>.', array('@here' => 'http://findmyfbid.com/', '@blank' => '_blank')),
    '#size' => 60,
    '#maxlength' => 100,
    '#required' => TRUE,
  );

  $form['status'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#default_value' => isset($status) ? $status : 0,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => $submit_text,
  );

  return $form;
}

/**
 *
 */
function socialpagefeed_add_facebook_page_validate($form, &$form_state) {
  $page_name = $form_state['values']['page_name'];

  if (isset($parameters['page_id']) && is_numeric($parameters['page_id']) && arg(6) == 'add') {
    $result = db_query('SELECT page_name from {socialpagefeed_pages} WHERE page_name=:page_name AND type=:type', [':page_name' => $page_name, ':type' => 'facebook'])->fetchField();
    if ($result) {
      form_set_error('page_name', t('This facebook page name already exists.'));
    }
  }
}

/**
 *
 */
function socialpagefeed_add_facebook_page_submit($form, &$form_state) {
  global $base_url;
  $parameters = drupal_get_query_parameters();
  $page_title = $form_state['values']['page_title'];
  $page_name = $form_state['values']['page_name'];
  $status = $form_state['values']['status'];

  if (isset($parameters['page_id']) && is_numeric($parameters['page_id']) && arg(6) == 'edit') {
    $id = db_update('socialpagefeed_pages')
      ->fields(array(
        'page_title' => $page_title,
        'page_name' => $page_name,
        'type' => 'facebook',
        'status' => $status,
        'updated' => time(),
      ))
      ->condition('id', $parameters['page_id'], '=')
      ->execute();

    drupal_set_message(t("Facebook Page $page_name has been updated successfully"));
  }
  else {
    $id = db_insert('socialpagefeed_pages')
      ->fields(array(
        'page_title' => $page_title,
        'page_name' => $page_name,
        'type' => 'facebook',
        'status' => $status,
        'updated' => time(),
      ))
      ->execute();

    drupal_set_message(t("New Facebook Page $page_name has been added successfully in the list."));
  }

  $url = $base_url . '/admin/config/services/socialpagefeed/facebook/pagelist';
  drupal_goto($url);
}

/**
 *
 */
function socialpagefeed_facebook_pagelist() {
  $header = array(
    array('data' => '#', 'field' => 'id', 'sort' => 'ASC'),
    array('data' => 'Facebook Page Title', 'field' => 'page_name', 'sort' => 'ASC'),
    array('data' => 'Facebook Page Name', 'field' => 'page_name', 'sort' => 'ASC'),
    array('data' => 'Status', 'field' => 'status', 'sort' => 'ASC'),
    array('data' => 'Modified Date', 'field' => 'updated', 'sort' => 'ASC'),
    array('data' => 'Action'),
  );

  $data = array();
  $select = db_select('socialpagefeed_pages', 'spf')
    ->extend('PagerDefault')
    ->extend('TableSort');

  $select
    ->fields('spf', array('id', 'page_title', 'page_name', 'status', 'updated'))
    ->limit(50)
    ->orderByHeader($header);

  $result = $select->execute();

  $rows = array();
  foreach ($result as $row) {
    $rows[] = [
      $row->id,
      $row->page_title,
      $row->page_name,
      !empty($row->status) ? t('Enabled') : t('Disabled'),
      date('m-d-Y', $row->updated),
      l('EDIT', 'admin/config/services/socialpagefeed/facebook/page/edit', ['query' => ['page_id' => $row->id]]) . '/' . l('DELETE', 'admin/config/services/socialpagefeed/facebook/page/delete', ['query' => ['page_id' => $row->id]]),
    ];
  }

  $output = theme('table', array('header' => $header, 'rows' => $rows));
  $output .= theme('pager');

  return $output;
}

/**
 *
 */
function socialpagefeed_delete_facebook_page() {
  global $base_url;
  $parameters = drupal_get_query_parameters();
  if (isset($parameters['page_id']) && is_numeric($parameters['page_id']) && arg(6) == 'delete') {

    db_delete('socialpagefeed_pages')
      ->condition('id', $parameters['page_id'])
      ->execute();
    drupal_set_message(t("Facebook Page has been deleted successfully."));

    $url = $base_url . '/admin/config/services/socialpagefeed/facebook/pagelist';
    drupal_goto($url);
  }
}
