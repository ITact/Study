#ifndef CPARAMS_H
#define CPARAMS_H

#include <fstream>
#include <windows.h>

using namespace std;	



class CParams
{

public:

  static double dPi;
  static double dHalfPi;
  static double dTwoPi;

  static int    WindowWidth;
  static int    WindowHeight;

  static int    iFramesPerSecond;

  
  static int    iNumInputs;
  static int    iNumHidden;
  static int    iSshenjingPerHiddenLayer;
  static int    iNumOutputs;

  static double dActivationResponse;
  static double dBias;

  static double dEnergyCostPerTick;

  static double dMaxTurnRate;

  static double dMaxSpeed;

  static int   iSweeperScale;

  static double dStartEnergy;

  static int    iNumSweepers;

  static int    iNumMines;

  static int    iNumTicks;

  static double dMineScale;

  static double dCrossoverRate;
  static double dMutationRate;

  static double dMaxPerturbation;

  static int    iNumElite;
  static int    iNumCopiesElite;

  CParams()
  {
    if(!LoadInParameters("params.ini"))
    {
      MessageBox(NULL, "Cannot find ini file!", "Error", 0);
    }
  }

  bool LoadInParameters(char* szFileName);
};






#endif