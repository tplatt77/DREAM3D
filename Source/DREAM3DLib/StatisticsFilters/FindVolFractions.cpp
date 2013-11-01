/* ============================================================================
 * Copyright (c) 2011 Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2011 Dr. Michael A. Groeber (US Air Force Research Laboratories)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Groeber, Michael A. Jackson, the US Air Force,
 * BlueQuartz Software nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  This code was written under United States Air Force Contract number
 *                           FA8650-07-D-5800
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "FindVolFractions.h"

#include <sstream>

#include "DREAM3DLib/Common/Constants.h"
#include "DREAM3DLib/DataArrays/IDataArray.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindVolFractions::FindVolFractions() :
AbstractFilter(),
m_CellPhasesArrayName(DREAM3D::CellData::Phases),
m_VolFractionsArrayName(DREAM3D::EnsembleData::VolFractions),
m_CellPhases(NULL),
m_VolFractions(NULL)
{

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
FindVolFractions::~FindVolFractions()
{
}
// -----------------------------------------------------------------------------
void FindVolFractions::readFilterParameters(AbstractFilterParametersReader* reader)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindVolFractions::writeFilterParameters(AbstractFilterParametersWriter* writer)

{
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindVolFractions::dataCheck(bool preflight, size_t voxels, size_t fields, size_t ensembles)
{

  setErrorCondition(0);
  std::stringstream ss;
  VoxelDataContainer* m = getVoxelDataContainer();

  GET_PREREQ_DATA(m, DREAM3D, CellData, CellPhases, ss, -301, int32_t, Int32ArrayType, voxels, 1)

  CREATE_NON_PREREQ_DATA(m, DREAM3D, EnsembleData, VolFractions, ss, float, FloatArrayType, 0, ensembles, 1)
}



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindVolFractions::preflight()
{
  dataCheck(true, 1, 1, 1);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void FindVolFractions::execute()
{
  setErrorCondition(0);
  VoxelDataContainer* m = getVoxelDataContainer();
  if(NULL == m)
  {
    setErrorCondition(-999);
    notifyErrorMessage("The DataContainer Object was NULL", -999);
    return;
  }
  setErrorCondition(0);

  int64_t totalPoints = m->getTotalPoints();
  size_t totalFields = m->getNumFieldTuples();
  size_t totalEnsembles = m->getNumEnsembleTuples();
  dataCheck(false, totalPoints, totalFields, totalEnsembles);
  if (getErrorCondition() < 0)
  {
    return;
  }

  for(size_t i = 1; i < totalEnsembles; i++)
  {
    m_VolFractions[i] = 0;
  }
  for(size_t i = 0; i < totalPoints; i++)
  {
    m_VolFractions[m_CellPhases[i]]++;
  }

  notifyStatusMessage("Complete");
}
