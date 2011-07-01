/*
 * filename :	plyloader.cpp
 *
 * programmer :	Cao Jiayin
 */

// include the header file
#include "plyloader.h"
#include "utility/path.h"
#include "managers/meshmanager.h"
#include "thirdparty/ply/ply.h"

// the maxmium length of a single line
static const unsigned LINE_MAXLENGTH = 4096;

static PlyProperty vert_props[] = 
{
  {"x", PLY_FLOAT, PLY_FLOAT, 0, 0, 0, 0, 0},
  {"y", PLY_FLOAT, PLY_FLOAT, 4, 0, 0, 0, 0},
  {"z", PLY_FLOAT, PLY_FLOAT, 8, 0, 0, 0, 0},
};

static PlyProperty face_props[] = 
{
  {"vertex_indices", PLY_UINT, PLY_UINT, 4 , 1, 4, PLY_UINT, 0},
};

// load obj from file
bool PlyLoader::LoadMesh( const string& str , BufferMemory* mem )
{
	// some variable that will be used later
	int nelems;
	char **elist;
	int file_type;
	float version;
	int nprops;
	int num_elems;
	char *elem_name;

	// open file for reading
	PlyFile *ply = ply_open_for_reading( GetFullPath(str).c_str() , &nelems, &elist, &file_type, &version);
	if( ply == 0 )
		return false;

	// set the memory
	mem->m_filename = str;
	mem->m_iTriNum = 0;

	char strVertex[] = "vertex";
	char strFace[] = "face";
	for ( int i = 0; i < nelems; i++) 
	{
		/* get the description of the first element */
		elem_name = elist[i];
		PlyProperty** properties = ply_get_element_description (ply, elem_name, &num_elems, &nprops);

		if (equal_strings ("vertex", elem_name))
		{
			ply_get_property (ply, elem_name, &vert_props[0]);
			ply_get_property (ply, elem_name, &vert_props[1]);
			ply_get_property (ply, elem_name, &vert_props[2]);

			for ( int j = 0; j < num_elems; j++) 
			{
				Point p;
				ply_get_element(ply, (void *)&p);
				mem->m_PositionBuffer.push_back( p );
			}
		}

		/* if we're on face elements, read them in */
		if (equal_strings ("face", elem_name)) 
		{
			/* set up for getting face elements */
			ply_get_property (ply, elem_name, &face_props[0]);

			/* grab all the face elements */
			for ( int j = 0; j < num_elems; j++)
			{
				PlyIndex index;
				ply_get_element (ply, (void *)&index);

				int curId = 1;
				for( unsigned i = 0 ; i < index.count - 2 ; i++ )
				{
					VertexIndex vid;
					vid.posIndex = index.index[0];
					mem->m_IndexBuffer.push_back( vid );
					vid.posIndex = index.index[curId];
					mem->m_IndexBuffer.push_back( vid );
					vid.posIndex = index.index[curId+1];
					mem->m_IndexBuffer.push_back( vid );

					curId++;
				}
			}
		}

		for( int k = 0 ; k < nprops ; k++ )
		{
			delete[] properties[k]->name;
			delete properties[k];
		}
		delete[] properties;
	}

	mem->m_iVBCount = mem->m_PositionBuffer.size();
	mem->m_iTBCount = mem->m_TexCoordBuffer.size();
	mem->m_iNBCount = mem->m_NormalBuffer.size();
	mem->m_iTriNum = mem->m_IndexBuffer.size() / 3;

	for( int i = 0 ; i < nelems ; i++ )
		delete[] elist[i];
	delete[] elist;

	// close ply file
	_closePly( ply );

	return true;
}

// close ply file
void PlyLoader::_closePly( PlyFile* ply )
{
	for( int i = 0 ; i < ply->num_comments ; i++ )
		delete ply->comments[i];
	delete[] ply->comments;

	for( int i = 0 ; i < ply->num_obj_info; i++ )
		delete[] ply->obj_info[i];
	delete[] ply->obj_info;

	for( int i = 0 ; i < ply->nelems ; i++ )
		_releaseElement( ply->elems[i] );
	delete ply->elems;

	// delte the ply file
	ply_close (ply);
}

// release element
void PlyLoader::_releaseElement( PlyElement* element )
{
	delete element->name;
	if( element->store_prop )
		delete element->store_prop;
	for( int k = 0 ; k < element->nprops; k++ )
	{
		delete element->props[k]->name;
		delete element->props[k];
	}
	delete[] element->props;
	delete element;
}