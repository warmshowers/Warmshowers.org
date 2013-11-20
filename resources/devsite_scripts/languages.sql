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
INSERT INTO `languages` VALUES ('cs','Czech','Čeština',0,0,3,'(((($n%10)==1)&&(($n%100)!=11))?(0):((((($n%10)>=2)&&(($n%10)<=4))&&((($n%100)<10)||(($n%100)>=20)))?(1):2))','https://cs.warmshowers.org','',7,''),('de','German','Deutsch',0,1,2,'($n!=1)','https://de.warmshowers.org','',3,'af0ad6109adef9b7a6290f2dca86561f'),('en','English','English',0,0,0,'','','en',0,''),('en-working','English','English',0,1,2,'($n!=1)','https://www.warmshowers.org','',0,''),('es','Spanish','Español',0,1,2,'($n!=1)','https://es.warmshowers.org','',2,'ada4d3d7443ce6139831fc94f3382382'),('fa','Persian','فارسی',1,1,2,'($n!=1)','https://fa.warmshowers.org','',9,'c5f5c97d14b4cbd73db330b433b466de'),('fr','French','Français',0,1,2,'($n>1)','https://fr.warmshowers.org','',1,'123219c9e0b22d13414afbfb67057f37'),('it','Italian','Italiano',0,1,2,'($n!=1)','https://it.warmshowers.org','',4,'237aa0601be8343d62610d131bce70db'),('ja','Japanese','日本語',0,1,2,'($n!=1)','https://ja.warmshowers.org','',9,'f843333e8f6710eb145bdbc9d1efba1c'),('pl','Polish','Polski',0,1,3,'(($n==1)?(0):((((($n%10)>=2)&&(($n%10)<=4))&&((($n%100)<10)||(($n%100)>=20)))?(1):2))','https://pl.warmshowers.org','',7,'c09075b7e75f4eb8c33db980ab5c3f7b'),('pt-br','Portuguese','Português',0,1,2,'($n!=1)','https://pt.warmshowers.org','',5,'fa84ca25f456723e459cf67b534541b0'),('ro','Romanian','Română',0,1,3,'(($n==1)?(0):((($n==0)||((($n%100)>0)&&(($n%100)<20)))?(1):2))','https://ro.warmshowers.org','',9,'378df767317dd887672f8b30f9f83b44'),('ru','Russian','Русский',0,1,3,'(((($n%10)==1)&&(($n%100)!=11))?(0):((((($n%10)>=2)&&(($n%10)<=4))&&((($n%100)<10)||(($n%100)>=20)))?(1):2))','https://ru.warmshowers.org','',9,'0a1111b33460f18f2eca440d62e3030b'),('sr','Serbian','Српски',0,1,3,'(((($n%10)==1)&&(($n%100)!=11))?(0):((((($n%10)>=2)&&(($n%10)<=4))&&((($n%100)<10)||(($n%100)>=20)))?(1):2))','https://rs.warmshowers.org','',9,'2eaaf7a474674228abd0bc8f3d16865d'),('tr','Turkish','Türkçe',0,1,0,'','https://tr.warmshowers.org','',10,'bf5ba802fb556cfa3a462f65bbc695e8'),('zh-hans','Chinese, Simplified','简体中文',0,1,2,'($n!=1)','https://cn.warmshowers.org','',8,'3c4534e7fb043a8e2601b34303acbeb5');
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

-- Dump completed on 2013-10-05 14:34:18
