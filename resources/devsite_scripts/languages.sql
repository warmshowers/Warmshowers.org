-- MySQL dump 10.14  Distrib 5.5.31-MariaDB, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: warmshowers_org
-- ------------------------------------------------------
-- Server version	5.5.31-MariaDB-1~precise-log

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
set autocommit=0;
INSERT INTO `languages` VALUES ('cs','Czech','Čeština',0,1,3,'(((($n%10)==1)&&(($n%100)!=11))?(0):((((($n%10)>=2)&&(($n%10)<=4))&&((($n%100)<10)||(($n%100)>=20)))?(1):2))','https://cs.warmshowers.org','',7,'21f90f7fca158778c0e92f8edecdf83c'),('de','German','Deutsch',0,1,2,'($n!=1)','https://de.warmshowers.org','',3,'1d515b450691547e71dc2942f7ec69f9'),('en','English','English',0,0,0,'','','en',0,''),('en-working','English','English',0,1,2,'($n!=1)','https://www.warmshowers.org','',0,''),('es','Spanish','Español',0,1,2,'($n!=1)','https://es.warmshowers.org','',2,'a38591995154108eb67902912ca42c9c'),('fa','Persian','فارسی',1,1,2,'($n!=1)','https://fa.warmshowers.org','',9,'c5f5c97d14b4cbd73db330b433b466de'),('fr','French','Français',0,1,2,'($n!=1)','https://fr.warmshowers.org','',1,'4a2906aaaabf9cc9de87a947ff88b50a'),('it','Italian','Italiano',0,1,2,'($n!=1)','https://it.warmshowers.org','',4,'41f37f538d177ba3be405e79c59c4238'),('ja','Japanese','日本語',0,1,2,'($n!=1)','https://ja.warmshowers.org','',9,'ed9b9e3716d1e643cb08832281f46363'),('pl','Polish','Polski',0,1,3,'(($n==1)?(0):((((($n%10)>=2)&&(($n%10)<=4))&&((($n%100)<10)||(($n%100)>=20)))?(1):2))','https://pl.warmshowers.org','',7,'ae7b3aab6d9a165b91a05a60fcf9faf4'),('pt-br','Portuguese','Português',0,1,2,'($n!=1)','https://pt.warmshowers.org','',5,'1f9962a177895eb8224a1893a5d47641'),('ro','Romanian','Română',0,0,3,'(($n==1)?(0):((($n==0)||((($n%100)>0)&&(($n%100)<20)))?(1):2))','https://ro.warmshowers.org','',9,'6180d9fe2be14466649e757858478c41'),('ru','Russian','Русский',0,1,3,'(((($n%10)==1)&&(($n%100)!=11))?(0):((((($n%10)>=2)&&(($n%10)<=4))&&((($n%100)<10)||(($n%100)>=20)))?(1):2))','https://ru.warmshowers.org','',9,'15e0c7d1a589ff4ce89b35b879e9af03'),('sr','Serbian','Српски',0,1,3,'(((($n%10)==1)&&(($n%100)!=11))?(0):((((($n%10)>=2)&&(($n%10)<=4))&&((($n%100)<10)||(($n%100)>=20)))?(1):2))','https://rs.warmshowers.org','',9,'0f6583f999c1e557a90150f4e394b2ef'),('sv','Swedish','Svenska',0,1,2,'($n!=1)','https://sv.warmshowers.org','',8,'0388c33ba79fab7c0870f856fa4fcd54'),('tr','Turkish','Türkçe',0,1,0,'','https://tr.warmshowers.org','',10,'a0b14fcaf2dadadb703d17d2604a41fb'),('zh-hans','Chinese, Simplified','简体中文',0,1,2,'($n!=1)','https://cn.warmshowers.org','',8,'483fe9062b8bc96f70aa06c968407d4a');
/*!40000 ALTER TABLE `languages` ENABLE KEYS */;
UNLOCK TABLES;
commit;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-12-27 14:49:23
