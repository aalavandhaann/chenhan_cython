// BaseModel.h: interface for the CBaseModel class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "stdafx.hpp"
#include "Point3D.hpp"
using namespace std;

struct CFace
{
	int verts[3];
	CFace(){}
	CFace(int x, int y, int z)
	{
		verts[0] = x;
		verts[1] = y;
		verts[2] = z;
	}

	void setIndices(int x, int y, int z)
	{
		verts[0] = x;
		verts[1] = y;
		verts[2] = z;
	}

	int& operator[](int index)
	{
		return verts[index];
	}
	int operator[](int index) const
	{
		return verts[index];
	}
};

class CBaseModel
{
public:
	CBaseModel();
	void LoadModel(std::vector<CPoint3D> vertices, std::vector<CFace> faces);
	void Finalize();
	bool isAValidModel();
	void AdjustScaleAndComputeNormalsToVerts();
	inline int GetNumOfVerts() const;
	inline int GetNumOfFaces() const;
	inline const CPoint3D& Vert(int vertIndex) const;
	inline const CPoint3D& Normal(int vertIndex) const;
	inline const CFace& Face(int faceIndex) const;
public:
    std::vector<CPoint3D> m_Verts;
    std::vector<CPoint3D> m_NormalsToVerts;
    CPoint3D m_ptUp;
	CPoint3D m_ptDown;
	CPoint3D m_center;
    std::vector<CFace> m_Faces;


	double m_scale;
	bool m_fBeLoaded;
};



int CBaseModel::GetNumOfVerts() const
{
	return (int)m_Verts.size();
}

int CBaseModel::GetNumOfFaces() const
{
	return (int)m_Faces.size();
}

const CPoint3D& CBaseModel::Vert(int vertIndex) const
{
	return m_Verts[vertIndex];
}

const CPoint3D& CBaseModel::Normal(int vertIndex) const
{
	return m_NormalsToVerts[vertIndex];
}

const CFace& CBaseModel::Face(int faceIndex) const
{
	return m_Faces[faceIndex];
}





//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBaseModel::CBaseModel()
{
	m_fBeLoaded = false;
}

void CBaseModel::LoadModel(std::vector<CPoint3D> vertices, std::vector<CFace> faces)
{
	m_fBeLoaded = false;

	for(CPoint3D p3d : vertices)
	{
		CPoint3D pt;
		pt.x = p3d.x;
		pt.y = p3d.y;
		pt.z = p3d.z;
		m_Verts.push_back(pt);
	}

	for (CFace face : faces)
	{
		CFace newface;
		newface.setIndices(face[0], face[1], face[2]);
		m_Faces.push_back(newface);
	}

	m_fBeLoaded = true;
	if(isAValidModel())
	{
		AdjustScaleAndComputeNormalsToVerts();
	}
}

void CBaseModel::Finalize()
{
	m_fBeLoaded = true;
	if(isAValidModel())
	{
		AdjustScaleAndComputeNormalsToVerts();
	}
}

bool CBaseModel::isAValidModel()
{
	return m_fBeLoaded && (int)m_Verts.size() != 0 && (int)m_Faces.size() != 0;
};

void CBaseModel::AdjustScaleAndComputeNormalsToVerts()
{
	if (m_Verts.empty())
		return;
	m_NormalsToVerts.resize(m_Verts.size(), CPoint3D(0, 0, 0));
	CPoint3D center(0, 0, 0);
	double sumArea(0);
	CPoint3D sumNormal(0, 0, 0);
	double deta(0);
	for (int i = 0; i < (int)m_Faces.size(); ++i)
	{
		CPoint3D normal = VectorCross(Vert(Face(i)[0]),
			Vert(Face(i)[1]),
			Vert(Face(i)[2]));
		double area = normal.Len();
		CPoint3D gravity3 = Vert(Face(i)[0]) +	Vert(Face(i)[1]) + Vert(Face(i)[2]);
		center += area * gravity3;
		sumArea += area;
		sumNormal += normal;
		deta += gravity3 ^ normal;
		normal.x /= area;
		normal.y /= area;
		normal.z /= area;
		for (int j = 0; j < 3; ++j)
		{
			m_NormalsToVerts[Face(i)[j]] += normal;
		}
	}
	center /= sumArea * 3;
	deta -= 3 * (center ^ sumNormal);
	if (true)//deta > 0)
	{
		for (int i = 0; i < GetNumOfVerts(); ++i)
		{
			if (fabs(m_NormalsToVerts[i].x)
				+ fabs(m_NormalsToVerts[i].y)
				+ fabs(m_NormalsToVerts[i].z) >= FLT_EPSILON)
			{
				m_NormalsToVerts[i].Normalize();
			}
		}
	}
	else
	{
		for (int i = 0; i < GetNumOfFaces(); ++i)
		{
			int temp = m_Faces[i][0];
			m_Faces[i][0] = m_Faces[i][1];
			m_Faces[i][1] = temp;
		}
		for (int i = 0; i < GetNumOfVerts(); ++i)
		{
			if (fabs(m_NormalsToVerts[i].x)
				+ fabs(m_NormalsToVerts[i].y)
				+ fabs(m_NormalsToVerts[i].z) >= FLT_EPSILON)
			{
				double len = m_NormalsToVerts[i].Len();
				m_NormalsToVerts[i].x /= -len;
				m_NormalsToVerts[i].y /= -len;
				m_NormalsToVerts[i].z /= -len;
			}
		}
	}

	CPoint3D ptUp(m_Verts[0]);
	CPoint3D ptDown(m_Verts[0]);
	for (int i = 1; i < GetNumOfVerts(); ++i)
	{
		if (m_Verts[i].x > ptUp.x)
			ptUp.x = m_Verts[i].x;
		else if (m_Verts[i].x < ptDown.x)
			ptDown.x = m_Verts[i].x;
		if (m_Verts[i].y > ptUp.y)
			ptUp.y = m_Verts[i].y;
		else if (m_Verts[i].y < ptDown.y)
			ptDown.y = m_Verts[i].y;
		if (m_Verts[i].z > ptUp.z)
			ptUp.z = m_Verts[i].z;
		else if (m_Verts[i].z < ptDown.z)
			ptDown.z = m_Verts[i].z;
	}

	double maxEdgeLenOfBoundingBox = -1;
	if (ptUp.x - ptDown.x > maxEdgeLenOfBoundingBox)
		maxEdgeLenOfBoundingBox = ptUp.x - ptDown.x;
	if (ptUp.y - ptDown.y > maxEdgeLenOfBoundingBox)
		maxEdgeLenOfBoundingBox = ptUp.y - ptDown.y;
	if (ptUp.z - ptDown.z > maxEdgeLenOfBoundingBox)
		maxEdgeLenOfBoundingBox = ptUp.z - ptDown.z;
	m_scale = 2.0 / maxEdgeLenOfBoundingBox;
	m_center = center;
	m_ptUp = ptUp;
	m_ptDown = ptDown;
//////////////////////////////////////////////////////////////////////////
	//the model is scaled.
	//////////////////////////////////////////////////////////////////////////
	//m_ptUp = (m_ptUp - center) * m_scale;
	//m_ptDown = (m_ptUp - m_ptDown) * m_scale;
	//for (int i = 0; i < (int)m_Verts.size(); ++i)
	//{
	//	m_Verts[i] = (m_Verts[i] - center) * m_scale;
	//}

	//m_scale = 1;
	//m_center = CPoint3D(0, 0, 0);
}



