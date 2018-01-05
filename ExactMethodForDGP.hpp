// ExactMethodForDGP.h: interface for the CExactMethodForDGP class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdlib>
#include <stdlib.h>

#include "RichModel.hpp"

struct InfoAtVertex
{
	bool fParentIsPseudoSource;
	char birthTime;
	int indexOfParent;
	int indexOfRootVertOfParent;
	int level;
	double disUptodate;
	double entryProp;

	InfoAtVertex()
	{
		birthTime = -1;
		disUptodate = DBL_MAX;
	}
};
struct QuoteInfoAtVertex
{
	char birthTime;
	int indexOfVert;
	double disUptodate;
	bool operator<(const QuoteInfoAtVertex& another) const
	{
		return disUptodate > another.disUptodate;
	}
	QuoteInfoAtVertex(){}
	QuoteInfoAtVertex(char birthTime, int indexOfVert, double disUptodate)
	{
		this->birthTime = birthTime;
		this->indexOfVert = indexOfVert;
		this->disUptodate = disUptodate;
	}
};

using namespace std;

class CExactMethodForDGP  
{
public:
    std::vector<InfoAtVertex> m_InfoAtVertices;

protected:	
	bool fComputationCompleted;	
	bool fLocked;
	double totalLen;
	int nTotalCurves;

    std::set<int> indexOfSourceVerts;
	int nCountOfWindows;
	double nTotalMilliSeconds;
	int nMaxLenOfWindowQueue;
	double nMaxLenOfPseudoSources;
	int depthOfResultingTree;
	double NPE;
	double memory;
	double farestDis;
    std::vector<std::list<CPoint3D> > m_tableOfResultingPaths;
	const CRichModel& model;
    std::string nameOfAlgorithm;
protected:
	void BackTrace(int indexOfVert);
	void BackTraceWithoutStoring(int indexOfVert) const;
public:
    CExactMethodForDGP(const CRichModel& inputModel, const std::set<int> &indexOfSourceVerts);
	virtual ~CExactMethodForDGP();
	inline int GetRootSourceOfVert(int index) const;
	vector<EdgePoint>& FindSourceVertex(int indexOfVert, std::vector<EdgePoint>& resultingPath) const;
	void PickShortestPaths(int num);
	virtual void Execute();
	virtual void InitContainers() = 0;
	virtual void BuildSequenceTree() = 0;
	virtual void ClearContainers() = 0;
	virtual void FillExperimentalResults() = 0;
	inline double GetRunTime() const;
	inline double GetMemoryCost() const;
	inline int GetWindowNum() const;
	inline int GetMaxLenOfQue() const;
	inline double GetNPE() const;
	inline int GetDepthOfSequenceTree() const;
    inline std::string GetAlgorithmName() const;
	inline bool HasBeenCompleted() const;
	std::vector<InfoAtVertex> GetVertexDistances();
};

double CExactMethodForDGP::GetRunTime() const
{
	return nTotalMilliSeconds;
}

double CExactMethodForDGP::GetMemoryCost() const
{
	return memory;
}

int CExactMethodForDGP::GetWindowNum() const
{
	return nCountOfWindows;
}

int CExactMethodForDGP::GetMaxLenOfQue() const
{
	return nMaxLenOfWindowQueue;
}

int CExactMethodForDGP::GetDepthOfSequenceTree() const
{
	return depthOfResultingTree;
}

double CExactMethodForDGP::GetNPE() const
{
	return NPE;
}

std::string CExactMethodForDGP::GetAlgorithmName() const
{
	return nameOfAlgorithm;
}

bool CExactMethodForDGP::HasBeenCompleted() const
{
	return fComputationCompleted;
}

int CExactMethodForDGP::GetRootSourceOfVert(int index) const
{
	if (m_InfoAtVertices[index].disUptodate > FLT_MAX)
		return index;

	while (m_InfoAtVertices[index].disUptodate > FLT_EPSILON)
	{
		int indexOfParent = m_InfoAtVertices[index].indexOfParent;
		if (m_InfoAtVertices[index].fParentIsPseudoSource)
		{
			index = indexOfParent;
		}
		else
		{
			index = m_InfoAtVertices[index].indexOfRootVertOfParent;
		}
	}
	return index;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExactMethodForDGP::CExactMethodForDGP(const CRichModel& inputModel, const set<int> &indexOfSourceVerts) : model(inputModel)
{
	this->indexOfSourceVerts = indexOfSourceVerts;
	nCountOfWindows = 0;
	nMaxLenOfPseudoSources = 0;
	nMaxLenOfWindowQueue = 0;
	depthOfResultingTree = 0;
	totalLen = 0;
	fComputationCompleted = false;
	fLocked = false;
	NPE = 0;
	memory = 0;
	nTotalCurves = 0;
	nameOfAlgorithm = "";
	m_InfoAtVertices.resize(model.GetNumOfVerts());
	memory += double(model.GetNumOfVerts()) * sizeof(InfoAtVertex) / 1024 / 1024;
}

CExactMethodForDGP::~CExactMethodForDGP()
{
}

std::vector<InfoAtVertex> CExactMethodForDGP::GetVertexDistances()
{
	return m_InfoAtVertices;
}

void CExactMethodForDGP::PickShortestPaths(int num)
{
	if (num >= model.GetNumOfVerts())
		num = model.GetNumOfVerts();
	nTotalCurves = num;
	m_tableOfResultingPaths.clear();
	if (num == 0)
		return;
	if (model.GetNumOfFaces() * num < 4e6)
	{
		if (num >= model.GetNumOfVerts())
		{
			m_tableOfResultingPaths.reserve(model.GetNumOfVerts());
			for (int i = 0; i < model.GetNumOfVerts(); ++i)
			{
				BackTrace(i);
			}
		}
		else
		{
			float step = model.GetNumOfVerts() / float(num);
            step = std::max(1.0f, step);
			m_tableOfResultingPaths.reserve(int(model.GetNumOfVerts() / step) + 1);
			for (float i = FLT_EPSILON; i < model.GetNumOfVerts(); i += step)
			{
				BackTrace(int(i));
			}
		}
	}
}

void CExactMethodForDGP::BackTrace(int indexOfVert)
{
	if (m_InfoAtVertices[indexOfVert].birthTime == -1)
	{
		assert(model.GetNumOfComponents() != 1 || model.Neigh(indexOfVert).empty());
		return;
	}
	m_tableOfResultingPaths.push_back(list<CPoint3D>());
	vector<int> vertexNodes;
	int index = indexOfVert;
	vertexNodes.push_back(index);
	while (m_InfoAtVertices[index].disUptodate > FLT_EPSILON)
	{
		int indexOfParent = m_InfoAtVertices[index].indexOfParent;
		if (m_InfoAtVertices[index].fParentIsPseudoSource)
		{
			index = indexOfParent;
		}
		else
		{
			index = m_InfoAtVertices[index].indexOfRootVertOfParent;
		}
		vertexNodes.push_back(index);
	};
	int indexOfSourceVert = index;
	int posOfTable = (int)m_tableOfResultingPaths.size() - 1;
	for (int i = 0; i < (int)vertexNodes.size() - 1; ++i)
	{
		int lastVert = vertexNodes[i];
		CPoint3D pt = model.ComputeShiftPoint(lastVert);
		m_tableOfResultingPaths[posOfTable].push_back(pt);

		if (m_InfoAtVertices[lastVert].fParentIsPseudoSource)
		{
			continue;
		}
		int parentEdgeIndex = m_InfoAtVertices[lastVert].indexOfParent;
		int edgeIndex = model.Edge(parentEdgeIndex).indexOfReverseEdge;
		pair<double, double> coord(model.GetNew2DCoordinatesByReversingCurrentEdge(parentEdgeIndex, model.Edge(parentEdgeIndex).coordOfOppositeVert));

		double proportion = 1 - m_InfoAtVertices[lastVert].entryProp;
		while (1)
		{
			CPoint3D pt1 = model.ComputeShiftPoint(model.Edge(edgeIndex).indexOfLeftVert);
			CPoint3D pt2 = model.ComputeShiftPoint(model.Edge(edgeIndex).indexOfRightVert);
			CPoint3D ptIntersection = CRichModel::CombineTwoNormalsTo(pt1, 1 - proportion, pt2, proportion);
			m_tableOfResultingPaths[posOfTable].push_back(ptIntersection);

			if (model.Edge(edgeIndex).indexOfOppositeVert == vertexNodes[i + 1])
				break;
			double oldProprotion = proportion;
			proportion = model.ProportionOnLeftEdgeByImage(edgeIndex, coord, oldProprotion);
			if (proportion >= -LENGTH_EPSILON_CONTROL && proportion <= 1)
			{
                proportion = max(proportion, 0.0);
				coord = model.GetNew2DCoordinatesByRotatingAroundLeftChildEdge(edgeIndex, coord);
				edgeIndex = model.Edge(edgeIndex).indexOfLeftEdge;
				//rightLen = disToAngle;
			}
			else
			{
				proportion = model.ProportionOnRightEdgeByImage(edgeIndex, coord, oldProprotion);
                proportion = max(proportion, 0.0);
                proportion = min(proportion, 1.0);
				coord = model.GetNew2DCoordinatesByRotatingAroundRightChildEdge(edgeIndex, coord);
				edgeIndex = model.Edge(edgeIndex).indexOfRightEdge;
			}
		};
	}
	m_tableOfResultingPaths[posOfTable].push_back(model.ComputeShiftPoint(indexOfSourceVert));
}

void CExactMethodForDGP::BackTraceWithoutStoring(int indexOfVert) const
{
	if (m_InfoAtVertices[indexOfVert].birthTime == -1)
	{
		assert(model.GetNumOfComponents() != 1 || model.Neigh(indexOfVert).empty());
		return;
	}
	vector<int> vertexNodes;
	int index = indexOfVert;
	vertexNodes.push_back(index);
	while (m_InfoAtVertices[index].disUptodate > FLT_EPSILON)
	{
		int indexOfParent = m_InfoAtVertices[index].indexOfParent;
		if (m_InfoAtVertices[index].fParentIsPseudoSource)
		{
			index = indexOfParent;
		}
		else
		{
			index = m_InfoAtVertices[index].indexOfRootVertOfParent;
		}
		vertexNodes.push_back(index);
	};
	int indexOfSourceVert = index;

	for (int i = 0; i < (int)vertexNodes.size() - 1; ++i)
	{
		int lastVert = vertexNodes[i];
		CPoint3D pt = model.ComputeShiftPoint(lastVert);
		if (m_InfoAtVertices[lastVert].fParentIsPseudoSource)
		{
			continue;
		}
		int parentEdgeIndex = m_InfoAtVertices[lastVert].indexOfParent;
		int edgeIndex = model.Edge(parentEdgeIndex).indexOfReverseEdge;
		pair<double, double> coord(model.GetNew2DCoordinatesByReversingCurrentEdge(parentEdgeIndex, model.Edge(parentEdgeIndex).coordOfOppositeVert));

		double proportion = 1 - m_InfoAtVertices[lastVert].entryProp;
		while (1)
		{
			CPoint3D pt1 = model.ComputeShiftPoint(model.Edge(edgeIndex).indexOfLeftVert);
			CPoint3D pt2 = model.ComputeShiftPoint(model.Edge(edgeIndex).indexOfRightVert);
			CPoint3D ptIntersection = CRichModel::CombineTwoNormalsTo(pt1, 1 - proportion, pt2, proportion);
			if (model.Edge(edgeIndex).indexOfOppositeVert == vertexNodes[i + 1])
				break;
			double oldProprotion = proportion;
			proportion = model.ProportionOnLeftEdgeByImage(edgeIndex, coord, oldProprotion);
			if (proportion >= -LENGTH_EPSILON_CONTROL && proportion <= 1)
			{
                proportion = max(proportion, 0.0);
				coord = model.GetNew2DCoordinatesByRotatingAroundLeftChildEdge(edgeIndex, coord);
				edgeIndex = model.Edge(edgeIndex).indexOfLeftEdge;
				//rightLen = disToAngle;
			}
			else
			{
				proportion = model.ProportionOnRightEdgeByImage(edgeIndex, coord, oldProprotion);
                proportion = max(proportion, 0.0);
                proportion = min(proportion, 1.0);
				coord = model.GetNew2DCoordinatesByRotatingAroundRightChildEdge(edgeIndex, coord);
				edgeIndex = model.Edge(edgeIndex).indexOfRightEdge;
			}
		};
	}
	CPoint3D pt = model.ComputeShiftPoint(indexOfSourceVert);
}

vector<EdgePoint>& CExactMethodForDGP::FindSourceVertex(int indexOfVert, vector<EdgePoint>& resultingPath) const
{
	resultingPath.clear();

	if (m_InfoAtVertices[indexOfVert].birthTime == -1 || m_InfoAtVertices[indexOfVert].disUptodate > FLT_MAX)
	{
		assert(model.GetNumOfComponents() != 1 || model.Neigh(indexOfVert).empty());
		return resultingPath;
	}
	vector<int> vertexNodes;
	int index = indexOfVert;
	vertexNodes.push_back(index);
	while (m_InfoAtVertices[index].disUptodate > FLT_EPSILON)
	{
		int indexOfParent = m_InfoAtVertices[index].indexOfParent;
		if (m_InfoAtVertices[index].fParentIsPseudoSource)
		{
			index = indexOfParent;
		}
		else
		{
			index = m_InfoAtVertices[index].indexOfRootVertOfParent;
		}
		vertexNodes.push_back(index);
	};
	int indexOfSourceVert = index;

	for (int i = 0; i < (int)vertexNodes.size() - 1; ++i)
	{
		int lastVert = vertexNodes[i];
		//if (lastVert != indexOfVert)
		resultingPath.push_back(EdgePoint(lastVert));
		if (m_InfoAtVertices[lastVert].fParentIsPseudoSource)
		{
			continue;
		}
		int parentEdgeIndex = m_InfoAtVertices[lastVert].indexOfParent;
		int edgeIndex = model.Edge(parentEdgeIndex).indexOfReverseEdge;
		pair<double, double> coord(model.GetNew2DCoordinatesByReversingCurrentEdge(parentEdgeIndex, model.Edge(parentEdgeIndex).coordOfOppositeVert));

		double proportion = 1 - m_InfoAtVertices[lastVert].entryProp;
		while (1)
		{
			resultingPath.push_back(EdgePoint(edgeIndex, proportion));
			if (model.Edge(edgeIndex).indexOfOppositeVert == vertexNodes[i + 1])
				break;
			double oldProprotion = proportion;
			proportion = model.ProportionOnLeftEdgeByImage(edgeIndex, coord, oldProprotion);
			if (model.Edge(edgeIndex).indexOfLeftEdge == -1 || model.Edge(edgeIndex).indexOfRightEdge == -1)
			{
				break;
			}

			if (proportion >= -LENGTH_EPSILON_CONTROL && proportion <= 1)
			{
                proportion = max(proportion, 0.0);
				coord = model.GetNew2DCoordinatesByRotatingAroundLeftChildEdge(edgeIndex, coord);
				edgeIndex = model.Edge(edgeIndex).indexOfLeftEdge;
				//rightLen = disToAngle;
			}
			else
			{
				proportion = model.ProportionOnRightEdgeByImage(edgeIndex, coord, oldProprotion);
                proportion = max(proportion, 0.0);
                proportion = min(proportion, 1.0);
				coord = model.GetNew2DCoordinatesByRotatingAroundRightChildEdge(edgeIndex, coord);
				edgeIndex = model.Edge(edgeIndex).indexOfRightEdge;
			}
		};
	}
	resultingPath.push_back(EdgePoint(indexOfSourceVert));
	return resultingPath;
}

int GetTickCount()
{
  // Something like GetTickCount but portable
  // It rolls over every ~ 12.1 days (0x100000/24/60/60)
  // Use GetMilliSpan to correct for rollover
  timeb tb;
  ftime( &tb );
  int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
  return nCount;
}

void CExactMethodForDGP::Execute()
{
	if (fComputationCompleted)
		return;
	if (!fLocked)
	{
		fLocked = true;
		nCountOfWindows = 0;
		nMaxLenOfWindowQueue = 0;
		depthOfResultingTree = 0;
		InitContainers();
        nTotalMilliSeconds = GetTickCount();
		BuildSequenceTree();
        nTotalMilliSeconds = GetTickCount()- nTotalMilliSeconds;
		FillExperimentalResults();
		ClearContainers();

		fComputationCompleted = true;
		fLocked = false;
	}
}

