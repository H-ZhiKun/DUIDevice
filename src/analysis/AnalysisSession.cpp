
// ProDataDll.cpp : 定义 DLL 的导出函数。
//
#include "stdafx.h"
#include "AnalysisSession.h"
#include <map>
#include <cmath>
#include <algorithm>
#include "Utils.h"
#include <opencv2/imgproc/imgproc_c.h>
#include "Logger.h"
#include "HttpClient.hpp"

bool AnalysisTools::LinearRegression(std::vector<double>& shearRate, std::vector<double>& shearStress,
	std::vector<double>& poptBH, std::vector<double>& poptPL, std::vector<double>& poptHB, std::vector<double>& poptL,
	std::vector<double>& rangePL, std::vector<double>& rangeHB)
{
	std::string res;
	Json::Value jsRoot;
	for (size_t i = 0; i < shearRate.size(); i++)
	{
		jsRoot["x"].append(shearRate[i]);
		jsRoot["y"].append(shearStress[i]);
	}
	for (const auto& val: rangePL)
	{
		jsRoot["rangePL"].append(val);
	}
	for (const auto& val : rangeHB)
	{
		jsRoot["rangeHB"].append(val);
	}
	std::string strData = Utils::JsonToStr(jsRoot);
	HttpClient::post("127.0.0.1", "65120", "/getRegression", strData, res);
	if (!res.empty())
	{
		Json::Value val = Utils::StrToJson(res);
		if (val.isMember("poptL"))
		{
			for (const auto& val : val["poptL"])
			{
				poptL.push_back(val.asDouble());
			}
			for (const auto& val : val["poptBH"])
			{
				poptBH.push_back(val.asDouble());
			}
			for (const auto& val : val["poptPL"])
			{
				poptPL.push_back(val.asDouble());
			}
			for (const auto& val : val["poptHB"])
			{
				poptHB.push_back(val.asDouble());
			}
		}
	}
	return true;
}

double AnalysisTools::GetReyCPP(double a, double k, double b, double dtDc)
{
	double f1, f2, d;
	int count = 0;  //统计迭代的次数
	double x = 1 + 16 / a;    //随机赋予的方程的解
	do {
		f1 = GetReyBaseFun(a, k, b, dtDc, x);   //方程的值 
		f2 = GetReyDrvtFun(a, k, b, dtDc, x);   //方程的导数 
		d = f1 / f2;//迭代步进
		x -= d;
		count++;
		if (count >= 20)
		{
			break;
		}
	} while (fabs(d) > 1e-5);
	return x;
}


int AnalysisTools::polyfit(const std::vector<double>& x, const std::vector<double>& y, std::vector<double>& result)
{
	cv::Mat A = cv::Mat::zeros(cv::Size(3, (int)x.size()), CV_64FC1);
	for (int i = 0; i < x.size(); i++)
	{
		A.at<double>(i, 0) = 1;
		A.at<double>(i, 1) = x[i];
		A.at<double>(i, 2) = x[i] * x[i];
	}

	cv::Mat b = cv::Mat::zeros(cv::Size(1, (int)y.size()), CV_64FC1);
	for (int i = 0; i < y.size(); i++)
	{
		b.at<double>(i, 0) = y[i];
	}

	cv::Mat c;
	c = A.t() * A;
	cv::Mat d;
	d = A.t() * b;

	cv::Mat args = cv::Mat::zeros(cv::Size(1, 3), CV_64FC1);
	cv::solve(c, d, args);
	double a0 = args.at<double>(0, 0);
	double a1 = args.at<double>(1, 0);
	double a2 = args.at<double>(2, 0);
	for (const auto& x0: x)
	{
		result.push_back(2 * a2 * x0 + a1);
	}
	return 0;
}



double AnalysisTools::GetR2(std::vector<double> stress0, std::vector<double> stress1)
{
	double R2 = 0.0;
	std::string res;
	Json::Value jsRoot;
	for (int i = 0; i < stress0.size(); i++)
	{
		jsRoot["x"].append(stress0[i]);
		jsRoot["y"].append(stress1[i]);
	}
	std::string strData = Utils::JsonToStr(jsRoot);
	HttpClient::post("127.0.0.1", "65120", "/getR2", strData, res);
	if (!res.empty())
	{
		Json::Value val = Utils::StrToJson(res);
		if (val.isMember("data"))
		{
			R2 = val["data"].asDouble();
		}
	}
	return R2;
}

AnalysisSession::AnalysisSession(const AnalysisConfig& cfg)
	:config_(cfg)
{
}

AnalysisSession::~AnalysisSession()
{
}

int AnalysisSession::Analysis(std::map<uint16_t, VectorSample> mapIndexSample)
{
	//1:预处理部分
	size_t mapCount = 0;
	for (auto& [index,samples] : mapIndexSample)
	{
		if (samples.size() < 10)
			continue;
		ProcessData data = PreClean(samples);
		processRecord.avg_density += (float)data.avgTestDensity;
		processRecord.avg_temp += (float)data.avgTemperature;
		processRecord.avg_pressure += (float)data.avgPressure;
		processDataBase_.push_back(data);
		++mapCount;
	}
	if (processDataBase_.size() < 10) 
		return -1;
	processRecord.avg_density /= mapCount;
	processRecord.avg_temp /= mapCount;
	processRecord.avg_pressure /= mapCount;
	MaxFlowSequence();
	//2:计算部分
	return Calc();
}

ProcessData AnalysisSession::PreClean(std::vector<SampleData>& vInSample)
{
	ProcessData data;
	std::vector<float> vFlow, vUpPress, vDownPress, vTemp, vDensity, vPumpPressure;

	for (auto& it : vInSample)
	{
		vFlow.push_back(it.massFlow);
		vUpPress.push_back(it.fUpstreamPressure);
		vDownPress.push_back(it.fDownstreamPressure);
		vTemp.push_back(it.temperature);
		vDensity.push_back(it.density);
		vPumpPressure.push_back(it.pumpPressure);
	}

	std::sort(vFlow.begin(), vFlow.end());
	std::sort(vUpPress.begin(), vUpPress.end());
	std::sort(vDownPress.begin(), vDownPress.end());
	std::sort(vDensity.begin(), vDensity.end());
	std::sort(vTemp.begin(), vTemp.end());
	std::sort(vPumpPressure.begin(), vPumpPressure.end());
	//sigma
	processRecord.max_temp = (std::max)(processRecord.max_temp, vTemp[vTemp.size() - 1]);
	processRecord.min_temp = (std::min)(processRecord.min_temp, vTemp[0]);
	processRecord.max_density = (std::max)(processRecord.max_density, vDensity[vDensity.size() - 1]);
	processRecord.min_density = (std::min)(processRecord.min_density, vDensity[0]);
	processRecord.max_pressure = (std::max)(processRecord.max_pressure, vPumpPressure[vPumpPressure.size() - 1]);
	processRecord.min_pressure = (std::min)(processRecord.min_pressure, vPumpPressure[0]);

	float meanUpPress = GetMeanFromSigmaClean(vUpPress);
	float meanDownPress = GetMeanFromSigmaClean(vDownPress);

	data.avgPressure = (vPumpPressure[vPumpPressure.size() - 1] + vPumpPressure[0]) / 2;
	data.avgTemperature = (vTemp[vTemp.size() - 1] + vTemp[0]) / 2;
	data.avgDiffPressure = meanUpPress - meanDownPress;
	data.avgTestDensity = GetMeanFromSigmaClean(vDensity);
	data.avgMassFlow = GetMeanFromSigmaClean(vFlow);
	data.volumetricFlow = data.avgMassFlow / data.avgTestDensity * 1000 / 60;
	data.pressureLossHel = data.avgDiffPressure / config_.fLength;
	return data;
}

float AnalysisSession::GetMeanFromSigmaClean(std::vector<float>& inputs)
{
	std::vector<float> temp;
	float mean = FloatDataClean::Average(inputs);
	float sigma = FloatDataClean::StandardDev(inputs);
	int i = FloatDataClean::RightFirstPos(inputs, mean - 2 * sigma);
	for (; i < inputs.size(); i++)
	{
		if (inputs[i] > mean + 2 * sigma)
		{
			break;
		}
		temp.push_back(inputs[i]);
	}
	return FloatDataClean::Average(temp);
}


int AnalysisSession::MaxFlowSequence()
{
	//1按照流量递减 找出最长子序列
	std::vector <ProcessData> sortFlow;

	POINT maxPos = { 1,-1 }; //x 最长序列长度，y 最长序列上一个 索引位置
	ProcessData maxData;     //保存最后一个节点的信息。
	std::vector<POINT> tableMaxLength(processDataBase_.size());
	tableMaxLength[0] = maxPos;
	for (int i = 1; i < processDataBase_.size(); i++)
	{
		tableMaxLength[i].x = 1;
		tableMaxLength[i].y = -1;
		for (int j = 0; j < i; j++)
		{
			if (processDataBase_[i].avgMassFlow < processDataBase_[j].avgMassFlow
				&& processDataBase_[i].avgDiffPressure < processDataBase_[j].avgDiffPressure)
			{
				if (tableMaxLength[i].x < tableMaxLength[j].x + 1)
				{
					tableMaxLength[i].x = tableMaxLength[j].x + 1;
					tableMaxLength[i].y = j;
					if (maxPos.x < tableMaxLength[i].x)
					{
						maxPos.x = tableMaxLength[i].x;
						maxPos.y = j;
						maxData = processDataBase_[i];
					}
				}
			}
		}
	}

	sortFlow.push_back(maxData);
	while (maxPos.y != -1)
	{
		sortFlow.push_back(processDataBase_[maxPos.y]);
		maxPos = tableMaxLength[maxPos.y];
	}

	if (sortFlow.size() < 6) return -1;
	std::swap(processDataBase_, sortFlow);

	return 0;
}


int AnalysisSession::Calc()
{
	int ret = 0;
	std::map<float, float> chooseABList;

	double densityTemp = 0;
	double densityTemp1 = 0;
	double densityTemp2 = 0;
	std::map<double, std::list<ABValue>> tempABChoose;
	for (int i = 0; i < config_.lvABInfo_.size(); i++)
	{
		densityTemp1 = fabs(config_.lvABInfo_[i].fDensityRange0 - processDataBase_[0].avgTestDensity);
		densityTemp2 = fabs(config_.lvABInfo_[i].fDensityRange1 - processDataBase_[0].avgTestDensity);
		if (densityTemp1 < densityTemp2)
		{
			densityTemp = densityTemp1;
		}
		else
		{
			densityTemp = densityTemp2;
		}
		tempABChoose[densityTemp] = config_.lvABInfo_[i].listAB;
	}
	int chooseIndex = 0;
	for (auto& it : tempABChoose)
	{
		if (chooseIndex == 5)
		{
			break;
		}
		for (auto item : it.second)
		{
			chooseABList[item.a] = item.b;
		}
		chooseIndex++;
	}

	auto iter = chooseABList.begin();
	while (iter != chooseABList.end())
	{
		ret = CalcWithAB(iter->first, iter->second);
		iter++;
	}
	return ret;
}

int AnalysisSession::CalcWithAB(double a, double b)
{
	RecordData record = processRecord;
	double dtDc = sqrt(config_.fdtc / config_.fdc);

	//k = np.log(16 * a * dtDc ** b)  # 使用fc / fs模型2计算Rey
	double k = log(16 * a * pow(dtDc, b));
	std::vector<double> lnShearStressTemp;
	std::vector<double> lnMidTemp;
	std::vector<double> lvShearStress;
	std::vector<double> vHels;
	std::vector<double> N;

	for (const auto& it : processDataBase_)
	{
		double vHel = 4 / acos(-1) / pow(config_.fdtc, 2) * it.volumetricFlow / 1000 / 60;
		double fHel = config_.fdtc / (2 * it.avgTestDensity * pow(vHel, 2)) * it.pressureLossHel * 1000;
		double ReyHel = AnalysisTools::GetReyCPP(fHel, k, b, dtDc);

		if (ReyHel == 0.0f || _isnan(ReyHel))
		{
			LOGINFO("ReyHel error = {}", ReyHel);
			continue;
		}
		double shearStress = 8 * it.avgTestDensity * pow(vHel, 2) / ReyHel;
		lvShearStress.push_back(shearStress);
		lnShearStressTemp.push_back(log(shearStress));
		lnMidTemp.push_back(log(8 * vHel / config_.fdtc));
		vHels.push_back(vHel);
	}
	std::vector<uint16_t> indexs = DoubleDataClean::MaxSequence(lnMidTemp, lnShearStressTemp);
	if (lnShearStressTemp.size() <= 6)
	{
		LOGINFO("lnShearStressTemp size = {} return -1", lnShearStressTemp.size());
		return -1;
	}
	AnalysisTools::polyfit(lnMidTemp, lnShearStressTemp, N);
	if (N.size() == 0)
	{
		LOGINFO("N size = {} return -2", N.size());
		return -2;
	}
	std::vector<double> shearRateHelTemp;
	std::vector<double> shearStressTemp;
	int indexN = 0;
	for (auto index : indexs)
	{
		double rate = 8 * vHels[index] / config_.fdtc * (3 * N[indexN] + 1) / (4 * N[indexN]);
		indexN++;
		double stress = lvShearStress[index];
		if (rate <= 0 || stress <= 0) continue;
		shearRateHelTemp.push_back(rate);
		shearStressTemp.push_back(stress);
	}
	DoubleDataClean::MaxSequence(shearStressTemp, shearRateHelTemp);
	if (shearRateHelTemp.size() <= 4)
	{
		LOGINFO("shearRateHelTemp size = {} return -6", shearRateHelTemp.size());
		return -6;
	}
	auto model = mapCalcModel_[config_.model_];

	std::vector<double> poptL, poptBH, poptPL, poptHB;
	std::vector<double> rangePL = { 0.1, 0.99, 0.05, 10 };
	std::vector<double> rangeHB = { 0.1, 0.99, 0.05, 10, 0, 15};
	AnalysisTools::LinearRegression(shearRateHelTemp, shearStressTemp,
		poptBH, poptPL, poptHB, poptL, rangePL, rangeHB);
	if (poptBH.size() == 0 || poptPL.size() == 0 || poptHB.size() == 0 || poptL.size() == 0)
	{
		LOGINFO("size poptBH={},poptPL={},poptHB={},poptL={} return -10", poptBH.size(), poptPL.size(), poptHB.size(), poptL.size());
		return -10;
	}
	std::vector<RealF> curPoint;
	TaylorExpansion(model, record, curPoint, shearRateHelTemp, shearStressTemp, poptL, poptBH, poptPL, poptHB);
	realPoint_[record.R2] = curPoint;
	caclRecord_[record.R2] = record;
	LOGINFO("model={}, R2={}, R600={},R300={},R3={},point size={}", record.model, record.R2, record.r600, record.r300, record.r3, curPoint.size());

	return 0;
}


//int AnalysisSession::NKReset(float R600, float R300, float R3)
//{
	// 	float nPL = 3.322 * log10(R600 / R300);
	// 
	// 	float KPL = 0.511 * R300 / pow(511, nPL);
	// 
	// 	float nHB = 3.322 * log10((R600 - R3) / (R300 - R3));
	// 
	// 	float KHB = 0.511 * (R300 - R3) / pow(511, nHB);

	//double Tau0 = 0.511 * R3;

// 	NKRangeInfo_.MinNPL = 0.1;
// 	NKRangeInfo_.MaxNPL = 0.99;
// 	NKRangeInfo_.MinKPL = 0.1;
// 	NKRangeInfo_.MaxKPL = 10;
// 
// 	NKRangeInfo_.MinNHB = 0.1;
// 	NKRangeInfo_.MaxNHB = 0.99;
// 	NKRangeInfo_.MinKHB = 0.1;
// 	NKRangeInfo_.MaxKHB = 10;
// 
// 	NKRangeInfo_.MinTau0 = Tau0 - 0.5;
// 	if (NKRangeInfo_.MinTau0 <= 0)
// 	{
// 		NKRangeInfo_.MinTau0 = 0.01;
// 	}
// 	NKRangeInfo_.MaxTau0 = Tau0 + 0.5;
//	LOGINFO("Tau0 Range {} to {}", NKRangeInfo_.MinTau0, NKRangeInfo_.MaxTau0);
// 	return 0;
// }

void AnalysisSession::TaylorExpansion(CalcModel model, RecordData& record, std::vector<RealF>& lvPoint, std::vector<double>& shearRateHelTemp, std::vector<double>& shearStressTemp,
	std::vector<double>& poptL, std::vector<double>& poptBH, std::vector<double>& poptPL, std::vector<double>& poptHB)
{
	std::vector<double> stress0, stress1, stress2, stress3;
	lvPoint.clear();
	for (int i = 0; i < shearRateHelTemp.size(); i++)
	{
		stress0.push_back(shearRateHelTemp[i] * poptL[0]);
		stress1.push_back(shearRateHelTemp[i] * poptBH[0] + poptBH[1]);
		stress2.push_back(pow(shearRateHelTemp[i], poptPL[0]) * poptPL[1]);
		stress3.push_back(pow(shearRateHelTemp[i], poptHB[0]) * poptHB[1] + poptHB[2]);
		RealF pt(shearRateHelTemp[i], shearStressTemp[i]);
		lvPoint.push_back(pt);
	}
	std::map<double, std::string> RetR2;
	std::vector<double> retFannVal;
	switch (model)
	{
	case auto_model:
	{
		RetR2[AnalysisTools::GetR2(shearStressTemp, stress2)] = "PL";
		RetR2[AnalysisTools::GetR2(shearStressTemp, stress3)] = "HB";
		break;
	}
	case BH_model:
	{
		RetR2[AnalysisTools::GetR2(shearStressTemp, stress1)] = "BH";
		break;
	}
	case HB_model:
	{
		RetR2[AnalysisTools::GetR2(shearStressTemp, stress3)] = "HB";
		break;
	}
	case PL_model:
	{
		RetR2[AnalysisTools::GetR2(shearStressTemp, stress2)] = "PL";
		break;
	}
	case NT_model:
	{
		RetR2[AnalysisTools::GetR2(shearStressTemp, stress0)] = "NT";
		break;
	}
	default:
		break;
	}

	if (RetR2.rbegin()->second == "BH")
	{
		retFannVal = AnalysisTools::GetFann(poptBH, BH_model);
		record.model = "BH";
	}
	if (RetR2.rbegin()->second == "PL")
	{
		retFannVal = AnalysisTools::GetFann(poptPL, PL_model);
		record.model = "PL";
	}
	if (RetR2.rbegin()->second == "HB")
	{
		retFannVal = AnalysisTools::GetFann(poptHB, HB_model);
		record.model = "HB";
	}
	if (RetR2.rbegin()->second == "NT")
	{
		retFannVal = AnalysisTools::GetFann(poptL, NT_model);
		record.model = "NT";
	}


	record.pv = (float)(retFannVal[0] - retFannVal[1]);
	record.yp = (float)(0.511 * (2 * retFannVal[1] - retFannVal[0]));
	record.R2 = (float)(RetR2.rbegin()->first);
	record.npl = (float)poptPL[0];
	record.kpl = (float)poptPL[1];
	record.nhb = (float)poptHB[0];
	record.khb = (float)poptHB[1];
	record.tau0 = (float)poptHB[2];

	record.r600 = (float)retFannVal[0];
	record.r300 = (float)retFannVal[1];
	record.r200 = (float)retFannVal[2];
	record.r100 = (float)retFannVal[3];

	record.r60 = (float)retFannVal[4];
	record.r30 = (float)retFannVal[5];
	record.r20 = (float)retFannVal[6];
	record.r10 = (float)retFannVal[7];

	record.r6 = (float)retFannVal[8];
	record.r3 = (float)retFannVal[9];
	record.r0 = (float)retFannVal[10];

	record.av = (float)record.r600 / 2;

	record.nline = (float)poptL[0];
	record.nbh = (float)poptBH[0];
	record.kbh = (float)poptBH[1];
}




