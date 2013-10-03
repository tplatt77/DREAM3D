#include "PerformMTRBridge.h"

#include "DREAM3DLib/Common/Constants.h"
#include "DREAM3DLib/Math/DREAM3DMath.h"
#include "DREAM3DLib/Math/MatrixMath.h"
#include "DREAM3DLib/OrientationOps/OrientationOps.h"
#include "DREAM3DLib/Utilities/DREAM3DRandom.h"

#include "DREAM3DLib/StatisticsFilters/FindNeighbors.h"
#include "DREAM3DLib/GenericFilters/FindGrainPhases.h"

#include "DREAM3DLib/GenericFilters/RenameCellArray.h"
#include "DREAM3DLib/GenericFilters/LinkFieldMapToCellArray.h"

#define ERROR_TXT_OUT 1
#define ERROR_TXT_OUT1 1




#define NEW_SHARED_ARRAY(var, m_msgType, size)\
 boost::shared_array<m_msgType> var##Array(new m_msgType[size]);\
 m_msgType* var = var##Array.get();

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PerformMTRBridge::PerformMTRBridge() :
 AbstractFilter(),
 m_GrainIdsArrayName(DREAM3D::CellData::GrainIds),
 m_CellParentIdsArrayName(DREAM3D::CellData::ParentIds),
 m_avgMTRgKAMArrayName(DREAM3D::FieldData::avgMTRgKAM),
 m_NumGrainsPerParentArrayName(DREAM3D::FieldData::NumGrainsPerParent),
 m_FieldParentIdsArrayName(DREAM3D::FieldData::ParentIds),
 m_MTRgKAMArrayName(DREAM3D::FieldData::MTRgKAM),
 m_CrystalStructuresArrayName(DREAM3D::EnsembleData::CrystalStructures),
 m_avgMTRgKAM(NULL),
 m_NumGrainsPerParent(NULL),
 m_FieldParentIds(NULL),
 m_MTRgKAM(NULL),
 m_CrystalStructures(NULL)
{
 m_OrientationOps = OrientationOps::getOrientationOpsVector();

 setupFilterParameters();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
PerformMTRBridge::~PerformMTRBridge()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PerformMTRBridge::setupFilterParameters()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PerformMTRBridge::readFilterParameters(AbstractFilterParametersReader* reader, int index)
{
 reader->openFilterGroup(this, index);
/* Code to read the values goes between these statements */
/* FILTER_WIDGETCODEGEN_AUTO_GENERATED_CODE BEGIN*/
/* FILTER_WIDGETCODEGEN_AUTO_GENERATED_CODE END*/
 reader->closeFilterGroup();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
int PerformMTRBridge::writeFilterParameters(AbstractFilterParametersWriter* writer, int index)
{
 writer->openFilterGroup(this, index);
 writer->closeFilterGroup();
 return ++index; // we want to return the next index that was just written to
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PerformMTRBridge::dataCheck(bool preflight, size_t voxels, size_t fields, size_t ensembles, bool afterLink)
{
 setErrorCondition(0);
 std::stringstream ss;
 VolumeDataContainer* m = getVolumeDataContainer();

 // Field Data
if(afterLink == false)
{
 GET_PREREQ_DATA(m, DREAM3D, FieldData, FieldParentIds, -302, int32_t, Int32ArrayType, fields, 1)
 GET_PREREQ_DATA(m, DREAM3D, FieldData, MTRgKAM, -303, float, FloatArrayType, fields, 1)
}
if(afterLink == true)
{
 CREATE_NON_PREREQ_DATA(m, DREAM3D, FieldData, avgMTRgKAM, float, FloatArrayType, 0, fields, 1)
 CREATE_NON_PREREQ_DATA(m, DREAM3D, FieldData, NumGrainsPerParent, int32_t, Int32ArrayType, 0, fields, 1)
}

 typedef DataArray<unsigned int> XTalStructArrayType;
 GET_PREREQ_DATA(m, DREAM3D, EnsembleData, CrystalStructures, -305, unsigned int, XTalStructArrayType, ensembles, 1)


}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PerformMTRBridge::preflight()
{
 dataCheck(true, 1, 1, 1, false);

 VolumeDataContainer* m = getVolumeDataContainer();
 if(NULL == m)
 {
   setErrorCondition(-999);
   notifyErrorMessage("The DataContainer Object was NULL", -999);
   return;
 }

 RenameCellArray::Pointer rename_cell_array = RenameCellArray::New();
 rename_cell_array->setObservers(this->getObservers());
 rename_cell_array->setVolumeDataContainer(m);
 rename_cell_array->setMessagePrefix(getMessagePrefix());
 rename_cell_array->setSelectedCellArrayName(m_CellParentIdsArrayName);
 rename_cell_array->setNewCellArrayName(m_GrainIdsArrayName);
 rename_cell_array->preflight();
 int err1 = rename_cell_array->getErrorCondition();
 if (err1 < 0)
 {
   setErrorCondition(rename_cell_array->getErrorCondition());
   addErrorMessages(rename_cell_array->getPipelineMessages());
   return;
 }

 LinkFieldMapToCellArray::Pointer link_field_map_to_cell_array = LinkFieldMapToCellArray::New();
 link_field_map_to_cell_array->setObservers(this->getObservers());
 link_field_map_to_cell_array->setVolumeDataContainer(m);
 link_field_map_to_cell_array->setMessagePrefix(getMessagePrefix());
 link_field_map_to_cell_array->setSelectedCellDataArrayName(m_GrainIdsArrayName);
 link_field_map_to_cell_array->preflight();
 int err2 = link_field_map_to_cell_array->getErrorCondition();
 if (err2 < 0)
 {
   setErrorCondition(link_field_map_to_cell_array->getErrorCondition());
   addErrorMessages(link_field_map_to_cell_array->getPipelineMessages());
   return;
 }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void PerformMTRBridge::execute()
{
 VolumeDataContainer* m = getVolumeDataContainer();
 if(NULL == m)
 {
   setErrorCondition(-999);
   notifyErrorMessage("The DataContainer Object was NULL", -999);
   return;
 }

 setErrorCondition(0);
 dataCheck(false, m->getTotalPoints(), m->getNumFieldTuples(), m->getNumEnsembleTuples(), false);
 if (getErrorCondition() < 0)
 {
   return;
 }

int numgrains = m->getNumFieldTuples();
std::vector<float> MTRgKAM(numgrains,0.0);
std::vector<int32_t> fieldParentIds(numgrains,0);
for(int i=0;i<numgrains;i++)
{
    MTRgKAM[i] = m_MTRgKAM[i];
    fieldParentIds[i] = m_FieldParentIds[i];
}

 RenameCellArray::Pointer rename_cell_array = RenameCellArray::New();
 rename_cell_array->setObservers(this->getObservers());
 rename_cell_array->setVolumeDataContainer(m);
 rename_cell_array->setMessagePrefix(getMessagePrefix());
 rename_cell_array->setSelectedCellArrayName(m_CellParentIdsArrayName);
 rename_cell_array->setNewCellArrayName(m_GrainIdsArrayName);
 rename_cell_array->execute();
 int err1 = rename_cell_array->getErrorCondition();
 if (err1 < 0)
 {
   setErrorCondition(rename_cell_array->getErrorCondition());
   addErrorMessages(rename_cell_array->getPipelineMessages());
   return;
 }

 LinkFieldMapToCellArray::Pointer link_field_map_to_cell_array = LinkFieldMapToCellArray::New();
 link_field_map_to_cell_array->setObservers(this->getObservers());
 link_field_map_to_cell_array->setVolumeDataContainer(m);
 link_field_map_to_cell_array->setMessagePrefix(getMessagePrefix());
 link_field_map_to_cell_array->setSelectedCellDataArrayName(m_GrainIdsArrayName);
 link_field_map_to_cell_array->execute();
 int err2 = link_field_map_to_cell_array->getErrorCondition();
 if (err2 < 0)
 {
   setErrorCondition(link_field_map_to_cell_array->getErrorCondition());
   addErrorMessages(link_field_map_to_cell_array->getPipelineMessages());
   return;
 }

dataCheck(false, m->getNumCellTuples(), m->getNumFieldTuples(), m->getNumEnsembleTuples(), true);

for(int i=0;i<numgrains;i++)
{
    int parentid = fieldParentIds[i];
    m_NumGrainsPerParent[parentid]++;
    m_avgMTRgKAM[parentid] += MTRgKAM[i];
}
for(int i=0;i<m->getNumFieldTuples();i++)
{
    m_avgMTRgKAM[i] /= m_NumGrainsPerParent[i];
}

 // If there is an error set this to something negative and also set a message
 notifyStatusMessage("PerformMTRBridge Completed");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------



