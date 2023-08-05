#pragma once
#include <vector>
#include <list>
#include <map>
#include "SystemDef.h"
#include <string>
#include <opencv2\opencv.hpp>
using namespace cv;
enum CalcModel
{
	auto_model = 0,
	HB_model,
	PL_model,
	BH_model,
	NT_model
};

class AnalysisTools {
public:
	/**
	@brief Calculates the value of y using the formula: y = log(x * a - 16) - k - b * log(x) + log(70 + x * dtDc)
	@param x Input value of x
	@param a Value of a
	@param k Value of k
	@param b Value of b
	@param dtDc Value of dtDc
	@return Calculated value of y
	*/
	static double GetReyBaseFun(double a, double k, double b, double dtDc, double x)
	{
		return log(x * a - 16) - k - b * log(x) + log(70 + x * dtDc);
	}

	/**
	 * @brief 原方程 y = log(x * a - 16) - k - b * log(x) + log(70 + x * dtDc);
	 * @details 导数方程 y = a / (a * x - 16) - (b / x) + dtDc / (70 + x * dtDc);
	 * @return y 计算得到的导数值
	 */
	static double GetReyDrvtFun(double a, double k, double b, double dtDc, double x)
	{
		return a / (a * x - 16) - (b / x) + dtDc / (70 + x * dtDc);
	}

	/**
	* @brief 对应不同转速值 x，不同模型返回对应 y 的数组
	* @param popt 方程系数数组
	* @param model 方程选择类型
	* @return y 数组，包含计算得到的 y 值
	*/
	static std::vector<double> GetFann(std::vector<double>& popt, CalcModel model)
	{
		std::vector <double> fann;
		std::vector<double> shearRate = { 1021.8, 510.9, 340.6, 170.3, 102.18, 51.09, 34.06, 17.03, 10.22, 5.11, 0 };
		switch (model)
		{
		case HB_model:
		{
			for (auto x : shearRate)
			{
				fann.push_back((popt[1] * pow(x, popt[0]) + popt[2]) / 0.511);
			}
		}
		break;
		case PL_model:
		{
			for (auto x : shearRate)
			{
				fann.push_back((popt[1] * pow(x, popt[0])) / 0.511);
			}
		}
		break;
		case BH_model:
		{
			for (auto x : shearRate)
			{
				fann.push_back((popt[0] * x + popt[1]) / 0.511);
			}
		}
		break;
		case NT_model:
		{
			for (auto& x : shearRate)
			{
				fann.push_back((popt[0] * x) / 0.511);
			}
		}
		break;
		default:
			break;
		}
		return fann;
	}
	/**
	 * @brief 非线性拟合算法实现，输入实际点坐标（x，y），通过不同参数范围拟合区间限定，
	 * 以及输入期望拟合方程公式，返回拟合方程系数
	 * @param shearRate 实际点坐标 x
	 * @param shearStress 实际点坐标 y
	 * @param poptBH 结果拟合方程 BH 系数
	 * @param poptPL 结果拟合方程 PL 系数
	 * @param poptHB 结果拟合方程 HB 系数
	 * @param poptL 结果拟合方程 L 系数
	 * @param rangePL 参数范围拟合区间 PL
	 * @param rangeHB 参数范围拟合区间 HB
	 * @return 拟合是否成功
	 */
	static bool LinearRegression(std::vector<double>& shearRate, std::vector<double>& shearStress,
		std::vector<double>& poptBH, std::vector<double>& poptPL, std::vector<double>& poptHB, std::vector<double>& poptL,
		std::vector<double>& rangePL, std::vector<double>& rangeHB);

	/**
	 * @brief 牛顿迭代法求输入方程的根
	 * @param a 方程系数
	 * @param k 方程系数
	 * @param b 方程系数
	 * @param dtDc 方程系数
	 * @return 根的值
	 */
	static double GetReyCPP(double a, double k, double b, double dtDc);
	/**
	 * @brief 多项式拟合算法进行系数回归
	 * @param x 坐标 x
	 * @param y 坐标 y
	 * @param result 系数回归
	 * @return 状态码
	 */
	static int polyfit(const std::vector<double>& x, const std::vector<double>& y, std::vector<double>& result);
	/**
	 * @brief 使用均方误差（MSE）评估 R2
	 * @param stress0 第一条线段 y 数列
	 * @param stress1 第二条线段 y 数列
	 * @return R2 值
	 */
	static double GetR2(std::vector<double> stress0, std::vector<double> stress1);
};


template<typename T = float>
class DataClean
{
public:
	/**
	 * @brief 计算数列的均值
	 * @param arr 数列
	 * @return 均值
	 */
	inline static T Average(const std::vector<T>& sequence)
	{
		T sum = 0;
		for (auto it : sequence) // 求和
			sum += it;
		return sum / sequence.size(); // 得到平均值
	}
	/**
	 * @brief 计算数列的方差
	 * @param arr 数列
	 * @return 方差
	 */
	inline static T Variance(const std::vector<T>& sequence)
	{
		T sum = 0;
		T avg = Average(sequence);
		for (auto it : sequence)
			sum += (T)std::pow(it - avg, 2);
		return sum / sequence.size(); // 得到方差
	}
	/**
	 * @brief 计算数列的标准差
	 * @param arr 数列
	 * @return 标准差
	 */
	inline static T StandardDev(const std::vector<T>& sequence)
	{
		T var = Variance(sequence);
		return sqrt(var); // 得到标准差
	}

	/**
	 * @brief 获取两个数列同时递增的最长序列数据
	 * @param yVals 数列1
	 * @param xVals 数列2
	 * @return 最大递增数列值序号
	 */
	inline static std::vector<uint16_t> MaxSequence(std::vector<T>& yVals, std::vector<T>& xVals)
	{
		std::vector<uint16_t> indexs;
		if (yVals.size() != xVals.size()) return indexs;
		std::vector<double> yTemp, xTemp;

		POINT maxPos = { 1,-1 }; //x 最长序列长度，y 最长序列上一个 索引位置
		uint16_t lastPos = -1;     //保存最后一个节点的信息。
		std::vector<POINT> tableNode(yVals.size());
		tableNode[0] = maxPos;
		for (int i = 1; i < yVals.size(); i++)
		{
			tableNode[i].x = 1;
			tableNode[i].y = -1;
			for (int j = 0; j < i; j++)
			{
				if (yVals[i] > yVals[j]
					&& xVals[i] > xVals[j])
				{
					if (tableNode[i].x < tableNode[j].x + 1)
					{
						tableNode[i].x = tableNode[j].x + 1;
						tableNode[i].y = j;
						if (maxPos.x < tableNode[i].x)
						{
							maxPos.x = tableNode[i].x;
							maxPos.y = j;
							lastPos = i;
						}
					}
				}
			}
		}

		yTemp.push_back(yVals[lastPos]);
		xTemp.push_back(xVals[lastPos]);
		indexs.push_back(lastPos);
		while (maxPos.y != -1)
		{
			uint16_t pos = (uint16_t)maxPos.y;
			yTemp.insert(yTemp.begin(), yVals[pos]);
			xTemp.insert(xTemp.begin(), xVals[pos]);
			indexs.insert(indexs.begin(), pos);
			maxPos = tableNode[pos];
		}
		std::swap(yVals, yTemp);
		std::swap(xVals, xTemp);
		return indexs;
	}

	/**
	 * @brief 在从小到大的有序数列中找到目标值右领域的第一个位置
	 * @param seqs 从小到大的有序数列
	 * @param target 目标位置值
	 * @return 大于等于 target 的第一个位置
	 */
	inline 	static int RightFirstPos(const std::vector<T>& seqs, const T target)
	{
		int low = 0;
		int high = static_cast<int>(seqs.size()) - 1;
		int mid = 0;
		while (low < high)
		{
			mid = low + (high - low) / 2;
			if (seqs[mid] < target)
				low = mid + 1;
			else
				high = mid;
		}
		return (seqs[low] >= target ? low : low + 1);
	}
};


struct ProcessData
{
	double avgPressure = 0;
	double avgDiffPressure = 0;
	double avgMassFlow = 0;
	double avgTemperature = 0;
	double avgTestDensity = 0;
	double volumetricFlow = 0;
	double pressureLossHel = 0;
};


class  AnalysisSession
{

public:
	using FloatDataClean = DataClean<float>;
	using DoubleDataClean = DataClean<double>;
	using VectorSample = std::vector <SampleData>;
	AnalysisSession(const AnalysisConfig& cfg);
	~AnalysisSession();
	/**
	 * @brief 数据分析入口函数
	 * @param mapIndexSample 数据源
	 * @return 分析状态码
	 */
	int Analysis(std::map<uint16_t, VectorSample> mapIndexSample);
protected:
	/**
	 * @brief 数据预处理函数
	 * @param vInSample 数据源数组
	 * @return 预处理结构
	 */
	ProcessData PreClean(std::vector<SampleData>& vInSample);
	/**
	 * @brief 使用2Sigma原则清洗数据，然后获取均值
	 * @param inputs 输入数列
	 * @return 均值
	 */
	float GetMeanFromSigmaClean(std::vector<float>& inputs);

	/**
	 * @brief 获取满足差压和流量同步递减的最大数列
	 * @return 处理状态码
	 */
	int MaxFlowSequence();
	/**
	 * @brief 数值计算入口
	 * @return 计算结果状态码
	 */
	int Calc();
	/**
	 * @brief 通过代入不同a,b值完成运算
	 * @return 运算结果码
	 */
	int CalcWithAB(double a, double b);
	/**
	 * @brief 根据输入模型优选模型，选择回归后方程的各个参数
	 * @param model 输入优选模型
	 * @param record 输出优选结果记录
	 * @param lvPoint 真实点坐标
	 * @param shearRateHelTemp 拟合点x
	 * @param shearStressTemp 拟合点y
	 * @param poptL 拟合L模型系数
	 * @param poptBH 拟合BH模型系数
	 * @param poptPL 拟合PL模型系数
	 * @param poptHB 拟合HB模型系数
	 */
	void TaylorExpansion(CalcModel model, RecordData& record, std::vector<RealF>& lvPoint, std::vector<double>& shearRateHelTemp, std::vector<double>& shearStressTemp,
		std::vector<double>& poptL, std::vector<double>& poptBH, std::vector<double>& poptPL, std::vector<double>& poptHB);
private:
	std::vector<ProcessData> processDataBase_;	//计算过程值
	AnalysisConfig config_;//输入辅助计算配置
	RecordData processRecord; //计算过程记录
public:
	std::map <double, std::vector<RealF>>  realPoint_;	//真实点数据  
	std::map<double, RecordData> caclRecord_; //计算结果记录
	std::map<std::string, CalcModel> mapCalcModel_{ {"AUTO",auto_model},{"Herschel-Bulkley",HB_model} ,
		{"Power Law",PL_model},{"BingHam",BH_model} ,{"Newtonian",NT_model} }; //模式字符映射关系
};

