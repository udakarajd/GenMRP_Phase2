-- MySQL dump 10.13  Distrib 5.5.41, for debian-linux-gnu (i686)
--
-- Host: localhost    Database: GenMRP
-- ------------------------------------------------------
-- Server version	5.5.41-0+wheezy1

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
-- Table structure for table `bom`
--

DROP TABLE IF EXISTS `bom`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `bom` (
  `compid` varchar(40) NOT NULL,
  `partid` varchar(40) NOT NULL,
  `childpartid` varchar(40) NOT NULL,
  `quantity` int(11) DEFAULT NULL,
  PRIMARY KEY (`compid`,`partid`,`childpartid`),
  KEY `partid` (`partid`),
  CONSTRAINT `bom_ibfk_1` FOREIGN KEY (`compid`) REFERENCES `company` (`compid`),
  CONSTRAINT `bom_ibfk_2` FOREIGN KEY (`partid`) REFERENCES `part` (`partid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `bom`
--

LOCK TABLES `bom` WRITE;
/*!40000 ALTER TABLE `bom` DISABLE KEYS */;
INSERT INTO `bom` VALUES ('IFS','A','C',3),('IFS','B','C',2),('IFS','B','D',5),('IFS','X','A',2),('IFS','X','B',1);
/*!40000 ALTER TABLE `bom` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `company`
--

DROP TABLE IF EXISTS `company`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `company` (
  `compid` varchar(40) NOT NULL,
  `name` varchar(200) NOT NULL,
  `password` varchar(20) NOT NULL,
  `description` varchar(400) DEFAULT NULL,
  PRIMARY KEY (`compid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `company`
--

LOCK TABLES `company` WRITE;
/*!40000 ALTER TABLE `company` DISABLE KEYS */;
INSERT INTO `company` VALUES ('IFS','IFS','IFS','IFS is a ERP company');
/*!40000 ALTER TABLE `company` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `costfunction`
--

DROP TABLE IF EXISTS `costfunction`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `costfunction` (
  `compid` varchar(40) NOT NULL,
  `partid` varchar(40) NOT NULL,
  `supplierid` varchar(40) NOT NULL,
  `funcid` varchar(40) NOT NULL,
  `functxt` varchar(400) NOT NULL,
  PRIMARY KEY (`compid`,`partid`,`supplierid`,`funcid`),
  KEY `partid` (`partid`),
  CONSTRAINT `costfunction_ibfk_1` FOREIGN KEY (`partid`) REFERENCES `part` (`partid`),
  CONSTRAINT `costfunction_ibfk_2` FOREIGN KEY (`supplierid`) REFERENCES `supplier` (`supplierid`),
  CONSTRAINT `costfunction_ibfk_3` FOREIGN KEY (`compid`) REFERENCES `part` (`compid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `costfunction`
--

LOCK TABLES `costfunction` WRITE;
/*!40000 ALTER TABLE `costfunction` DISABLE KEYS */;
INSERT INTO `costfunction` VALUES ('IFS','A','X','HandlingCost','((Q-(Q-(Q/5)*5))/5) *5 +( (Q-(Q/5)*5)>0?5:0)'),('IFS','A','X','ProCost','((Q-(Q-(Q/20)*20))/20) *200 +  ((Q-(Q/20)*20) * 20)'),('IFS','B','X','ProcCost','((Q-(Q-(Q/50)*50))/50) *150 + ((Q-(Q/50)*50) * 20)'),('IFS','C','X','ProCost','((Q-(Q-(Q/60)*60))/60) *200 + ((Q-(Q/60)*60) * 10)'),('IFS','D','X','ProCost','((Q-(Q-(Q/20)*20))/20) *200 + ((Q-(Q/20)*20) * 50)'),('IFS','X','X','MakeCost','Q * 30');
/*!40000 ALTER TABLE `costfunction` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `genengine`
--

DROP TABLE IF EXISTS `genengine`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `genengine` (
  `compid` varchar(40) NOT NULL,
  `poolsize` int(11) DEFAULT '10',
  `iterations` int(11) DEFAULT '10',
  `maxorder` int(11) DEFAULT '200',
  `workers` int(11) DEFAULT '4',
  `reports` varchar(400) DEFAULT '.',
  KEY `compid` (`compid`),
  CONSTRAINT `genengine_ibfk_1` FOREIGN KEY (`compid`) REFERENCES `company` (`compid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `genengine`
--

LOCK TABLES `genengine` WRITE;
/*!40000 ALTER TABLE `genengine` DISABLE KEYS */;
INSERT INTO `genengine` VALUES ('IFS',50,80,300,2,'~/Projects/');
/*!40000 ALTER TABLE `genengine` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary table structure for view `orders`
--

DROP TABLE IF EXISTS `orders`;
/*!50001 DROP VIEW IF EXISTS `orders`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `orders` (
  `compid` tinyint NOT NULL,
  `partid` tinyint NOT NULL,
  `orderid` tinyint NOT NULL,
  `scheduleid` tinyint NOT NULL,
  `days` tinyint NOT NULL,
  `quantity` tinyint NOT NULL
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `ordersnapshot`
--

DROP TABLE IF EXISTS `ordersnapshot`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ordersnapshot` (
  `compid` varchar(40) NOT NULL,
  `partid` varchar(40) NOT NULL,
  `orderid` int(11) NOT NULL,
  `scheduleid` int(11) NOT NULL,
  `days` int(11) DEFAULT NULL,
  `quantity` int(11) DEFAULT NULL,
  `pordays` int(11) DEFAULT NULL,
  `porquantity` int(11) DEFAULT NULL,
  PRIMARY KEY (`orderid`,`scheduleid`),
  KEY `partid` (`partid`),
  KEY `compid` (`compid`),
  CONSTRAINT `ordersnapshot_ibfk_1` FOREIGN KEY (`partid`) REFERENCES `part` (`partid`),
  CONSTRAINT `ordersnapshot_ibfk_2` FOREIGN KEY (`compid`) REFERENCES `company` (`compid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ordersnapshot`
--

LOCK TABLES `ordersnapshot` WRITE;
/*!40000 ALTER TABLE `ordersnapshot` DISABLE KEYS */;
INSERT INTO `ordersnapshot` VALUES ('IFS','X',10,10,10,95,NULL,NULL),('IFS','A',20,10,9,12,NULL,NULL),('IFS','B',30,10,7,7,NULL,NULL),('IFS','C',40,10,8,10,NULL,NULL),('IFS','D',50,10,6,15,NULL,NULL);
/*!40000 ALTER TABLE `ordersnapshot` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `part`
--

DROP TABLE IF EXISTS `part`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `part` (
  `compid` varchar(40) NOT NULL,
  `partid` varchar(40) NOT NULL,
  `partname` varchar(255) NOT NULL,
  `leadtime` int(11) DEFAULT NULL,
  `onhand` int(11) DEFAULT '0',
  PRIMARY KEY (`partid`),
  KEY `compid` (`compid`),
  CONSTRAINT `part_ibfk_1` FOREIGN KEY (`compid`) REFERENCES `company` (`compid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `part`
--

LOCK TABLES `part` WRITE;
/*!40000 ALTER TABLE `part` DISABLE KEYS */;
INSERT INTO `part` VALUES ('IFS','A','A',3,75),('IFS','B','B',1,25),('IFS','C','C',2,10),('IFS','D','D',2,20),('IFS','X','X',2,50);
/*!40000 ALTER TABLE `part` ENABLE KEYS */;
UNLOCK TABLES;




DROP TABLE IF EXISTS `supplier`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `supplier` (
  `compid` varchar(40) NOT NULL,
  `supplierid` varchar(40) NOT NULL,
  `suppliername` varchar(255) NOT NULL,
  PRIMARY KEY (`supplierid`),
  KEY `compid` (`compid`),
  CONSTRAINT `supplier_ibfk_1` FOREIGN KEY (`compid`) REFERENCES `company` (`compid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `part`
--

LOCK TABLES `supplier` WRITE;
/*!40000 ALTER TABLE `supplier` DISABLE KEYS */;
INSERT INTO `supplier` VALUES ('IFS','X','tns'),('IFS','Y','pns'),('IFS','Z','atleas');
/*!40000 ALTER TABLE `supplier` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Final view structure for view `orders`
--



DROP TABLE IF EXISTS `discounts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `discounts` (
  `compid` varchar(40) NOT NULL,
  `supplierid` varchar(40) NOT NULL,
  `partid` varchar(255) NOT NULL,
  `quantity` int(11) NOT NULL,
  `extrapartid` varchar(255) NOT NULL,
  `discountpresent` int(11) DEFAULT NULL,
  PRIMARY KEY (`supplierid`,partid,`quantity`,`extrapartid`),
  KEY `compid` (`compid`),
  CONSTRAINT `discounts_ibfk_1` FOREIGN KEY (`compid`) REFERENCES `company` (`compid`),
  CONSTRAINT `discounts_ibfk_2` FOREIGN KEY (`supplierid`) REFERENCES `supplier` (`supplierid`),
  CONSTRAINT `discounts_ibfk_3` FOREIGN KEY (`partid`) REFERENCES `part` (`partid`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `part`
--

LOCK TABLES `discounts` WRITE;
/*!40000 ALTER TABLE `discounts` DISABLE KEYS */;
INSERT INTO `discounts` VALUES ('IFS','X','A',15,'A',10),('IFS','X','B',15,'C',2),('IFS','X','B',15,'D',3),('IFS','Y','A',15,'C',3),('IFS','Y','B',20,'B',15),('IFS','Y','D',100,'D',20),('IFS','Z','A',20,'A',8),('IFS','Z','C',25,'C',15),('IFS','Z','D',125,'D',25);
/*!40000 ALTER TABLE `discounts` ENABLE KEYS */;
UNLOCK TABLES;





/*!50001 DROP TABLE IF EXISTS `orders`*/;
/*!50001 DROP VIEW IF EXISTS `orders`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `orders` AS select `ordersnapshot`.`compid` AS `compid`,`ordersnapshot`.`partid` AS `partid`,`ordersnapshot`.`orderid` AS `orderid`,`ordersnapshot`.`scheduleid` AS `scheduleid`,`ordersnapshot`.`days` AS `days`,`ordersnapshot`.`quantity` AS `quantity` from `ordersnapshot` order by `ordersnapshot`.`days` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-04-15 22:44:12
