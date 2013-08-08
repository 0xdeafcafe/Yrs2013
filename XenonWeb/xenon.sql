-- phpMyAdmin SQL Dump
-- version 3.5.8
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Aug 08, 2013 at 02:28 PM
-- Server version: 5.5.32-cll
-- PHP Version: 5.3.17

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `arborvi1_cw`
--

-- --------------------------------------------------------

--
-- Table structure for table `xenon`
--

CREATE TABLE IF NOT EXISTS `xenon` (
  `citiesid` int(11) NOT NULL AUTO_INCREMENT,
  `city_a` varchar(100) NOT NULL,
  `city_b` varchar(100) NOT NULL,
  `victor` varchar(100) NOT NULL,
  `raw` longtext NOT NULL,
  PRIMARY KEY (`citiesid`)
) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=5 ;

--
-- Dumping data for table `xenon`
--

INSERT INTO `xenon` (`citiesid`, `city_a`, `city_b`, `victor`, `raw`) VALUES
(1, 'London', 'New York', 'London', 'Data Point A: 39 Data Point B: 29'),
(2, 'Seoul', 'Pyongyang', 'Seoul', 'Data Point A: 28 Data Point B: 10'),
(3, 'London', 'New York', 'New York', 'Data Point A: 12\r\nData Point B: 83'),
(4, 'London', 'New York', 'London', 'Data Point A: 53\r\nData Point B: -2');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
