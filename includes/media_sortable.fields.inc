<?php

/**
 * @file
 * Provide an extended media selector widget with sortable plugin support.
 */


/**
 * Implements hook_field_widget_info().
 */
function media_sortable_field_widget_info() {
  $widgets = array();

  $widgets['media_sortable_widget'] = array(
    'label' => t('Media Sortable'),
    'field types' => array('file', 'image'),
    'settings' => array(
      'allowed_types' => array(
        'image' => 'image',
      ),
      'browser_plugins' => array(),
      'allowed_schemes' => array(
        'public' => 'public',
      ),
    ),
    'behaviors' => array(
      'multiple values' => FIELD_BEHAVIOR_CUSTOM,
      'default value' => FIELD_BEHAVIOR_NONE,
    ),
    'weight' => 3,
  );

  return $widgets;
}


/**
 * Implements hook_field_widget_settings_form().
 */
function media_sortable_field_widget_settings_form($field, $instance) {

  // Settings for this widget are identical to settings for default media widget,
  // so we simply reuse it.
  return media_field_widget_settings_form($field, $instance);

}


/**
 * Implements hook_field_widget_form().
 */
function media_sortable_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {

  // Most of the data here comes from standard media widget form. We load it here for starters
  $elements = media_field_widget_form($form, $form_state, $field, $instance, $langcode, $items, $delta, $element);

  // The difference is in how the files are represented. For that, we need to overwrite the theming for multiple items
  if(isset($elements['#theme']) && $elements['#theme'] === 'media_widget_multiple') {
    $elements['#theme'] = 'media_sortable_widget_multiple';
  }

  // Finally, we add the necessary libraries.
  drupal_add_library('system', 'ui.sortable');
  drupal_add_library('system', 'ui.droppable');
  drupal_add_library('system', 'ui.selectable');
  $elements['#attached']['js'][] = drupal_get_path('module', 'media_sortable') . '/js/media_sortable.js';
  $elements['#attached']['css'][] = drupal_get_path('module', 'media_sortable') . '/css/media_sortable.css';

  return $elements;

}


/**
 * Returns HTML for a group of media_sortable widgets.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element representing the widgets.
 *
 * @ingroup themeable
 *
 * This function is the core of the module. Most of the elements up to this point
 * were about reusing the default media widget. This function is built after
 * theme_media_widget_multiple, but replaces the draggable table with a draggable list.
 * This will allow us to theme it in a grid, which is more appropriate for files  than a long table.
 */
function theme_media_sortable_widget_multiple($variables) {

  $element = $variables['element'];

  // Special ID and classes for draggable list.
  $weight_class = $element['#id'] . '-weight';
  $list_id = drupal_html_id($element['#id'] . '-list');
  $trash_id = drupal_html_id($element['#id'] . '-trash');

  // Get our list of widgets in order (needed when the form comes back after
  // preview or failed validation).
  $widgets = array();
  foreach (element_children($element) as $key) {
    $widgets[] = &$element[$key];
  }
  usort($widgets, '_field_sort_items_value_helper');

  // Prepare items for the theme_item_list
  $items = array();

  foreach($widgets as $key => &$widget) {

    // Save the uploading field for last. We'll add it outside the list;
    if($widget['#file'] == FALSE) {
      $widget['#title'] = $element['#file_upload_title'];
      $widget['#description'] = $element['#file_upload_description'];
      continue;
    }

    // Delay rendering of the buttons.
    // TODO: decide what to do with buttons
    $operations_elements = array();
    foreach (element_children($widget) as $sub_key) {
      if (isset($widget[$sub_key]['#type']) && ($widget[$sub_key]['#type'] == 'submit' || $widget[$sub_key]['#type'] == 'link')) {
        hide($widget[$sub_key]);
        $operations_elements[] = &$widget[$sub_key];
      }
    }

    // Hide the display option. TODO: add display option;
    if($element['#display_field']) {
      hide($widget['display']);
    }

    // Also hide weight. We'll show it later
    hide($widget['_weight']);

    // Render everything else together in a list item, without the normal wrappers.
    $widget['#theme_wrappers'] = array();

    $information = drupal_render($widget);

    $widget['_weight']['#attributes']['class'] = array($weight_class, 'media-sortable-weight');
    $weight = render($widget['_weight']);

    $operations = '';
    foreach ($operations_elements as $operation_element) {
      if($operation_element['#type'] === 'submit' && strpos($operation_element['#name'], 'remove') !== FALSE) {
        $operations .= render($operation_element);
      }
    }

    $items[] = $information . $weight . $operations;
  }

  $variables = array(
    'items' => $items,
    'attributes' => array(
      'id' => $list_id,
      'class' => array('media-sortable'),
    ),
  );

  $output = theme('item_list', $variables);

  // Lastly, add the library button
  $output .= drupal_render_children($element);

  // And trash
  $trash_title = t('Drag image here to remove');
  $output .= '<div id="' . $trash_id . '" class="media-sortable-trash">' . $trash_title . '</div>';

  return $output;
}
