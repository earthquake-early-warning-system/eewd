-- phpMyAdmin SQL Dump
-- version 4.8.5
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: Jul 22, 2019 at 12:28 PM
-- Server version: 10.3.16-MariaDB
-- PHP Version: 7.3.2

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+05:30";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `id10062120_devices`
--

-- --------------------------------------------------------

--
-- Table structure for table `device1`
--

CREATE TABLE `device1` (
  `data_id` bigint(20) UNSIGNED NOT NULL,
  `sr` int(11) NOT NULL,
  `dt` text NOT NULL,
  `time` datetime NOT NULL DEFAULT current_timestamp(),
  `uptm` int(11) NOT NULL,
  `temp_filter` float NOT NULL,
  `temp_raw` float NOT NULL,
  `curr_filter` float NOT NULL,
  `curr_raw` float NOT NULL,
  `accel_filter` float NOT NULL,
  `accel_raw` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

--
-- Dumping data for table `device1`
--

INSERT INTO `device1` (`data_id`, `sr`, `dt`, `time`, `uptm`, `temp_filter`, `temp_raw`, `curr_filter`, `curr_raw`, `accel_filter`, `accel_raw`) VALUES
(1, 8, '80110,1561297451415', '2019-06-23 19:14:11', 80110, 34.65, 34.93, 0.1, 0.07, 0.02, 0.02);

-- --------------------------------------------------------

--
-- Table structure for table `device2`
--

CREATE TABLE `device2` (
  `data_id` bigint(20) UNSIGNED NOT NULL,
  `sr` int(11) NOT NULL,
  `dt` text NOT NULL,
  `time` datetime NOT NULL DEFAULT current_timestamp(),
  `uptm` int(11) NOT NULL,
  `temp_filter` float NOT NULL,
  `temp_raw` float NOT NULL,
  `curr_filter` float NOT NULL,
  `curr_raw` float NOT NULL,
  `accel_filter` float NOT NULL,
  `accel_raw` float NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1 ROW_FORMAT=COMPACT;

-- --------------------------------------------------------

--
-- Table structure for table `devices_incidents`
--

CREATE TABLE `devices_incidents` (
  `incident_id` bigint(20) NOT NULL,
  `device_id` int(11) NOT NULL,
  `device_mac_id` int(11) DEFAULT NULL,
  `device_mac_id_str` int(11) DEFAULT NULL,
  `device_code_type` text COLLATE utf8_unicode_ci NOT NULL,
  `device_code_version` text COLLATE utf8_unicode_ci NOT NULL,
  `incident_entry_time` timestamp NOT NULL DEFAULT current_timestamp(),
  `incident_type` text COLLATE utf8_unicode_ci NOT NULL,
  `incident_magnitude` float NOT NULL,
  `device_location` text COLLATE utf8_unicode_ci DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- --------------------------------------------------------

--
-- Table structure for table `device_config`
--

CREATE TABLE `device_config` (
  `config_id` int(11) NOT NULL,
  `whether_update_available` bit(1) NOT NULL,
  `device_code_to_update_to` text COLLATE utf8_unicode_ci NOT NULL,
  `device_code_type` text COLLATE utf8_unicode_ci NOT NULL,
  `server_host_address_data` text COLLATE utf8_unicode_ci NOT NULL,
  `server_host_port_data` int(11) NOT NULL,
  `host_server_query_path` text COLLATE utf8_unicode_ci NOT NULL,
  `server_host_address_config` text COLLATE utf8_unicode_ci NOT NULL,
  `server_host_port_config` text COLLATE utf8_unicode_ci NOT NULL,
  `host_config_server_query_path` text COLLATE utf8_unicode_ci NOT NULL,
  `device_min_code_version` text COLLATE utf8_unicode_ci NOT NULL,
  `device_max_code_version` text COLLATE utf8_unicode_ci NOT NULL,
  `available_sensor_count` int(11) NOT NULL,
  `sensor_current_enabled` tinyint(1) NOT NULL,
  `sensor_temperature_enabled` tinyint(1) NOT NULL,
  `sensor_vibration_enabled` tinyint(1) NOT NULL,
  `sensor_current_threshold_normal` float NOT NULL,
  `sensor_current_threshold_alert` float NOT NULL,
  `sensor_current_threshold_warning` float NOT NULL,
  `sensor_current_threshold_critical` float NOT NULL,
  `sensor_vibration_threshold_normal` float NOT NULL,
  `sensor_vibration_threshold_alert` float NOT NULL,
  `sensor_vibration_threshold_warning` float NOT NULL,
  `sensor_vibration_threshold_critical` float NOT NULL,
  `sensor_temperature_threshold_normal` float NOT NULL,
  `sensor_temperature_threshold_alert` float NOT NULL,
  `sensor_temperature_threshold_warning` float NOT NULL,
  `sensor_temperature_threshold_critical` float NOT NULL,
  `sensor_current_sample_time_duration` int(11) NOT NULL,
  `sensor_vibration_sample_time_duration` int(11) NOT NULL,
  `sensor_temperature_sample_time_duration` int(11) NOT NULL,
  `sensor_current_total_time_duration` int(11) NOT NULL,
  `sensor_vibration_total_time_duration` int(11) NOT NULL,
  `sensor_temperature_total_time_duration` int(11) NOT NULL,
  `sensor_current_buzzer_light_notify_level` bit(8) NOT NULL,
  `sensor_temperature_buzzer_light_notify_level` bit(8) NOT NULL,
  `sensor_vibration_buzzer_light_notify_level` bit(8) NOT NULL,
  `development_print_level` bit(64) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `device_config`
--

INSERT INTO `device_config` (`config_id`, `whether_update_available`, `device_code_to_update_to`, `device_code_type`, `server_host_address_data`, `server_host_port_data`, `host_server_query_path`, `server_host_address_config`, `server_host_port_config`, `host_config_server_query_path`, `device_min_code_version`, `device_max_code_version`, `available_sensor_count`, `sensor_current_enabled`, `sensor_temperature_enabled`, `sensor_vibration_enabled`, `sensor_current_threshold_normal`, `sensor_current_threshold_alert`, `sensor_current_threshold_warning`, `sensor_current_threshold_critical`, `sensor_vibration_threshold_normal`, `sensor_vibration_threshold_alert`, `sensor_vibration_threshold_warning`, `sensor_vibration_threshold_critical`, `sensor_temperature_threshold_normal`, `sensor_temperature_threshold_alert`, `sensor_temperature_threshold_warning`, `sensor_temperature_threshold_critical`, `sensor_current_sample_time_duration`, `sensor_vibration_sample_time_duration`, `sensor_temperature_sample_time_duration`, `sensor_current_total_time_duration`, `sensor_vibration_total_time_duration`, `sensor_temperature_total_time_duration`, `sensor_current_buzzer_light_notify_level`, `sensor_temperature_buzzer_light_notify_level`, `sensor_vibration_buzzer_light_notify_level`, `development_print_level`) VALUES
(0, b'0', 'v0.0.2-60-ge1482d5', 'DEVT', 'device1-eews.000webhostapp.com', 80, '/devices/insert_new_data.php?', 'device1-eews.000webhostapp.com', '80', '/devices/get_config.php?', '0.0.3-3', '0.0.3-5', 3, 0, 1, 1, 1, 8, 16, 20, 0.1, 1, 3, 5, 25, 35, 40, 55, 400000, 80000, 0, 1000, 3300, 0, b'11111111', b'11111111', b'11111111', b'0000000000000000000000000000000000000000000000000000000000000000'),
(1, b'0', 'v0.0.2-61-ga043ba3', 'DEVT', ' na ', 80, ' na ', ' na ', '80', ' na ', ' na ', ' na ', 3, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 54354, 435435435, 5435435, 4354, 3545, 35435, b'00001111', b'00000111', b'00001111', b'0000000000000000000000000000000000000000000000000000000000000001'),
(2, b'0', 'v0.0.2-61-ga043ba3', 'DEVT', 'device1-eews.000webhostapp.com', 80, '/devices/insert_new_data.php?', 'device1-eews.000webhostapp.com', '80', '/devices/get_config.php?', '0.0.3-3', '0.0.3-5', 3, 0, 1, 1, 1, 8, 16, 20, 0.1, 1, 3, 5, 25, 35, 40, 55, 400000, 80000, 0, 1000, 3300, 0, b'11111111', b'11111111', b'11111111', b'0000000000000000000000000000000000000000000000000000000000000000'),
(3, b'0', 'v0.0.2-61-ga043ba3', 'DEVT', 'device1-eews.000webhostapp.com', 80, '/devices/insert_new_data.php?', 'device1-eews.000webhostapp.com', '80', '/devices/get_config.php?', '0.0.3-3', '0.0.3-5', 3, 0, 1, 1, 1, 8, 16, 20, 0.1, 1, 3, 5, 25, 35, 40, 55, 400000, 80000, 0, 1000, 3300, 0, b'11111111', b'11111111', b'11111111', b'0000000000000000000000000000000000000000000000000000000000000000');

-- --------------------------------------------------------

--
-- Table structure for table `device_info`
--

CREATE TABLE `device_info` (
  `Device_id` int(11) NOT NULL,
  `Device_config_id` int(11) DEFAULT NULL,
  `Device_mac_id` int(11) DEFAULT NULL,
  `Device_mac_id_str` text COLLATE utf8_unicode_ci NOT NULL,
  `Device_code_type` text COLLATE utf8_unicode_ci NOT NULL,
  `Device_code_version` text COLLATE utf8_unicode_ci DEFAULT NULL,
  `Device_location_gps` text COLLATE utf8_unicode_ci NOT NULL,
  `Device_code_update_datetime` datetime DEFAULT NULL,
  `This_info_entry_datetimestamp` timestamp NULL DEFAULT current_timestamp()
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

--
-- Dumping data for table `device_info`
--

INSERT INTO `device_info` (`Device_id`, `Device_config_id`, `Device_mac_id`, `Device_mac_id_str`, `Device_code_type`, `Device_code_version`, `Device_location_gps`, `Device_code_update_datetime`, `This_info_entry_datetimestamp`) VALUES
(1, 0, NULL, '2C3AE806C203', 'DEVT', 'v0.0.2-60-ge1482d5', '28.4070418,76.9641391', NULL, '2019-07-18 14:22:50'),
(2, 0, NULL, '0', 'DEVT', NULL, '28.686273800000002,77.2217831', NULL, '2019-07-18 14:23:23');

--
-- Indexes for dumped tables
--

--
-- Indexes for table `device1`
--
ALTER TABLE `device1`
  ADD UNIQUE KEY `data_id` (`data_id`);

--
-- Indexes for table `device2`
--
ALTER TABLE `device2`
  ADD UNIQUE KEY `data_id` (`data_id`);

--
-- Indexes for table `devices_incidents`
--
ALTER TABLE `devices_incidents`
  ADD UNIQUE KEY `incident_id` (`incident_id`);

--
-- Indexes for table `device_config`
--
ALTER TABLE `device_config`
  ADD UNIQUE KEY `config_id` (`config_id`);

--
-- Indexes for table `device_info`
--
ALTER TABLE `device_info`
  ADD UNIQUE KEY `Device_id` (`Device_id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `device1`
--
ALTER TABLE `device1`
  MODIFY `data_id` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=43184;

--
-- AUTO_INCREMENT for table `device2`
--
ALTER TABLE `device2`
  MODIFY `data_id` bigint(20) UNSIGNED NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `devices_incidents`
--
ALTER TABLE `devices_incidents`
  MODIFY `incident_id` bigint(20) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `device_info`
--
ALTER TABLE `device_info`
  MODIFY `Device_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
