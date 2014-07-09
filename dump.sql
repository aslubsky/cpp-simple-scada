/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;

-- Dumping database structure for scada_db
DROP DATABASE IF EXISTS `scada_db`;
CREATE DATABASE IF NOT EXISTS `scada_db` /*!40100 DEFAULT CHARACTER SET utf8 COLLATE utf8_unicode_ci */;
USE `scada_db`;


-- Dumping structure for таблиця scada_db.archive_bool
DROP TABLE IF EXISTS `archive_bool`;
CREATE TABLE IF NOT EXISTS `archive_bool` (
  `date` datetime NOT NULL,
  `source_id` smallint(10) unsigned NOT NULL,
  `value` tinyint(1) NOT NULL,
  `prev_value` tinyint(1) NOT NULL,
  PRIMARY KEY (`date`,`source_id`),
  KEY `name` (`source_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Data exporting was unselected.


-- Dumping structure for таблиця scada_db.archive_numeric
DROP TABLE IF EXISTS `archive_numeric`;
CREATE TABLE IF NOT EXISTS `archive_numeric` (
  `date` datetime NOT NULL,
  `source_id` smallint(10) unsigned NOT NULL,
  `value` float NOT NULL,
  PRIMARY KEY (`date`,`source_id`),
  KEY `name` (`source_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- Data exporting was unselected.


/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
