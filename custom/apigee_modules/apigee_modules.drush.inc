<?php

use Drupal\Core\Database\Database;
use Drupal\migrate_custom\Controller\MigrationSummary;
use Symfony\Component\Process\Process;

/**
 * @file Contains the code to generate the custom drush commands.
 */
/**
 * Implements hook_drush_command().
 */
function apigee_modules_drush_command() {
  $items = [];
  $items['enable_modules'] = [
    'description' => 'Enables the required modules for Apigee Migration',
    'drupal dependencies' => ['apigee_modules'],
    'aliases' => ['enable:modules'],
  ];
  $items['disable_modules'] = [
    'description' => 'Uninstall the required modules for Apigee Migration',
    'drupal dependencies' => ['apigee_modules'],
    'aliases' => ['disable:modules'],
  ];
  $items['list_modules'] = [
    'description' => 'List all modules in D7',
    'drupal dependencies' => ['apigee_modules'],
    'aliases' => ['list:modules'],
  ];
  $items['count_entities'] = [
    'description' => 'Count all entities in D7',
    'drupal dependencies' => ['apigee_modules'],
    'aliases' => ['count:entities'],
  ];
  return $items;
}

/**
 * Call back function drush_apigee_modules_disable_modules()
 * The call back function name in the following format
 *   drush_{module_name}_{item_id_for_command}()
 */
function drush_apigee_modules_disable_modules() {
  $modules = [
    'admin_toolbar',
    'admin_toolbar_tools',
    'apigee_edge',
    'autologout',
    'block_class',
    'captcha',
    'commerce',
    'commerce_price',
    'commerce_product',
    'content_access',
    'context',
    'ctools',
    'devel',
    'eck',
    'entity',
    'entity_reference_revisions',
    'environment_indicator',
    'faq',
    'file_entity',
    'google_analytics',
    'ldap',
    'libraries',
    'mailsystem',
    'markdown',
    'mediaelement',
    'metatag',
    //'migrate_drupal', core
    'migrate_plus',
    'migrate_tools',
    'mimemail',
    'oauth',
    'pathauto',
    'profile',
    'readonlymode',
    'recaptcha',
    'redirect',
    'redis',
    'remote_stream_wrapper',
    'rules',
    'services',
    'smtp',
    'state_machine',
    'syntaxhighlighter',
    'token',
    //'views', Core
    'views_accordion',
    'views_bulk_operations',
    'views_slideshow',
    'webform',
    // Finish install required modules after downloading dependencies.
    'commerce_cart',
    'commerce_order',
  ];
  foreach ($modules as $module) {
    $process = new Process('composer remove drupal/' . $module);
    $process->run();
    //    $module_process = new Process('drush pmu ' . $module . ' -y');
    //    $module_process->run();
    drush_print(t('Successfully un-installed @module', ['@module' => $module]));
  }
}

/**
 * Call back function drush_apigee_modules_enable_modules()
 * The call back function name in the following format
 *   drush_{module_name}_{item_id_for_command}()
 */
function drush_apigee_modules_enable_modules() {
  $drush_modules = [
    'entity',
  ];
  foreach ($drush_modules as $drush_module) {
    $module_process = new Process('drush en ' . $drush_module . ' -y');
    $module_process->mustRun();
    if ($module_process->getOutput()) {
      drush_print(t($module_process->getOutput()));
    }
    drush_print(t('Successfully installed @module', ['@module' => $drush_module]));
  }
  // Install adminimal and bootstrap theme.
  $themes = ['bootstrap', 'adminimal_theme'];
  foreach ($themes as $theme) {
    $theme_process = new Process('composer require drupal/' . $theme . ' --no-update');
    $theme_process->mustRun();
    if ($theme_process->getOutput()) {
      drush_print(t($theme_process->getOutput()));
    }
    \Drupal::service('theme_installer')->install([$theme]);
    if ($theme == 'adminimal_theme') {
      \Drupal::service('theme_handler')->setDefault('adminimal_theme');
      drush_print(t('Successfully installed @module', ['@module' => 'adminimal_theme']));
    }
  }
  $modules = [
    'admin_toolbar',
    'admin_toolbar_tools',
    'apigee_edge',
    'autologout',
    'block_class',
    'captcha',
    'commerce',
    'commerce_price',
    'commerce_product',
    'content_access',
    'context',
    'ctools',
    'devel',
    'devel_php',
    'eck',
    'entity_reference_revisions',
    'environment_indicator',
    'faq',
    'google_analytics',
    'ldap',
    'libraries',
    'mailsystem',
    'markdown',
    'mediaelement',
    'metatag',
    'migrate_plus',
    'migrate_tools',
    'mimemail',
    'pathauto',
    'profile',
    'readonlymode',
    'recaptcha',
    'redirect',
    'redis',
    'remote_stream_wrapper',
    'rules',
    'services',
    'smtp',
    'state_machine',
    'swagger_ui_formatter',
    'token',
    'views_accordion',
    'views_bulk_operations',
    'views_slideshow',
    'webform',
    // Finish install required modules after downloading dependencies.
    'commerce_cart',
    'commerce_order',
    'commerce_product',
  ];
  foreach ($modules as $module) {
    $process = new Process('composer require drupal/' . $module . ' --no-update');
    $process->mustRun();
    if ($process->getOutput()) {
      drush_print(t($process->getOutput()));
    }
    $module_process = new Process('drush en ' . $module . ' -y');
    $module_process->mustRun();
    if ($module_process->getOutput()) {
      drush_print(t($module_process->getOutput()));
    }
    drush_print(t('Successfully installed @module', ['@module' => $module]));
  }
  // Download required libraries
  $libraries = [
    'swagger_ui' => "https://api.github.com/repos/swagger-api/swagger-ui/releases/latest",
    'php-markdown' => "https://github.com/michelf/php-markdown/archive/1.8.0.tar.gz",
  ];
  foreach ($libraries as $library_name => $url) {
    $releases = file_get_contents($url, FALSE);
    $releases = json_decode($releases);
    if ($releases) {
      $download_url = $releases->tarball_url;
    }
    else {
      $download_url = $url;
    }
    $temp_dir = drush_tempdir();
    drush_print("Downloading Swagger library");
    $temp_filepath = $temp_dir . '/' . basename(current(explode('?', $download_url, 2)));
    drush_download_file($download_url, $temp_filepath);
    // Extract ZIP archive.
    $download_location = DRUPAL_ROOT . "/libraries/$library_name";
    drush_print("Extracting to $download_location");
    // Extract to temp location.
    $temp_location = drush_tempdir();
    if (!drush_tarball_extract($temp_filepath, $temp_location)) {
      drush_set_error("Unable to extract $library_name");
    }
    $files = scandir($temp_location);
    unset($files[0], $files[1]);
    if ((count($files) == 1) && is_dir($temp_location . '/' . current($files))) {
      $temp_location .= '/' . current($files);
    }
    drush_move_dir($temp_location, $download_location);
    if (file_exists($temp_filepath)) {
      drush_delete_dir($temp_filepath, TRUE);
    }
  }
}

/**
 * Call back function drush_apigee_modules_list_modules()
 * The call back function name in the following format
 *   drush_{module_name}_{item_id_for_command}()
 */
function drush_apigee_modules_list_modules() {
  $database = Database::getConnectionInfo('migrate');
  if ($database) {
    Database::setActiveConnection('migrate');
    $db = Database::getConnection();
    $table_exists = $db->schema()->tableExists('system');
    if ($table_exists) {
      $results = $db->select('system', 's', [
        'fetch' => \PDO::FETCH_ASSOC,
      ])
        ->fields('s')
        ->execute();
      foreach ($results as $result) {
        $system_data[$result['type']][$result['name']] = $result;
      }
    }
    // Set back D8 database active.
    Database::setActiveConnection();
    drush_print_r($system_data);
  }
}


/**
 * Call back function drush_apigee_modules_count_entities()
 * The call back function name in the following format
 *   drush_{module_name}_{item_id_for_command}()
 */
function drush_apigee_modules_count_entities() {
  $migration = MigrationSummary::generateReport();
  $headers = [
    t('Content Types'),
    t('Nodes'),
    t('Files'),
    t('User Roles'),
    t('Users'),
    t('Vocabulary'),
    t('Taxonomy Terms'),
    t('Views'),
    t('Views Display'),
    t('Enabled Modules'),
    t('Enabled Themes'),
  ];
  $table_rows = drush_rows_of_key_value_to_array_table($migration, $headers, []);
  drush_print_table($table_rows, TRUE);
}