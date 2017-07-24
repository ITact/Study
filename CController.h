#ifndef CCONTROLLER_H
#define CCONTROLLER_H

#include <vector>
#include <sstream>
#include <string>
#include <windows.h>

#include "CMinesweeper.h"
#include "CGenAlg.h"
#include "utils.h"
#include "C2DMatrix.h"
#include "SVector2D.h"
#include "CParams.h"


using namespace std;



class CController
{

private:

	vector<SGenome> m_vecThePopulation; //Ⱦɫ����Ⱥ
 
	vector<CMinesweeper> m_vecSweepers; //ɨ�׻�����

	//����
	vector<SVector2D> m_vecMines; //���׼���

	//pointer to the GA
	CGenAlg* m_pGA;

	int m_NumSweepers;

	int m_NumMines;

	//������Ȩ�ص�����
	int m_NumWeightsInNN;

	//ɨ�׻���״����
	vector<SPoint> m_SweeperVB;

	//������״�Ķ���
	vector<SPoint> m_MineVB;

	//�洢ÿһ����ƽ����Ӧ����
	vector<double> m_vecAvFitness; 

	//�洢ÿһ����õ���Ӧ����
	vector<double> m_vecBestFitness;

	//��ͬ��ɫ�Ļ���
	HPEN m_RedPen;
	HPEN m_BluePen;
	HPEN m_GreenPen;
	HPEN m_OldPen;
	
	HWND m_hwndMain;

	bool m_bFastRender;
	
	int m_iTicks;

	//��������
	int m_iGenerations;

  // ���ڳߴ�
  int cxClient, cyClient;

  void PlotStats(HDC surface);


public:

	CController(HWND hwndMain);

	~CController();


	void Render(HDC surface);

	void WorldTransform(vector<SPoint> &VBuffer, SVector2D vPos);
	
	bool Update();


	//accessor methods
	bool FastRender() {return m_bFastRender;}
	void FastRender(bool arg){m_bFastRender = arg;}
	void FastRenderToggle()  {m_bFastRender = !m_bFastRender;}

};

#endif
	
