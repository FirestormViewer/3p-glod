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
#include <stdlib.h>
#ifdef _WIN32
#define _WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "mt.h"

// used for special coloring options
#define NEW_FRAME_COUNT    10

// histogram of points of different screen-space sizes
static int pointsPerSize[MAX_POINT_SIZE];

// some variables from the cut that I don't want to bother passing to all
// the low-level rendering routines
static mtCut emptyCut;
static char gDrawMode;
static gStat *gCutStat = &emptyCut.stat;


/*****************************************************************************
*****************************************************************************/
void mtTriangle::print(MT *mt)
{
    printf("Triangle compriding vert nos. %d, %d and %d:\n  ",
           verts[0], verts[1], verts[3]);
    mt->getVert(verts[0])->print();
    mt->getVert(verts[1])->print();
    mt->getVert(verts[2])->print();
    printf("\n");
};


/*****************************************************************************
*****************************************************************************/
void MT::flushPointCaches()
{
	memset(numPointsInCache, 0, MAX_POINT_SIZE*sizeof(int));
}


/*****************************************************************************
*****************************************************************************/
void MT::cachePoint(mtPoint *pt, int pointSize)
{
   if (pointSize < 0 || pointSize >= MAX_POINT_SIZE) {
      fprintf(stderr, "Unsupported point size for cache (%i)\n", pointSize);
      return;
   }

   if (numPointsInCache[pointSize] == maxPointsInCache[pointSize]) {
      mtPoint **oldcache = pointCache[pointSize];
      maxPointsInCache[pointSize] *= 2;
      pointCache[pointSize] = new mtPoint *[maxPointsInCache[pointSize]];
      memcpy(pointCache[pointSize], oldcache, 
             sizeof(mtPoint *)*numPointsInCache[pointSize]);
      delete oldcache;
   }

   pointsPerSize[pointSize]++;
   pointCache[pointSize][numPointsInCache[pointSize]] = pt;
   numPointsInCache[pointSize]++;
}
