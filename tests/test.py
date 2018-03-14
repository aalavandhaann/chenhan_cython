from py_chenhancc import CPoint3D, CFace, CBaseModel, CRichModel, CICHWithFurtherPriorityQueue;

def rotate(l, n):
    return l[n:] + l[:n];


def test():
    points = [CPoint3D(0.0,0.0,0.0),CPoint3D(0.5,1.0,0.0),CPoint3D(1.0,0.0,0.0) ];
    faces = [CFace(0,1,2)];
    bmodel = CBaseModel();
    rmodel = CRichModel();
    bmodel.LoadModel(points, faces);
    print(bmodel.GetNumOfVerts());
    
    rmodel.LoadModel(points, faces);
    rmodel.Preprocess();
    print(rmodel.GetNumOfVerts());
    
    emethod = CICHWithFurtherPriorityQueue(rmodel, set([0]));
    emethod.Execute();
    
    print('RUN TIMER ::: ', emethod.GetRunTime());
    print('MEMORY COST ::: ', emethod.GetMemoryCost());
    print('GET TOTAL WINDOWS CREATED ::: ', emethod.GetWindowNum());
    print('LENGTH OF THE QUEUE ::: ', emethod.GetMaxLenOfQue());
    print('WHAT IS NPE BTW? ::: ', emethod.GetNPE());
    print('Get Sequence TREE DEPTH ::: ', emethod.GetDepthOfSequenceTree());
    print('HAS ALGORITHM COMPLETED ITS WORK?::: ', emethod.HasBeenCompleted());
    print(emethod.HasBeenCompleted());
    print([iav.disUptodate for iav in emethod.GetVertexDistances()]);
    
    paths = emethod.FindSourceVertex(2,[]);
    paths = rotate(paths, 1);
    for epoint in paths:
        pt = epoint.Get3DPoint(rmodel);
        print('PATH POINTS :::', pt.x, pt.y, pt.z, epoint.leftVertIndex, epoint.rightVertIndex, epoint.isVertex);
        
if __name__ == "__main__":
    test();
    
    path_verts = [];
    
    