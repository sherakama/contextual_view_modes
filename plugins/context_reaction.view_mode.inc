<?php
/**
 * @file
 *
 */

class context_reaction_view_mode extends context_reaction {

  /**
   * Override of options form.
   */
  function options_form($context) {
    $form = parent::options_form($context);
    $options = $this->fetch_from_context($context);
    $entity_info = entity_get_info();

    // Do not support these.
    unset($entity_info['comment']);
    unset($entity_info['taxonomy_vocabulary']);

    $form['entity_types'] = array(
      '#type' => 'vertical_tabs',
      '#attached' => array(
        'js' => array(
          'vertical-tabs' => drupal_get_path('module', 'vertical_tabs_example') . '/vertical_tabs_example.js',
        ),
      ),
    );

    foreach ($entity_info as $entity_type => $info) {

      $form['entity_types'][$entity_type] = array(
        '#title' => check_plain($info['label']),
        '#description' => t('View mode selection for ') . $info['label'],
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
        '#group' => 'entity_types',
        '#tree' => TRUE,
      );

      $modes = array("none" => t("Not set"));
      foreach ($info['view modes'] as $mode_key => $mode_info) {
        $modes[$mode_key] = $mode_info['label'];
      }

      foreach ($info['bundles'] as $machine_name => $bundle_info) {
        $default = isset($options['entity_types'][$entity_type][$machine_name]) ? $options['entity_types'][$entity_type][$machine_name] : 'none';
        $form['entity_types'][$entity_type][$machine_name] = array(
          '#title' => $bundle_info['label'] . " " . t("view mode"),
          '#description' => t("Select the view mode for this entity bundle type when this context is active"),
          '#type' => 'select',
          '#options' => $modes,
          '#default_value' => $default,
        );
      }

    }

    return $form;
  }


  /**
   * [execute description]
   * @return [type] [description]
   */
  function execute(&$entity, $type) {
    $wrapped = entity_metadata_wrapper($type, $entity);
    $bundle = $wrapped->getBundle();

    $contexts = $this->get_contexts();
    foreach ($contexts as $context) {
      $options = $this->fetch_from_context($context);
      if (!isset($options['entity_types'][$type][$bundle])) {
        continue;
      }
      if ($options['entity_types'][$type][$bundle] !== "none") {
        return $options['entity_types'][$type][$bundle];
      }
    }
  }

}
