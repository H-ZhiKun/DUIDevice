//stamp:2c63052dfc752d0c
/*<------------------------------------------------------------------------------------------------->*/
/*该文件由uiresbuilder生成，请不要手动修改*/
/*<------------------------------------------------------------------------------------------------->*/
#pragma once
#include <res.mgr/snamedvalue.h>
#define ROBJ_IN_CPP \
namespace SOUI \
{\
    const _R R;\
    const _UIRES UIRES;\
}
namespace SOUI
{
	class _UIRES{
		public:
		class _UIDEF{
			public:
			_UIDEF(){
				XML_INIT = _T("UIDEF:XML_INIT");
			}
			const TCHAR * XML_INIT;
		}UIDEF;
		class _values{
			public:
			_values(){
				string = _T("values:string");
				color = _T("values:color");
				skin = _T("values:skin");
			}
			const TCHAR * string;
			const TCHAR * color;
			const TCHAR * skin;
		}values;
		class _LAYOUT{
			public:
			_LAYOUT(){
				xml_main = _T("LAYOUT:xml_main");
				xml_diag = _T("LAYOUT:xml_diag");
				xml_fluid_info = _T("LAYOUT:xml_fluid_info");
				xml_system_options = _T("LAYOUT:xml_system_options");
				xml_login = _T("LAYOUT:xml_login");
				xml_system_menu = _T("LAYOUT:xml_system_menu");
				xml_home = _T("LAYOUT:xml_home");
				xml_home_type = _T("LAYOUT:xml_home_type");
				xml_home_real = _T("LAYOUT:xml_home_real");
				xml_keyboard = _T("LAYOUT:xml_keyboard");
				xml_logs = _T("LAYOUT:xml_logs");
				xml_logs_datalogs = _T("LAYOUT:xml_logs_datalogs");
				xml_logs_oplogs = _T("LAYOUT:xml_logs_oplogs");
				xml_advance = _T("LAYOUT:xml_advance");
				xml_advance_debug = _T("LAYOUT:xml_advance_debug");
				xml_advance_init = _T("LAYOUT:xml_advance_init");
				xml_logs_datalogs_calcdata = _T("LAYOUT:xml_logs_datalogs_calcdata");
				xml_logs_datalogs_wbm = _T("LAYOUT:xml_logs_datalogs_wbm");
				xml_logs_datalogs_obm = _T("LAYOUT:xml_logs_datalogs_obm");
				xml_message = _T("LAYOUT:xml_message");
				xml_restore = _T("LAYOUT:xml_restore");
			}
			const TCHAR * xml_main;
			const TCHAR * xml_diag;
			const TCHAR * xml_fluid_info;
			const TCHAR * xml_system_options;
			const TCHAR * xml_login;
			const TCHAR * xml_system_menu;
			const TCHAR * xml_home;
			const TCHAR * xml_home_type;
			const TCHAR * xml_home_real;
			const TCHAR * xml_keyboard;
			const TCHAR * xml_logs;
			const TCHAR * xml_logs_datalogs;
			const TCHAR * xml_logs_oplogs;
			const TCHAR * xml_advance;
			const TCHAR * xml_advance_debug;
			const TCHAR * xml_advance_init;
			const TCHAR * xml_logs_datalogs_calcdata;
			const TCHAR * xml_logs_datalogs_wbm;
			const TCHAR * xml_logs_datalogs_obm;
			const TCHAR * xml_message;
			const TCHAR * xml_restore;
		}LAYOUT;
		class _IMG{
			public:
			_IMG(){
				system_logo = _T("IMG:system.logo");
				system_border = _T("IMG:system.border");
				system_drop = _T("IMG:system.drop");
				system_snapshot = _T("IMG:system.snapshot");
				system_setup = _T("IMG:system.setup");
				system_menu = _T("IMG:system.menu");
				system_bg_menu = _T("IMG:system.bg_menu");
				system_menu_item = _T("IMG:system.menu_item");
				system_home_page = _T("IMG:system.home_page");
				system_end_page = _T("IMG:system.end_page");
				system_prev_page = _T("IMG:system.prev_page");
				system_next_page = _T("IMG:system.next_page");
				system_chk_left = _T("IMG:system.chk_left");
				system_chk_right = _T("IMG:system.chk_right");
				system_block = _T("IMG:system.block");
				system_search = _T("IMG:system.search");
				system_close = _T("IMG:system.close");
				system_ok = _T("IMG:system.ok");
				system_bg_message = _T("IMG:system.bg_message");
				system_warning = _T("IMG:system.warning");
				system_chart_item_120_20 = _T("IMG:system.chart_item_120_20");
				system_chart_item_110_30 = _T("IMG:system.chart_item_110_30");
				login_logo = _T("IMG:login.logo");
				login_bg = _T("IMG:login.bg");
				login_img_user = _T("IMG:login.img_user");
				login_img_password = _T("IMG:login.img_password");
				login_btn_close = _T("IMG:login.btn_close");
				login_btn_login = _T("IMG:login.btn_login");
				home_bg_type = _T("IMG:home.bg_type");
				home_block0 = _T("IMG:home.block0");
				home_block1 = _T("IMG:home.block1");
				home_img_real = _T("IMG:home.img_real");
				home_alert_up = _T("IMG:home.alert_up");
				home_alert_down = _T("IMG:home.alert_down");
				home_btn_units = _T("IMG:home.btn_units");
				home_btn_start = _T("IMG:home.btn_start");
				home_btn_stop = _T("IMG:home.btn_stop");
				home_btn_clean = _T("IMG:home.btn_clean");
				keyboard_bg = _T("IMG:keyboard.bg");
				keyboard_backup = _T("IMG:keyboard.backup");
				keyboard_enter = _T("IMG:keyboard.enter");
				keyboard_close = _T("IMG:keyboard.close");
				keyboard_key = _T("IMG:keyboard.key");
				logs_export = _T("IMG:logs.export");
				logs_next_group = _T("IMG:logs.next_group");
				logs_prev_group = _T("IMG:logs.prev_group");
				logs_btn41 = _T("IMG:logs.btn41");
				logs_btn51 = _T("IMG:logs.btn51");
				logs_btn61 = _T("IMG:logs.btn61");
				logs_btnh44 = _T("IMG:logs.btnh44");
				advance_pump = _T("IMG:advance.pump");
				advance_model = _T("IMG:advance.model");
				advance_block_item = _T("IMG:advance.block_item");
				advance_bg_item = _T("IMG:advance.bg_item");
				diag_bg = _T("IMG:diag.bg");
				diag_data_bg = _T("IMG:diag.data_bg");
				diag_lights = _T("IMG:diag.lights");
				diag_btn_code = _T("IMG:diag.btn_code");
				diag_btn_time = _T("IMG:diag.btn_time");
				diag_btn_trip = _T("IMG:diag.btn_trip");
				diag_check_alarm = _T("IMG:diag.check_alarm");
				diag_check_comm = _T("IMG:diag.check_comm");
				diag_check_hardware = _T("IMG:diag.check_hardware");
				diag_check_warning = _T("IMG:diag.check_warning");
				fluid_setup_bg = _T("IMG:fluid_setup.bg");
				fluid_setup_bg_combo97 = _T("IMG:fluid_setup.bg_combo97");
				fluid_setup_bg_combo238 = _T("IMG:fluid_setup.bg_combo238");
				fluid_setup_bg_edit134 = _T("IMG:fluid_setup.bg_edit134");
				fluid_setup_bg_edit238 = _T("IMG:fluid_setup.bg_edit238");
				system_options_bg = _T("IMG:system_options.bg");
				system_options_browser = _T("IMG:system_options.browser");
				system_options_delete = _T("IMG:system_options.delete");
				system_options_pump = _T("IMG:system_options.pump");
				system_options_combo208 = _T("IMG:system_options.combo208");
				system_options_edit208 = _T("IMG:system_options.edit208");
				system_options_new = _T("IMG:system_options.new");
			}
			const TCHAR * system_logo;
			const TCHAR * system_border;
			const TCHAR * system_drop;
			const TCHAR * system_snapshot;
			const TCHAR * system_setup;
			const TCHAR * system_menu;
			const TCHAR * system_bg_menu;
			const TCHAR * system_menu_item;
			const TCHAR * system_home_page;
			const TCHAR * system_end_page;
			const TCHAR * system_prev_page;
			const TCHAR * system_next_page;
			const TCHAR * system_chk_left;
			const TCHAR * system_chk_right;
			const TCHAR * system_block;
			const TCHAR * system_search;
			const TCHAR * system_close;
			const TCHAR * system_ok;
			const TCHAR * system_bg_message;
			const TCHAR * system_warning;
			const TCHAR * system_chart_item_120_20;
			const TCHAR * system_chart_item_110_30;
			const TCHAR * login_logo;
			const TCHAR * login_bg;
			const TCHAR * login_img_user;
			const TCHAR * login_img_password;
			const TCHAR * login_btn_close;
			const TCHAR * login_btn_login;
			const TCHAR * home_bg_type;
			const TCHAR * home_block0;
			const TCHAR * home_block1;
			const TCHAR * home_img_real;
			const TCHAR * home_alert_up;
			const TCHAR * home_alert_down;
			const TCHAR * home_btn_units;
			const TCHAR * home_btn_start;
			const TCHAR * home_btn_stop;
			const TCHAR * home_btn_clean;
			const TCHAR * keyboard_bg;
			const TCHAR * keyboard_backup;
			const TCHAR * keyboard_enter;
			const TCHAR * keyboard_close;
			const TCHAR * keyboard_key;
			const TCHAR * logs_export;
			const TCHAR * logs_next_group;
			const TCHAR * logs_prev_group;
			const TCHAR * logs_btn41;
			const TCHAR * logs_btn51;
			const TCHAR * logs_btn61;
			const TCHAR * logs_btnh44;
			const TCHAR * advance_pump;
			const TCHAR * advance_model;
			const TCHAR * advance_block_item;
			const TCHAR * advance_bg_item;
			const TCHAR * diag_bg;
			const TCHAR * diag_data_bg;
			const TCHAR * diag_lights;
			const TCHAR * diag_btn_code;
			const TCHAR * diag_btn_time;
			const TCHAR * diag_btn_trip;
			const TCHAR * diag_check_alarm;
			const TCHAR * diag_check_comm;
			const TCHAR * diag_check_hardware;
			const TCHAR * diag_check_warning;
			const TCHAR * fluid_setup_bg;
			const TCHAR * fluid_setup_bg_combo97;
			const TCHAR * fluid_setup_bg_combo238;
			const TCHAR * fluid_setup_bg_edit134;
			const TCHAR * fluid_setup_bg_edit238;
			const TCHAR * system_options_bg;
			const TCHAR * system_options_browser;
			const TCHAR * system_options_delete;
			const TCHAR * system_options_pump;
			const TCHAR * system_options_combo208;
			const TCHAR * system_options_edit208;
			const TCHAR * system_options_new;
		}IMG;
		class _ICON{
			public:
			_ICON(){
				system_appico = _T("ICON:system.appico");
			}
			const TCHAR * system_appico;
		}ICON;
		class _translator{
			public:
			_translator(){
				lang_cn = _T("translator:lang_cn");
			}
			const TCHAR * lang_cn;
		}translator;
	};
	const SNamedID::NAMEDVALUE namedXmlID[]={
		{L"Cl-_end",65851},
		{L"Cl-_home",65849},
		{L"Cl-_lvdata",65846},
		{L"Cl-_next",65852},
		{L"Cl-_page",65847},
		{L"Cl-_prev",65850},
		{L"Cl-_total",65848},
		{L"ES_end",65861},
		{L"ES_home",65859},
		{L"ES_lvdata",65856},
		{L"ES_next",65862},
		{L"ES_page",65857},
		{L"ES_prev",65860},
		{L"ES_total",65858},
		{L"PH_end",65841},
		{L"PH_home",65839},
		{L"PH_lvdata",65835},
		{L"PH_next",65842},
		{L"PH_page",65837},
		{L"PH_prev",65840},
		{L"PH_total",65838},
		{L"R0",65815},
		{L"R10",65812},
		{L"R100",65808},
		{L"R20",65811},
		{L"R200",65807},
		{L"R3",65814},
		{L"R30",65810},
		{L"R300",65806},
		{L"R6",65813},
		{L"R60",65809},
		{L"R600",65805},
		{L"_name_start",65535},
		{L"advance_chartreal",65710},
		{L"advance_chk_debug",65700},
		{L"advance_chk_init",65701},
		{L"advance_circulating_pump",65713},
		{L"advance_cooling_valve",65719},
		{L"advance_debug",65711},
		{L"advance_density",65736},
		{L"advance_dfp1",65731},
		{L"advance_dfp2",65732},
		{L"advance_ev",65734},
		{L"advance_ev0n",65735},
		{L"advance_log",65712},
		{L"advance_mflow",65739},
		{L"advance_mudlevel",65740},
		{L"advance_p1",65727},
		{L"advance_p2",65728},
		{L"advance_p3",65729},
		{L"advance_p4",65730},
		{L"advance_page",65601},
		{L"advance_purge_valve",65715},
		{L"advance_return_valve_detergent",65718},
		{L"advance_return_valve_mud",65714},
		{L"advance_search",65704},
		{L"advance_security_valve",65716},
		{L"advance_starttime",600001},
		{L"advance_temp",65737},
		{L"advance_vflow",65738},
		{L"advance_washing_pump",65717},
		{L"alarm_interval",65747},
		{L"alarm_ip",65745},
		{L"alarm_lights",65540},
		{L"alarm_port",65746},
		{L"av",65819},
		{L"blurWnd",65602},
		{L"btn_bk",65666},
		{L"btn_cl_units",65649},
		{L"btn_clean",65598},
		{L"btn_close",65538},
		{L"btn_code",65544},
		{L"btn_density_units",65644},
		{L"btn_diag",65580},
		{L"btn_dot",65664},
		{L"btn_end",65555},
		{L"btn_enter",65667},
		{L"btn_fluid_setup",65579},
		{L"btn_home",65552},
		{L"btn_key0",65665},
		{L"btn_key1",65655},
		{L"btn_key2",65656},
		{L"btn_key3",65657},
		{L"btn_key4",65658},
		{L"btn_key5",65659},
		{L"btn_key6",65660},
		{L"btn_key7",65661},
		{L"btn_key8",65662},
		{L"btn_key9",65663},
		{L"btn_logout",65583},
		{L"btn_min",65582},
		{L"btn_next",65554},
		{L"btn_ok",65565},
		{L"btn_ph_units",65654},
		{L"btn_prev",65551},
		{L"btn_pv_units",65631},
		{L"btn_restore",65865},
		{L"btn_snapshot",65603},
		{L"btn_start",65599},
		{L"btn_stop",65597},
		{L"btn_system_options",65581},
		{L"btn_temp_units",65640},
		{L"btn_time",65543},
		{L"btn_trip",65556},
		{L"btn_yp_units",65636},
		{L"cbc_calcmodel",65748},
		{L"cbv_active",65563},
		{L"cbv_chart_time",65592},
		{L"cbv_density",65560},
		{L"cbv_flowmeter_choose",65742},
		{L"cbv_model",65626},
		{L"cbv_temp",65562},
		{L"cbv_type",65557},
		{L"chart_datalog",700000},
		{L"chart_line",65586},
		{L"chart_time",65591},
		{L"chart_time_lmax",65593},
		{L"chart_time_lmin",65595},
		{L"chart_time_rmax",65594},
		{L"chart_time_rmin",65596},
		{L"check_alarm",11},
		{L"check_comm",13},
		{L"check_hardware",10},
		{L"check_warning",12},
		{L"checked_BH",65588},
		{L"checked_HB",65590},
		{L"checked_NT",65587},
		{L"checked_PL",65589},
		{L"checked_diffpress",65707},
		{L"checked_mlevel",65709},
		{L"checked_opening",65708},
		{L"checked_press",65705},
		{L"checked_vflow",65706},
		{L"cl_alert_down",65648},
		{L"cl_alert_up",65647},
		{L"clean_interval",65755},
		{L"clean_start_time",600001},
		{L"clean_times",65756},
		{L"comm_lights",65542},
		{L"datalog_AV",65779},
		{L"datalog_AV_text1",65780},
		{L"datalog_AV_text2",65781},
		{L"datalog_AvgDensity",65777},
		{L"datalog_AvgPress",65762},
		{L"datalog_AvgPress_text1",65763},
		{L"datalog_AvgPress_text2",65764},
		{L"datalog_AvgTemp",65759},
		{L"datalog_AvgTemp_text1",65760},
		{L"datalog_AvgTemp_text2",65761},
		{L"datalog_Cl-",65843},
		{L"datalog_Cl-_text1",65844},
		{L"datalog_Cl-_text2",65845},
		{L"datalog_ES",65853},
		{L"datalog_ES_text1",65854},
		{L"datalog_ES_text2",65855},
		{L"datalog_KHB",65795},
		{L"datalog_KHB_text1",65796},
		{L"datalog_KHB_text2",65797},
		{L"datalog_KPL",65790},
		{L"datalog_KPL_text1",65791},
		{L"datalog_KPL_text2",65792},
		{L"datalog_MaxDensity",65776},
		{L"datalog_MinDensity",65778},
		{L"datalog_PH",65833},
		{L"datalog_PH_text1",65834},
		{L"datalog_PV",65782},
		{L"datalog_PV_text1",65783},
		{L"datalog_PV_text2",65784},
		{L"datalog_R0",65775},
		{L"datalog_R10",65772},
		{L"datalog_R100",65768},
		{L"datalog_R20",65771},
		{L"datalog_R200",65767},
		{L"datalog_R3",65774},
		{L"datalog_R30",65770},
		{L"datalog_R300",65766},
		{L"datalog_R6",65773},
		{L"datalog_R60",65769},
		{L"datalog_R600",65765},
		{L"datalog_Tau0",65798},
		{L"datalog_Tau0_text1",65799},
		{L"datalog_Tau0_text2",65800},
		{L"datalog_YP",65785},
		{L"datalog_YP_text1",65786},
		{L"datalog_YP_text2",65787},
		{L"datalog_comment_date",65677},
		{L"datalog_comments",65676},
		{L"datalog_end",65832},
		{L"datalog_export",65675},
		{L"datalog_home",65829},
		{L"datalog_lvdata",65801},
		{L"datalog_nHB",65793},
		{L"datalog_nHB_text1",65794},
		{L"datalog_nPL",65788},
		{L"datalog_nPL_text1",65789},
		{L"datalog_next",65831},
		{L"datalog_obm",65689},
		{L"datalog_page",65827},
		{L"datalog_prev",65828},
		{L"datalog_record",65687},
		{L"datalog_search",65674},
		{L"datalog_tableft",65685},
		{L"datalog_tabright",65686},
		{L"datalog_timeend",65673},
		{L"datalog_timestart",65672},
		{L"datalog_title0",65678},
		{L"datalog_title1",65679},
		{L"datalog_title2",65680},
		{L"datalog_title3",65681},
		{L"datalog_title4",65682},
		{L"datalog_title5",65683},
		{L"datalog_title6",65684},
		{L"datalog_total",65830},
		{L"datalog_wbm",65688},
		{L"date_time",65564},
		{L"datetime",65572},
		{L"den_avg",65817},
		{L"den_max",65816},
		{L"den_min",65818},
		{L"density_alert_down",65643},
		{L"density_alert_up",65642},
		{L"des",65864},
		{L"device_ip",65741},
		{L"edit_density",65559},
		{L"edit_name",20001},
		{L"edit_temp",65561},
		{L"enable_cyclic_clean",65758},
		{L"ev0_flow",65722},
		{L"ev_set",65721},
		{L"event",65693},
		{L"evn_flow",65723},
		{L"evn_press",65724},
		{L"fr",65720},
		{L"hardware_lights",65539},
		{L"home_page",65585},
		{L"khb",65825},
		{L"kpl",65823},
		{L"login_btn_close",65566},
		{L"login_btn_login",65569},
		{L"logs_chk_datalogs",65668},
		{L"logs_chk_oplogs",65669},
		{L"logs_page",65600},
		{L"lt",65725},
		{L"lv_info",65545},
		{L"lv_oplog",65690},
		{L"muti_cl",65646},
		{L"muti_density",65641},
		{L"muti_ph",65651},
		{L"muti_pv",65628},
		{L"muti_temp",65637},
		{L"muti_yp",65633},
		{L"next_clean_time",65757},
		{L"nhb",65824},
		{L"npl",65822},
		{L"oplog_end",65699},
		{L"oplog_home",65696},
		{L"oplog_next",65698},
		{L"oplog_page",65694},
		{L"oplog_prev",65695},
		{L"oplog_total",65697},
		{L"page",65550},
		{L"page_advance_debug",65702},
		{L"page_advance_init",65703},
		{L"page_datalogs",65670},
		{L"page_oplogs",65671},
		{L"ph_alert_down",65653},
		{L"ph_alert_up",65652},
		{L"press_avg",65804},
		{L"pv",65820},
		{L"pv_alert_down",65630},
		{L"pv_alert_up",65629},
		{L"sample_index",65733},
		{L"sensors_Cl-",65753},
		{L"sensors_ES",65751},
		{L"sensors_PH",65754},
		{L"sensors_obm",65750},
		{L"sensors_obm_wnd",65749},
		{L"sensors_wbm",1000031},
		{L"sensors_wbm_wnd",65752},
		{L"set_ev_v0",65726},
		{L"speed_0_title",65604},
		{L"speed_0_value",65605},
		{L"speed_10_title",65620},
		{L"speed_10_value",65621},
		{L"speed_1_title",65610},
		{L"speed_1_value",65611},
		{L"speed_2_title",65616},
		{L"speed_2_value",65617},
		{L"speed_3_title",65622},
		{L"speed_3_value",65623},
		{L"speed_4_title",65606},
		{L"speed_4_value",65607},
		{L"speed_5_title",65612},
		{L"speed_5_value",65613},
		{L"speed_6_title",65618},
		{L"speed_6_value",65619},
		{L"speed_7_title",65624},
		{L"speed_7_value",65625},
		{L"speed_8_title",65608},
		{L"speed_8_value",65609},
		{L"speed_9_title",65614},
		{L"speed_9_value",65615},
		{L"state_bg",65584},
		{L"state_title",65570},
		{L"system_chk_adv",65576},
		{L"system_chk_home",65574},
		{L"system_chk_logs",65575},
		{L"system_menu",65578},
		{L"system_setup",65577},
		{L"temp_alert_down",65639},
		{L"temp_alert_up",65638},
		{L"temp_avg",65803},
		{L"text",65558},
		{L"text_cl",65645},
		{L"text_code",65548},
		{L"text_des",65549},
		{L"text_pv",65627},
		{L"text_time",65546},
		{L"text_time_end",65547},
		{L"text_total",65553},
		{L"text_yp",65632},
		{L"time",65691},
		{L"title",65863},
		{L"ty",65826},
		{L"user",65692},
		{L"user_name",65573},
		{L"val",65836},
		{L"warning_lights",65541},
		{L"well_name",65571},
		{L"winedit_password",65568},
		{L"winedit_user",65567},
		{L"wits_ip",65743},
		{L"wits_port",65744},
		{L"wnd_bg",65802},
		{L"wnd_ph_data",65650},
		{L"xml_home",65537},
		{L"xml_login",65536},
		{L"yp",65821},
		{L"yp_alert_down",65635},
		{L"yp_alert_up",65634}		};
	class _R{
	public:
		class _name{
		public:
		_name(){
			Cl__end = namedXmlID[0].strName;
			Cl__home = namedXmlID[1].strName;
			Cl__lvdata = namedXmlID[2].strName;
			Cl__next = namedXmlID[3].strName;
			Cl__page = namedXmlID[4].strName;
			Cl__prev = namedXmlID[5].strName;
			Cl__total = namedXmlID[6].strName;
			ES_end = namedXmlID[7].strName;
			ES_home = namedXmlID[8].strName;
			ES_lvdata = namedXmlID[9].strName;
			ES_next = namedXmlID[10].strName;
			ES_page = namedXmlID[11].strName;
			ES_prev = namedXmlID[12].strName;
			ES_total = namedXmlID[13].strName;
			PH_end = namedXmlID[14].strName;
			PH_home = namedXmlID[15].strName;
			PH_lvdata = namedXmlID[16].strName;
			PH_next = namedXmlID[17].strName;
			PH_page = namedXmlID[18].strName;
			PH_prev = namedXmlID[19].strName;
			PH_total = namedXmlID[20].strName;
			R0 = namedXmlID[21].strName;
			R10 = namedXmlID[22].strName;
			R100 = namedXmlID[23].strName;
			R20 = namedXmlID[24].strName;
			R200 = namedXmlID[25].strName;
			R3 = namedXmlID[26].strName;
			R30 = namedXmlID[27].strName;
			R300 = namedXmlID[28].strName;
			R6 = namedXmlID[29].strName;
			R60 = namedXmlID[30].strName;
			R600 = namedXmlID[31].strName;
			_name_start = namedXmlID[32].strName;
			advance_chartreal = namedXmlID[33].strName;
			advance_chk_debug = namedXmlID[34].strName;
			advance_chk_init = namedXmlID[35].strName;
			advance_circulating_pump = namedXmlID[36].strName;
			advance_cooling_valve = namedXmlID[37].strName;
			advance_debug = namedXmlID[38].strName;
			advance_density = namedXmlID[39].strName;
			advance_dfp1 = namedXmlID[40].strName;
			advance_dfp2 = namedXmlID[41].strName;
			advance_ev = namedXmlID[42].strName;
			advance_ev0n = namedXmlID[43].strName;
			advance_log = namedXmlID[44].strName;
			advance_mflow = namedXmlID[45].strName;
			advance_mudlevel = namedXmlID[46].strName;
			advance_p1 = namedXmlID[47].strName;
			advance_p2 = namedXmlID[48].strName;
			advance_p3 = namedXmlID[49].strName;
			advance_p4 = namedXmlID[50].strName;
			advance_page = namedXmlID[51].strName;
			advance_purge_valve = namedXmlID[52].strName;
			advance_return_valve_detergent = namedXmlID[53].strName;
			advance_return_valve_mud = namedXmlID[54].strName;
			advance_search = namedXmlID[55].strName;
			advance_security_valve = namedXmlID[56].strName;
			advance_starttime = namedXmlID[57].strName;
			advance_temp = namedXmlID[58].strName;
			advance_vflow = namedXmlID[59].strName;
			advance_washing_pump = namedXmlID[60].strName;
			alarm_interval = namedXmlID[61].strName;
			alarm_ip = namedXmlID[62].strName;
			alarm_lights = namedXmlID[63].strName;
			alarm_port = namedXmlID[64].strName;
			av = namedXmlID[65].strName;
			blurWnd = namedXmlID[66].strName;
			btn_bk = namedXmlID[67].strName;
			btn_cl_units = namedXmlID[68].strName;
			btn_clean = namedXmlID[69].strName;
			btn_close = namedXmlID[70].strName;
			btn_code = namedXmlID[71].strName;
			btn_density_units = namedXmlID[72].strName;
			btn_diag = namedXmlID[73].strName;
			btn_dot = namedXmlID[74].strName;
			btn_end = namedXmlID[75].strName;
			btn_enter = namedXmlID[76].strName;
			btn_fluid_setup = namedXmlID[77].strName;
			btn_home = namedXmlID[78].strName;
			btn_key0 = namedXmlID[79].strName;
			btn_key1 = namedXmlID[80].strName;
			btn_key2 = namedXmlID[81].strName;
			btn_key3 = namedXmlID[82].strName;
			btn_key4 = namedXmlID[83].strName;
			btn_key5 = namedXmlID[84].strName;
			btn_key6 = namedXmlID[85].strName;
			btn_key7 = namedXmlID[86].strName;
			btn_key8 = namedXmlID[87].strName;
			btn_key9 = namedXmlID[88].strName;
			btn_logout = namedXmlID[89].strName;
			btn_min = namedXmlID[90].strName;
			btn_next = namedXmlID[91].strName;
			btn_ok = namedXmlID[92].strName;
			btn_ph_units = namedXmlID[93].strName;
			btn_prev = namedXmlID[94].strName;
			btn_pv_units = namedXmlID[95].strName;
			btn_restore = namedXmlID[96].strName;
			btn_snapshot = namedXmlID[97].strName;
			btn_start = namedXmlID[98].strName;
			btn_stop = namedXmlID[99].strName;
			btn_system_options = namedXmlID[100].strName;
			btn_temp_units = namedXmlID[101].strName;
			btn_time = namedXmlID[102].strName;
			btn_trip = namedXmlID[103].strName;
			btn_yp_units = namedXmlID[104].strName;
			cbc_calcmodel = namedXmlID[105].strName;
			cbv_active = namedXmlID[106].strName;
			cbv_chart_time = namedXmlID[107].strName;
			cbv_density = namedXmlID[108].strName;
			cbv_flowmeter_choose = namedXmlID[109].strName;
			cbv_model = namedXmlID[110].strName;
			cbv_temp = namedXmlID[111].strName;
			cbv_type = namedXmlID[112].strName;
			chart_datalog = namedXmlID[113].strName;
			chart_line = namedXmlID[114].strName;
			chart_time = namedXmlID[115].strName;
			chart_time_lmax = namedXmlID[116].strName;
			chart_time_lmin = namedXmlID[117].strName;
			chart_time_rmax = namedXmlID[118].strName;
			chart_time_rmin = namedXmlID[119].strName;
			check_alarm = namedXmlID[120].strName;
			check_comm = namedXmlID[121].strName;
			check_hardware = namedXmlID[122].strName;
			check_warning = namedXmlID[123].strName;
			checked_BH = namedXmlID[124].strName;
			checked_HB = namedXmlID[125].strName;
			checked_NT = namedXmlID[126].strName;
			checked_PL = namedXmlID[127].strName;
			checked_diffpress = namedXmlID[128].strName;
			checked_mlevel = namedXmlID[129].strName;
			checked_opening = namedXmlID[130].strName;
			checked_press = namedXmlID[131].strName;
			checked_vflow = namedXmlID[132].strName;
			cl_alert_down = namedXmlID[133].strName;
			cl_alert_up = namedXmlID[134].strName;
			clean_interval = namedXmlID[135].strName;
			clean_start_time = namedXmlID[136].strName;
			clean_times = namedXmlID[137].strName;
			comm_lights = namedXmlID[138].strName;
			datalog_AV = namedXmlID[139].strName;
			datalog_AV_text1 = namedXmlID[140].strName;
			datalog_AV_text2 = namedXmlID[141].strName;
			datalog_AvgDensity = namedXmlID[142].strName;
			datalog_AvgPress = namedXmlID[143].strName;
			datalog_AvgPress_text1 = namedXmlID[144].strName;
			datalog_AvgPress_text2 = namedXmlID[145].strName;
			datalog_AvgTemp = namedXmlID[146].strName;
			datalog_AvgTemp_text1 = namedXmlID[147].strName;
			datalog_AvgTemp_text2 = namedXmlID[148].strName;
			datalog_Cl_ = namedXmlID[149].strName;
			datalog_Cl__text1 = namedXmlID[150].strName;
			datalog_Cl__text2 = namedXmlID[151].strName;
			datalog_ES = namedXmlID[152].strName;
			datalog_ES_text1 = namedXmlID[153].strName;
			datalog_ES_text2 = namedXmlID[154].strName;
			datalog_KHB = namedXmlID[155].strName;
			datalog_KHB_text1 = namedXmlID[156].strName;
			datalog_KHB_text2 = namedXmlID[157].strName;
			datalog_KPL = namedXmlID[158].strName;
			datalog_KPL_text1 = namedXmlID[159].strName;
			datalog_KPL_text2 = namedXmlID[160].strName;
			datalog_MaxDensity = namedXmlID[161].strName;
			datalog_MinDensity = namedXmlID[162].strName;
			datalog_PH = namedXmlID[163].strName;
			datalog_PH_text1 = namedXmlID[164].strName;
			datalog_PV = namedXmlID[165].strName;
			datalog_PV_text1 = namedXmlID[166].strName;
			datalog_PV_text2 = namedXmlID[167].strName;
			datalog_R0 = namedXmlID[168].strName;
			datalog_R10 = namedXmlID[169].strName;
			datalog_R100 = namedXmlID[170].strName;
			datalog_R20 = namedXmlID[171].strName;
			datalog_R200 = namedXmlID[172].strName;
			datalog_R3 = namedXmlID[173].strName;
			datalog_R30 = namedXmlID[174].strName;
			datalog_R300 = namedXmlID[175].strName;
			datalog_R6 = namedXmlID[176].strName;
			datalog_R60 = namedXmlID[177].strName;
			datalog_R600 = namedXmlID[178].strName;
			datalog_Tau0 = namedXmlID[179].strName;
			datalog_Tau0_text1 = namedXmlID[180].strName;
			datalog_Tau0_text2 = namedXmlID[181].strName;
			datalog_YP = namedXmlID[182].strName;
			datalog_YP_text1 = namedXmlID[183].strName;
			datalog_YP_text2 = namedXmlID[184].strName;
			datalog_comment_date = namedXmlID[185].strName;
			datalog_comments = namedXmlID[186].strName;
			datalog_end = namedXmlID[187].strName;
			datalog_export = namedXmlID[188].strName;
			datalog_home = namedXmlID[189].strName;
			datalog_lvdata = namedXmlID[190].strName;
			datalog_nHB = namedXmlID[191].strName;
			datalog_nHB_text1 = namedXmlID[192].strName;
			datalog_nPL = namedXmlID[193].strName;
			datalog_nPL_text1 = namedXmlID[194].strName;
			datalog_next = namedXmlID[195].strName;
			datalog_obm = namedXmlID[196].strName;
			datalog_page = namedXmlID[197].strName;
			datalog_prev = namedXmlID[198].strName;
			datalog_record = namedXmlID[199].strName;
			datalog_search = namedXmlID[200].strName;
			datalog_tableft = namedXmlID[201].strName;
			datalog_tabright = namedXmlID[202].strName;
			datalog_timeend = namedXmlID[203].strName;
			datalog_timestart = namedXmlID[204].strName;
			datalog_title0 = namedXmlID[205].strName;
			datalog_title1 = namedXmlID[206].strName;
			datalog_title2 = namedXmlID[207].strName;
			datalog_title3 = namedXmlID[208].strName;
			datalog_title4 = namedXmlID[209].strName;
			datalog_title5 = namedXmlID[210].strName;
			datalog_title6 = namedXmlID[211].strName;
			datalog_total = namedXmlID[212].strName;
			datalog_wbm = namedXmlID[213].strName;
			date_time = namedXmlID[214].strName;
			datetime = namedXmlID[215].strName;
			den_avg = namedXmlID[216].strName;
			den_max = namedXmlID[217].strName;
			den_min = namedXmlID[218].strName;
			density_alert_down = namedXmlID[219].strName;
			density_alert_up = namedXmlID[220].strName;
			des = namedXmlID[221].strName;
			device_ip = namedXmlID[222].strName;
			edit_density = namedXmlID[223].strName;
			edit_name = namedXmlID[224].strName;
			edit_temp = namedXmlID[225].strName;
			enable_cyclic_clean = namedXmlID[226].strName;
			ev0_flow = namedXmlID[227].strName;
			ev_set = namedXmlID[228].strName;
			event_cpp = namedXmlID[229].strName;
			evn_flow = namedXmlID[230].strName;
			evn_press = namedXmlID[231].strName;
			fr = namedXmlID[232].strName;
			hardware_lights = namedXmlID[233].strName;
			home_page = namedXmlID[234].strName;
			khb = namedXmlID[235].strName;
			kpl = namedXmlID[236].strName;
			login_btn_close = namedXmlID[237].strName;
			login_btn_login = namedXmlID[238].strName;
			logs_chk_datalogs = namedXmlID[239].strName;
			logs_chk_oplogs = namedXmlID[240].strName;
			logs_page = namedXmlID[241].strName;
			lt = namedXmlID[242].strName;
			lv_info = namedXmlID[243].strName;
			lv_oplog = namedXmlID[244].strName;
			muti_cl = namedXmlID[245].strName;
			muti_density = namedXmlID[246].strName;
			muti_ph = namedXmlID[247].strName;
			muti_pv = namedXmlID[248].strName;
			muti_temp = namedXmlID[249].strName;
			muti_yp = namedXmlID[250].strName;
			next_clean_time = namedXmlID[251].strName;
			nhb = namedXmlID[252].strName;
			npl = namedXmlID[253].strName;
			oplog_end = namedXmlID[254].strName;
			oplog_home = namedXmlID[255].strName;
			oplog_next = namedXmlID[256].strName;
			oplog_page = namedXmlID[257].strName;
			oplog_prev = namedXmlID[258].strName;
			oplog_total = namedXmlID[259].strName;
			page = namedXmlID[260].strName;
			page_advance_debug = namedXmlID[261].strName;
			page_advance_init = namedXmlID[262].strName;
			page_datalogs = namedXmlID[263].strName;
			page_oplogs = namedXmlID[264].strName;
			ph_alert_down = namedXmlID[265].strName;
			ph_alert_up = namedXmlID[266].strName;
			press_avg = namedXmlID[267].strName;
			pv = namedXmlID[268].strName;
			pv_alert_down = namedXmlID[269].strName;
			pv_alert_up = namedXmlID[270].strName;
			sample_index = namedXmlID[271].strName;
			sensors_Cl_ = namedXmlID[272].strName;
			sensors_ES = namedXmlID[273].strName;
			sensors_PH = namedXmlID[274].strName;
			sensors_obm = namedXmlID[275].strName;
			sensors_obm_wnd = namedXmlID[276].strName;
			sensors_wbm = namedXmlID[277].strName;
			sensors_wbm_wnd = namedXmlID[278].strName;
			set_ev_v0 = namedXmlID[279].strName;
			speed_0_title = namedXmlID[280].strName;
			speed_0_value = namedXmlID[281].strName;
			speed_10_title = namedXmlID[282].strName;
			speed_10_value = namedXmlID[283].strName;
			speed_1_title = namedXmlID[284].strName;
			speed_1_value = namedXmlID[285].strName;
			speed_2_title = namedXmlID[286].strName;
			speed_2_value = namedXmlID[287].strName;
			speed_3_title = namedXmlID[288].strName;
			speed_3_value = namedXmlID[289].strName;
			speed_4_title = namedXmlID[290].strName;
			speed_4_value = namedXmlID[291].strName;
			speed_5_title = namedXmlID[292].strName;
			speed_5_value = namedXmlID[293].strName;
			speed_6_title = namedXmlID[294].strName;
			speed_6_value = namedXmlID[295].strName;
			speed_7_title = namedXmlID[296].strName;
			speed_7_value = namedXmlID[297].strName;
			speed_8_title = namedXmlID[298].strName;
			speed_8_value = namedXmlID[299].strName;
			speed_9_title = namedXmlID[300].strName;
			speed_9_value = namedXmlID[301].strName;
			state_bg = namedXmlID[302].strName;
			state_title = namedXmlID[303].strName;
			system_chk_adv = namedXmlID[304].strName;
			system_chk_home = namedXmlID[305].strName;
			system_chk_logs = namedXmlID[306].strName;
			system_menu = namedXmlID[307].strName;
			system_setup = namedXmlID[308].strName;
			temp_alert_down = namedXmlID[309].strName;
			temp_alert_up = namedXmlID[310].strName;
			temp_avg = namedXmlID[311].strName;
			text = namedXmlID[312].strName;
			text_cl = namedXmlID[313].strName;
			text_code = namedXmlID[314].strName;
			text_des = namedXmlID[315].strName;
			text_pv = namedXmlID[316].strName;
			text_time = namedXmlID[317].strName;
			text_time_end = namedXmlID[318].strName;
			text_total = namedXmlID[319].strName;
			text_yp = namedXmlID[320].strName;
			time = namedXmlID[321].strName;
			title = namedXmlID[322].strName;
			ty = namedXmlID[323].strName;
			user = namedXmlID[324].strName;
			user_name = namedXmlID[325].strName;
			val = namedXmlID[326].strName;
			warning_lights = namedXmlID[327].strName;
			well_name = namedXmlID[328].strName;
			winedit_password = namedXmlID[329].strName;
			winedit_user = namedXmlID[330].strName;
			wits_ip = namedXmlID[331].strName;
			wits_port = namedXmlID[332].strName;
			wnd_bg = namedXmlID[333].strName;
			wnd_ph_data = namedXmlID[334].strName;
			xml_home = namedXmlID[335].strName;
			xml_login = namedXmlID[336].strName;
			yp = namedXmlID[337].strName;
			yp_alert_down = namedXmlID[338].strName;
			yp_alert_up = namedXmlID[339].strName;
		}
		 const wchar_t * Cl__end;
		 const wchar_t * Cl__home;
		 const wchar_t * Cl__lvdata;
		 const wchar_t * Cl__next;
		 const wchar_t * Cl__page;
		 const wchar_t * Cl__prev;
		 const wchar_t * Cl__total;
		 const wchar_t * ES_end;
		 const wchar_t * ES_home;
		 const wchar_t * ES_lvdata;
		 const wchar_t * ES_next;
		 const wchar_t * ES_page;
		 const wchar_t * ES_prev;
		 const wchar_t * ES_total;
		 const wchar_t * PH_end;
		 const wchar_t * PH_home;
		 const wchar_t * PH_lvdata;
		 const wchar_t * PH_next;
		 const wchar_t * PH_page;
		 const wchar_t * PH_prev;
		 const wchar_t * PH_total;
		 const wchar_t * R0;
		 const wchar_t * R10;
		 const wchar_t * R100;
		 const wchar_t * R20;
		 const wchar_t * R200;
		 const wchar_t * R3;
		 const wchar_t * R30;
		 const wchar_t * R300;
		 const wchar_t * R6;
		 const wchar_t * R60;
		 const wchar_t * R600;
		 const wchar_t * _name_start;
		 const wchar_t * advance_chartreal;
		 const wchar_t * advance_chk_debug;
		 const wchar_t * advance_chk_init;
		 const wchar_t * advance_circulating_pump;
		 const wchar_t * advance_cooling_valve;
		 const wchar_t * advance_debug;
		 const wchar_t * advance_density;
		 const wchar_t * advance_dfp1;
		 const wchar_t * advance_dfp2;
		 const wchar_t * advance_ev;
		 const wchar_t * advance_ev0n;
		 const wchar_t * advance_log;
		 const wchar_t * advance_mflow;
		 const wchar_t * advance_mudlevel;
		 const wchar_t * advance_p1;
		 const wchar_t * advance_p2;
		 const wchar_t * advance_p3;
		 const wchar_t * advance_p4;
		 const wchar_t * advance_page;
		 const wchar_t * advance_purge_valve;
		 const wchar_t * advance_return_valve_detergent;
		 const wchar_t * advance_return_valve_mud;
		 const wchar_t * advance_search;
		 const wchar_t * advance_security_valve;
		 const wchar_t * advance_starttime;
		 const wchar_t * advance_temp;
		 const wchar_t * advance_vflow;
		 const wchar_t * advance_washing_pump;
		 const wchar_t * alarm_interval;
		 const wchar_t * alarm_ip;
		 const wchar_t * alarm_lights;
		 const wchar_t * alarm_port;
		 const wchar_t * av;
		 const wchar_t * blurWnd;
		 const wchar_t * btn_bk;
		 const wchar_t * btn_cl_units;
		 const wchar_t * btn_clean;
		 const wchar_t * btn_close;
		 const wchar_t * btn_code;
		 const wchar_t * btn_density_units;
		 const wchar_t * btn_diag;
		 const wchar_t * btn_dot;
		 const wchar_t * btn_end;
		 const wchar_t * btn_enter;
		 const wchar_t * btn_fluid_setup;
		 const wchar_t * btn_home;
		 const wchar_t * btn_key0;
		 const wchar_t * btn_key1;
		 const wchar_t * btn_key2;
		 const wchar_t * btn_key3;
		 const wchar_t * btn_key4;
		 const wchar_t * btn_key5;
		 const wchar_t * btn_key6;
		 const wchar_t * btn_key7;
		 const wchar_t * btn_key8;
		 const wchar_t * btn_key9;
		 const wchar_t * btn_logout;
		 const wchar_t * btn_min;
		 const wchar_t * btn_next;
		 const wchar_t * btn_ok;
		 const wchar_t * btn_ph_units;
		 const wchar_t * btn_prev;
		 const wchar_t * btn_pv_units;
		 const wchar_t * btn_restore;
		 const wchar_t * btn_snapshot;
		 const wchar_t * btn_start;
		 const wchar_t * btn_stop;
		 const wchar_t * btn_system_options;
		 const wchar_t * btn_temp_units;
		 const wchar_t * btn_time;
		 const wchar_t * btn_trip;
		 const wchar_t * btn_yp_units;
		 const wchar_t * cbc_calcmodel;
		 const wchar_t * cbv_active;
		 const wchar_t * cbv_chart_time;
		 const wchar_t * cbv_density;
		 const wchar_t * cbv_flowmeter_choose;
		 const wchar_t * cbv_model;
		 const wchar_t * cbv_temp;
		 const wchar_t * cbv_type;
		 const wchar_t * chart_datalog;
		 const wchar_t * chart_line;
		 const wchar_t * chart_time;
		 const wchar_t * chart_time_lmax;
		 const wchar_t * chart_time_lmin;
		 const wchar_t * chart_time_rmax;
		 const wchar_t * chart_time_rmin;
		 const wchar_t * check_alarm;
		 const wchar_t * check_comm;
		 const wchar_t * check_hardware;
		 const wchar_t * check_warning;
		 const wchar_t * checked_BH;
		 const wchar_t * checked_HB;
		 const wchar_t * checked_NT;
		 const wchar_t * checked_PL;
		 const wchar_t * checked_diffpress;
		 const wchar_t * checked_mlevel;
		 const wchar_t * checked_opening;
		 const wchar_t * checked_press;
		 const wchar_t * checked_vflow;
		 const wchar_t * cl_alert_down;
		 const wchar_t * cl_alert_up;
		 const wchar_t * clean_interval;
		 const wchar_t * clean_start_time;
		 const wchar_t * clean_times;
		 const wchar_t * comm_lights;
		 const wchar_t * datalog_AV;
		 const wchar_t * datalog_AV_text1;
		 const wchar_t * datalog_AV_text2;
		 const wchar_t * datalog_AvgDensity;
		 const wchar_t * datalog_AvgPress;
		 const wchar_t * datalog_AvgPress_text1;
		 const wchar_t * datalog_AvgPress_text2;
		 const wchar_t * datalog_AvgTemp;
		 const wchar_t * datalog_AvgTemp_text1;
		 const wchar_t * datalog_AvgTemp_text2;
		 const wchar_t * datalog_Cl_;
		 const wchar_t * datalog_Cl__text1;
		 const wchar_t * datalog_Cl__text2;
		 const wchar_t * datalog_ES;
		 const wchar_t * datalog_ES_text1;
		 const wchar_t * datalog_ES_text2;
		 const wchar_t * datalog_KHB;
		 const wchar_t * datalog_KHB_text1;
		 const wchar_t * datalog_KHB_text2;
		 const wchar_t * datalog_KPL;
		 const wchar_t * datalog_KPL_text1;
		 const wchar_t * datalog_KPL_text2;
		 const wchar_t * datalog_MaxDensity;
		 const wchar_t * datalog_MinDensity;
		 const wchar_t * datalog_PH;
		 const wchar_t * datalog_PH_text1;
		 const wchar_t * datalog_PV;
		 const wchar_t * datalog_PV_text1;
		 const wchar_t * datalog_PV_text2;
		 const wchar_t * datalog_R0;
		 const wchar_t * datalog_R10;
		 const wchar_t * datalog_R100;
		 const wchar_t * datalog_R20;
		 const wchar_t * datalog_R200;
		 const wchar_t * datalog_R3;
		 const wchar_t * datalog_R30;
		 const wchar_t * datalog_R300;
		 const wchar_t * datalog_R6;
		 const wchar_t * datalog_R60;
		 const wchar_t * datalog_R600;
		 const wchar_t * datalog_Tau0;
		 const wchar_t * datalog_Tau0_text1;
		 const wchar_t * datalog_Tau0_text2;
		 const wchar_t * datalog_YP;
		 const wchar_t * datalog_YP_text1;
		 const wchar_t * datalog_YP_text2;
		 const wchar_t * datalog_comment_date;
		 const wchar_t * datalog_comments;
		 const wchar_t * datalog_end;
		 const wchar_t * datalog_export;
		 const wchar_t * datalog_home;
		 const wchar_t * datalog_lvdata;
		 const wchar_t * datalog_nHB;
		 const wchar_t * datalog_nHB_text1;
		 const wchar_t * datalog_nPL;
		 const wchar_t * datalog_nPL_text1;
		 const wchar_t * datalog_next;
		 const wchar_t * datalog_obm;
		 const wchar_t * datalog_page;
		 const wchar_t * datalog_prev;
		 const wchar_t * datalog_record;
		 const wchar_t * datalog_search;
		 const wchar_t * datalog_tableft;
		 const wchar_t * datalog_tabright;
		 const wchar_t * datalog_timeend;
		 const wchar_t * datalog_timestart;
		 const wchar_t * datalog_title0;
		 const wchar_t * datalog_title1;
		 const wchar_t * datalog_title2;
		 const wchar_t * datalog_title3;
		 const wchar_t * datalog_title4;
		 const wchar_t * datalog_title5;
		 const wchar_t * datalog_title6;
		 const wchar_t * datalog_total;
		 const wchar_t * datalog_wbm;
		 const wchar_t * date_time;
		 const wchar_t * datetime;
		 const wchar_t * den_avg;
		 const wchar_t * den_max;
		 const wchar_t * den_min;
		 const wchar_t * density_alert_down;
		 const wchar_t * density_alert_up;
		 const wchar_t * des;
		 const wchar_t * device_ip;
		 const wchar_t * edit_density;
		 const wchar_t * edit_name;
		 const wchar_t * edit_temp;
		 const wchar_t * enable_cyclic_clean;
		 const wchar_t * ev0_flow;
		 const wchar_t * ev_set;
		 const wchar_t * event_cpp;
		 const wchar_t * evn_flow;
		 const wchar_t * evn_press;
		 const wchar_t * fr;
		 const wchar_t * hardware_lights;
		 const wchar_t * home_page;
		 const wchar_t * khb;
		 const wchar_t * kpl;
		 const wchar_t * login_btn_close;
		 const wchar_t * login_btn_login;
		 const wchar_t * logs_chk_datalogs;
		 const wchar_t * logs_chk_oplogs;
		 const wchar_t * logs_page;
		 const wchar_t * lt;
		 const wchar_t * lv_info;
		 const wchar_t * lv_oplog;
		 const wchar_t * muti_cl;
		 const wchar_t * muti_density;
		 const wchar_t * muti_ph;
		 const wchar_t * muti_pv;
		 const wchar_t * muti_temp;
		 const wchar_t * muti_yp;
		 const wchar_t * next_clean_time;
		 const wchar_t * nhb;
		 const wchar_t * npl;
		 const wchar_t * oplog_end;
		 const wchar_t * oplog_home;
		 const wchar_t * oplog_next;
		 const wchar_t * oplog_page;
		 const wchar_t * oplog_prev;
		 const wchar_t * oplog_total;
		 const wchar_t * page;
		 const wchar_t * page_advance_debug;
		 const wchar_t * page_advance_init;
		 const wchar_t * page_datalogs;
		 const wchar_t * page_oplogs;
		 const wchar_t * ph_alert_down;
		 const wchar_t * ph_alert_up;
		 const wchar_t * press_avg;
		 const wchar_t * pv;
		 const wchar_t * pv_alert_down;
		 const wchar_t * pv_alert_up;
		 const wchar_t * sample_index;
		 const wchar_t * sensors_Cl_;
		 const wchar_t * sensors_ES;
		 const wchar_t * sensors_PH;
		 const wchar_t * sensors_obm;
		 const wchar_t * sensors_obm_wnd;
		 const wchar_t * sensors_wbm;
		 const wchar_t * sensors_wbm_wnd;
		 const wchar_t * set_ev_v0;
		 const wchar_t * speed_0_title;
		 const wchar_t * speed_0_value;
		 const wchar_t * speed_10_title;
		 const wchar_t * speed_10_value;
		 const wchar_t * speed_1_title;
		 const wchar_t * speed_1_value;
		 const wchar_t * speed_2_title;
		 const wchar_t * speed_2_value;
		 const wchar_t * speed_3_title;
		 const wchar_t * speed_3_value;
		 const wchar_t * speed_4_title;
		 const wchar_t * speed_4_value;
		 const wchar_t * speed_5_title;
		 const wchar_t * speed_5_value;
		 const wchar_t * speed_6_title;
		 const wchar_t * speed_6_value;
		 const wchar_t * speed_7_title;
		 const wchar_t * speed_7_value;
		 const wchar_t * speed_8_title;
		 const wchar_t * speed_8_value;
		 const wchar_t * speed_9_title;
		 const wchar_t * speed_9_value;
		 const wchar_t * state_bg;
		 const wchar_t * state_title;
		 const wchar_t * system_chk_adv;
		 const wchar_t * system_chk_home;
		 const wchar_t * system_chk_logs;
		 const wchar_t * system_menu;
		 const wchar_t * system_setup;
		 const wchar_t * temp_alert_down;
		 const wchar_t * temp_alert_up;
		 const wchar_t * temp_avg;
		 const wchar_t * text;
		 const wchar_t * text_cl;
		 const wchar_t * text_code;
		 const wchar_t * text_des;
		 const wchar_t * text_pv;
		 const wchar_t * text_time;
		 const wchar_t * text_time_end;
		 const wchar_t * text_total;
		 const wchar_t * text_yp;
		 const wchar_t * time;
		 const wchar_t * title;
		 const wchar_t * ty;
		 const wchar_t * user;
		 const wchar_t * user_name;
		 const wchar_t * val;
		 const wchar_t * warning_lights;
		 const wchar_t * well_name;
		 const wchar_t * winedit_password;
		 const wchar_t * winedit_user;
		 const wchar_t * wits_ip;
		 const wchar_t * wits_port;
		 const wchar_t * wnd_bg;
		 const wchar_t * wnd_ph_data;
		 const wchar_t * xml_home;
		 const wchar_t * xml_login;
		 const wchar_t * yp;
		 const wchar_t * yp_alert_down;
		 const wchar_t * yp_alert_up;
		}name;

		class _id{
		public:
		const static int Cl__end	=	65851;
		const static int Cl__home	=	65849;
		const static int Cl__lvdata	=	65846;
		const static int Cl__next	=	65852;
		const static int Cl__page	=	65847;
		const static int Cl__prev	=	65850;
		const static int Cl__total	=	65848;
		const static int ES_end	=	65861;
		const static int ES_home	=	65859;
		const static int ES_lvdata	=	65856;
		const static int ES_next	=	65862;
		const static int ES_page	=	65857;
		const static int ES_prev	=	65860;
		const static int ES_total	=	65858;
		const static int PH_end	=	65841;
		const static int PH_home	=	65839;
		const static int PH_lvdata	=	65835;
		const static int PH_next	=	65842;
		const static int PH_page	=	65837;
		const static int PH_prev	=	65840;
		const static int PH_total	=	65838;
		const static int R0	=	65815;
		const static int R10	=	65812;
		const static int R100	=	65808;
		const static int R20	=	65811;
		const static int R200	=	65807;
		const static int R3	=	65814;
		const static int R30	=	65810;
		const static int R300	=	65806;
		const static int R6	=	65813;
		const static int R60	=	65809;
		const static int R600	=	65805;
		const static int _name_start	=	65535;
		const static int advance_chartreal	=	65710;
		const static int advance_chk_debug	=	65700;
		const static int advance_chk_init	=	65701;
		const static int advance_circulating_pump	=	65713;
		const static int advance_cooling_valve	=	65719;
		const static int advance_debug	=	65711;
		const static int advance_density	=	65736;
		const static int advance_dfp1	=	65731;
		const static int advance_dfp2	=	65732;
		const static int advance_ev	=	65734;
		const static int advance_ev0n	=	65735;
		const static int advance_log	=	65712;
		const static int advance_mflow	=	65739;
		const static int advance_mudlevel	=	65740;
		const static int advance_p1	=	65727;
		const static int advance_p2	=	65728;
		const static int advance_p3	=	65729;
		const static int advance_p4	=	65730;
		const static int advance_page	=	65601;
		const static int advance_purge_valve	=	65715;
		const static int advance_return_valve_detergent	=	65718;
		const static int advance_return_valve_mud	=	65714;
		const static int advance_search	=	65704;
		const static int advance_security_valve	=	65716;
		const static int advance_starttime	=	600001;
		const static int advance_temp	=	65737;
		const static int advance_vflow	=	65738;
		const static int advance_washing_pump	=	65717;
		const static int alarm_interval	=	65747;
		const static int alarm_ip	=	65745;
		const static int alarm_lights	=	65540;
		const static int alarm_port	=	65746;
		const static int av	=	65819;
		const static int blurWnd	=	65602;
		const static int btn_bk	=	65666;
		const static int btn_cl_units	=	65649;
		const static int btn_clean	=	65598;
		const static int btn_close	=	65538;
		const static int btn_code	=	65544;
		const static int btn_density_units	=	65644;
		const static int btn_diag	=	65580;
		const static int btn_dot	=	65664;
		const static int btn_end	=	65555;
		const static int btn_enter	=	65667;
		const static int btn_fluid_setup	=	65579;
		const static int btn_home	=	65552;
		const static int btn_key0	=	65665;
		const static int btn_key1	=	65655;
		const static int btn_key2	=	65656;
		const static int btn_key3	=	65657;
		const static int btn_key4	=	65658;
		const static int btn_key5	=	65659;
		const static int btn_key6	=	65660;
		const static int btn_key7	=	65661;
		const static int btn_key8	=	65662;
		const static int btn_key9	=	65663;
		const static int btn_logout	=	65583;
		const static int btn_min	=	65582;
		const static int btn_next	=	65554;
		const static int btn_ok	=	65565;
		const static int btn_ph_units	=	65654;
		const static int btn_prev	=	65551;
		const static int btn_pv_units	=	65631;
		const static int btn_restore	=	65865;
		const static int btn_snapshot	=	65603;
		const static int btn_start	=	65599;
		const static int btn_stop	=	65597;
		const static int btn_system_options	=	65581;
		const static int btn_temp_units	=	65640;
		const static int btn_time	=	65543;
		const static int btn_trip	=	65556;
		const static int btn_yp_units	=	65636;
		const static int cbc_calcmodel	=	65748;
		const static int cbv_active	=	65563;
		const static int cbv_chart_time	=	65592;
		const static int cbv_density	=	65560;
		const static int cbv_flowmeter_choose	=	65742;
		const static int cbv_model	=	65626;
		const static int cbv_temp	=	65562;
		const static int cbv_type	=	65557;
		const static int chart_datalog	=	700000;
		const static int chart_line	=	65586;
		const static int chart_time	=	65591;
		const static int chart_time_lmax	=	65593;
		const static int chart_time_lmin	=	65595;
		const static int chart_time_rmax	=	65594;
		const static int chart_time_rmin	=	65596;
		const static int check_alarm	=	11;
		const static int check_comm	=	13;
		const static int check_hardware	=	10;
		const static int check_warning	=	12;
		const static int checked_BH	=	65588;
		const static int checked_HB	=	65590;
		const static int checked_NT	=	65587;
		const static int checked_PL	=	65589;
		const static int checked_diffpress	=	65707;
		const static int checked_mlevel	=	65709;
		const static int checked_opening	=	65708;
		const static int checked_press	=	65705;
		const static int checked_vflow	=	65706;
		const static int cl_alert_down	=	65648;
		const static int cl_alert_up	=	65647;
		const static int clean_interval	=	65755;
		const static int clean_start_time	=	600001;
		const static int clean_times	=	65756;
		const static int comm_lights	=	65542;
		const static int datalog_AV	=	65779;
		const static int datalog_AV_text1	=	65780;
		const static int datalog_AV_text2	=	65781;
		const static int datalog_AvgDensity	=	65777;
		const static int datalog_AvgPress	=	65762;
		const static int datalog_AvgPress_text1	=	65763;
		const static int datalog_AvgPress_text2	=	65764;
		const static int datalog_AvgTemp	=	65759;
		const static int datalog_AvgTemp_text1	=	65760;
		const static int datalog_AvgTemp_text2	=	65761;
		const static int datalog_Cl_	=	65843;
		const static int datalog_Cl__text1	=	65844;
		const static int datalog_Cl__text2	=	65845;
		const static int datalog_ES	=	65853;
		const static int datalog_ES_text1	=	65854;
		const static int datalog_ES_text2	=	65855;
		const static int datalog_KHB	=	65795;
		const static int datalog_KHB_text1	=	65796;
		const static int datalog_KHB_text2	=	65797;
		const static int datalog_KPL	=	65790;
		const static int datalog_KPL_text1	=	65791;
		const static int datalog_KPL_text2	=	65792;
		const static int datalog_MaxDensity	=	65776;
		const static int datalog_MinDensity	=	65778;
		const static int datalog_PH	=	65833;
		const static int datalog_PH_text1	=	65834;
		const static int datalog_PV	=	65782;
		const static int datalog_PV_text1	=	65783;
		const static int datalog_PV_text2	=	65784;
		const static int datalog_R0	=	65775;
		const static int datalog_R10	=	65772;
		const static int datalog_R100	=	65768;
		const static int datalog_R20	=	65771;
		const static int datalog_R200	=	65767;
		const static int datalog_R3	=	65774;
		const static int datalog_R30	=	65770;
		const static int datalog_R300	=	65766;
		const static int datalog_R6	=	65773;
		const static int datalog_R60	=	65769;
		const static int datalog_R600	=	65765;
		const static int datalog_Tau0	=	65798;
		const static int datalog_Tau0_text1	=	65799;
		const static int datalog_Tau0_text2	=	65800;
		const static int datalog_YP	=	65785;
		const static int datalog_YP_text1	=	65786;
		const static int datalog_YP_text2	=	65787;
		const static int datalog_comment_date	=	65677;
		const static int datalog_comments	=	65676;
		const static int datalog_end	=	65832;
		const static int datalog_export	=	65675;
		const static int datalog_home	=	65829;
		const static int datalog_lvdata	=	65801;
		const static int datalog_nHB	=	65793;
		const static int datalog_nHB_text1	=	65794;
		const static int datalog_nPL	=	65788;
		const static int datalog_nPL_text1	=	65789;
		const static int datalog_next	=	65831;
		const static int datalog_obm	=	65689;
		const static int datalog_page	=	65827;
		const static int datalog_prev	=	65828;
		const static int datalog_record	=	65687;
		const static int datalog_search	=	65674;
		const static int datalog_tableft	=	65685;
		const static int datalog_tabright	=	65686;
		const static int datalog_timeend	=	65673;
		const static int datalog_timestart	=	65672;
		const static int datalog_title0	=	65678;
		const static int datalog_title1	=	65679;
		const static int datalog_title2	=	65680;
		const static int datalog_title3	=	65681;
		const static int datalog_title4	=	65682;
		const static int datalog_title5	=	65683;
		const static int datalog_title6	=	65684;
		const static int datalog_total	=	65830;
		const static int datalog_wbm	=	65688;
		const static int date_time	=	65564;
		const static int datetime	=	65572;
		const static int den_avg	=	65817;
		const static int den_max	=	65816;
		const static int den_min	=	65818;
		const static int density_alert_down	=	65643;
		const static int density_alert_up	=	65642;
		const static int des	=	65864;
		const static int device_ip	=	65741;
		const static int edit_density	=	65559;
		const static int edit_name	=	20001;
		const static int edit_temp	=	65561;
		const static int enable_cyclic_clean	=	65758;
		const static int ev0_flow	=	65722;
		const static int ev_set	=	65721;
		const static int event_cpp	=	65693;
		const static int evn_flow	=	65723;
		const static int evn_press	=	65724;
		const static int fr	=	65720;
		const static int hardware_lights	=	65539;
		const static int home_page	=	65585;
		const static int khb	=	65825;
		const static int kpl	=	65823;
		const static int login_btn_close	=	65566;
		const static int login_btn_login	=	65569;
		const static int logs_chk_datalogs	=	65668;
		const static int logs_chk_oplogs	=	65669;
		const static int logs_page	=	65600;
		const static int lt	=	65725;
		const static int lv_info	=	65545;
		const static int lv_oplog	=	65690;
		const static int muti_cl	=	65646;
		const static int muti_density	=	65641;
		const static int muti_ph	=	65651;
		const static int muti_pv	=	65628;
		const static int muti_temp	=	65637;
		const static int muti_yp	=	65633;
		const static int next_clean_time	=	65757;
		const static int nhb	=	65824;
		const static int npl	=	65822;
		const static int oplog_end	=	65699;
		const static int oplog_home	=	65696;
		const static int oplog_next	=	65698;
		const static int oplog_page	=	65694;
		const static int oplog_prev	=	65695;
		const static int oplog_total	=	65697;
		const static int page	=	65550;
		const static int page_advance_debug	=	65702;
		const static int page_advance_init	=	65703;
		const static int page_datalogs	=	65670;
		const static int page_oplogs	=	65671;
		const static int ph_alert_down	=	65653;
		const static int ph_alert_up	=	65652;
		const static int press_avg	=	65804;
		const static int pv	=	65820;
		const static int pv_alert_down	=	65630;
		const static int pv_alert_up	=	65629;
		const static int sample_index	=	65733;
		const static int sensors_Cl_	=	65753;
		const static int sensors_ES	=	65751;
		const static int sensors_PH	=	65754;
		const static int sensors_obm	=	65750;
		const static int sensors_obm_wnd	=	65749;
		const static int sensors_wbm	=	1000031;
		const static int sensors_wbm_wnd	=	65752;
		const static int set_ev_v0	=	65726;
		const static int speed_0_title	=	65604;
		const static int speed_0_value	=	65605;
		const static int speed_10_title	=	65620;
		const static int speed_10_value	=	65621;
		const static int speed_1_title	=	65610;
		const static int speed_1_value	=	65611;
		const static int speed_2_title	=	65616;
		const static int speed_2_value	=	65617;
		const static int speed_3_title	=	65622;
		const static int speed_3_value	=	65623;
		const static int speed_4_title	=	65606;
		const static int speed_4_value	=	65607;
		const static int speed_5_title	=	65612;
		const static int speed_5_value	=	65613;
		const static int speed_6_title	=	65618;
		const static int speed_6_value	=	65619;
		const static int speed_7_title	=	65624;
		const static int speed_7_value	=	65625;
		const static int speed_8_title	=	65608;
		const static int speed_8_value	=	65609;
		const static int speed_9_title	=	65614;
		const static int speed_9_value	=	65615;
		const static int state_bg	=	65584;
		const static int state_title	=	65570;
		const static int system_chk_adv	=	65576;
		const static int system_chk_home	=	65574;
		const static int system_chk_logs	=	65575;
		const static int system_menu	=	65578;
		const static int system_setup	=	65577;
		const static int temp_alert_down	=	65639;
		const static int temp_alert_up	=	65638;
		const static int temp_avg	=	65803;
		const static int text	=	65558;
		const static int text_cl	=	65645;
		const static int text_code	=	65548;
		const static int text_des	=	65549;
		const static int text_pv	=	65627;
		const static int text_time	=	65546;
		const static int text_time_end	=	65547;
		const static int text_total	=	65553;
		const static int text_yp	=	65632;
		const static int time	=	65691;
		const static int title	=	65863;
		const static int ty	=	65826;
		const static int user	=	65692;
		const static int user_name	=	65573;
		const static int val	=	65836;
		const static int warning_lights	=	65541;
		const static int well_name	=	65571;
		const static int winedit_password	=	65568;
		const static int winedit_user	=	65567;
		const static int wits_ip	=	65743;
		const static int wits_port	=	65744;
		const static int wnd_bg	=	65802;
		const static int wnd_ph_data	=	65650;
		const static int xml_home	=	65537;
		const static int xml_login	=	65536;
		const static int yp	=	65821;
		const static int yp_alert_down	=	65635;
		const static int yp_alert_up	=	65634;
		}id;

		class _string{
		public:
		const static int title	=	0;
		const static int ver	=	1;
		}string;

		class _color{
		public:
		const static int blue	=	0;
		const static int gray	=	1;
		const static int green	=	2;
		const static int red	=	3;
		const static int white	=	4;
		}color;

	};

#ifdef R_IN_CPP
	 extern const _R R;
	 extern const _UIRES UIRES;
#else
	 extern const __declspec(selectany) _R & R = _R();
	 extern const __declspec(selectany) _UIRES & UIRES = _UIRES();
#endif//R_IN_CPP
}
