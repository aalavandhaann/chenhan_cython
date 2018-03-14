from libcpp cimport bool
from libcpp.vector cimport vector as libcpp_vector
from libcpp.set cimport set as libcpp_set

cdef extern from "Point3D.hpp":        
    cdef cppclass CPoint3D:
        double x
        double y
        double z
        CPoint3D()
        CPoint3D(CPoint3D other)
        CPoint3D(double, double, double)        
        void Normalize()
        double Len()

cdef extern from "BaseModel.hpp": 
    cdef cppclass CFace:
        CFace();
        CFace(int, int, int)
        void setIndices(int, int, int)
    
    cdef cppclass CBaseModel:
        CBaseModel();
        void LoadModel(libcpp_vector[CPoint3D] vertices, libcpp_vector[CFace] faces)
        int GetNumOfVerts();
        int GetNumOfFaces();
        bool isAValidModel();
        CPoint3D Vert(int);

cdef extern from "RichModel.hpp":
    cdef cppclass CEdge:
        int indexOfLeftVert;
        int indexOfRightVert;
        int indexOfOppositeVert;
        int indexOfLeftEdge;
        int indexOfRightEdge;
        int indexOfReverseEdge;
        int indexOfFrontFace;
        double length;      
        CEdge();            
    
    cdef cppclass EdgePoint:
        int index;
        int leftVertIndex;
        int rightVertIndex;
        double proportion;
        bool isVertex;
        EdgePoint();
        EdgePoint(EdgePoint other);
        EdgePoint(int);
        EdgePoint(int, double);
        EdgePoint(CRichModel, int, int, double);
        CPoint3D Get3DPoint(CRichModel);
    
    cdef cppclass CRichModel(CBaseModel):
        # wrap-inherits:
        #     CBaseModel
        #        
        CRichModel();
        void Preprocess();
        int GetNumOfEdges();
        int GetNumOfValidDirectedEdges();
        int GetNumOfTotalUndirectedEdges();
        int GetNumOfGenera();
        int GetNumOfIsolated();
        int GetNumOfComponents();
        int GetNumOfBoundries();
        bool IsConvexVert(int);
        bool isBoundaryVert(int);
        bool IsClosedModel();
        bool IsExtremeEdge(int)
        bool IsStartEdge(int);
        bool HasBeenProcessed();
#         CEdge Edge(int);
    
        
cdef extern from "ExactMethodForDGP.hpp":
    cdef cppclass InfoAtVertex:
        InfoAtVertex()
        InfoAtVertex(InfoAtVertex other)
        bool fParentIsPseudoSource
        char birthTime
        int indexOfParent
        int indexOfRootVertOfParent
        int level
        double disUptodate
        double entryProp
        
    cdef cppclass CExactMethodForDGP:
        # wrap-ignore    
        # ABSTRACT class
        #
        CExactMethodForDGP(CRichModel inputModel, libcpp_set[int] indexOfSourceVerts)  except +
        int GetRootSourceOfVert(int index)
        libcpp_vector[EdgePoint] FindSourceVertex(int indexOfVert, libcpp_vector[EdgePoint] resultingPath)
        void PickShortestPaths(int num)
        void Execute()        
        double GetRunTime()
        double GetMemoryCost()
        int GetWindowNum()
        int GetMaxLenOfQue()
        double GetNPE()
        int GetDepthOfSequenceTree()
        bool HasBeenCompleted()
        libcpp_vector[InfoAtVertex] GetVertexDistances();

cdef extern from "PreviousCH.hpp":
    cdef cppclass CPreviousCH(CExactMethodForDGP):
        # wrap-inherits:
        #     CExactMethodForDGP
        #
        CPreviousCH(CRichModel inputModel, libcpp_set[int] indexOfSourceVerts)  except +


cdef extern from "ImprovedCHWithEdgeValve.hpp":
    cdef cppclass CImprovedCHWithEdgeValve(CPreviousCH):
        # wrap-inherits:
        #     CPreviousCH
        #
        CImprovedCHWithEdgeValve(CRichModel inputModel, libcpp_set[int] indexOfSourceVerts)  except +

cdef extern from "ICHWithFurtherPriorityQueue.hpp":
    cdef cppclass CICHWithFurtherPriorityQueue(CImprovedCHWithEdgeValve):
        # wrap-inherits:
        #     CImprovedCHWithEdgeValve
        #
        CICHWithFurtherPriorityQueue(CRichModel inputModel, libcpp_set[int] indexOfSourceVerts)  except +    
    
    