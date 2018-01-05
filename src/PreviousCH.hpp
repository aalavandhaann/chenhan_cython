// PreviousCH.h: interface for the CPreviousCH class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "stdafx.hpp"
#include "ExactMethodForDGP.hpp"


using namespace std;


struct InfoAtAngle
{
	char birthTime;
	double disUptodate;
	double entryProp;
	InfoAtAngle()
	{
		birthTime = -1;
		disUptodate = DBL_MAX;
	}
};
struct Window
{
	bool fIsOnLeftSubtree;
	bool fParentIsPseudoSource;
	bool fDirectParentEdgeOnLeft; //may removed
	bool fDirectParenIsPseudoSource; //may removed
	char birthTimeOfParent;
	int indexOfParent;
	int indexOfRoot;
	int indexOfCurEdge;
	int level;//may removed
	double disToRoot;
	double proportions[2];
	double entryPropOfParent;
	std::pair<double, double> coordOfPseudoSource;
};//at least 64 bytes.
struct QuoteWindow
{
	Window* pWindow;
	double disUptodate;
	bool operator<(const QuoteWindow& another) const
	{
		return disUptodate > another.disUptodate;
	}
};


class CPreviousCH : public CExactMethodForDGP
{
protected:	
    std::queue<QuoteWindow> m_QueueForWindows;
    std::queue<QuoteInfoAtVertex> m_QueueForPseudoSources;
    std::vector<InfoAtAngle> m_InfoAtAngles;
protected:
	inline bool IsTooNarrowWindow(const Window& w) const;		

	virtual void InitContainers();
	virtual void ClearContainers();	
	virtual void BuildSequenceTree();
    virtual void AddIntoQueueOfPseudoSources(QuoteInfoAtVertex quoteOfPseudoSource);
	virtual void AddIntoQueueOfWindows(QuoteWindow& quoteW);	
	virtual bool UpdateTreeDepthBackWithChoice();
	virtual bool CheckValidityOfWindow(Window& w);
	virtual void FillExperimentalResults();

	inline void ComputeChildrenOfPseudoSource(int indexOfParentVertex);
	inline void ComputeChildrenOfPseudoSourceFromPseudoSource(int indexOfParentVertex);
	inline void ComputeChildrenOfPseudoSourceFromWindow(int indexOfParentVertex);
	inline void CreateIntervalChildOfPseudoSource(int source, int subIndexOfIncidentEdge);
	inline void FillVertChildOfPseudoSource(int source, int subIndexOfVert);
	inline void ComputeChildrenOfSource();
	inline void ComputeChildrenOfSource(int indexOfSourceVert);

	inline void ComputeTheOnlyLeftChild(const Window& w);
	inline void ComputeTheOnlyLeftTrimmedChild(const Window& w);
	inline void ComputeLeftTrimmedChildWithParent(const Window& w);
	inline void ComputeTheOnlyRightChild(const Window& w);
	inline void ComputeTheOnlyRightTrimmedChild(const Window& w);
	inline void ComputeRightTrimmedChildWithParent(const Window& w);
	inline void ComputeChildrenOfWindow(QuoteWindow& quoteParentWindow);
public:
    CPreviousCH(const CRichModel& inputModel, const std::set<int> &indexOfSourceVerts);
	virtual ~CPreviousCH();	
};

bool CPreviousCH::IsTooNarrowWindow(const Window& w) const
{
	return w.proportions[1] - w.proportions[0] < LENGTH_EPSILON_CONTROL;
}

void CPreviousCH::ComputeChildrenOfSource(int indexOfSourceVert)
{
	//m_InfoAtVertices[indexOfSourceVert].fParentIsPseudoSource;
	++m_InfoAtVertices[indexOfSourceVert].birthTime;
	//m_InfoAtVertices[indexOfSourceVert].indexOfParent;
	//m_InfoAtVertices[indexOfSourceVert].indexOfRootVertOfParent;
	m_InfoAtVertices[indexOfSourceVert].level = 0;
	m_InfoAtVertices[indexOfSourceVert].disUptodate = 0;
	//m_InfoAtVertices[indexOfSourceVert].entryProp;	

	int degree = (int)model.Neigh(indexOfSourceVert).size();
	for (int i = 0; i < degree; ++i) // vertex-nodes
	{
		FillVertChildOfPseudoSource(indexOfSourceVert, i);
	}
	
	for (int i = 0; i < degree; ++i)
	{
		CreateIntervalChildOfPseudoSource(indexOfSourceVert, i);	
	}
}

void CPreviousCH::ComputeChildrenOfSource()
{
    for (std::set<int>::const_iterator it = indexOfSourceVerts.begin();
		it != indexOfSourceVerts.end(); ++it)
	{
		if (*it >= model.GetNumOfVerts())
			continue;
		ComputeChildrenOfSource(*it);
	}
}

void CPreviousCH::ComputeChildrenOfPseudoSourceFromPseudoSource(int indexOfParentVertex)
{
	int degree = (int)model.Neigh(indexOfParentVertex).size();
    const std::vector<std::pair<int, double> >& neighs = model.Neigh(indexOfParentVertex);
	int indexOfParentOfParent = m_InfoAtVertices[indexOfParentVertex].indexOfParent;
	int subIndex = model.GetSubindexToVert(indexOfParentVertex, indexOfParentOfParent);
	double angleSum(0);
	int indexPlus;
	for (indexPlus = subIndex; indexPlus != (subIndex - 1 + degree) % degree; indexPlus = (indexPlus + 1) % degree)
	{
		angleSum += neighs[indexPlus].second;
		if (angleSum > M_PI - ToleranceOfConvexAngle)
			break;
	}
	angleSum = 0;
	int indexMinus;
	for (indexMinus = (subIndex - 1 + degree) % degree; 
	indexMinus == (subIndex - 1 + degree) % degree || indexMinus != (indexPlus - 1 + degree) % degree; 
	indexMinus = (indexMinus - 1 + degree) % degree)
	{
		angleSum += neighs[indexMinus].second;
		if (angleSum > M_PI - ToleranceOfConvexAngle)
			break;
	}
	if (indexMinus == (indexPlus - 1 + degree) % degree)
		return;
	//vertices;
	for (int i = (indexPlus + 1) % degree; i != (indexMinus + 1) % degree; i = (i + 1) % degree)
	{
		FillVertChildOfPseudoSource(indexOfParentVertex, i);
	}
	
	//windows
	for (int i = indexPlus; i != (indexMinus + 1) % degree; i = (i + 1) % degree)
	{
		CreateIntervalChildOfPseudoSource(indexOfParentVertex, i);
	}	
}

void CPreviousCH::ComputeChildrenOfPseudoSourceFromWindow(int indexOfParentVertex)
{
	int degree = (int)model.Neigh(indexOfParentVertex).size();
    const std::vector<std::pair<int, double> >& neighs = model.Neigh(indexOfParentVertex);
	int indexOfParentOfParent = m_InfoAtVertices[indexOfParentVertex].indexOfParent;
	int leftVert = model.Edge(indexOfParentOfParent).indexOfLeftVert;
	int rightVert = model.Edge(indexOfParentOfParent).indexOfRightVert;
	int subIndexLeft = model.GetSubindexToVert(indexOfParentVertex, leftVert);
	int subIndexRight = (subIndexLeft + 1) % degree;
	double x1 = m_InfoAtVertices[indexOfParentVertex].entryProp * model.Edge(indexOfParentOfParent).length;
	double y1 = 0;
	double x2 = model.Edge(indexOfParentOfParent).length;
	double y2 = 0;
	x1 -= model.Edge(indexOfParentOfParent).coordOfOppositeVert.first;
	y1 -= model.Edge(indexOfParentOfParent).coordOfOppositeVert.second;
	x2 -= model.Edge(indexOfParentOfParent).coordOfOppositeVert.first;
	y2 -= model.Edge(indexOfParentOfParent).coordOfOppositeVert.second;

	double anglePlus = acos((x1 * x2 + y1 * y2) / sqrt((x1 * x1 + y1 * y1) * (x2 * x2 + y2 * y2)));
	double angleSum(anglePlus);
	int indexPlus;
	for (indexPlus = subIndexRight; indexPlus != subIndexLeft; indexPlus = (indexPlus + 1) % degree)
	{
		angleSum += neighs[indexPlus].second;
		if (angleSum > M_PI - ToleranceOfConvexAngle)
			break;
	}
	angleSum = neighs[subIndexLeft].second - anglePlus;
	int indexMinus;
	for (indexMinus = (subIndexLeft - 1 + degree) % degree; indexMinus != (indexPlus - 1 + degree) % degree; indexMinus = (indexMinus - 1 + degree) % degree)
	{
		angleSum += neighs[indexMinus].second;
		if (angleSum > M_PI - ToleranceOfConvexAngle)
			break;
	}
	if (indexMinus == (indexPlus - 1 + degree) % degree)
		return;
	for (int i = 0; i < degree; ++i)
	{
		FillVertChildOfPseudoSource(indexOfParentVertex, i);
	}	
	//windows
	for (int i = indexPlus; i != (indexMinus + 1) % degree; i = (i + 1) % degree)
	{
		CreateIntervalChildOfPseudoSource(indexOfParentVertex, i);
	}
}

void CPreviousCH::ComputeChildrenOfWindow(QuoteWindow& quoteParentWindow)
{
	const Window& w = *quoteParentWindow.pWindow;
	const CEdge& edge = model.Edge(w.indexOfCurEdge);
	double entryProp = model.ProportionOnEdgeByImage(w.indexOfCurEdge, w.coordOfPseudoSource);	
	if (entryProp >= w.proportions[1])
	{
		ComputeTheOnlyLeftChild(w);
		return;
	}
	if (entryProp <= w.proportions[0])
	{
		ComputeTheOnlyRightChild(w);
		return;
	}
	double disToAngle = model.DistanceToIncidentAngle(w.indexOfCurEdge, w.coordOfPseudoSource);
	int incidentVertex = edge.indexOfOppositeVert;
	bool fLeftChildToCompute(false), fRightChildToCompute(false);
	bool fWIsWinning(false);
	double totalDis = w.disToRoot + disToAngle;

	if (m_InfoAtAngles[w.indexOfCurEdge].birthTime == -1)
	{
		fLeftChildToCompute = fRightChildToCompute = true;
		fWIsWinning = true;
	}
	else
	{
		if (totalDis < m_InfoAtAngles[w.indexOfCurEdge].disUptodate
			- LENGTH_EPSILON_CONTROL)
		{
			fLeftChildToCompute = fRightChildToCompute = true;
			fWIsWinning = true;
		}
		else
		{
			fLeftChildToCompute = entryProp < m_InfoAtAngles[w.indexOfCurEdge].entryProp;
			fRightChildToCompute = !fLeftChildToCompute;
			fWIsWinning = false;
		}
		
	}
	if (!fWIsWinning)
	{		
		if (fLeftChildToCompute)
		{
			ComputeTheOnlyLeftTrimmedChild(w);				
		}
		if (fRightChildToCompute)
		{
			ComputeTheOnlyRightTrimmedChild(w);				
		}
		return;
	}

	m_InfoAtAngles[w.indexOfCurEdge].disUptodate = totalDis;
	m_InfoAtAngles[w.indexOfCurEdge].entryProp = entryProp;
	++m_InfoAtAngles[w.indexOfCurEdge].birthTime;

	ComputeLeftTrimmedChildWithParent(w);
	ComputeRightTrimmedChildWithParent(w);	
	if (totalDis < m_InfoAtVertices[incidentVertex].disUptodate - LENGTH_EPSILON_CONTROL)
	{
		m_InfoAtVertices[incidentVertex].fParentIsPseudoSource = false;
		++m_InfoAtVertices[incidentVertex].birthTime;
		m_InfoAtVertices[incidentVertex].indexOfParent = w.indexOfCurEdge;
		m_InfoAtVertices[incidentVertex].indexOfRootVertOfParent = w.indexOfRoot;
		m_InfoAtVertices[incidentVertex].level = w.level + 1;
		m_InfoAtVertices[incidentVertex].disUptodate = totalDis;
		m_InfoAtVertices[incidentVertex].entryProp = entryProp;
		
		if (!model.IsConvexVert(incidentVertex))
            AddIntoQueueOfPseudoSources(QuoteInfoAtVertex(m_InfoAtVertices[incidentVertex].birthTime, incidentVertex, totalDis));
	}
}

void CPreviousCH::ComputeChildrenOfPseudoSource(int indexOfParentVertex)
{
	if (m_InfoAtVertices[indexOfParentVertex].fParentIsPseudoSource)
		ComputeChildrenOfPseudoSourceFromPseudoSource(indexOfParentVertex);
	else
		ComputeChildrenOfPseudoSourceFromWindow(indexOfParentVertex);
}
void CPreviousCH::CreateIntervalChildOfPseudoSource(int source, int subIndexOfIncidentEdge)
{
	int indexOfIncidentEdge = model.Neigh(source)[subIndexOfIncidentEdge].first;
	if (model.IsExtremeEdge(indexOfIncidentEdge))
		return;
	const CEdge& edge = model.Edge(indexOfIncidentEdge);
	int edgeIndex = edge.indexOfRightEdge;
	if (model.IsExtremeEdge(edgeIndex))
		return;
	QuoteWindow quoteW;
	quoteW.pWindow = new Window;
	//quoteW.pWindow->fIsOnLeftSubtree;
	quoteW.pWindow->fParentIsPseudoSource = true;
	//quoteW.pWindow->fDirectParentEdgeOnLeft;
	quoteW.pWindow->fDirectParenIsPseudoSource = true;
	quoteW.pWindow->birthTimeOfParent = m_InfoAtVertices[source].birthTime;
	quoteW.pWindow->indexOfParent = source;
	quoteW.pWindow->indexOfRoot = source;
	quoteW.pWindow->indexOfCurEdge = edgeIndex;
	quoteW.pWindow->level = m_InfoAtVertices[source].level + 1;
	quoteW.pWindow->disToRoot = m_InfoAtVertices[source].disUptodate;
	quoteW.pWindow->proportions[0] = 0;
	quoteW.pWindow->proportions[1] = 1;
	//quoteW.pWindow->entryPropOfParent;
	int reverseEdge = model.Edge(edgeIndex).indexOfReverseEdge;
	quoteW.pWindow->coordOfPseudoSource = model.GetNew2DCoordinatesByReversingCurrentEdge(reverseEdge,
		model.Edge(reverseEdge).coordOfOppositeVert);
	AddIntoQueueOfWindows(quoteW);
}

void CPreviousCH::FillVertChildOfPseudoSource(int source, int subIndexOfVert)
{
	const CEdge& edge = model.Edge(model.Neigh(source)[subIndexOfVert].first);
	int index = edge.indexOfRightVert;		
	double dis = m_InfoAtVertices[source].disUptodate + edge.length;
	if (dis >= m_InfoAtVertices[index].disUptodate - LENGTH_EPSILON_CONTROL)
		return;
	m_InfoAtVertices[index].fParentIsPseudoSource = true;
	++m_InfoAtVertices[index].birthTime;
	m_InfoAtVertices[index].indexOfParent = source;
	//m_InfoAtVertices[index].indexOfRootVertOfParent;
	m_InfoAtVertices[index].level = m_InfoAtVertices[source].level + 1;
	m_InfoAtVertices[index].disUptodate = dis;
	//m_InfoAtVertices[index].entryProp;		
	if (!model.IsConvexVert(index))
        AddIntoQueueOfPseudoSources(QuoteInfoAtVertex(m_InfoAtVertices[index].birthTime, index, dis));
}


void CPreviousCH::ComputeTheOnlyLeftChild(const Window& w)
{	
	if (model.IsExtremeEdge(model.Edge(w.indexOfCurEdge).indexOfLeftEdge))
		return;
	QuoteWindow quoteW;
	quoteW.pWindow = new Window;
	quoteW.pWindow->proportions[0] = model.ProportionOnLeftEdgeByImage(w.indexOfCurEdge,
		w.coordOfPseudoSource, w.proportions[0]);
    quoteW.pWindow->proportions[0] = std::max(0.0, quoteW.pWindow->proportions[0]);
	quoteW.pWindow->proportions[1] = model.ProportionOnLeftEdgeByImage(w.indexOfCurEdge,
		w.coordOfPseudoSource, w.proportions[1]);
    quoteW.pWindow->proportions[1] = std::min(1.0, quoteW.pWindow->proportions[1]);
	if (IsTooNarrowWindow(*quoteW.pWindow))
	{
		delete quoteW.pWindow;
		return;
	}
	quoteW.pWindow->fParentIsPseudoSource = w.fParentIsPseudoSource;
	quoteW.pWindow->fDirectParenIsPseudoSource = false;
	quoteW.pWindow->fDirectParentEdgeOnLeft = true;
	quoteW.pWindow->indexOfCurEdge = model.Edge(w.indexOfCurEdge).indexOfLeftEdge;
	quoteW.pWindow->disToRoot = w.disToRoot;

	quoteW.pWindow->coordOfPseudoSource = model.GetNew2DCoordinatesByRotatingAroundLeftChildEdge(w.indexOfCurEdge, w.coordOfPseudoSource);
	if (!CheckValidityOfWindow(*quoteW.pWindow))
	{
		delete quoteW.pWindow;
		return;
	}
	quoteW.pWindow->fIsOnLeftSubtree = w.fIsOnLeftSubtree;
	quoteW.pWindow->level = w.level + 1;	
	quoteW.pWindow->entryPropOfParent = w.entryPropOfParent;
	quoteW.pWindow->birthTimeOfParent = w.birthTimeOfParent;
	quoteW.pWindow->indexOfParent = w.indexOfParent;
	quoteW.pWindow->indexOfRoot = w.indexOfRoot;	
	AddIntoQueueOfWindows(quoteW);
}

void CPreviousCH::ComputeTheOnlyRightChild(const Window& w)
{
	if (model.IsExtremeEdge(model.Edge(w.indexOfCurEdge).indexOfRightEdge))
		return;
	QuoteWindow quoteW;
	quoteW.pWindow = new Window;
	quoteW.pWindow->proportions[0] = model.ProportionOnRightEdgeByImage(w.indexOfCurEdge,
		w.coordOfPseudoSource, w.proportions[0]);
    quoteW.pWindow->proportions[0] = std::max(0.0, quoteW.pWindow->proportions[0]);
	quoteW.pWindow->proportions[1] = model.ProportionOnRightEdgeByImage(w.indexOfCurEdge,
		w.coordOfPseudoSource, w.proportions[1]);
    quoteW.pWindow->proportions[1] = std::min(1.0, quoteW.pWindow->proportions[1]);
	if (IsTooNarrowWindow(*quoteW.pWindow))
	{
		delete quoteW.pWindow;
		return;
	}
	quoteW.pWindow->fParentIsPseudoSource = w.fParentIsPseudoSource;
	quoteW.pWindow->fDirectParenIsPseudoSource = false;
	quoteW.pWindow->fDirectParentEdgeOnLeft = false;
	quoteW.pWindow->indexOfCurEdge = model.Edge(w.indexOfCurEdge).indexOfRightEdge;
	quoteW.pWindow->disToRoot = w.disToRoot;
	quoteW.pWindow->coordOfPseudoSource = model.GetNew2DCoordinatesByRotatingAroundRightChildEdge(w.indexOfCurEdge, w.coordOfPseudoSource);
	if (!CheckValidityOfWindow(*quoteW.pWindow))
	{
		delete quoteW.pWindow;
		return;
	}
	quoteW.pWindow->level = w.level + 1;	
	quoteW.pWindow->birthTimeOfParent = w.birthTimeOfParent;
	quoteW.pWindow->indexOfParent = w.indexOfParent;
	quoteW.pWindow->indexOfRoot = w.indexOfRoot;
	quoteW.pWindow->fIsOnLeftSubtree = w.fIsOnLeftSubtree;
	quoteW.pWindow->entryPropOfParent = w.entryPropOfParent;
	AddIntoQueueOfWindows(quoteW);
}

void CPreviousCH::ComputeTheOnlyLeftTrimmedChild(const Window& w)
{
	if (model.IsExtremeEdge(model.Edge(w.indexOfCurEdge).indexOfLeftEdge))
		return;
	QuoteWindow quoteW;
	quoteW.pWindow = new Window;
	quoteW.pWindow->proportions[0] = model.ProportionOnLeftEdgeByImage(w.indexOfCurEdge,
		w.coordOfPseudoSource, w.proportions[0]);
    quoteW.pWindow->proportions[0] = std::max(0.0, quoteW.pWindow->proportions[0]);
	quoteW.pWindow->proportions[1] = 1;
	if (IsTooNarrowWindow(*quoteW.pWindow))
	{
		delete quoteW.pWindow;
		return;
	}
	quoteW.pWindow->fParentIsPseudoSource = w.fParentIsPseudoSource;
	quoteW.pWindow->fDirectParenIsPseudoSource = false;
	quoteW.pWindow->fDirectParentEdgeOnLeft = true;
	quoteW.pWindow->indexOfCurEdge = model.Edge(w.indexOfCurEdge).indexOfLeftEdge;
	quoteW.pWindow->disToRoot = w.disToRoot;
	quoteW.pWindow->coordOfPseudoSource = model.GetNew2DCoordinatesByRotatingAroundLeftChildEdge(w.indexOfCurEdge, w.coordOfPseudoSource);
	if (!CheckValidityOfWindow(*quoteW.pWindow))
	{
		delete quoteW.pWindow;
		return;
	}
	quoteW.pWindow->level = w.level + 1;	
	quoteW.pWindow->birthTimeOfParent = w.birthTimeOfParent;
	quoteW.pWindow->indexOfParent = w.indexOfParent;
	quoteW.pWindow->indexOfRoot = w.indexOfRoot;
	quoteW.pWindow->fIsOnLeftSubtree = w.fIsOnLeftSubtree;
	quoteW.pWindow->entryPropOfParent = w.entryPropOfParent;
	AddIntoQueueOfWindows(quoteW);	
}

void CPreviousCH::ComputeTheOnlyRightTrimmedChild(const Window& w)
{
	if (model.IsExtremeEdge(model.Edge(w.indexOfCurEdge).indexOfRightEdge))
		return;
	QuoteWindow quoteW;
	quoteW.pWindow = new Window;
	quoteW.pWindow->proportions[0] = 0;
	quoteW.pWindow->proportions[1] = model.ProportionOnRightEdgeByImage(w.indexOfCurEdge,
		w.coordOfPseudoSource, w.proportions[1]);
    quoteW.pWindow->proportions[1] = std::min(1.0, quoteW.pWindow->proportions[1]);
	if (IsTooNarrowWindow(*quoteW.pWindow))
	{
		delete quoteW.pWindow;
		return;
	}
	quoteW.pWindow->fParentIsPseudoSource = w.fParentIsPseudoSource;
	quoteW.pWindow->fDirectParenIsPseudoSource = false;
	quoteW.pWindow->fDirectParentEdgeOnLeft = false;
	quoteW.pWindow->indexOfCurEdge = model.Edge(w.indexOfCurEdge).indexOfRightEdge;
	quoteW.pWindow->disToRoot = w.disToRoot;
	quoteW.pWindow->coordOfPseudoSource = model.GetNew2DCoordinatesByRotatingAroundRightChildEdge(w.indexOfCurEdge, w.coordOfPseudoSource);
	if (!CheckValidityOfWindow(*quoteW.pWindow))
	{
		delete quoteW.pWindow;
		return;
	}
	quoteW.pWindow->level = w.level + 1;	
	quoteW.pWindow->birthTimeOfParent = w.birthTimeOfParent;
	quoteW.pWindow->indexOfParent = w.indexOfParent;
	quoteW.pWindow->indexOfRoot = w.indexOfRoot;
	quoteW.pWindow->fIsOnLeftSubtree = w.fIsOnLeftSubtree;
	quoteW.pWindow->entryPropOfParent = w.entryPropOfParent;
	AddIntoQueueOfWindows(quoteW);	
}

void CPreviousCH::ComputeLeftTrimmedChildWithParent(const Window& w)
{
	if (model.IsExtremeEdge(model.Edge(w.indexOfCurEdge).indexOfLeftEdge))
		return;
	QuoteWindow quoteW;
	quoteW.pWindow = new Window;
	quoteW.pWindow->proportions[0] = model.ProportionOnLeftEdgeByImage(w.indexOfCurEdge,
		w.coordOfPseudoSource, w.proportions[0]);
    quoteW.pWindow->proportions[0] = std::max(0.0, quoteW.pWindow->proportions[0]);
	quoteW.pWindow->proportions[1] = 1;
	if (IsTooNarrowWindow(*quoteW.pWindow))
	{
		delete quoteW.pWindow;
		return;
	}
	quoteW.pWindow->fParentIsPseudoSource = false;
	quoteW.pWindow->fDirectParenIsPseudoSource = false;
	quoteW.pWindow->fDirectParentEdgeOnLeft = true;
	quoteW.pWindow->indexOfCurEdge = model.Edge(w.indexOfCurEdge).indexOfLeftEdge;
	quoteW.pWindow->disToRoot = w.disToRoot;
	quoteW.pWindow->coordOfPseudoSource = model.GetNew2DCoordinatesByRotatingAroundLeftChildEdge(w.indexOfCurEdge, w.coordOfPseudoSource);
	if (!CheckValidityOfWindow(*quoteW.pWindow))
	{
		delete quoteW.pWindow;
		return;
	}
	quoteW.pWindow->level = w.level + 1;	
	quoteW.pWindow->birthTimeOfParent = m_InfoAtAngles[w.indexOfCurEdge].birthTime;
	quoteW.pWindow->indexOfParent = w.indexOfCurEdge;
	quoteW.pWindow->indexOfRoot = w.indexOfRoot;
	quoteW.pWindow->fIsOnLeftSubtree = true;
	quoteW.pWindow->entryPropOfParent = m_InfoAtAngles[w.indexOfCurEdge].entryProp;
	AddIntoQueueOfWindows(quoteW);
}


void CPreviousCH::ComputeRightTrimmedChildWithParent(const Window& w)
{
	if (model.IsExtremeEdge(model.Edge(w.indexOfCurEdge).indexOfRightEdge))
		return;
	QuoteWindow quoteW;
	quoteW.pWindow = new Window;
	quoteW.pWindow->proportions[0] = 0;
	quoteW.pWindow->proportions[1] = model.ProportionOnRightEdgeByImage(w.indexOfCurEdge,
		w.coordOfPseudoSource, w.proportions[1]);
    quoteW.pWindow->proportions[1] = std::min(1.0, quoteW.pWindow->proportions[1]);
	if (IsTooNarrowWindow(*quoteW.pWindow))
	{
		delete quoteW.pWindow;
		return;
	}
	quoteW.pWindow->fParentIsPseudoSource = false;
	quoteW.pWindow->fDirectParenIsPseudoSource = false;
	quoteW.pWindow->fDirectParentEdgeOnLeft = false;
	quoteW.pWindow->indexOfCurEdge = model.Edge(w.indexOfCurEdge).indexOfRightEdge;
	quoteW.pWindow->disToRoot = w.disToRoot;
	quoteW.pWindow->coordOfPseudoSource = model.GetNew2DCoordinatesByRotatingAroundRightChildEdge(w.indexOfCurEdge, w.coordOfPseudoSource);
	if (!CheckValidityOfWindow(*quoteW.pWindow))
	{
		delete quoteW.pWindow;
		return;
	}
	quoteW.pWindow->fIsOnLeftSubtree = false;
	quoteW.pWindow->birthTimeOfParent = m_InfoAtAngles[w.indexOfCurEdge].birthTime;
	quoteW.pWindow->indexOfParent = w.indexOfCurEdge;
	quoteW.pWindow->indexOfRoot = w.indexOfRoot;
	quoteW.pWindow->level = w.level + 1;	
	quoteW.pWindow->entryPropOfParent = m_InfoAtAngles[w.indexOfCurEdge].entryProp;
	AddIntoQueueOfWindows(quoteW);
}

























// PreviousCH.cpp: implementation of the CPreviousCH class.
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPreviousCH::CPreviousCH(const CRichModel& inputModel, const set<int> &indexOfSourceVerts) : CExactMethodForDGP(inputModel, indexOfSourceVerts)
{
	nameOfAlgorithm = "CH";
}

CPreviousCH::~CPreviousCH()
{
}

void CPreviousCH::InitContainers()
{
	m_InfoAtAngles.resize(model.GetNumOfEdges());
	memory += double(model.GetNumOfEdges()) * sizeof(InfoAtAngle) / 1024 / 1024;
}

void CPreviousCH::BuildSequenceTree()
{
	ComputeChildrenOfSource();
	bool fFromQueueOfPseudoSources = UpdateTreeDepthBackWithChoice();
	while (depthOfResultingTree < model.GetNumOfFaces() && !(m_QueueForPseudoSources.empty() && m_QueueForWindows.empty()))
	{
		if ((int)m_QueueForWindows.size() > nMaxLenOfWindowQueue)
			nMaxLenOfWindowQueue = (int)m_QueueForWindows.size();
		if (m_QueueForPseudoSources.size() > nMaxLenOfPseudoSources)
			nMaxLenOfPseudoSources = (int)m_QueueForPseudoSources.size();
		if (fFromQueueOfPseudoSources) //pseudosource
		{
			int indexOfVert = m_QueueForPseudoSources.front().indexOfVert;
			m_QueueForPseudoSources.pop();
			ComputeChildrenOfPseudoSource(indexOfVert);
		}
		else
		{
			QuoteWindow quoteW = m_QueueForWindows.front();
			m_QueueForWindows.pop();
			ComputeChildrenOfWindow(quoteW);
			delete quoteW.pWindow;
		}
		fFromQueueOfPseudoSources = UpdateTreeDepthBackWithChoice();
	}
}

void CPreviousCH::FillExperimentalResults()
{
	NPE = 1;
	//memory += double(nMaxLenOfPseudoSources) * sizeof(QuoteInfoAtVertex) / 1024 / 1024;
	//memory += double(nMaxLenOfWindowQueue) * (sizeof(QuoteWindow) + 64) / 1024 / 1024;
}

void CPreviousCH::ClearContainers()
{
	while (!m_QueueForWindows.empty())
	{
		delete m_QueueForWindows.front().pWindow;
		m_QueueForWindows.pop();
	}

	while (!m_QueueForPseudoSources.empty())
	{
		m_QueueForPseudoSources.pop();
	}
}


void CPreviousCH::AddIntoQueueOfPseudoSources(QuoteInfoAtVertex quoteOfPseudoSource)
{
	m_QueueForPseudoSources.push(quoteOfPseudoSource);
}

void CPreviousCH::AddIntoQueueOfWindows(QuoteWindow& quoteW)
{
	m_QueueForWindows.push(quoteW);
	++nCountOfWindows;
}

bool CPreviousCH::UpdateTreeDepthBackWithChoice()
{
	while (!m_QueueForPseudoSources.empty()
		&& m_QueueForPseudoSources.front().birthTime != m_InfoAtVertices[m_QueueForPseudoSources.front().indexOfVert].birthTime)
		m_QueueForPseudoSources.pop();

	while (!m_QueueForWindows.empty())
	{
		const QuoteWindow& quoteW = m_QueueForWindows.front();
		if (quoteW.pWindow->fParentIsPseudoSource)
		{
			if (quoteW.pWindow->birthTimeOfParent !=
				m_InfoAtVertices[quoteW.pWindow->indexOfParent].birthTime)
			{
				delete quoteW.pWindow;
				m_QueueForWindows.pop();
			}
			else
				break;
		}
		else
		{
			if (quoteW.pWindow->birthTimeOfParent ==
				m_InfoAtAngles[quoteW.pWindow->indexOfParent].birthTime)
				break;
			else if (quoteW.pWindow->fIsOnLeftSubtree ==
				(quoteW.pWindow->entryPropOfParent < m_InfoAtAngles[quoteW.pWindow->indexOfParent].entryProp))
				break;
			else
			{
				delete quoteW.pWindow;
				m_QueueForWindows.pop();
			}
		}
	}

	bool fFromQueueOfPseudoSources(false);
	if (m_QueueForWindows.empty())
	{
		if (!m_QueueForPseudoSources.empty())
		{
			const InfoAtVertex& infoOfHeadElemOfPseudoSources = m_InfoAtVertices[m_QueueForPseudoSources.front().indexOfVert];
			depthOfResultingTree = max(depthOfResultingTree,
				infoOfHeadElemOfPseudoSources.level);
			fFromQueueOfPseudoSources = true;
		}
	}
	else
	{
		if (m_QueueForPseudoSources.empty())
		{
			const Window& infoOfHeadElemOfWindows = *m_QueueForWindows.front().pWindow;
			depthOfResultingTree = max(depthOfResultingTree,
				infoOfHeadElemOfWindows.level);
			fFromQueueOfPseudoSources = false;
		}
		else
		{
			const InfoAtVertex& infoOfHeadElemOfPseudoSources = m_InfoAtVertices[m_QueueForPseudoSources.front().indexOfVert];
			const Window& infoOfHeadElemOfWindows = *m_QueueForWindows.front().pWindow;
			if (infoOfHeadElemOfPseudoSources.level <=
				infoOfHeadElemOfWindows.level)
			{
				depthOfResultingTree = max(depthOfResultingTree,
					infoOfHeadElemOfPseudoSources.level);
				fFromQueueOfPseudoSources = true;
			}
			else
			{
				depthOfResultingTree = max(depthOfResultingTree,
					infoOfHeadElemOfWindows.level);
				fFromQueueOfPseudoSources = false;
			}
		}
	}
	return fFromQueueOfPseudoSources;
}

bool CPreviousCH::CheckValidityOfWindow(Window& w)
{
	return true;
}



