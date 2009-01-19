
#ifndef __SURFACE_H
#define __SURFACE_H

// Utility functions, Bernstein polynomials of order 1-3 and their derivatives.
double Bernstein(int k, int deg, double t);
double BernsteinDerivative(int k, int deg, double t);

class hSSurface {
public:
    DWORD v;
};

class hSCurve {
public:
    DWORD v;
};

// Stuff for rational polynomial curves, of degree one to three. These are
// our inputs.
class SBezier {
public:
    int             tag;
    int             deg;
    Vector          ctrl[4];
    double          weight[4];

    Vector PointAt(double t);
    Vector Start(void);
    Vector Finish(void);
    void MakePwlInto(List<Vector> *l);
    void MakePwlWorker(List<Vector> *l, double ta, double tb);

    void Reverse(void);

    static SBezier From(Vector p0, Vector p1, Vector p2, Vector p3);
    static SBezier From(Vector p0, Vector p1, Vector p2);
    static SBezier From(Vector p0, Vector p1);
};

class SBezierList {
public:
    List<SBezier>   l;

    void Clear(void);
};

class SBezierLoop {
public:
    List<SBezier>   l;

    inline void Clear(void) { l.Clear(); }
    void Reverse(void);
    void MakePwlInto(SContour *sc);

    static SBezierLoop FromCurves(SBezierList *spcl,
                                  bool *allClosed, SEdge *errorAt);
};

class SBezierLoopSet {
public:
    List<SBezierLoop> l;
    Vector normal;

    static SBezierLoopSet From(SBezierList *spcl, SPolygon *poly,
                          bool *allClosed, SEdge *errorAt);

    void Clear(void);
};

// Stuff for the surface trim curves: piecewise linear
class SCurve {
public:
    hSCurve         h;

    SBezier         exact; // or deg = 0 if we don't know the exact form
    List<Vector>    pts;
    hSSurface       srfA;
    hSSurface       srfB;
};

// A segment of a curve by which a surface is trimmed: indicates which curve,
// by its handle, and the starting and ending points of our segment of it.
// The vector out points out of the surface; it, the surface outer normal,
// and a tangent to the beginning of the curve are all orthogonal.
class STrimBy {
public:
    hSCurve     curve;
    Vector      start;
    Vector      finish;
    Vector      out;
};

class SSurface {
public:
    hSSurface       h;

    int             degm, degn;
    Vector          ctrl[4][4];
    double          weight[4][4];

    List<STrimBy>   trim;

    static SSurface FromExtrusionOf(SBezier *spc, Vector t0, Vector t1);

    void ClosestPointTo(Vector p, double *u, double *v);
    Vector PointAt(double u, double v);
    Vector TangentWrtUAt(double u, double v);
    Vector TangentWrtVAt(double u, double v);
    Vector NormalAt(double u, double v);

    void TriangulateInto(SMesh *sm);
};

class SShell {
public:
    IdList<SCurve,hSCurve>      curve;
    IdList<SSurface,hSSurface>  surface;

    static SShell FromExtrusionOf(SBezierList *spcl, Vector t0, Vector t1);

    static SShell FromUnionOf(SShell *a, SShell *b);
};

#endif
