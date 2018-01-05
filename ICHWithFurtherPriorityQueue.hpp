// ICHWithFurtherPriorityQueue.h: interface for the CICHWithFurtherPriorityQueue class.
//
//////////////////////////////////////////////////////////////////////

#pragma once
#include "stdafx.hpp"
#include "ImprovedCHWithEdgeValve.hpp"

using namespace std;



class CICHWithFurtherPriorityQueue : public CImprovedCHWithEdgeValve  
{
protected:
    std::priority_queue<QuoteWindow> m_QueueForWindows;
    std::priority_queue<QuoteInfoAtVertex> m_QueueForPseudoSources;
protected:
    inline void AddIntoQueueOfPseudoSources(QuoteInfoAtVertex quoteOfPseudoSource);
	inline void AddIntoQueueOfWindows(QuoteWindow& quoteW);
	inline bool UpdateTreeDepthBackWithChoice();
	inline double GetMinDisOfWindow(const Window& w) const;
	virtual void BuildSequenceTree();
	virtual void InitContainers();
	virtual void ClearContainers();
public:
    CICHWithFurtherPriorityQueue(const CRichModel& inputModel, const std::set<int> &indexOfSourceVerts);
	virtual ~CICHWithFurtherPriorityQueue();	
};

double CICHWithFurtherPriorityQueue::GetMinDisOfWindow(const Window& w) const
{
	double projProp = w.coordOfPseudoSource.first / model.Edge(w.indexOfCurEdge).length;
	if (projProp <= w.proportions[0])
	{
		double detaX = w.coordOfPseudoSource.first - w.proportions[0] * model.Edge(w.indexOfCurEdge).length;
		return w.disToRoot + sqrt(detaX * detaX + w.coordOfPseudoSource.second * w.coordOfPseudoSource.second);
	}
	if (projProp >= w.proportions[1])
	{
		double detaX = w.coordOfPseudoSource.first - w.proportions[1] * model.Edge(w.indexOfCurEdge).length;
		return w.disToRoot + sqrt(detaX * detaX + w.coordOfPseudoSource.second * w.coordOfPseudoSource.second);
	}
	return w.disToRoot - w.coordOfPseudoSource.second;
}

void CICHWithFurtherPriorityQueue::AddIntoQueueOfPseudoSources(QuoteInfoAtVertex quoteOfPseudoSource)
{
	m_QueueForPseudoSources.push(quoteOfPseudoSource);
}

void CICHWithFurtherPriorityQueue::AddIntoQueueOfWindows(QuoteWindow& quoteW)
{
	quoteW.disUptodate = GetMinDisOfWindow(*quoteW.pWindow);
	m_QueueForWindows.push(quoteW);
	++nCountOfWindows;
}

#define max(a,b) (((a) >= (b)) ? (a) : (b))

bool CICHWithFurtherPriorityQueue::UpdateTreeDepthBackWithChoice()
{
	while (!m_QueueForPseudoSources.empty()
		&& m_QueueForPseudoSources.top().birthTime 
		!= m_InfoAtVertices[m_QueueForPseudoSources.top().indexOfVert].birthTime)
		m_QueueForPseudoSources.pop();

	while (!m_QueueForWindows.empty())
	{
		const QuoteWindow& quoteW = m_QueueForWindows.top();
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
			const InfoAtVertex& infoOfHeadElemOfPseudoSources = m_InfoAtVertices[m_QueueForPseudoSources.top().indexOfVert];
            depthOfResultingTree = max(depthOfResultingTree,
				infoOfHeadElemOfPseudoSources.level);
			fFromQueueOfPseudoSources = true;
		}
	}
	else 
	{
		if (m_QueueForPseudoSources.empty())
		{
			const Window& infoOfHeadElemOfWindows = *m_QueueForWindows.top().pWindow;
            depthOfResultingTree = max(depthOfResultingTree,
				infoOfHeadElemOfWindows.level);
			fFromQueueOfPseudoSources = false;
		}
		else
		{
			const QuoteInfoAtVertex& headElemOfPseudoSources = m_QueueForPseudoSources.top();
			const QuoteWindow& headElemOfWindows = m_QueueForWindows.top();
			if (headElemOfPseudoSources.disUptodate <= 
				headElemOfWindows.disUptodate)
			{
                depthOfResultingTree = max(depthOfResultingTree,
					m_InfoAtVertices[headElemOfPseudoSources.indexOfVert].level);
				fFromQueueOfPseudoSources = true;
			}
			else
			{
                depthOfResultingTree = max(depthOfResultingTree,
					headElemOfWindows.pWindow->level);
				fFromQueueOfPseudoSources = false;
			}
		}
	}	
	return fFromQueueOfPseudoSources;
}

#undef max


// ICHWithFurtherPriorityQueue.cpp: implementation of the CICHWithFurtherPriorityQueue class.
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CICHWithFurtherPriorityQueue::CICHWithFurtherPriorityQueue(const CRichModel& inputModel, const set<int> &indexOfSourceVerts) : CImprovedCHWithEdgeValve(inputModel, indexOfSourceVerts)
{
	nameOfAlgorithm = "ICH2";
}

CICHWithFurtherPriorityQueue::~CICHWithFurtherPriorityQueue()
{
}



void CICHWithFurtherPriorityQueue::InitContainers()
{
	m_InfoAtAngles.resize(model.GetNumOfEdges());
}

void CICHWithFurtherPriorityQueue::ClearContainers()
{
	while (!m_QueueForWindows.empty())
	{
		delete m_QueueForWindows.top().pWindow;
		m_QueueForWindows.pop();
	}

	while (!m_QueueForPseudoSources.empty())
	{
		m_QueueForPseudoSources.pop();
	}
}

void CICHWithFurtherPriorityQueue::BuildSequenceTree()
{
	ComputeChildrenOfSource();
	bool fFromQueueOfPseudoSources = UpdateTreeDepthBackWithChoice();
	while (!m_QueueForPseudoSources.empty() || !m_QueueForWindows.empty())
	{
		if ((int)m_QueueForWindows.size() > nMaxLenOfWindowQueue)
			nMaxLenOfWindowQueue = (int)m_QueueForWindows.size();
		if (m_QueueForPseudoSources.size() > nMaxLenOfPseudoSources)
			nMaxLenOfPseudoSources = m_QueueForPseudoSources.size();
		if (fFromQueueOfPseudoSources) //pseudosource
		{
			int indexOfVert = m_QueueForPseudoSources.top().indexOfVert;
			m_QueueForPseudoSources.pop();
			ComputeChildrenOfPseudoSource(indexOfVert);
		}
		else
		{
			QuoteWindow quoteW = m_QueueForWindows.top();
			m_QueueForWindows.pop();
			ComputeChildrenOfWindow(quoteW);
			delete quoteW.pWindow;
		}
		fFromQueueOfPseudoSources = UpdateTreeDepthBackWithChoice();
	}
}

