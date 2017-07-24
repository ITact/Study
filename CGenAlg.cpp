#include "CGenAlg.h"

CGenAlg::CGenAlg(int	  popsize,
                 double	MutRat,
                 double	CrossRat,
                 int	  numweights,
                 vector<int> splits):	m_iPopSize(popsize),
                                      m_dMutationRate(MutRat),
										                  m_dCrossoverRate(CrossRat),
										                  m_iChromoLength(numweights),
										                  m_dTotalFitness(0),
										                  m_cGeneration(0),
										                  m_iFittestGenome(0),
										                  m_dBestFitness(0),
										                  m_dWorstFitness(99999999),
										                  m_dAverageFitness(0),
                                      m_vecSplitPoints(splits)
{
	//使用随机的权重和适应性分数初始化种群
	for (int i=0; i<m_iPopSize; ++i)
	{
		m_vecPop.push_back(SGenome());

		for (int j=0; j<m_iChromoLength; ++j)
		{
			m_vecPop[i].vecWeights.push_back(RandomClamped());
		}
	}
}

//突变
void CGenAlg::Mutate(vector<double> &chromo)
{
	for (int i=0; i<chromo.size(); ++i)
	{
		if (RandFloat() < m_dMutationRate)
		{
			//随机改变权重
			chromo[i] += (RandomClamped() * CParams::dMaxPerturbation);
		}
	}
}

//轮盘选择算法
SGenome CGenAlg::GetChromoRoulette()
{
	//在0个适应分数总数之间产生一个随机数
	double Slice = (double)(RandFloat() * m_dTotalFitness);

	//选中的染色体
	SGenome TheChosenOne;
	
	//目前的适应性总数
	double FitnessSoFar = 0;
	
	for (int i=0; i<m_iPopSize; ++i)
	{
		FitnessSoFar += m_vecPop[i].dFitness;
		
		//如果这个适应性总数大于随机选中的这个数
		if (FitnessSoFar >= Slice)
		{
			TheChosenOne = m_vecPop[i];
			break;
		}		
	}

	return TheChosenOne;
}

//杂交
void CGenAlg::Crossover(const vector<double> &mum,
                        const vector<double> &dad,
                        vector<double>       &baby1,
                        vector<double>       &baby2)
{
	//如果父母的染色体一模一样，就直接返回父母就好了
	if ( (RandFloat() > m_dCrossoverRate) || (mum == dad)) 
	{
		baby1 = mum;
		baby2 = dad;

		return;
	}

	//选择一个杂交点
	int cp = RandInt(0, m_iChromoLength - 1);

	//产生后代，交换权重。
	for (int i=0; i<cp; ++i)
	{
		baby1.push_back(mum[i]);
		baby2.push_back(dad[i]);
	}

	for (i=cp; i<mum.size(); ++i)
	{
		baby1.push_back(dad[i]);
		baby2.push_back(mum[i]);
	}
	
	
	return;
}

void CGenAlg::CrossoverAtSplits(const vector<double> &mum,
                                const vector<double> &dad,
                                vector<double>       &baby1,
                                vector<double>       &baby2)
{
	if ( (RandFloat() > m_dCrossoverRate) || (mum == dad)) 
	{
		baby1 = mum;
		baby2 = dad;

		return;
	}

	//决定两个杂交点
	int cp1 = m_vecSplitPoints[RandInt(0, m_vecSplitPoints.size()-2)];
	int cp2 = m_vecSplitPoints[RandInt(cp1, m_vecSplitPoints.size()-1)];
  
	for (int i=0; i<mum.size(); ++i)
	{
		if ( (i<cp1) || (i>=cp2) )
		{
			baby1.push_back(mum[i]);
			baby2.push_back(dad[i]);
		}

		else
		{
			baby1.push_back(dad[i]);
			baby2.push_back(mum[i]);
		}
	}	
	return;
}

//繁殖
vector<SGenome> CGenAlg::Epoch(vector<SGenome> &old_pop)
{
	//存储老的一代
	m_vecPop = old_pop;

	Reset();

	sort(m_vecPop.begin(), m_vecPop.end());

	//计算最好最差和平均适应性分数
	CalculateBestWorstAvTot();

	//临时的，存储新的染色体
	vector <SGenome> vecNewPop;

	//精英选择
	if (!(CParams::iNumCopiesElite * CParams::iNumElite % 2))
	{
		GrabNBest(CParams::iNumElite, CParams::iNumCopiesElite, vecNewPop);
	}

	//不断繁殖
	while (vecNewPop.size() < m_iPopSize)
	{
		//产生两个父母
		SGenome mum = GetChromoRoulette();
		SGenome dad = GetChromoRoulette();

		//创建儿子变量
		vector<double>		baby1, baby2;

		CrossoverAtSplits(mum.vecWeights, dad.vecWeights, baby1, baby2);

		//突变
		Mutate(baby1);
		Mutate(baby2);

		vecNewPop.push_back( SGenome(baby1, 0) );
		vecNewPop.push_back( SGenome(baby2, 0) );
	}

	//新的种群放在旧的种群里面
	m_vecPop = vecNewPop;

	return m_vecPop;
}


void CGenAlg::FitnessScaleRank()
{
	const int FitnessMultiplier = 1;

	for (int i=0; i<m_iPopSize; i++)
	{
		m_vecPop[i].dFitness = i * FitnessMultiplier;
	}


	CalculateBestWorstAvTot();
} 

void CGenAlg::GrabNBest(int NBest, const int NumCopies, vector<SGenome>	&Pop)
{

	while(NBest--)
	{
		for (int i=0; i<NumCopies; ++i)
		{
			Pop.push_back(m_vecPop[(m_iPopSize - 1) - NBest]);
	  }
	}
}

//计算种群中适应性分数的最好，最差，平均和总和
void CGenAlg::CalculateBestWorstAvTot()
{
	m_dTotalFitness = 0;
	
	double HighestSoFar = 0;
	double LowestSoFar = 9999999;
	
	for (int i=0; i<m_iPopSize; ++i)
	{
		if (m_vecPop[i].dFitness > HighestSoFar)
		{
			HighestSoFar = m_vecPop[i].dFitness;
			
			m_iFittestGenome = i;

			m_dBestFitness = HighestSoFar;
		}
		
		if (m_vecPop[i].dFitness < LowestSoFar)
		{
			LowestSoFar = m_vecPop[i].dFitness;
			
			m_dWorstFitness = LowestSoFar;
		}
		
		m_dTotalFitness	+= m_vecPop[i].dFitness;
				
	}
	
	m_dAverageFitness = m_dTotalFitness / m_iPopSize;
}


void CGenAlg::Reset()
{

	m_dTotalFitness = 0;
	m_dBestFitness = 0;
	m_dWorstFitness = 9999999;
	m_dAverageFitness = 0;

}

