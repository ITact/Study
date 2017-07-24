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
	//ʹ�������Ȩ�غ���Ӧ�Է�����ʼ����Ⱥ
	for (int i=0; i<m_iPopSize; ++i)
	{
		m_vecPop.push_back(SGenome());

		for (int j=0; j<m_iChromoLength; ++j)
		{
			m_vecPop[i].vecWeights.push_back(RandomClamped());
		}
	}
}

//ͻ��
void CGenAlg::Mutate(vector<double> &chromo)
{
	for (int i=0; i<chromo.size(); ++i)
	{
		if (RandFloat() < m_dMutationRate)
		{
			//����ı�Ȩ��
			chromo[i] += (RandomClamped() * CParams::dMaxPerturbation);
		}
	}
}

//����ѡ���㷨
SGenome CGenAlg::GetChromoRoulette()
{
	//��0����Ӧ��������֮�����һ�������
	double Slice = (double)(RandFloat() * m_dTotalFitness);

	//ѡ�е�Ⱦɫ��
	SGenome TheChosenOne;
	
	//Ŀǰ����Ӧ������
	double FitnessSoFar = 0;
	
	for (int i=0; i<m_iPopSize; ++i)
	{
		FitnessSoFar += m_vecPop[i].dFitness;
		
		//��������Ӧ�������������ѡ�е������
		if (FitnessSoFar >= Slice)
		{
			TheChosenOne = m_vecPop[i];
			break;
		}		
	}

	return TheChosenOne;
}

//�ӽ�
void CGenAlg::Crossover(const vector<double> &mum,
                        const vector<double> &dad,
                        vector<double>       &baby1,
                        vector<double>       &baby2)
{
	//�����ĸ��Ⱦɫ��һģһ������ֱ�ӷ��ظ�ĸ�ͺ���
	if ( (RandFloat() > m_dCrossoverRate) || (mum == dad)) 
	{
		baby1 = mum;
		baby2 = dad;

		return;
	}

	//ѡ��һ���ӽ���
	int cp = RandInt(0, m_iChromoLength - 1);

	//�������������Ȩ�ء�
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

	//���������ӽ���
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

//��ֳ
vector<SGenome> CGenAlg::Epoch(vector<SGenome> &old_pop)
{
	//�洢�ϵ�һ��
	m_vecPop = old_pop;

	Reset();

	sort(m_vecPop.begin(), m_vecPop.end());

	//�����������ƽ����Ӧ�Է���
	CalculateBestWorstAvTot();

	//��ʱ�ģ��洢�µ�Ⱦɫ��
	vector <SGenome> vecNewPop;

	//��Ӣѡ��
	if (!(CParams::iNumCopiesElite * CParams::iNumElite % 2))
	{
		GrabNBest(CParams::iNumElite, CParams::iNumCopiesElite, vecNewPop);
	}

	//���Ϸ�ֳ
	while (vecNewPop.size() < m_iPopSize)
	{
		//����������ĸ
		SGenome mum = GetChromoRoulette();
		SGenome dad = GetChromoRoulette();

		//�������ӱ���
		vector<double>		baby1, baby2;

		CrossoverAtSplits(mum.vecWeights, dad.vecWeights, baby1, baby2);

		//ͻ��
		Mutate(baby1);
		Mutate(baby2);

		vecNewPop.push_back( SGenome(baby1, 0) );
		vecNewPop.push_back( SGenome(baby2, 0) );
	}

	//�µ���Ⱥ���ھɵ���Ⱥ����
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

//������Ⱥ����Ӧ�Է�������ã���ƽ�����ܺ�
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

