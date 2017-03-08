// at some point, it makes sense for glod to simply link to and use the 
// stderror and stdrender libraries included with the vds distribution,
// replacing these callbacks, as they are just copied from those libraries
// (probably not worth the trouble to do it now, but putting this here as a
// reminder)
/******************************************************************************
 * Copyright 2003 Jonathan Cohen, Nat Duca, David Luebke, Brenden Schubert    *
 *                Johns Hopkins University and University of Virginia         *
 ******************************************************************************
 * This file is distributed as part of the GLOD library, and as such, falls   *
 * under the terms of the GLOD public license. GLOD is distributed without    *
 * any warranty, implied or otherwise. See the GLOD license for more details. *
 *                                                                            *
 * You should have recieved a copy of the GLOD Open-Source License with this  *
 * copy of GLOD; if not, please visit the GLOD web page,                      *
 * http://www.cs.jhu.edu/~graphics/GLOD/license for more information          *
 ******************************************************************************/

#ifdef _WIN32
#include <windows.h>
#include <GL/gl.h>
#else
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/glx.h>
#endif
#endif

#include "vds_callbacks.h"
#include "Hierarchy.h"

#define BIGFLOAT 3e38

using namespace VDS;

// NODE ERROR CALLBACKS ******************************************

VDS::Float StdErrorScreenSpace(VDS::BudgetItem *pItem, const VDS::Cut *pCut)
{
    GLOD_View *view = (GLOD_View *)pCut->mpExternalViewClass;
    xbsVec3 center(pItem->mBBoxCenter.X, pItem->mBBoxCenter.Y, pItem->mBBoxCenter.Z);
    xbsVec3 offsets(pItem->mXBBoxOffset, pItem->mYBBoxOffset, pItem->mZBBoxOffset);
    return view->computePixelsOfError(center, offsets, pCut->mpForest->mpErrorParams[pCut->mpForest->mpNodes[pItem->miNode].miErrorParamIndex]); // 1
}

VDS::Float StdErrorScreenSpaceNoFrustum(VDS::BudgetItem *pItem, const VDS::Cut *pCut)
{
    GLOD_View *view = (GLOD_View *)pCut->mpExternalViewClass;
    xbsVec3 center(pItem->mBBoxCenter.X, pItem->mBBoxCenter.Y, pItem->mBBoxCenter.Z);
    xbsVec3 offsets(pItem->mXBBoxOffset, pItem->mYBBoxOffset, pItem->mZBBoxOffset);
    return view->computePixelsOfError(center, offsets, pCut->mpForest->mpErrorParams[pCut->mpForest->mpNodes[pItem->miNode].miErrorParamIndex]); // 1
}

VDS::Float StdErrorObjectSpace(VDS::BudgetItem *pItem, const VDS::Cut *pCut)
{
    GLOD_View *view = (GLOD_View *)pCut->mpExternalViewClass;
    xbsVec3 center(pItem->mBBoxCenter.X, pItem->mBBoxCenter.Y, pItem->mBBoxCenter.Z);
    xbsVec3 offsets(pItem->mXBBoxOffset, pItem->mYBBoxOffset, pItem->mZBBoxOffset);
    //return view->computePixelsOfError(center, offsets,  pCut->mpForest->mpErrorParams[pCut->mpForest->mpNodes[pItem->miNode].miErrorParamIndex]);
    if (view->checkFrustrum(center,offsets)==1){
        return pCut->mpForest->mpErrorParams[pCut->mpForest->mpNodes[pItem->miNode].miErrorParamIndex];
    }
    else 
        return 0;
}


VDS::Float StdErrorObjectSpaceNoFrustum(VDS::BudgetItem *pItem, const VDS::Cut *pCut)
{
    return pCut->mpForest->mpErrorParams[pCut->mpForest->mpNodes[pItem->miNode].miErrorParamIndex];
}
