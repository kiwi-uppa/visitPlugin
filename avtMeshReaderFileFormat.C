/*****************************************************************************
*
* Copyright (c) 2000 - 2013, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-442911
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

// ************************************************************************* //
//                            avtMeshReaderFileFormat.C                           //
// ************************************************************************* //

#include <avtMeshReaderFileFormat.h>

#include <string>

#include <vtkFloatArray.h>
#include <vtkRectilinearGrid.h>
#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCellType.h>
#include <vtkIdList.h>

#include <avtDatabaseMetaData.h>

#include <DBOptionsAttributes.h>
#include <Expression.h>

#include <InvalidVariableException.h>

#include<DebugStream.h>


using     std::string;


// ****************************************************************************
//  Method: avtMeshReaderFileFormat constructor
//
//  Programmer: guillaume -- generated by xml2avt
//  Creation:   Sat Nov 23 18:58:36 PST 2013
//
// ****************************************************************************

avtMeshReaderFileFormat::avtMeshReaderFileFormat(const char *filename)
    : avtSTSDFileFormat(filename)
{
    _filename = filename;

    _grid = NULL;
    _var = NULL;
}


// ****************************************************************************
//  Method: avtMeshReaderFileFormat::FreeUpResources
//
//  Purpose:
//      When VisIt is done focusing on a particular timestep, it asks that
//      timestep to free up any resources (memory, file descriptors) that
//      it has associated with it.  This method is the mechanism for doing
//      that.
//
//  Programmer: guillaume -- generated by xml2avt
//  Creation:   Sat Nov 23 18:58:36 PST 2013
//
// ****************************************************************************

void
avtMeshReaderFileFormat::FreeUpResources(void)
{
}


// ****************************************************************************
//  Method: avtMeshReaderFileFormat::PopulateDatabaseMetaData
//
//  Purpose:
//      This database meta-data object is like a table of contents for the
//      file.  By populating it, you are telling the rest of VisIt what
//      information it can request from you.
//
//  Programmer: guillaume -- generated by xml2avt
//  Creation:   Sat Nov 23 18:58:36 PST 2013
//
// ****************************************************************************

void
avtMeshReaderFileFormat::PopulateDatabaseMetaData(avtDatabaseMetaData *md)
{
    avtMeshMetaData * mesh = new avtMeshMetaData;
    mesh->name = "mesh";
    mesh->meshType = AVT_UNSTRUCTURED_MESH;
    mesh->numBlocks = 1;
    mesh->blockOrigin = 0;
    mesh->spatialDimension = 3;
    mesh->topologicalDimension = 2;
    mesh->hasSpatialExtents=  false;
    md->Add(mesh);

    AddScalarVarToMetaData(md,"var", "mesh", AVT_NODECENT);
}


// ****************************************************************************
//  Method: avtMeshReaderFileFormat::GetMesh
//
//  Purpose:
//      Gets the mesh associated with this file.  The mesh is returned as a
//      derived type of vtkDataSet (ie vtkRectilinearGrid, vtkStructuredGrid,
//      vtkUnstructuredGrid, etc).
//
//  Arguments:
//      meshname    The name of the mesh of interest.  This can be ignored if
//                  there is only one mesh.
//
//  Programmer: guillaume -- generated by xml2avt
//  Creation:   Sat Nov 23 18:58:36 PST 2013
//
// ****************************************************************************

vtkDataSet *
avtMeshReaderFileFormat::GetMesh(const char *meshname)
{
    ReadData();

    _grid->Register(NULL);
    return _grid;
}


// ****************************************************************************
//  Method: avtMeshReaderFileFormat::GetVar
//
//  Purpose:
//      Gets a scalar variable associated with this file.  Although VTK has
//      support for many different types, the best bet is vtkFloatArray, since
//      that is supported everywhere through VisIt.
//
//  Arguments:
//      varname    The name of the variable requested.
//
//  Programmer: guillaume -- generated by xml2avt
//  Creation:   Sat Nov 23 18:58:36 PST 2013
//
// ****************************************************************************

vtkDataArray *
avtMeshReaderFileFormat::GetVar(const char *varname)
{
    ReadData();
    vtkDataArray * rv = _var;
    rv->Register(NULL);
    return rv;
}


// ****************************************************************************
//  Method: avtMeshReaderFileFormat::GetVectorVar
//
//  Purpose:
//      Gets a vector variable associated with this file.  Although VTK has
//      support for many different types, the best bet is vtkFloatArray, since
//      that is supported everywhere through VisIt.
//
//  Arguments:
//      varname    The name of the variable requested.
//
//  Programmer: guillaume -- generated by xml2avt
//  Creation:   Sat Nov 23 18:58:36 PST 2013
//
// ****************************************************************************

vtkDataArray *
avtMeshReaderFileFormat::GetVectorVar(const char *varname)
{
    //YOU MUST IMPLEMENT THIS

    //
    // If you have a file format where variables don't apply (for example a
    // strictly polygonal format like the STL (Stereo Lithography) format,
    // then uncomment the code below.
    //
    // EXCEPTION1(InvalidVariableException, varname);
    //

    //
    // If you do have a vector variable, here is some code that may be helpful.
    //
    // int ncomps = YYY;  // This is the rank of the vector - typically 2 or 3.
    // int ntuples = XXX; // this is the number of entries in the variable.
    // vtkFloatArray *rv = vtkFloatArray::New();
    // int ucomps = (ncomps == 2 ? 3 : ncomps);
    // rv->SetNumberOfComponents(ucomps);
    // rv->SetNumberOfTuples(ntuples);
    // float *one_entry = new float[ucomps];
    // for (int i = 0 ; i < ntuples ; i++)
    // {
    //      int j;
    //      for (j = 0 ; j < ncomps ; j++)
    //           one_entry[j] = ...
    //      for (j = ncomps ; j < ucomps ; j++)
    //           one_entry[j] = 0.;
    //      rv->SetTuple(i, one_entry); 
    // }
    //
    // delete [] one_entry;
    // return rv;
    //
}

void avtMeshReaderFileFormat::ReadData()
{

    _grid = vtkUnstructuredGrid::New();
    _var = vtkFloatArray::New();

    FILE * pFile;
    char buffer [100];

    pFile = fopen (_filename , "r");
    if (pFile == NULL) perror ("Error opening file");
    else
    {
        fscanf(pFile, "%s", buffer);
        while ( strcmp(buffer, "Vertices")!= 0 )
        {
            fscanf(pFile, "%s", buffer);
        }

        int nbVertices;
        fscanf(pFile,"%d", &nbVertices);

        float x,y,z,v;
        vtkPoints * points = vtkPoints::New();

        for(int i=0; i<nbVertices; i++)
        {
            fscanf(pFile,"%f %f %f %f", &x, &y, &z, &v);
            points->InsertNextPoint(x,y,z);
        }

        _grid->SetPoints(points);
        points->Delete();

        char null[100];
        fscanf(pFile,"%s", null);

        int nbTriangles;
        fscanf(pFile,"%d", &nbTriangles);

        int id[3];
        float v2;

        for(int i=0; i<nbTriangles; i++)
        {
            fscanf(pFile,"%d %d %d %f", &id[0], &id[1], &id[2], &v2);
            vtkIdList *ids = vtkIdList::New();
            ids->SetNumberOfIds(3);
            ids->SetId(0, id[0]-1);
            ids->SetId(1, id[1]-1);
            ids->SetId(2, id[2]-1);
            _grid->InsertNextCell(VTK_TRIANGLE, ids);
            _var->InsertNextTuple1(v2);
        }

        fclose (pFile);
    }
    return;
}
