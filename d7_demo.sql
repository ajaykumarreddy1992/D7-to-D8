SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;


CREATE TABLE `block_content` (
  `id` int(10) UNSIGNED NOT NULL,
  `revision_id` int(10) UNSIGNED DEFAULT NULL,
  `type` varchar(32) CHARACTER SET ascii NOT NULL COMMENT 'The ID of the target entity.',
  `uuid` varchar(128) CHARACTER SET ascii NOT NULL,
  `langcode` varchar(12) CHARACTER SET ascii NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='The base table for block_content entities.';

TRUNCATE TABLE `block_content`;
CREATE TABLE `block_content_field_data` (
  `id` int(10) UNSIGNED NOT NULL,
  `revision_id` int(10) UNSIGNED NOT NULL,
  `type` varchar(32) CHARACTER SET ascii NOT NULL COMMENT 'The ID of the target entity.',
  `langcode` varchar(12) CHARACTER SET ascii NOT NULL,
  `status` tinyint(4) NOT NULL,
  `info` varchar(255) DEFAULT NULL,
  `changed` int(11) DEFAULT NULL,
  `reusable` tinyint(4) DEFAULT NULL,
  `default_langcode` tinyint(4) NOT NULL,
  `revision_translation_affected` tinyint(4) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='The data table for block_content entities.';

TRUNCATE TABLE `block_content_field_data`;
CREATE TABLE `users` (
  `uid` int(10) UNSIGNED NOT NULL,
  `uuid` varchar(128) CHARACTER SET ascii NOT NULL,
  `langcode` varchar(12) CHARACTER SET ascii NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='The base table for user entities.';

TRUNCATE TABLE `users`;
INSERT INTO `users` (`uid`, `uuid`, `langcode`) VALUES
(0, '4b3e5ae7-432f-4aff-8d06-caf55cd97f0e', 'en'),
(1, 'c1130888-d7dc-4e43-b356-01dc75a83e39', 'en');


ALTER TABLE `block_content`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `block_content_field__uuid__value` (`uuid`),
  ADD UNIQUE KEY `block_content__revision_id` (`revision_id`),
  ADD KEY `block_content_field__type__target_id` (`type`);

ALTER TABLE `block_content_field_data`
  ADD PRIMARY KEY (`id`,`langcode`),
  ADD KEY `block_content__id__default_langcode__langcode` (`id`,`default_langcode`,`langcode`),
  ADD KEY `block_content__revision_id` (`revision_id`),
  ADD KEY `block_content_field__type__target_id` (`type`),
  ADD KEY `block_content__status_type` (`status`,`type`,`id`);

ALTER TABLE `users`
  ADD PRIMARY KEY (`uid`),
  ADD UNIQUE KEY `user_field__uuid__value` (`uuid`);


ALTER TABLE `block_content`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
