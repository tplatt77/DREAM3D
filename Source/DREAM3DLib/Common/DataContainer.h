/* ============================================================================
 * Copyright (c) 2010, Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2010, Dr. Michael A. Groeber (US Air Force Research Laboratories)
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
#ifndef _GrainGeneratorFunc_H
#define _GrainGeneratorFunc_H

#if defined (_MSC_VER)
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif


//-- C++ includes
#include <vector>
#include <map>
#include <sstream>
#include <list>

//-- EBSD Lib Includes
#include "EbsdLib/EbsdConstants.h"

//-- DREAM3D Includes
#include "DREAM3DLib/DREAM3DLib.h"
#include "DREAM3DLib/Common/DREAM3DSetGetMacros.h"
#include "DREAM3DLib/Common/DataArray.hpp"
#include "DREAM3DLib/Common/Field.h"
#include "DREAM3DLib/Common/Observable.h"


#define INITIALIZE_NAMED_ARRAY_TO_PTR(dataContainer, field, name, typeClass, type, size, valuePtr, numComp) \
type* valuePtr = NULL;\
{\
  IDataArray::Pointer iDataArray = dataContainer->get##field##Data(name);\
  if (iDataArray.get() == NULL) { \
    iDataArray = typeClass::CreateArray(size);\
    dataContainer->add##field##Data(name, iDataArray);\
  } \
  iDataArray->SetNumberOfComponents(numComp);\
  valuePtr =\
  IDataArray::SafeReinterpretCast<IDataArray*, typeClass*, type* >(dataContainer->get##field##Data(name).get());\
  if (NULL == valuePtr) {\
    std::stringstream s;\
    s << "Array " << name << " from the DataContainer class could not be cast to type " << #type;\
    setErrorCondition(-12);\
    setErrorMessage(s.str());\
    return;\
  }\
}

#define INITIALIZE_FLOAT_NAMED_ARRAY_TO_PTR(m, field, name, size, valuePtr, numComp) \
INITIALIZE_NAMED_ARRAY_TO_PTR(m, field, name, FloatArrayType, float, size, valuePtr, numComp)

#define INITIALIZE_INT32_NAMED_ARRAY_TO_PTR(m, field, name, size, valuePtr, numComp) \
INITIALIZE_NAMED_ARRAY_TO_PTR(m, field, name, Int32ArrayType, int32_t, size, valuePtr, numComp)

#define INITIALIZE_INT8_NAMED_ARRAY_TO_PTR(m, field, name, size, valuePtr, numComp) \
INITIALIZE_NAMED_ARRAY_TO_PTR(m, field, name, Int8ArrayType, int8_t, size, valuePtr, numComp)

#define INITIALIZE_BOOL_NAMED_ARRAY_TO_PTR(m, field, name, size, valuePtr, numComp) \
INITIALIZE_NAMED_ARRAY_TO_PTR(m, field, name, BoolArrayType, bool, size, valuePtr, numComp)





#define GET_NAMED_ARRAY_SIZE_CHK(dataContainer, field, name, typeClass, type, size, valuePtr) \
type* valuePtr = NULL;\
{\
  IDataArray::Pointer iDataArray = dataContainer->get##field##Data(name);\
  if (iDataArray.get() == NULL) { \
    std::stringstream s;\
    s << "Array " << name << " from the DataContainer class was not in the DataContainer";\
    setErrorCondition(-10);\
    setErrorMessage(s.str());\
    return;\
  } \
  if (static_cast<size_t>(size) != iDataArray->GetNumberOfTuples()) {\
    std::stringstream s;\
    s << "Array " << name << " from the DataContainer class did not have the correct number of elements.";\
    setErrorCondition(-11);\
    setErrorMessage(s.str());\
    return;\
  }\
  valuePtr =\
  IDataArray::SafeReinterpretCast<IDataArray*, typeClass*, type* >(dataContainer->get##field##Data(name).get());\
  if (NULL == valuePtr) {\
    std::stringstream s;\
    s << "Array " << name << " from the DataContainer class could not be cast to type " << #type;\
    setErrorCondition(-13);\
    setErrorMessage(s.str());\
    return;\
  }\
}

#define GET_NAMED_ARRAY_SIZE_CHK_NOMSG(dataContainer, field, name, typeClass, type, size, valuePtr) \
type* valuePtr = NULL;\
{\
  IDataArray::Pointer iDataArray = dataContainer->get##field##Data(name);\
  if (iDataArray.get() == NULL) { \
    return;\
  } \
  if (static_cast<size_t>(size) != iDataArray->GetNumberOfTuples()) {\
    return;\
  }\
  valuePtr =\
  IDataArray::SafeReinterpretCast<IDataArray*, typeClass*, type* >(dataContainer->get##field##Data(name).get());\
  if (NULL == valuePtr) {\
    return;\
  }\
}

#define GET_NAMED_ARRAY_SIZE_CHK_NOMSG_RET(dataContainer, field, name, typeClass, type, size, valuePtr) \
type* valuePtr = NULL;\
{\
  IDataArray::Pointer iDataArray = dataContainer->get##field##Data(name);\
  if (iDataArray.get() == NULL) { \
    return -10;\
  } \
  if (static_cast<size_t>(size) != iDataArray->GetNumberOfTuples()) {\
    return -20;\
  }\
  valuePtr =\
  IDataArray::SafeReinterpretCast<IDataArray*, typeClass*, type* >(dataContainer->get##field##Data(name).get());\
  if (NULL == valuePtr) {\
    return -30;\
  }\
}





namespace DREAM3D
{
  namespace VoxelData
  {
    const std::string GrainIds("GrainIds");
    const std::string Phases("Phases");
    const std::string Euler1("Euler1");
    const std::string Euler2("Euler2");
    const std::string Euler3("Euler3");
    const std::string SurfaceVoxels("SurfaceVoxels");
    const std::string Neighbors("Neighbors");
    const std::string Quats("Quats");
    const std::string AlreadyChecked("AlreadyChecked");
    const std::string GoodVoxels("GoodVoxels");
    const std::string NearestNeighbors("NearestNeighbors");
    const std::string NearestNeighborDistances("NearestNeighborDistances");
    const std::string GrainMisorientations("GrainMisorientations");
    const std::string MisorientationGradients("MisorientationGradients");
    const std::string KernelAverageMisorientations("KernelAverageMisorientations");
    const std::string ImageQuality("ImageQuality");
  }

  namespace FieldData
  {
    const std::string NeighborList("NeighborList");
    const std::string SharedSurfaceAreaList("SharedSurfaceAreaList");
  }

  namespace EnsembleData
  {
    const std::string TotalSurfaceArea("TotalSurfaceArea");
  }
}

/**
 * @class GrainGeneratorFunc GrainGeneratorFunc.h AIM/Common/GrainGeneratorFunc.h
 * @brief
 * @author
 * @date
 * @version 1.0
 */
class DREAM3DLib_EXPORT DataContainer : public Observable
{
  public:
    DREAM3D_SHARED_POINTERS(DataContainer)
    DREAM3D_STATIC_NEW_MACRO(DataContainer)
    DREAM3D_TYPE_MACRO_SUPER(DataContainer, Observable);

    virtual ~DataContainer();


    /* *********** These methods will eventually replace those below **********/

  /**
   * @brief Adds/overwrites the data for a named array
   * @param name The name that the array will be known by
   * @param data The IDataArray::Pointer that will hold the data
   */
    void addVoxelData(const std::string &name, IDataArray::Pointer data);

    /**
     * @brief Returns the array for a given named array or the equivelant to a
     * null pointer if the name does not exist.
     * @param name The name of the data array
     */
    IDataArray::Pointer getVoxelData(const std::string &name);

    std::list<std::string> getVoxelArrayNameList();

    int getNumVoxelArrays();


    /**
    * @brief Adds/overwrites the data for a named array
    * @param name The name that the array will be known by
    * @param data The IDataArray::Pointer that will hold the data
    */
    void addFieldData(const std::string &name, IDataArray::Pointer data);

    /**
    * @brief Returns the array for a given named array or the equivelant to a
    * null pointer if the name does not exist.
    * @param name The name of the data array
    */
    IDataArray::Pointer getFieldData(const std::string &name);

    std::list<std::string> getFieldArrayNameList();

    int getNumFieldArrays();

    /**
    * @brief Adds/overwrites the data for a named array
    * @param name The name that the array will be known by
    * @param data The IDataArray::Pointer that will hold the data
    */
    void addEnsembleData(const std::string &name, IDataArray::Pointer data);

    /**
     * @brief Returns the array for a given named array or the equivelant to a
     * null pointer if the name does not exist.
     * @param name The name of the data array
     */
    IDataArray::Pointer getEnsembleData(const std::string &name);

    std::list<std::string> getEnsembleArrayNameList();

    int getNumEnsembleArrays();

    /* ****************** END Map Based Methods *******************************/

    void setDimensions(int dims[3]) {
       xpoints = dims[0];
       ypoints = dims[1];
       zpoints = dims[2];
       totalpoints = xpoints * ypoints * zpoints;
    }
    void setDimensions(int d0, int d1, int d2)
    {
      xpoints = d0;
      ypoints = d1;
      zpoints = d2;
      totalpoints = xpoints * ypoints * zpoints;
    }
    void getDimensions(int &d0, int &d1, int &d2)
    {
      d0 = xpoints;
      d1 = ypoints;
      d2 = zpoints;
    }
    void getDimensions(int dims[3])
    {
      dims[0] = xpoints;
      dims[1] = ypoints;
      dims[2] = zpoints;
    }

    int totalPoints() { return xpoints * ypoints * zpoints; }
// -----------------------------------------------------------------------------
//  Resolution Methods
// -----------------------------------------------------------------------------
    void setResolution(float x, float y, float z)
    {
      resx = x; resy = y; resz = z;
    }
    void setResolution(float resolution[3])
    {
      resx = resolution[0];
      resy = resolution[1];
      resz = resolution[2];
    }
    void getResolution(float res[3])
    {
      res[0] = resx;
      res[1] = resy;
      res[2] = resz;
    }

    void setOrigin(float orig[3])
    {
       origin[0] = orig[0];
       origin[1] = orig[1];
       origin[2] = orig[2];
    }

    // Volume Dimensional Information
    float resx;
    float resy;
    float resz;
    int xpoints;
    int ypoints;
    int zpoints;
    int totalpoints;

    float origin[3];

    // Cell Data
//    DECLARE_WRAPPED_ARRAY(grain_indicies, m_GrainIndicies, int)
//    DECLARE_WRAPPED_ARRAY(phases, m_Phases, int)
//    DECLARE_WRAPPED_ARRAY(euler1s, m_Euler1s, float)
//    DECLARE_WRAPPED_ARRAY(euler2s, m_Euler2s, float)
//    DECLARE_WRAPPED_ARRAY(euler3s, m_Euler3s, float)
//    DECLARE_WRAPPED_ARRAY(surfacevoxels, m_SurfaceVoxels, char)
//    DECLARE_WRAPPED_ARRAY(neighbors, m_Neighbors, int);
//    DECLARE_WRAPPED_ARRAY(quats, m_Quats, float); // n x 5 array
//    DECLARE_WRAPPED_ARRAY(alreadychecked, m_AlreadyChecked, bool);
//    DECLARE_WRAPPED_ARRAY(goodVoxels, m_GoodVoxels, bool);
 //   DECLARE_WRAPPED_ARRAY(nearestneighbors, m_NearestNeighbors, int); // N x 3 Array
 //   DECLARE_WRAPPED_ARRAY(nearestneighbordistances, m_NearestNeighborDistances, float); // N x 3 Array
//    DECLARE_WRAPPED_ARRAY(grainmisorientations, m_GrainMisorientations, float);
//    DECLARE_WRAPPED_ARRAY(misorientationgradients, m_MisorientationGradients, float);
//    DECLARE_WRAPPED_ARRAY(kernelmisorientations, m_KernelMisorientations, float);

    // Field Data Pointer Array
    std::vector<Field::Pointer> m_Grains;

    // Ensemble Data??
    // Phase Information (crystal structures, phase types, and shape types)
    std::vector<Ebsd::CrystalStructure> crystruct;
    std::vector<DREAM3D::Reconstruction::PhaseType> phaseType;
    std::vector<float> pptFractions;
    std::vector<DREAM3D::SyntheticBuilder::ShapeType> shapeTypes;
    std::vector<float> phasefraction;

  protected:
    DataContainer();

  private:
    std::map<std::string, IDataArray::Pointer> m_VoxelData;
    std::map<std::string, IDataArray::Pointer> m_FieldData;
    std::map<std::string, IDataArray::Pointer> m_EnsembleData;

    DataContainer(const DataContainer&);
    void operator =(const DataContainer&);
};

#endif
