#cython: c_string_encoding=ascii  # for cython>=0.19
#cython: embedsignature=False
from  libcpp.string  cimport string as libcpp_string
from  libcpp.string  cimport string as libcpp_utf8_string
from  libcpp.set     cimport set as libcpp_set
from  libcpp.vector  cimport vector as libcpp_vector
from  libcpp.pair    cimport pair as libcpp_pair
from  libcpp.map     cimport map  as libcpp_map
from  libcpp cimport bool
from  libc.string cimport const_char
from cython.operator cimport dereference as deref, preincrement as inc, address as address
from  AutowrapRefHolder cimport AutowrapRefHolder
from  AutowrapPtrHolder cimport AutowrapPtrHolder
from  AutowrapConstPtrHolder cimport AutowrapConstPtrHolder
from  smart_ptr cimport shared_ptr
from chenhancc cimport CBaseModel as _CBaseModel
from chenhancc cimport CEdge as _CEdge
from chenhancc cimport CExactMethodForDGP as _CExactMethodForDGP
from chenhancc cimport CFace as _CFace
from chenhancc cimport CICHWithFurtherPriorityQueue as _CICHWithFurtherPriorityQueue
from chenhancc cimport CImprovedCHWithEdgeValve as _CImprovedCHWithEdgeValve
from chenhancc cimport CPoint3D as _CPoint3D
from chenhancc cimport CPreviousCH as _CPreviousCH
from chenhancc cimport CRichModel as _CRichModel
from chenhancc cimport EdgePoint as _EdgePoint
from chenhancc cimport InfoAtVertex as _InfoAtVertex

cdef extern from "autowrap_tools.hpp":
    char * _cast_const_away(char *) 

cdef class EdgePoint:

    cdef shared_ptr[_EdgePoint] inst

    def __dealloc__(self):
         self.inst.reset()

    
    property index:
        def __set__(self,  index):
        
            self.inst.get().index = (<int>index)
        
    
        def __get__(self):
            cdef int _r = self.inst.get().index
            py_result = <int>_r
            return py_result
    
    property leftVertIndex:
        def __set__(self,  leftVertIndex):
        
            self.inst.get().leftVertIndex = (<int>leftVertIndex)
        
    
        def __get__(self):
            cdef int _r = self.inst.get().leftVertIndex
            py_result = <int>_r
            return py_result
    
    property rightVertIndex:
        def __set__(self,  rightVertIndex):
        
            self.inst.get().rightVertIndex = (<int>rightVertIndex)
        
    
        def __get__(self):
            cdef int _r = self.inst.get().rightVertIndex
            py_result = <int>_r
            return py_result
    
    property proportion:
        def __set__(self, double proportion):
        
            self.inst.get().proportion = (<double>proportion)
        
    
        def __get__(self):
            cdef double _r = self.inst.get().proportion
            py_result = <double>_r
            return py_result
    
    property isVertex:
        def __set__(self,  isVertex):
        
            self.inst.get().isVertex = (<bool>isVertex)
        
    
        def __get__(self):
            cdef bool _r = self.inst.get().isVertex
            py_result = <bool>_r
            return py_result
    
    def _init_0(self):
        """Cython signature: void EdgePoint()"""
        self.inst = shared_ptr[_EdgePoint](new _EdgePoint())
    
    def _init_1(self, EdgePoint other ):
        """Cython signature: void EdgePoint(EdgePoint other)"""
        assert isinstance(other, EdgePoint), 'arg other wrong type'
    
        self.inst = shared_ptr[_EdgePoint](new _EdgePoint((deref(other.inst.get()))))
    
    def _init_2(self,  in_0 ):
        """Cython signature: void EdgePoint(int)"""
        assert isinstance(in_0, (int, long)), 'arg in_0 wrong type'
    
        self.inst = shared_ptr[_EdgePoint](new _EdgePoint((<int>in_0)))
    
    def _init_3(self,  in_0 , double in_1 ):
        """Cython signature: void EdgePoint(int, double)"""
        assert isinstance(in_0, (int, long)), 'arg in_0 wrong type'
        assert isinstance(in_1, float), 'arg in_1 wrong type'
    
    
        self.inst = shared_ptr[_EdgePoint](new _EdgePoint((<int>in_0), (<double>in_1)))
    
    def _init_4(self, CRichModel in_0 ,  in_1 ,  in_2 , double in_3 ):
        """Cython signature: void EdgePoint(CRichModel, int, int, double)"""
        assert isinstance(in_0, CRichModel), 'arg in_0 wrong type'
        assert isinstance(in_1, (int, long)), 'arg in_1 wrong type'
        assert isinstance(in_2, (int, long)), 'arg in_2 wrong type'
        assert isinstance(in_3, float), 'arg in_3 wrong type'
    
    
    
    
        self.inst = shared_ptr[_EdgePoint](new _EdgePoint((deref(in_0.inst.get())), (<int>in_1), (<int>in_2), (<double>in_3)))
    
    def __init__(self, *args , **kwargs):
        if not args:
             self._init_0(*args)
        elif (len(args)==1) and (isinstance(args[0], EdgePoint)):
             self._init_1(*args)
        elif (len(args)==1) and (isinstance(args[0], (int, long))):
             self._init_2(*args)
        elif (len(args)==2) and (isinstance(args[0], (int, long))) and (isinstance(args[1], float)):
             self._init_3(*args)
        elif (len(args)==4) and (isinstance(args[0], CRichModel)) and (isinstance(args[1], (int, long))) and (isinstance(args[2], (int, long))) and (isinstance(args[3], float)):
             self._init_4(*args)
        else:
               raise Exception('can not handle type of %s' % (args,))
    
    def Get3DPoint(self, CRichModel in_0 ):
        """Cython signature: CPoint3D Get3DPoint(CRichModel)"""
        assert isinstance(in_0, CRichModel), 'arg in_0 wrong type'
    
        cdef _CPoint3D * _r = new _CPoint3D(self.inst.get().Get3DPoint((deref(in_0.inst.get()))))
        cdef CPoint3D py_result = CPoint3D.__new__(CPoint3D)
        py_result.inst = shared_ptr[_CPoint3D](_r)
        return py_result 

cdef class CImprovedCHWithEdgeValve:

    cdef shared_ptr[_CImprovedCHWithEdgeValve] inst

    def __dealloc__(self):
         self.inst.reset()

    
    def GetNPE(self):
        """Cython signature: double GetNPE()"""
        cdef double _r = self.inst.get().GetNPE()
        py_result = <double>_r
        return py_result
    
    def GetMaxLenOfQue(self):
        """Cython signature: int GetMaxLenOfQue()"""
        cdef int _r = self.inst.get().GetMaxLenOfQue()
        py_result = <int>_r
        return py_result
    
    def GetWindowNum(self):
        """Cython signature: int GetWindowNum()"""
        cdef int _r = self.inst.get().GetWindowNum()
        py_result = <int>_r
        return py_result
    
    def FindSourceVertex(self,  indexOfVert , list resultingPath ):
        """Cython signature: libcpp_vector[EdgePoint] FindSourceVertex(int indexOfVert, libcpp_vector[EdgePoint] resultingPath)"""
        assert isinstance(indexOfVert, (int, long)), 'arg indexOfVert wrong type'
        assert isinstance(resultingPath, list) and all(isinstance(elemt_rec, EdgePoint) for elemt_rec in resultingPath), 'arg resultingPath wrong type'
    
        cdef libcpp_vector[_EdgePoint] * v1 = new libcpp_vector[_EdgePoint]()
        cdef EdgePoint item1
        for item1 in resultingPath:
            v1.push_back(deref(item1.inst.get()))
        _r = self.inst.get().FindSourceVertex((<int>indexOfVert), deref(v1))
        del v1
        py_result = []
        cdef libcpp_vector[_EdgePoint].iterator it__r = _r.begin()
        cdef EdgePoint item_py_result
        while it__r != _r.end():
           item_py_result = EdgePoint.__new__(EdgePoint)
           item_py_result.inst = shared_ptr[_EdgePoint](new _EdgePoint(deref(it__r)))
           py_result.append(item_py_result)
           inc(it__r)
        return py_result
    
    def GetRootSourceOfVert(self,  index ):
        """Cython signature: int GetRootSourceOfVert(int index)"""
        assert isinstance(index, (int, long)), 'arg index wrong type'
    
        cdef int _r = self.inst.get().GetRootSourceOfVert((<int>index))
        py_result = <int>_r
        return py_result
    
    def GetMemoryCost(self):
        """Cython signature: double GetMemoryCost()"""
        cdef double _r = self.inst.get().GetMemoryCost()
        py_result = <double>_r
        return py_result
    
    def Execute(self):
        """Cython signature: void Execute()"""
        self.inst.get().Execute()
    
    def GetRunTime(self):
        """Cython signature: double GetRunTime()"""
        cdef double _r = self.inst.get().GetRunTime()
        py_result = <double>_r
        return py_result
    
    def __init__(self, CRichModel inputModel , set indexOfSourceVerts ):
        """Cython signature: void CImprovedCHWithEdgeValve(CRichModel inputModel, libcpp_set[int] indexOfSourceVerts)"""
        assert isinstance(inputModel, CRichModel), 'arg inputModel wrong type'
        assert isinstance(indexOfSourceVerts, set) and all(isinstance(li, (int, long)) for li in indexOfSourceVerts), 'arg indexOfSourceVerts wrong type'
    
        cdef libcpp_set[int] v1 = indexOfSourceVerts
        self.inst = shared_ptr[_CImprovedCHWithEdgeValve](new _CImprovedCHWithEdgeValve((deref(inputModel.inst.get())), v1))
    
    def GetVertexDistances(self):
        """Cython signature: libcpp_vector[InfoAtVertex] GetVertexDistances()"""
        _r = self.inst.get().GetVertexDistances()
        py_result = []
        cdef libcpp_vector[_InfoAtVertex].iterator it__r = _r.begin()
        cdef InfoAtVertex item_py_result
        while it__r != _r.end():
           item_py_result = InfoAtVertex.__new__(InfoAtVertex)
           item_py_result.inst = shared_ptr[_InfoAtVertex](new _InfoAtVertex(deref(it__r)))
           py_result.append(item_py_result)
           inc(it__r)
        return py_result
    
    def GetDepthOfSequenceTree(self):
        """Cython signature: int GetDepthOfSequenceTree()"""
        cdef int _r = self.inst.get().GetDepthOfSequenceTree()
        py_result = <int>_r
        return py_result
    
    def PickShortestPaths(self,  num ):
        """Cython signature: void PickShortestPaths(int num)"""
        assert isinstance(num, (int, long)), 'arg num wrong type'
    
        self.inst.get().PickShortestPaths((<int>num))
    
    def HasBeenCompleted(self):
        """Cython signature: bool HasBeenCompleted()"""
        cdef bool _r = self.inst.get().HasBeenCompleted()
        py_result = <bool>_r
        return py_result 

cdef class CPreviousCH:

    cdef shared_ptr[_CPreviousCH] inst

    def __dealloc__(self):
         self.inst.reset()

    
    def GetNPE(self):
        """Cython signature: double GetNPE()"""
        cdef double _r = self.inst.get().GetNPE()
        py_result = <double>_r
        return py_result
    
    def GetWindowNum(self):
        """Cython signature: int GetWindowNum()"""
        cdef int _r = self.inst.get().GetWindowNum()
        py_result = <int>_r
        return py_result
    
    def GetMaxLenOfQue(self):
        """Cython signature: int GetMaxLenOfQue()"""
        cdef int _r = self.inst.get().GetMaxLenOfQue()
        py_result = <int>_r
        return py_result
    
    def GetMemoryCost(self):
        """Cython signature: double GetMemoryCost()"""
        cdef double _r = self.inst.get().GetMemoryCost()
        py_result = <double>_r
        return py_result
    
    def GetRootSourceOfVert(self,  index ):
        """Cython signature: int GetRootSourceOfVert(int index)"""
        assert isinstance(index, (int, long)), 'arg index wrong type'
    
        cdef int _r = self.inst.get().GetRootSourceOfVert((<int>index))
        py_result = <int>_r
        return py_result
    
    def Execute(self):
        """Cython signature: void Execute()"""
        self.inst.get().Execute()
    
    def GetRunTime(self):
        """Cython signature: double GetRunTime()"""
        cdef double _r = self.inst.get().GetRunTime()
        py_result = <double>_r
        return py_result
    
    def __init__(self, CRichModel inputModel , set indexOfSourceVerts ):
        """Cython signature: void CPreviousCH(CRichModel inputModel, libcpp_set[int] indexOfSourceVerts)"""
        assert isinstance(inputModel, CRichModel), 'arg inputModel wrong type'
        assert isinstance(indexOfSourceVerts, set) and all(isinstance(li, (int, long)) for li in indexOfSourceVerts), 'arg indexOfSourceVerts wrong type'
    
        cdef libcpp_set[int] v1 = indexOfSourceVerts
        self.inst = shared_ptr[_CPreviousCH](new _CPreviousCH((deref(inputModel.inst.get())), v1))
    
    def PickShortestPaths(self,  num ):
        """Cython signature: void PickShortestPaths(int num)"""
        assert isinstance(num, (int, long)), 'arg num wrong type'
    
        self.inst.get().PickShortestPaths((<int>num))
    
    def GetVertexDistances(self):
        """Cython signature: libcpp_vector[InfoAtVertex] GetVertexDistances()"""
        _r = self.inst.get().GetVertexDistances()
        py_result = []
        cdef libcpp_vector[_InfoAtVertex].iterator it__r = _r.begin()
        cdef InfoAtVertex item_py_result
        while it__r != _r.end():
           item_py_result = InfoAtVertex.__new__(InfoAtVertex)
           item_py_result.inst = shared_ptr[_InfoAtVertex](new _InfoAtVertex(deref(it__r)))
           py_result.append(item_py_result)
           inc(it__r)
        return py_result
    
    def GetDepthOfSequenceTree(self):
        """Cython signature: int GetDepthOfSequenceTree()"""
        cdef int _r = self.inst.get().GetDepthOfSequenceTree()
        py_result = <int>_r
        return py_result
    
    def FindSourceVertex(self,  indexOfVert , list resultingPath ):
        """Cython signature: libcpp_vector[EdgePoint] FindSourceVertex(int indexOfVert, libcpp_vector[EdgePoint] resultingPath)"""
        assert isinstance(indexOfVert, (int, long)), 'arg indexOfVert wrong type'
        assert isinstance(resultingPath, list) and all(isinstance(elemt_rec, EdgePoint) for elemt_rec in resultingPath), 'arg resultingPath wrong type'
    
        cdef libcpp_vector[_EdgePoint] * v1 = new libcpp_vector[_EdgePoint]()
        cdef EdgePoint item1
        for item1 in resultingPath:
            v1.push_back(deref(item1.inst.get()))
        _r = self.inst.get().FindSourceVertex((<int>indexOfVert), deref(v1))
        del v1
        py_result = []
        cdef libcpp_vector[_EdgePoint].iterator it__r = _r.begin()
        cdef EdgePoint item_py_result
        while it__r != _r.end():
           item_py_result = EdgePoint.__new__(EdgePoint)
           item_py_result.inst = shared_ptr[_EdgePoint](new _EdgePoint(deref(it__r)))
           py_result.append(item_py_result)
           inc(it__r)
        return py_result
    
    def HasBeenCompleted(self):
        """Cython signature: bool HasBeenCompleted()"""
        cdef bool _r = self.inst.get().HasBeenCompleted()
        py_result = <bool>_r
        return py_result 

cdef class InfoAtVertex:

    cdef shared_ptr[_InfoAtVertex] inst

    def __dealloc__(self):
         self.inst.reset()

    
    property fParentIsPseudoSource:
        def __set__(self,  fParentIsPseudoSource):
        
            self.inst.get().fParentIsPseudoSource = (<bool>fParentIsPseudoSource)
        
    
        def __get__(self):
            cdef bool _r = self.inst.get().fParentIsPseudoSource
            py_result = <bool>_r
            return py_result
    
    property birthTime:
        def __set__(self, bytes birthTime):
        
            self.inst.get().birthTime = (<char>((birthTime)[0]))
        
    
        def __get__(self):
            cdef char  _r = self.inst.get().birthTime
            py_result = chr(<char>(_r))
            return py_result
    
    property indexOfParent:
        def __set__(self,  indexOfParent):
        
            self.inst.get().indexOfParent = (<int>indexOfParent)
        
    
        def __get__(self):
            cdef int _r = self.inst.get().indexOfParent
            py_result = <int>_r
            return py_result
    
    property indexOfRootVertOfParent:
        def __set__(self,  indexOfRootVertOfParent):
        
            self.inst.get().indexOfRootVertOfParent = (<int>indexOfRootVertOfParent)
        
    
        def __get__(self):
            cdef int _r = self.inst.get().indexOfRootVertOfParent
            py_result = <int>_r
            return py_result
    
    property level:
        def __set__(self,  level):
        
            self.inst.get().level = (<int>level)
        
    
        def __get__(self):
            cdef int _r = self.inst.get().level
            py_result = <int>_r
            return py_result
    
    property disUptodate:
        def __set__(self, double disUptodate):
        
            self.inst.get().disUptodate = (<double>disUptodate)
        
    
        def __get__(self):
            cdef double _r = self.inst.get().disUptodate
            py_result = <double>_r
            return py_result
    
    property entryProp:
        def __set__(self, double entryProp):
        
            self.inst.get().entryProp = (<double>entryProp)
        
    
        def __get__(self):
            cdef double _r = self.inst.get().entryProp
            py_result = <double>_r
            return py_result
    
    def _init_0(self):
        """Cython signature: void InfoAtVertex()"""
        self.inst = shared_ptr[_InfoAtVertex](new _InfoAtVertex())
    
    def _init_1(self, InfoAtVertex other ):
        """Cython signature: void InfoAtVertex(InfoAtVertex other)"""
        assert isinstance(other, InfoAtVertex), 'arg other wrong type'
    
        self.inst = shared_ptr[_InfoAtVertex](new _InfoAtVertex((deref(other.inst.get()))))
    
    def __init__(self, *args , **kwargs):
        if not args:
             self._init_0(*args)
        elif (len(args)==1) and (isinstance(args[0], InfoAtVertex)):
             self._init_1(*args)
        else:
               raise Exception('can not handle type of %s' % (args,)) 

cdef class CFace:

    cdef shared_ptr[_CFace] inst

    def __dealloc__(self):
         self.inst.reset()

    
    def _init_0(self):
        """Cython signature: void CFace()"""
        self.inst = shared_ptr[_CFace](new _CFace())
    
    def _init_1(self,  in_0 ,  in_1 ,  in_2 ):
        """Cython signature: void CFace(int, int, int)"""
        assert isinstance(in_0, (int, long)), 'arg in_0 wrong type'
        assert isinstance(in_1, (int, long)), 'arg in_1 wrong type'
        assert isinstance(in_2, (int, long)), 'arg in_2 wrong type'
    
    
    
        self.inst = shared_ptr[_CFace](new _CFace((<int>in_0), (<int>in_1), (<int>in_2)))
    
    def __init__(self, *args , **kwargs):
        if not args:
             self._init_0(*args)
        elif (len(args)==3) and (isinstance(args[0], (int, long))) and (isinstance(args[1], (int, long))) and (isinstance(args[2], (int, long))):
             self._init_1(*args)
        else:
               raise Exception('can not handle type of %s' % (args,))
    
    def setIndices(self,  in_0 ,  in_1 ,  in_2 ):
        """Cython signature: void setIndices(int, int, int)"""
        assert isinstance(in_0, (int, long)), 'arg in_0 wrong type'
        assert isinstance(in_1, (int, long)), 'arg in_1 wrong type'
        assert isinstance(in_2, (int, long)), 'arg in_2 wrong type'
    
    
    
        self.inst.get().setIndices((<int>in_0), (<int>in_1), (<int>in_2)) 

cdef class CICHWithFurtherPriorityQueue:

    cdef shared_ptr[_CICHWithFurtherPriorityQueue] inst

    def __dealloc__(self):
         self.inst.reset()

    
    def GetNPE(self):
        """Cython signature: double GetNPE()"""
        cdef double _r = self.inst.get().GetNPE()
        py_result = <double>_r
        return py_result
    
    def GetWindowNum(self):
        """Cython signature: int GetWindowNum()"""
        cdef int _r = self.inst.get().GetWindowNum()
        py_result = <int>_r
        return py_result
    
    def GetMaxLenOfQue(self):
        """Cython signature: int GetMaxLenOfQue()"""
        cdef int _r = self.inst.get().GetMaxLenOfQue()
        py_result = <int>_r
        return py_result
    
    def GetRootSourceOfVert(self,  index ):
        """Cython signature: int GetRootSourceOfVert(int index)"""
        assert isinstance(index, (int, long)), 'arg index wrong type'
    
        cdef int _r = self.inst.get().GetRootSourceOfVert((<int>index))
        py_result = <int>_r
        return py_result
    
    def GetMemoryCost(self):
        """Cython signature: double GetMemoryCost()"""
        cdef double _r = self.inst.get().GetMemoryCost()
        py_result = <double>_r
        return py_result
    
    def Execute(self):
        """Cython signature: void Execute()"""
        self.inst.get().Execute()
    
    def GetRunTime(self):
        """Cython signature: double GetRunTime()"""
        cdef double _r = self.inst.get().GetRunTime()
        py_result = <double>_r
        return py_result
    
    def __init__(self, CRichModel inputModel , set indexOfSourceVerts ):
        """Cython signature: void CICHWithFurtherPriorityQueue(CRichModel inputModel, libcpp_set[int] indexOfSourceVerts)"""
        assert isinstance(inputModel, CRichModel), 'arg inputModel wrong type'
        assert isinstance(indexOfSourceVerts, set) and all(isinstance(li, (int, long)) for li in indexOfSourceVerts), 'arg indexOfSourceVerts wrong type'
    
        cdef libcpp_set[int] v1 = indexOfSourceVerts
        self.inst = shared_ptr[_CICHWithFurtherPriorityQueue](new _CICHWithFurtherPriorityQueue((deref(inputModel.inst.get())), v1))
    
    def GetVertexDistances(self):
        """Cython signature: libcpp_vector[InfoAtVertex] GetVertexDistances()"""
        _r = self.inst.get().GetVertexDistances()
        py_result = []
        cdef libcpp_vector[_InfoAtVertex].iterator it__r = _r.begin()
        cdef InfoAtVertex item_py_result
        while it__r != _r.end():
           item_py_result = InfoAtVertex.__new__(InfoAtVertex)
           item_py_result.inst = shared_ptr[_InfoAtVertex](new _InfoAtVertex(deref(it__r)))
           py_result.append(item_py_result)
           inc(it__r)
        return py_result
    
    def GetDepthOfSequenceTree(self):
        """Cython signature: int GetDepthOfSequenceTree()"""
        cdef int _r = self.inst.get().GetDepthOfSequenceTree()
        py_result = <int>_r
        return py_result
    
    def PickShortestPaths(self,  num ):
        """Cython signature: void PickShortestPaths(int num)"""
        assert isinstance(num, (int, long)), 'arg num wrong type'
    
        self.inst.get().PickShortestPaths((<int>num))
    
    def FindSourceVertex(self,  indexOfVert , list resultingPath ):
        """Cython signature: libcpp_vector[EdgePoint] FindSourceVertex(int indexOfVert, libcpp_vector[EdgePoint] resultingPath)"""
        assert isinstance(indexOfVert, (int, long)), 'arg indexOfVert wrong type'
        assert isinstance(resultingPath, list) and all(isinstance(elemt_rec, EdgePoint) for elemt_rec in resultingPath), 'arg resultingPath wrong type'
    
        cdef libcpp_vector[_EdgePoint] * v1 = new libcpp_vector[_EdgePoint]()
        cdef EdgePoint item1
        for item1 in resultingPath:
            v1.push_back(deref(item1.inst.get()))
        _r = self.inst.get().FindSourceVertex((<int>indexOfVert), deref(v1))
        del v1
        py_result = []
        cdef libcpp_vector[_EdgePoint].iterator it__r = _r.begin()
        cdef EdgePoint item_py_result
        while it__r != _r.end():
           item_py_result = EdgePoint.__new__(EdgePoint)
           item_py_result.inst = shared_ptr[_EdgePoint](new _EdgePoint(deref(it__r)))
           py_result.append(item_py_result)
           inc(it__r)
        return py_result
    
    def HasBeenCompleted(self):
        """Cython signature: bool HasBeenCompleted()"""
        cdef bool _r = self.inst.get().HasBeenCompleted()
        py_result = <bool>_r
        return py_result 

cdef class CRichModel:

    cdef shared_ptr[_CRichModel] inst

    def __dealloc__(self):
         self.inst.reset()

    
    def GetNumOfFaces(self):
        """Cython signature: int GetNumOfFaces()"""
        cdef int _r = self.inst.get().GetNumOfFaces()
        py_result = <int>_r
        return py_result
    
    def IsExtremeEdge(self,  in_0 ):
        """Cython signature: bool IsExtremeEdge(int)"""
        assert isinstance(in_0, (int, long)), 'arg in_0 wrong type'
    
        cdef bool _r = self.inst.get().IsExtremeEdge((<int>in_0))
        py_result = <bool>_r
        return py_result
    
    def GetNumOfTotalUndirectedEdges(self):
        """Cython signature: int GetNumOfTotalUndirectedEdges()"""
        cdef int _r = self.inst.get().GetNumOfTotalUndirectedEdges()
        py_result = <int>_r
        return py_result
    
    def HasBeenProcessed(self):
        """Cython signature: bool HasBeenProcessed()"""
        cdef bool _r = self.inst.get().HasBeenProcessed()
        py_result = <bool>_r
        return py_result
    
    def IsConvexVert(self,  in_0 ):
        """Cython signature: bool IsConvexVert(int)"""
        assert isinstance(in_0, (int, long)), 'arg in_0 wrong type'
    
        cdef bool _r = self.inst.get().IsConvexVert((<int>in_0))
        py_result = <bool>_r
        return py_result
    
    def Vert(self,  in_0 ):
        """Cython signature: CPoint3D Vert(int)"""
        assert isinstance(in_0, (int, long)), 'arg in_0 wrong type'
    
        cdef _CPoint3D * _r = new _CPoint3D(self.inst.get().Vert((<int>in_0)))
        cdef CPoint3D py_result = CPoint3D.__new__(CPoint3D)
        py_result.inst = shared_ptr[_CPoint3D](_r)
        return py_result
    
    def IsStartEdge(self,  in_0 ):
        """Cython signature: bool IsStartEdge(int)"""
        assert isinstance(in_0, (int, long)), 'arg in_0 wrong type'
    
        cdef bool _r = self.inst.get().IsStartEdge((<int>in_0))
        py_result = <bool>_r
        return py_result
    
    def GetNumOfIsolated(self):
        """Cython signature: int GetNumOfIsolated()"""
        cdef int _r = self.inst.get().GetNumOfIsolated()
        py_result = <int>_r
        return py_result
    
    def isBoundaryVert(self,  in_0 ):
        """Cython signature: bool isBoundaryVert(int)"""
        assert isinstance(in_0, (int, long)), 'arg in_0 wrong type'
    
        cdef bool _r = self.inst.get().isBoundaryVert((<int>in_0))
        py_result = <bool>_r
        return py_result
    
    def GetNumOfValidDirectedEdges(self):
        """Cython signature: int GetNumOfValidDirectedEdges()"""
        cdef int _r = self.inst.get().GetNumOfValidDirectedEdges()
        py_result = <int>_r
        return py_result
    
    def Preprocess(self):
        """Cython signature: void Preprocess()"""
        self.inst.get().Preprocess()
    
    def GetNumOfComponents(self):
        """Cython signature: int GetNumOfComponents()"""
        cdef int _r = self.inst.get().GetNumOfComponents()
        py_result = <int>_r
        return py_result
    
    def isAValidModel(self):
        """Cython signature: bool isAValidModel()"""
        cdef bool _r = self.inst.get().isAValidModel()
        py_result = <bool>_r
        return py_result
    
    def GetNumOfVerts(self):
        """Cython signature: int GetNumOfVerts()"""
        cdef int _r = self.inst.get().GetNumOfVerts()
        py_result = <int>_r
        return py_result
    
    def LoadModel(self, list vertices , list faces ):
        """Cython signature: void LoadModel(libcpp_vector[CPoint3D] vertices, libcpp_vector[CFace] faces)"""
        assert isinstance(vertices, list) and all(isinstance(elemt_rec, CPoint3D) for elemt_rec in vertices), 'arg vertices wrong type'
        assert isinstance(faces, list) and all(isinstance(elemt_rec, CFace) for elemt_rec in faces), 'arg faces wrong type'
        cdef libcpp_vector[_CPoint3D] * v0 = new libcpp_vector[_CPoint3D]()
        cdef CPoint3D item0
        for item0 in vertices:
            v0.push_back(deref(item0.inst.get()))
        cdef libcpp_vector[_CFace] * v1 = new libcpp_vector[_CFace]()
        cdef CFace item1
        for item1 in faces:
            v1.push_back(deref(item1.inst.get()))
        self.inst.get().LoadModel(deref(v0), deref(v1))
        del v1
        del v0
    
    def IsClosedModel(self):
        """Cython signature: bool IsClosedModel()"""
        cdef bool _r = self.inst.get().IsClosedModel()
        py_result = <bool>_r
        return py_result
    
    def GetNumOfBoundries(self):
        """Cython signature: int GetNumOfBoundries()"""
        cdef int _r = self.inst.get().GetNumOfBoundries()
        py_result = <int>_r
        return py_result
    
    def __init__(self):
        """Cython signature: void CRichModel()"""
        self.inst = shared_ptr[_CRichModel](new _CRichModel())
    
    def GetNumOfGenera(self):
        """Cython signature: int GetNumOfGenera()"""
        cdef int _r = self.inst.get().GetNumOfGenera()
        py_result = <int>_r
        return py_result
    
    def GetNumOfEdges(self):
        """Cython signature: int GetNumOfEdges()"""
        cdef int _r = self.inst.get().GetNumOfEdges()
        py_result = <int>_r
        return py_result 

cdef class CEdge:

    cdef shared_ptr[_CEdge] inst

    def __dealloc__(self):
         self.inst.reset()

    
    property indexOfLeftVert:
        def __set__(self,  indexOfLeftVert):
        
            self.inst.get().indexOfLeftVert = (<int>indexOfLeftVert)
        
    
        def __get__(self):
            cdef int _r = self.inst.get().indexOfLeftVert
            py_result = <int>_r
            return py_result
    
    property indexOfRightVert:
        def __set__(self,  indexOfRightVert):
        
            self.inst.get().indexOfRightVert = (<int>indexOfRightVert)
        
    
        def __get__(self):
            cdef int _r = self.inst.get().indexOfRightVert
            py_result = <int>_r
            return py_result
    
    property indexOfOppositeVert:
        def __set__(self,  indexOfOppositeVert):
        
            self.inst.get().indexOfOppositeVert = (<int>indexOfOppositeVert)
        
    
        def __get__(self):
            cdef int _r = self.inst.get().indexOfOppositeVert
            py_result = <int>_r
            return py_result
    
    property indexOfLeftEdge:
        def __set__(self,  indexOfLeftEdge):
        
            self.inst.get().indexOfLeftEdge = (<int>indexOfLeftEdge)
        
    
        def __get__(self):
            cdef int _r = self.inst.get().indexOfLeftEdge
            py_result = <int>_r
            return py_result
    
    property indexOfRightEdge:
        def __set__(self,  indexOfRightEdge):
        
            self.inst.get().indexOfRightEdge = (<int>indexOfRightEdge)
        
    
        def __get__(self):
            cdef int _r = self.inst.get().indexOfRightEdge
            py_result = <int>_r
            return py_result
    
    property indexOfReverseEdge:
        def __set__(self,  indexOfReverseEdge):
        
            self.inst.get().indexOfReverseEdge = (<int>indexOfReverseEdge)
        
    
        def __get__(self):
            cdef int _r = self.inst.get().indexOfReverseEdge
            py_result = <int>_r
            return py_result
    
    property indexOfFrontFace:
        def __set__(self,  indexOfFrontFace):
        
            self.inst.get().indexOfFrontFace = (<int>indexOfFrontFace)
        
    
        def __get__(self):
            cdef int _r = self.inst.get().indexOfFrontFace
            py_result = <int>_r
            return py_result
    
    property length:
        def __set__(self, double length):
        
            self.inst.get().length = (<double>length)
        
    
        def __get__(self):
            cdef double _r = self.inst.get().length
            py_result = <double>_r
            return py_result
    
    def __init__(self):
        """Cython signature: void CEdge()"""
        self.inst = shared_ptr[_CEdge](new _CEdge()) 

cdef class CBaseModel:

    cdef shared_ptr[_CBaseModel] inst

    def __dealloc__(self):
         self.inst.reset()

    
    def GetNumOfFaces(self):
        """Cython signature: int GetNumOfFaces()"""
        cdef int _r = self.inst.get().GetNumOfFaces()
        py_result = <int>_r
        return py_result
    
    def isAValidModel(self):
        """Cython signature: bool isAValidModel()"""
        cdef bool _r = self.inst.get().isAValidModel()
        py_result = <bool>_r
        return py_result
    
    def LoadModel(self, list vertices , list faces ):
        """Cython signature: void LoadModel(libcpp_vector[CPoint3D] vertices, libcpp_vector[CFace] faces)"""
        assert isinstance(vertices, list) and all(isinstance(elemt_rec, CPoint3D) for elemt_rec in vertices), 'arg vertices wrong type'
        assert isinstance(faces, list) and all(isinstance(elemt_rec, CFace) for elemt_rec in faces), 'arg faces wrong type'
        cdef libcpp_vector[_CPoint3D] * v0 = new libcpp_vector[_CPoint3D]()
        cdef CPoint3D item0
        for item0 in vertices:
            v0.push_back(deref(item0.inst.get()))
        cdef libcpp_vector[_CFace] * v1 = new libcpp_vector[_CFace]()
        cdef CFace item1
        for item1 in faces:
            v1.push_back(deref(item1.inst.get()))
        self.inst.get().LoadModel(deref(v0), deref(v1))
        del v1
        del v0
    
    def Vert(self,  in_0 ):
        """Cython signature: CPoint3D Vert(int)"""
        assert isinstance(in_0, (int, long)), 'arg in_0 wrong type'
    
        cdef _CPoint3D * _r = new _CPoint3D(self.inst.get().Vert((<int>in_0)))
        cdef CPoint3D py_result = CPoint3D.__new__(CPoint3D)
        py_result.inst = shared_ptr[_CPoint3D](_r)
        return py_result
    
    def GetNumOfVerts(self):
        """Cython signature: int GetNumOfVerts()"""
        cdef int _r = self.inst.get().GetNumOfVerts()
        py_result = <int>_r
        return py_result
    
    def __init__(self):
        """Cython signature: void CBaseModel()"""
        self.inst = shared_ptr[_CBaseModel](new _CBaseModel()) 

cdef class CPoint3D:

    cdef shared_ptr[_CPoint3D] inst

    def __dealloc__(self):
         self.inst.reset()

    
    property x:
        def __set__(self, double x):
        
            self.inst.get().x = (<double>x)
        
    
        def __get__(self):
            cdef double _r = self.inst.get().x
            py_result = <double>_r
            return py_result
    
    property y:
        def __set__(self, double y):
        
            self.inst.get().y = (<double>y)
        
    
        def __get__(self):
            cdef double _r = self.inst.get().y
            py_result = <double>_r
            return py_result
    
    property z:
        def __set__(self, double z):
        
            self.inst.get().z = (<double>z)
        
    
        def __get__(self):
            cdef double _r = self.inst.get().z
            py_result = <double>_r
            return py_result
    
    def Len(self):
        """Cython signature: double Len()"""
        cdef double _r = self.inst.get().Len()
        py_result = <double>_r
        return py_result
    
    def Normalize(self):
        """Cython signature: void Normalize()"""
        self.inst.get().Normalize()
    
    def _init_0(self):
        """Cython signature: void CPoint3D()"""
        self.inst = shared_ptr[_CPoint3D](new _CPoint3D())
    
    def _init_1(self, CPoint3D other ):
        """Cython signature: void CPoint3D(CPoint3D other)"""
        assert isinstance(other, CPoint3D), 'arg other wrong type'
    
        self.inst = shared_ptr[_CPoint3D](new _CPoint3D((deref(other.inst.get()))))
    
    def _init_2(self, double in_0 , double in_1 , double in_2 ):
        """Cython signature: void CPoint3D(double, double, double)"""
        assert isinstance(in_0, float), 'arg in_0 wrong type'
        assert isinstance(in_1, float), 'arg in_1 wrong type'
        assert isinstance(in_2, float), 'arg in_2 wrong type'
    
    
    
        self.inst = shared_ptr[_CPoint3D](new _CPoint3D((<double>in_0), (<double>in_1), (<double>in_2)))
    
    def __init__(self, *args , **kwargs):
        if not args:
             self._init_0(*args)
        elif (len(args)==1) and (isinstance(args[0], CPoint3D)):
             self._init_1(*args)
        elif (len(args)==3) and (isinstance(args[0], float)) and (isinstance(args[1], float)) and (isinstance(args[2], float)):
             self._init_2(*args)
        else:
               raise Exception('can not handle type of %s' % (args,)) 
