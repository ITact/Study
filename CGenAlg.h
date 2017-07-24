#ifndef CGENALG_H
#define CGENALG_H

#include <vector>
#include <algorithm>
#include <iostream>

#include "utils.h"
#include "CParams.h"

using namespace std;


//基因结构体
struct SGenome
{
  vector <double> vecWeights;

	double dFitness;

	SGenome():dFitness(0){}

	SGenome( vector <double> w, double f): vecWeights(w), dFitness(f){}

	//重载 <符号，改为比较适应性
	friend bool operator<(const SGenome& lhs, const SGenome& rhs)
	{
		return (lhs.dFitness < rhs.dFitness);
	}
};


//遗传算法类
class CGenAlg
{
private:
	
	//存储整个种群的染色体
	vector <SGenome>	 m_vecPop;

	//种群大学
	int m_iPopSize;
	
	//每个染色体的权重
	int m_iChromoLength;

	//杂交操作的切分点
	vector<int> m_vecSplitPoints;

	//适应性总数
	double m_dTotalFitness;

	//种群中的最佳适应数
	double m_dBestFitness;

	//平均适应性
	double m_dAverageFitness;  

	//最糟糕的适应性
	double m_dWorstFitness; 

	//对最好的基因的跟踪
	int		m_iFittestGenome;

	//突变率
	double m_dMutationRate;

	//杂交率
	double m_dCrossoverRate;

	//第几代
	int m_cGeneration;

	void Crossover(const vector<double> &mum,
                     const vector<double> &dad,
                     vector<double>       &baby1,
                     vector<double>       &baby2);

  void CrossoverAtSplits(const vector<double> &mum,
                             const vector<double> &dad,
                             vector<double>       &baby1,
                             vector<double>       &baby2);
	

	void Mutate(vector<double> &chromo);
	
	SGenome	GetChromoRoulette();

	void GrabNBest(int	            NBest,
	                  const int       NumCopies,
	                  vector<SGenome>	&vecPop);

	void FitnessScaleRank();

	void CalculateBestWorstAvTot();

	void Reset();



public:
		
	CGenAlg(int popsize, double	MutRat, 
		double	CrossRat, int numweights, 
		vector<int> splits);

	//繁殖算法
	vector<SGenome>	Epoch(vector<SGenome> &old_pop);

	vector<SGenome>	GetChromos()const{return m_vecPop;}
	double AverageFitness()const{return m_dTotalFitness / m_iPopSize;}
	double BestFitness()const{return m_dBestFitness;}
};

#endif

