// RichModel.h: interface for the CRichModel class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include <math.h>
#include "stdafx.hpp"
#define _USE_MATH_DEFINES
#include "BaseModel.hpp"

using namespace std;

struct CEdge
{
	int indexOfLeftVert;
	int indexOfRightVert;
	int indexOfOppositeVert;
	int indexOfLeftEdge;
	int indexOfRightEdge;
	int indexOfReverseEdge;
	int indexOfFrontFace;
	double length;
	std::pair<double, double> coordOfOppositeVert;
	// |unitX   -unitY|
	// |unitY    unitX|
	std::pair<double, double> matrixRotatedToLeftEdge;
	std::pair<double, double> matrixRotatedToRightEdge;
	CEdge()
	{
		indexOfOppositeVert = -1;	//key
		indexOfLeftEdge = -1;
		indexOfRightEdge = -1;
		indexOfFrontFace = -1;
	}
};

class CRichModel : public CBaseModel
{
//protected:

public:
	CRichModel();
	void Preprocess();	
	void ClearEdges()
	{
		fLocked = false; fBePreprocessed = false; m_nBoundries = 0;
		m_nIsolatedVerts = 0; 
		m_NeighsAndAngles.clear(); 
		m_FlagsForCheckingConvexVerts.clear(); 
		m_Edges.clear();
	}


	void CreateEdgesFromVertsAndFaces();
	void CollectAndArrangeNeighs();
	void ComputeAnglesAroundVerts();
	void ComputePlanarCoordsOfIncidentVertForEdges();
	void ComputeNumOfHoles();
	void ComputeNumOfComponents();

	inline int GetSubindexToVert(int root, int neigh) const;
	inline const CEdge& Edge(int edgeIndex) const;	
    inline const std::vector<std::pair<int, double> >& Neigh(int root) const;
	inline double AngleSum(int vertIndex) const;
	//inline double Curvature(int vertIndex) const;
    inline double ProportionOnEdgeByImage(int edgeIndex, const std::pair<double, double> &coord) const;
    inline double ProportionOnLeftEdgeByImage(int edgeIndex, const std::pair<double, double> &coord, double proportion) const;
    inline double ProportionOnRightEdgeByImage(int edgeIndex, const std::pair<double, double> &coord, double proportion) const;
	inline double ProportionOnEdgeByImage(int edgeIndex, double x1, double y1, double x2, double y2) const;
    inline std::pair<double, double> GetNew2DCoordinatesByRotatingAroundLeftChildEdge(int edgeIndex, const std::pair<double, double>& input2DCoordinates) const;
    inline std::pair<double, double> GetNew2DCoordinatesByRotatingAroundRightChildEdge(int edgeIndex, const std::pair<double, double>& input2DCoordinates) const;
    inline std::pair<double, double> GetNew2DCoordinatesByReversingCurrentEdge(int edgeIndex, const std::pair<double, double>& input2DCoordinates) const;
    inline double DistanceToIncidentAngle(int edgeIndex, const std::pair<double, double>& coord) const;
	inline int GetNumOfEdges() const;
	inline int GetNumOfValidDirectedEdges() const;
	inline int GetNumOfTotalUndirectedEdges() const;
	inline int GetNumOfGenera() const;
	inline int GetNumOfIsolated() const;
	inline int GetNumOfComponents() const;
	inline int GetNumOfBoundries() const;
	inline bool IsConvexVert(int index) const;
	inline bool isBoundaryVert(int index) const;
	inline bool IsClosedModel() const;
	inline bool IsExtremeEdge(int edgeIndex) const;
	inline bool IsStartEdge(int edgeIndex) const;	
	inline bool HasBeenProcessed() const;
	//inline int GetFirstEdgeIndex(int faceIndex) const;
	//inline int GetSecondEdgeIndex(int faceIndex) const;
	//inline int GetThirdEdgeIndex(int faceIndex) const;
	//inline int GetEdgeIndexFromFace(int faceIndex, int subIndex) const;
	inline int GetEdgeIndexFromTwoVertices(int leftVert, int rightVert) const;
	void SetEdgeLength(int leftVert, int rightVert, double newLen, bool apply_reverse);
	void UpdateAfterChangingEdgeLengths();
	inline CPoint3D ComputeShiftPoint(int indexOfVert) const;
	inline CPoint3D ComputeShiftPoint(int indexOfVert, double epsilon) const;
	static CPoint3D CombinePointAndNormalTo(const CPoint3D& pt, const CPoint3D& normal);
	static CPoint3D CombineTwoNormalsTo(const CPoint3D& pt1, double coef1, const CPoint3D& pt2, double coef2);	
protected:
	bool fLocked;
	bool fBePreprocessed;
	int m_nBoundries;
	int m_nIsolatedVerts;
	int m_nComponents;
    std::vector<std::vector<std::pair<int, double> > > m_NeighsAndAngles;
    std::vector<bool> m_FlagsForCheckingConvexVerts;
public:
    std::vector<CEdge> m_Edges;
};

int CRichModel::GetNumOfValidDirectedEdges() const
{
	return (int)m_Faces.size() * 3;
}

int CRichModel::GetNumOfTotalUndirectedEdges() const
{
	return (int)m_Edges.size() / 2;
}

int CRichModel::GetNumOfGenera() const
{
	return int(GetNumOfTotalUndirectedEdges() - (GetNumOfVerts() - m_nIsolatedVerts) - GetNumOfFaces() - GetNumOfBoundries()) / 2 + 1;
}

int CRichModel::GetNumOfComponents() const
{
	return m_nComponents;
}

int CRichModel::GetNumOfBoundries() const
{
	return m_nBoundries;
}

bool CRichModel::IsClosedModel() const
{
	return GetNumOfValidDirectedEdges() ==  GetNumOfEdges();
}

int CRichModel::GetNumOfIsolated() const
{
	return m_nIsolatedVerts;
}

int CRichModel::GetNumOfEdges() const
{
	return (int)m_Edges.size();
}

bool CRichModel::isBoundaryVert(int index) const
{
	return IsStartEdge(Neigh(index).front().first);
}

bool CRichModel::IsConvexVert(int index) const
{
	return m_FlagsForCheckingConvexVerts[index];
	//double sumAngle = 0;
	//for (int i = 0; i < Neigh(index).size(); ++i)
	//{
	//	sumAngle += Neigh(index)[i].second;
	//}
	//return sumAngle < 2 * M_PI - ToleranceOfConvexAngle;
}

bool CRichModel::IsExtremeEdge(int edgeIndex) const
{
	return Edge(edgeIndex).indexOfOppositeVert == -1;
}

bool CRichModel::IsStartEdge(int edgeIndex) const
{
	return Edge(Edge(edgeIndex).indexOfReverseEdge).indexOfOppositeVert == -1;
}

const CEdge& CRichModel::Edge(int edgeIndex) const
{
	return m_Edges[edgeIndex];
}

const std::vector<std::pair<int, double> >& CRichModel::Neigh(int root) const
{
	return m_NeighsAndAngles[root];
}

double CRichModel::ProportionOnEdgeByImage(int edgeIndex, const std::pair<double, double>& coord) const
{
	double res = Edge(edgeIndex).coordOfOppositeVert.first * coord.second - Edge(edgeIndex).coordOfOppositeVert.second * coord.first;
	return res / ((coord.second - Edge(edgeIndex).coordOfOppositeVert.second) * Edge(edgeIndex).length);
}

double CRichModel::ProportionOnEdgeByImage(int edgeIndex, double x1, double y1, double x2, double y2) const
{
	double res = x1 * y2 - x2 * y1;
	return res / ((y2 - y1) * Edge(edgeIndex).length);
}

double CRichModel::ProportionOnLeftEdgeByImage(int edgeIndex, const std::pair<double, double> &coord, double proportion) const
{
	double xBalance = proportion * Edge(edgeIndex).length;
	double res = Edge(edgeIndex).coordOfOppositeVert.first * coord.second - Edge(edgeIndex).coordOfOppositeVert.second * (coord.first - xBalance);
	return xBalance * coord.second / res;
}

double CRichModel::ProportionOnRightEdgeByImage(int edgeIndex, const std::pair<double, double> &coord, double proportion) const
{
	double part1 = Edge(edgeIndex).length * coord.second;
	double part2 = proportion * Edge(edgeIndex).length * Edge(edgeIndex).coordOfOppositeVert.second;
	double part3 = Edge(edgeIndex).coordOfOppositeVert.second * coord.first - Edge(edgeIndex).coordOfOppositeVert.first * coord.second;	
	return (part3 + proportion * part1 - part2) / (part3 + part1 - part2);
}

std::pair<double, double> CRichModel::GetNew2DCoordinatesByRotatingAroundLeftChildEdge(int edgeIndex, const std::pair<double, double>& input2DCoordinates) const
{
    return std::make_pair(Edge(edgeIndex).matrixRotatedToLeftEdge.first * input2DCoordinates.first - Edge(edgeIndex).matrixRotatedToLeftEdge.second * input2DCoordinates.second,
		Edge(edgeIndex).matrixRotatedToLeftEdge.second * input2DCoordinates.first + Edge(edgeIndex).matrixRotatedToLeftEdge.first * input2DCoordinates.second);
}

std::pair<double, double> CRichModel::GetNew2DCoordinatesByRotatingAroundRightChildEdge(int edgeIndex, const std::pair<double, double>& input2DCoordinates) const
{
	int reverseEdge = Edge(Edge(edgeIndex).indexOfRightEdge).indexOfReverseEdge;
    std::pair<double, double> coordOfLeftEnd = GetNew2DCoordinatesByReversingCurrentEdge(reverseEdge, Edge(reverseEdge).coordOfOppositeVert);
    return std::make_pair(Edge(edgeIndex).matrixRotatedToRightEdge.first * input2DCoordinates.first - Edge(edgeIndex).matrixRotatedToRightEdge.second * input2DCoordinates.second + coordOfLeftEnd.first,
		Edge(edgeIndex).matrixRotatedToRightEdge.second * input2DCoordinates.first + Edge(edgeIndex).matrixRotatedToRightEdge.first * input2DCoordinates.second + coordOfLeftEnd.second);
}

std::pair<double, double> CRichModel::GetNew2DCoordinatesByReversingCurrentEdge(int edgeIndex, const std::pair<double, double>& input2DCoordinates) const
{
    return std::make_pair(Edge(edgeIndex).length - input2DCoordinates.first, - input2DCoordinates.second);
}

bool CRichModel::HasBeenProcessed() const
{
	return fBePreprocessed;
}

int CRichModel::GetSubindexToVert(int root, int neigh) const
{
	for (int i = 0; i < (int)Neigh(root).size(); ++i)
	{
		if (Edge(Neigh(root)[i].first).indexOfRightVert == neigh)
			return i;
	}
	return -1;
}

CPoint3D CRichModel::ComputeShiftPoint(int indexOfVert) const
{
	return Vert(indexOfVert) + Normal(indexOfVert) * RateOfNormalShift / m_scale;
}

CPoint3D CRichModel::ComputeShiftPoint(int indexOfVert, double epsilon) const
{
	return Vert(indexOfVert) +  Normal(indexOfVert) * epsilon;
}

double CRichModel::AngleSum(int vertIndex) const
{
	double angleSum(0);
	for (int j = 0; j < (int)m_NeighsAndAngles[vertIndex].size(); ++j)
	{		
		angleSum += m_NeighsAndAngles[vertIndex][j].second;			
	}
	return angleSum;
}

double CRichModel::DistanceToIncidentAngle(int edgeIndex, const std::pair<double, double>& coord) const
{
	double detaX = coord.first - Edge(edgeIndex).coordOfOppositeVert.first;
	double detaY = coord.second - Edge(edgeIndex).coordOfOppositeVert.second;
	return sqrt(detaX * detaX + detaY * detaY);
}

//int CRichModel::GetFirstEdgeIndex(int faceIndex) const
//{
//	int root = m_Faces[faceIndex][0];
//	int subIndex = GetSubindexToVert(root, m_Faces[faceIndex][1]);
//	return Neigh(root)[subIndex].first;
//}
//int CRichModel::GetSecondEdgeIndex(int faceIndex) const
//{
//	int root = m_Faces[faceIndex][1];
//	int subIndex = GetSubindexToVert(root, m_Faces[faceIndex][2]);
//	return Neigh(root)[subIndex].first;
//}
//int CRichModel::GetThirdEdgeIndex(int faceIndex) const
//{
//	int root = m_Faces[faceIndex][2];
//	int subIndex = GetSubindexToVert(root, m_Faces[faceIndex][0]);
//	return Neigh(root)[subIndex].first;
//}
//int CRichModel::GetEdgeIndexFromFace(int faceIndex, int subIndex) const
//{
//	if (subIndex == 0)
//	{
//		int edgeIndex = GetFirstEdgeIndex(faceIndex);
//		assert (Edge(edgeIndex).indexOfFrontFace == faceIndex);
//		return edgeIndex;
//	}
//	if (subIndex == 1)
//	{
//		int edgeIndex = GetSecondEdgeIndex(faceIndex);
//		assert (Edge(edgeIndex).indexOfFrontFace == faceIndex);
//		return edgeIndex;
//	}
//	if (subIndex == 2)
//	{
//		int edgeIndex = GetThirdEdgeIndex(faceIndex);
//		assert (Edge(edgeIndex).indexOfFrontFace == faceIndex);
//		return edgeIndex;
//	}
//	assert(false);
//	return -1;
//}
//
int CRichModel::GetEdgeIndexFromTwoVertices(int leftVert, int rightVert) const
{
	int subIndex = GetSubindexToVert(leftVert, rightVert);
	assert (subIndex != -1);
	return Neigh(leftVert)[subIndex].first;
}


struct EdgePoint
{
    int index;
    double proportion; //[0 --> left endpoint; 1 --> right endpoint]
	bool isVertex;
	EdgePoint()
	{
	}
	EdgePoint(int index) : index(index), isVertex(true){}
	EdgePoint(int index, double proportion) : index(index), proportion(proportion), isVertex(false) {}
	EdgePoint(const CRichModel& model, int leftVert, int rightVert, double proportion) : proportion(proportion), isVertex(false)
	{
		index = model.GetEdgeIndexFromTwoVertices(leftVert, rightVert);
	}
	CPoint3D Get3DPoint(const CRichModel& model)
	{
		if (isVertex)
			return model.Vert(index);
		return (1 - proportion) * model.Vert(model.Edge(index).indexOfLeftVert)
			+ proportion * model.Vert(model.Edge(index).indexOfRightVert);
	}
	bool operator <(const EdgePoint& other) const
	{
		if (isVertex == false && other.isVertex == true)
			return true;
		if (isVertex == true && other.isVertex == false)
			return false;
		if (index < other.index)
			return true;
		if (index > other.index)
			return false;
		if (proportion < other.proportion)
			return true;
		if (proportion > other.proportion)
			return false;
		return false;
	}
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CRichModel::CRichModel() : CBaseModel()
{
	fLocked = false;
}

void CRichModel::CreateEdgesFromVertsAndFaces()
{
	m_Edges.reserve(2 * (GetNumOfVerts() + GetNumOfFaces() - 2));
	map<pair<int, int>, int> pondOfUndeterminedEdges;
	int szFaces = GetNumOfFaces();
	for (int i = 0; i < szFaces; ++i)
	{
		int threeIndices[3];
		for (int j = 0; j < 3; ++j)
		{
			int post = (j + 1) % 3;
			int pre = (j + 2) % 3;

			int leftVert = Face(i)[pre];
			int rightVert = Face(i)[j];

			map<pair<int, int>, int>::const_iterator it = pondOfUndeterminedEdges.find(make_pair(leftVert, rightVert));
			if (it != pondOfUndeterminedEdges.end())
			{
				int posInEdgeList = it->second;
				if (m_Edges[posInEdgeList].indexOfOppositeVert != -1)
				{
					throw "Repeated edges!";
				}
				threeIndices[j] = posInEdgeList;
				m_Edges[posInEdgeList].indexOfOppositeVert = Face(i)[post];
				m_Edges[posInEdgeList].indexOfFrontFace = i;
			}
			else
			{
				CEdge edge;
				edge.indexOfLeftVert = leftVert;
				edge.indexOfRightVert = rightVert;
				edge.indexOfFrontFace = i;
				edge.indexOfOppositeVert = Face(i)[post];
				edge.indexOfReverseEdge = (int)m_Edges.size() + 1;
				edge.length = (Vert(leftVert) - Vert(rightVert)).Len();
				m_Edges.push_back(edge);
				pondOfUndeterminedEdges[make_pair(leftVert, rightVert)] = threeIndices[j] = (int)m_Edges.size() - 1;

				edge.indexOfLeftVert = rightVert;
				edge.indexOfRightVert = leftVert;
				edge.indexOfReverseEdge = (int)m_Edges.size() - 1;
				edge.indexOfOppositeVert = -1;
				edge.indexOfFrontFace = -1;
				m_Edges.push_back(edge);
				pondOfUndeterminedEdges[make_pair(rightVert, leftVert)] = (int)m_Edges.size() - 1;
			}
		}
		for (int j = 0; j < 3; ++j)
		{
			m_Edges[threeIndices[j]].indexOfLeftEdge = Edge(threeIndices[(j + 2) % 3]).indexOfReverseEdge;
			m_Edges[threeIndices[j]].indexOfRightEdge = Edge(threeIndices[(j + 1) % 3]).indexOfReverseEdge;
		}
	}
    m_Edges = (vector<CEdge>(m_Edges));
}

void CRichModel::CollectAndArrangeNeighs()
{
	m_nIsolatedVerts = 0;
	vector<int> sequenceOfDegrees(GetNumOfVerts(), 0);
	m_NeighsAndAngles.resize(GetNumOfVerts());
	for (int i = 0; i < (int)m_NeighsAndAngles.size(); ++i)
	{
		m_NeighsAndAngles[i].resize(1, make_pair(-1, 0));
	}
	for (int i = 0; i < (int)GetNumOfEdges(); ++i)
	{
		const CEdge& edge = Edge(i);
		++sequenceOfDegrees[edge.indexOfLeftVert];
		int &indexOfStartEdge = m_NeighsAndAngles[edge.indexOfLeftVert][0].first;
		if (indexOfStartEdge == -1 || !IsStartEdge(indexOfStartEdge))
		{
			indexOfStartEdge = i;
		}
		else if (IsStartEdge(i))
		{
			m_NeighsAndAngles[edge.indexOfLeftVert].push_back(make_pair(i, 0));
		}
	}
	for (int i = 0; i < GetNumOfVerts(); ++i)
	{
		if (m_NeighsAndAngles[i][0].first == -1)
		{
			m_NeighsAndAngles[i].clear();
			m_nIsolatedVerts++;
			continue;
		}
		vector<int> startEdges;
		for (int j = 0; j < (int)Neigh(i).size(); ++j)
		{
			startEdges.push_back(Neigh(i)[j].first);
		}
		m_NeighsAndAngles[i].resize(sequenceOfDegrees[i], make_pair(0, 0));
		int num(0);
		for (int j = 0; j < (int)startEdges.size(); ++j)
		{
			int curEdge = startEdges[j];
			while (1)
			{
				m_NeighsAndAngles[i][num].first = curEdge;
				++num;
				if (num >= sequenceOfDegrees[i])
					break;
				if (IsExtremeEdge(curEdge))
					break;
				curEdge = Edge(curEdge).indexOfLeftEdge;
				if (curEdge == startEdges[j])
				{
					break;
				}
			}
		}
		if (num != sequenceOfDegrees[i])
		{
			throw "Complex vertices";
		}
	}
}

void CRichModel::ComputeAnglesAroundVerts()
{
	m_FlagsForCheckingConvexVerts.resize(GetNumOfVerts());
	for (int i = 0; i < (int)m_NeighsAndAngles.size(); ++i)
	{
		m_NeighsAndAngles[i].resize(Neigh(i).size());
	}
	for (int i = 0; i < (int)m_NeighsAndAngles.size(); ++i)
	{
		double angleSum(0);
		for (int j = 0; j < (int)m_NeighsAndAngles[i].size(); ++j)
		{
			if (IsExtremeEdge(Neigh(i)[j].first))
				m_NeighsAndAngles[i][j].second = 2 * M_PI + 0.1;
			else
			{
				int next = j + 1;
				if (next >= (int)m_NeighsAndAngles[i].size())
				{
					next = 0;
				}
				double l = Edge(Neigh(i)[j].first).length;
				double r = Edge(Neigh(i)[next].first).length;
				double b = Edge(Edge(Neigh(i)[j].first).indexOfRightEdge).length;
				m_NeighsAndAngles[i][j].second = acos((l * l + r * r - b * b) / (2 * l * r));
			}
			angleSum += m_NeighsAndAngles[i][j].second;
		}
		m_FlagsForCheckingConvexVerts[i] = (angleSum < 2 * M_PI - ToleranceOfConvexAngle);
	}
}

void CRichModel::ComputePlanarCoordsOfIncidentVertForEdges()
{
	for (int i = 0; i < GetNumOfEdges(); ++i)
	{
		if (IsExtremeEdge(i))
			continue;
		double bottom = Edge(i).length;
		double leftLen = Edge(Edge(i).indexOfLeftEdge).length;
		double squareOfLeftLen = leftLen * leftLen;
		double rightLen = Edge(Edge(i).indexOfRightEdge).length;
		double x = (squareOfLeftLen - rightLen * rightLen) / bottom + bottom;
		x /= 2.0;
		m_Edges[i].coordOfOppositeVert = make_pair(x, sqrt(max(0.0, squareOfLeftLen - x * x)));
	}
	for (int i = 0; i < GetNumOfEdges(); ++i)
	{
		if (IsExtremeEdge(i))
			continue;
		{
			int reverseEdge = m_Edges[m_Edges[i].indexOfLeftEdge].indexOfReverseEdge;
			pair<double, double> coord = GetNew2DCoordinatesByReversingCurrentEdge(reverseEdge, m_Edges[reverseEdge].coordOfOppositeVert);
            double scale = fabs(coord.first) + fabs(coord.second);
			coord.first /= scale;
			coord.second /= scale;
			double len = sqrt(coord.first * coord.first + coord.second * coord.second);
			m_Edges[i].matrixRotatedToLeftEdge = make_pair(coord.first / len, coord.second / len);
		}
		{
			int reverseEdge = m_Edges[m_Edges[i].indexOfRightEdge].indexOfReverseEdge;
			double rightX = m_Edges[reverseEdge].length;
			double rightY = 0;
			double leftX = m_Edges[reverseEdge].length - m_Edges[reverseEdge].coordOfOppositeVert.first;
			double leftY = -m_Edges[reverseEdge].coordOfOppositeVert.second;

			double detaX = rightX - leftX;
			double detaY = rightY - leftY;
            double scale = fabs(detaX) + fabs(detaY);
			detaX /= scale;
			detaY /= scale;
			double len = sqrt(detaX * detaX + detaY * detaY);
			m_Edges[i].matrixRotatedToRightEdge = make_pair(detaX / len, detaY / len);
		}
	}
}

void CRichModel::Preprocess()
{
	ClearEdges();
	if (fBePreprocessed)
		return;

	if (!fLocked)
	{
		fLocked = true;
		CreateEdgesFromVertsAndFaces();
		CollectAndArrangeNeighs();
		ComputeNumOfHoles();
		ComputeNumOfComponents();
		ComputeAnglesAroundVerts();
		ComputePlanarCoordsOfIncidentVertForEdges();
		fBePreprocessed = true;
		fLocked = false;
	}
}

CPoint3D CRichModel::CombinePointAndNormalTo(const CPoint3D& pt, const CPoint3D& normal)
{
	return pt + normal * RateOfNormalShift;
}

CPoint3D CRichModel::CombineTwoNormalsTo(const CPoint3D& pt1, double coef1, const CPoint3D& pt2, double coef2)
{
	return coef1 * pt1 + coef2 * pt2;
}

void CRichModel::ComputeNumOfHoles()
{
	m_nBoundries = 0;
	if (IsClosedModel())
	{
		return;
	}
	set<int> extremeEdges;
	for (int i = 0; i < (int)m_Edges.size(); ++i)
	{
		if (m_Edges[i].indexOfOppositeVert != -1)
			continue;
		extremeEdges.insert(i);
	}

	while (!extremeEdges.empty())
	{
		++m_nBoundries;
		int firstEdge = *extremeEdges.begin();
		int edge = firstEdge;
		do
		{
			extremeEdges.erase(edge);
			int root = Edge(edge).indexOfRightVert;
			int index = GetSubindexToVert(root, Edge(edge).indexOfLeftVert);
			edge  = Neigh(root)[(index - 1 + (int)Neigh(root).size()) % (int)Neigh(root).size()].first;
		} while (edge != firstEdge && !extremeEdges.empty());
	}
}

void CRichModel::ComputeNumOfComponents()
{
	m_nComponents = 0;
	vector<bool> flags(GetNumOfVerts(), false);
	int cnt(0);
	while (cnt < GetNumOfVerts())
	{
		int v;
		for (int i = 0; i < (int)flags.size(); ++i)
		{
			if (!flags[i])
			{
				v = i;
				break;
			}
		}
		queue<int> Que;
		Que.push(v);
		while (!Que.empty())
		{
			int v = Que.front();
			Que.pop();
			if (flags[v])
				continue;
			flags[v] = true;
			cnt++;
			for (int i = 0; i < (int)Neigh(v).size(); ++i)
			{
				if (!flags[Edge(Neigh(v)[i].first).indexOfRightVert])
				{
					Que.push(Edge(Neigh(v)[i].first).indexOfRightVert);
				}
			}
		}
		m_nComponents++;
	}
}

void CRichModel::SetEdgeLength(int leftVert, int rightVert, double newLength, bool applyReverse)
{
	int edgeID = GetEdgeIndexFromTwoVertices(leftVert, rightVert);
	int reverseID = Edge(edgeID).indexOfReverseEdge;
	m_Edges[edgeID].length = newLength;
	if(applyReverse)
	{
		m_Edges[reverseID].length = newLength;
	}
}

void CRichModel::UpdateAfterChangingEdgeLengths()
{
	ComputeAnglesAroundVerts();
	ComputePlanarCoordsOfIncidentVertForEdges();
}






