from py_chenhancc import CPoint3D, CFace, CBaseModel, CRichModel, CICHWithFurtherPriorityQueue;

def rotate(l, n):
    return l[n:] + l[:n];

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

print(emethod.GetRunTime());
print(emethod.GetMemoryCost());
print(emethod.GetWindowNum());
print(emethod.GetMaxLenOfQue());
print(emethod.GetNPE());
print(emethod.GetDepthOfSequenceTree());
print(emethod.HasBeenCompleted());
print(emethod.HasBeenCompleted());
print([iav.disUptodate for iav in emethod.GetVertexDistances()]);

paths = emethod.FindSourceVertex(2,[]);
paths = rotate(paths, 1);

path_verts = [];

for epoint in paths:
    pt = epoint.Get3DPoint(rmodel);
    print(pt.x, pt.y, pt.z);