-- MySQL dump 10.14  Distrib 5.5.25-MariaDB, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: warmshowers_thefays_us
-- ------------------------------------------------------
-- Server version	5.5.25-MariaDB-mariadb1~lucid-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `languages`
--

DROP TABLE IF EXISTS `languages`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `languages` (
  `language` varchar(12) NOT NULL DEFAULT '',
  `name` varchar(64) NOT NULL DEFAULT '',
  `native` varchar(64) NOT NULL DEFAULT '',
  `direction` int(11) NOT NULL DEFAULT '0',
  `enabled` int(11) NOT NULL DEFAULT '0',
  `plurals` int(11) NOT NULL DEFAULT '0',
  `formula` varchar(128) NOT NULL DEFAULT '',
  `domain` varchar(128) NOT NULL DEFAULT '',
  `prefix` varchar(128) NOT NULL DEFAULT '',
  `weight` int(11) NOT NULL DEFAULT '0',
  `javascript` varchar(32) NOT NULL DEFAULT '',
  PRIMARY KEY (`language`),
  KEY `list` (`weight`,`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `languages`
--

LOCK TABLES `languages` WRITE;
/*!40000 ALTER TABLE `languages` DISABLE KEYS */;
INSERT INTO `languages` VALUES ('de','German','Deutsch',0,1,2,'($n!=1)','http://de.warmshowers.org','',5,'7bdbce288e372be7dd372b80da06ac60'),('en','English','English',0,0,0,'','','en',0,''),('en-working','English','English',0,1,0,'','http://www.warmshowers.org','',0,''),('es','Spanish','Español',0,1,2,'($n!=1)','http://es.warmshowers.org','',1,'f0b8ff15e04ecc32880ff7b4fd5b6049'),('fr','French','Français',0,1,2,'($n!=1)','http://fr.warmshowers.org','',3,'ac7e0dfd27a1c99c0725f5883f5797be'),('it','Italian','Italiano',0,1,2,'($n!=1)','http://it.warmshowers.org','',6,'e8ddc032b61f488e52dd42cffd53087a'),('pt-br','Portuguese','Português',0,1,2,'($n!=1)','http://pt.warmshowers.org','',4,'1c65ba4f2bfba160c31804d0c5016c46');
/*!40000 ALTER TABLE `languages` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2012-09-27 11:02:06
