CREATE DATABASE IF NOT EXISTS realology DEFAULT CHARSET utf8 COLLATE utf8_bin;
USE realology;
SET NAMES utf8;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for system_config
-- ----------------------------
CREATE TABLE IF NOT EXISTS  `system_config`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `title` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL UNIQUE,
  `config_json` json NULL,
  `updated_time` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 7 CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of system_config
-- ----------------------------
INSERT INTO `system_config` VALUES (1, 'Calculation', '{\"calcmodel\":\"AUTO\",\"trend_ratio\": \"0.1\", \"fitting_round\": \"2\", \"fitting_target\": \"0.99\", \"flow_fluctuation\": \"90.0\", \"viscosity_change\": \"10\", \"confirmation_round\": \"3\"}', '2022/11/20 23:36:37');
INSERT INTO `system_config` VALUES (2, 'Communication', '{\"device_ip\": \"192.167.10.12\",\"wits_ip\": \"127.0.0.1\", \"alarm_ip\": \"realology-alarm-api.cloudipc.com\", \"wits_port\": \"6789\", \"alarm_port\": \"33201\", \"alarm_interval\": \"10000\"}', '2022/11/20 23:36:37');
INSERT INTO `system_config` VALUES (3, 'MainPageConfig', '{\"chart_time_rmin\": \"0\",\"chart_time_rmax\": \"50\",\"chart_time_lmin\": \"0\",\"chart_time_lmax\": \"50\",\"cl_alert_up\": \"0\", \"es_alert_up\": \"0\", \"ph_alert_up\": \"0\", \"pv_alert_up\": \"0\", \"yp_alert_up\": \"0\", \"cl_alert_down\": \"0\", \"es_alert_down\": \"0\", \"khb_alert_up\": \"0\", \"nhb_alert_up\": \"0\", \"ph_alert_down\": \"0\", \"kpl_alert_up\": \"0\", \"npl_alert_up\": \"0\", \"pv_alert_down\": \"0\", \"temp_alert_up\": \"0\", \"yp_alert_down\": \"0\", \"khb_alert_down\": \"0\", \"nhb_alert_down\": \"0\", \"kpl_alert_down\": \"0\", \"npl_alert_down\": \"0\", \"temp_alert_down\": \"0\", \"density_alert_up\": \"0\", \"density_alert_down\": \"0\"}', '2022/11/20 23:36:37');
INSERT INTO `system_config` VALUES (4, 'Other', '{\"clean_interval\": \"24\", \"clean_times\": \"2\", \"enable_cyclic_clean\": \"0\"}', '2022/11/20 23:36:37');
INSERT INTO `system_config` VALUES (5, 'Parameter', '{\"flowmeter_choose\":\"CDKSD\",\"fr\": \"0.015\", \"lt\": \"0.000\", \"ev_set\": \"0.000\", \"ev0_flow\": \"0.35\", \"evn_flow\": \"0.85\", \"evn_press\": \"0.6\", \"set_ev_v0\": \"0\"}', '2022/11/20 23:36:37');
INSERT INTO `system_config` VALUES (6, 'Runtime', '{\"isExit\" : \"1\",\"isTest\" : \"0\",\"language\" : \"English\",\"saveFilePath\" : \"\",\"unit\" : \"SI\",\"userName\" : \"\",\"wellName\" : \"\"}','2022/11/20 23:36:37');

-- ----------------------------
-- Table structure for system_op_logs
-- ----------------------------
CREATE TABLE IF NOT EXISTS `system_operation_logs`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `time` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `username` varchar(10) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `event` varchar(50) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = DYNAMIC;


-- ----------------------------
-- Table structure for system_options
-- ----------------------------

CREATE TABLE IF NOT EXISTS `system_options`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `well_name` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL UNIQUE,
  `date` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  `location` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin NULL DEFAULT NULL,
  `field` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin NULL DEFAULT NULL,
  `operator` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin NULL DEFAULT NULL,
  `rig_constractor` varchar(255) CHARACTER SET utf8 COLLATE utf8_bin NULL DEFAULT NULL,
  `updated_time` varchar(20) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Table structure for system_user
-- ----------------------------
CREATE TABLE IF NOT EXISTS `system_user`  (
  `id` int UNSIGNED NOT NULL AUTO_INCREMENT,
  `username` varchar(10) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL UNIQUE,
  `password` varchar(64) CHARACTER SET utf8 COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 3 CHARACTER SET = utf8 COLLATE = utf8_bin ROW_FORMAT = DYNAMIC;

-- ----------------------------
-- Records of system_user
-- ----------------------------
INSERT INTO `system_user` VALUES (1, 'engineer', 'EGBzURZX');
INSERT INTO `system_user` VALUES (2, 'admin', 'QDYtDE0=');

SET FOREIGN_KEY_CHECKS = 1;
