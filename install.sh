#!/bin/bash
# Install only Drush 8 version as most of the vendor files depende on this.
composer require drush/drush:8.x-dev --prefer-source
# Install entity first as it required for apigee modules.
drush dl entity -y
drush en entity -y
# Run composer install to download all required vendor files.
composer install
# Download and enable swagger module as we are importing new content type
# with swagger field formatter.
drush dl swagger_ui_formatter -y
drush en swagger_ui_formatter -y
drush en apigee_modules -y
drush cr
drush cc drush
# Download and install all apigee required modules using composer and drush
drush enable:modules
# Download all webform required libraries
drush webform-libraries-download
drush en apigee_migration -y
drush cr
drush ms
# Run drush migration scripts which are required to import users,content,etc.,
drush mim d7_user_role --update
drush mim d7_node_type --update
drush mim d7_field --update
drush mim d7_field_instance --update
drush mim d7_field_instance_widget_settings --update
drush mim d7_view_modes --update
drush mim d7_field_formatter_settings --update
drush mim d7_user --update
drush mim apigee_migration_user --update
drush mim apigee_migration_vocabulary --update
drush mim apigee_migration_term --update
drush mim d7_node:article --update
drush mim apigee_migration_article --update
drush mim block_content_type --update
drush mim d7_filter_format --update
drush mim block_content_body_field --update
drush mim d7_custom_block --update
