#ifndef CGENALG_H
#define CGENALG_H

#include <vector>
#include <algorithm>
#include <iostream>

#include "utils.h"
#include "CParams.h"

using namespace std;


//����ṹ��
struct SGenome
{
  vector <double> vecWeights;

	double dFitness;

	SGenome():dFitness(0){}

	SGenome( vector <double> w, double f): vecWeights(w), dFitness(f){}

	//���� <���ţ���Ϊ�Ƚ���Ӧ��
	friend bool operator<(const SGenome& lhs, const SGenome& rhs)
	{
		return (lhs.dFitness < rhs.dFitness);
	}
};


//�Ŵ��㷨��
class CGenAlg
{
private:
	
	//�洢������Ⱥ��Ⱦɫ��
	vector <SGenome>	 m_vecPop;

	//��Ⱥ��ѧ
	int m_iPopSize;
	
	//ÿ��Ⱦɫ���Ȩ��
	int m_iChromoLength;

	//�ӽ��������зֵ�
	vector<int> m_vecSplitPoints;

	//��Ӧ������
	double m_dTotalFitness;

	//��Ⱥ�е������Ӧ��
	double m_dBestFitness;

	//ƽ����Ӧ��
	double m_dAverageFitness;  

	//��������Ӧ��
	double m_dWorstFitness; 

	//����õĻ���ĸ���
	int		m_iFittestGenome;

	//ͻ����
	double m_dMutationRate;

	//�ӽ���
	double m_dCrossoverRate;

	//�ڼ���
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

	//��ֳ�㷨
	vector<SGenome>	Epoch(vector<SGenome> &old_pop);

	vector<SGenome>	GetChromos()const{return m_vecPop;}
	double AverageFitness()const{return m_dTotalFitness / m_iPopSize;}
	double BestFitness()const{return m_dBestFitness;}
};

#endif

