#ifndef RI_H
#define RI_H
/*
* RenderMan Interface Standard Include File
* (for ANSI Standard C)
*/
/* Definitions of Abstract Types used in RI */

typedef short RtBoolean;
typedef int RtInt;
typedef float RtFloat;
typedef char *RtToken;
typedef RtFloat RtColor[3];
typedef RtFloat RtPoint[3];
typedef RtFloat RtVector[3];
typedef RtFloat RtNormal[3];
typedef RtFloat RtHpoint[4];
typedef RtFloat RtMatrix[4][4];
typedef RtFloat RtBasis[4][4];
typedef RtFloat RtBound[6];
typedef char *RtString;
typedef void *RtPointer;
#define RtVoid void
typedef RtFloat(*RtFilterFunc)(RtFloat, RtFloat, RtFloat, RtFloat);
typedef RtVoid(*RtErrorHandler)(RtInt, RtInt, char *);
typedef RtVoid(*RtProcSubdivFunc)(RtPointer, RtFloat);
typedef RtVoid(*RtProcFreeFunc)(RtPointer);
typedef RtVoid(*RtArchiveCallback)(RtToken, char *, ...);
typedef RtPointer RtObjectHandle;
typedef RtPointer RtLightHandle;
typedef RtPointer RtContextHandle;
/* Extern Declarations for Predefined RI Data Structures */
#define RI_FALSE 0
#define RI_TRUE (! RI_FALSE)
#define RI_INFINITY 1.0e38
#define RI_EPSILON 1.0e-10
#define RI_NULL ((RtToken)0)
extern RtToken RI_FRAMEBUFFER, RI_FILE;
extern RtToken RI_RGB, RI_RGBA, RI_RGBZ, RI_RGBAZ, RI_A, RI_Z, RI_AZ;
extern RtToken RI_PERSPECTIVE, RI_ORTHOGRAPHIC;
extern RtToken RI_HIDDEN, RI_PAINT;
extern RtToken RI_CONSTANT, RI_SMOOTH;
extern RtToken RI_FLATNESS, RI_FOV;
extern RtToken RI_AMBIENTLIGHT, RI_POINTLIGHT, RI_DISTANTLIGHT,
RI_SPOTLIGHT;
extern RtToken RI_INTENSITY, RI_LIGHTCOLOR, RI_FROM, RI_TO, RI_CONEANGLE,
RI_CONEDELTAANGLE, RI_BEAMDISTRIBUTION;
extern RtToken RI_MATTE, RI_METAL, RI_SHINYMETAL,
RI_PLASTIC, RI_PAINTEDPLASTIC;
extern RtToken RI_KA, RI_KD, RI_KS, RI_ROUGHNESS, RI_KR,
RI_TEXTURENAME, RI_SPECULARCOLOR;
extern RtToken RI_DEPTHCUE, RI_FOG, RI_BUMPY;
extern RtToken RI_MINDISTANCE, RI_MAXDISTANCE, RI_BACKGROUND,
RI_DISTANCE, RI_AMPLITUDE;
extern RtToken RI_RASTER, RI_SCREEN, RI_CAMERA, RI_WORLD, RI_OBJECT;
extern RtToken RI_INSIDE, RI_OUTSIDE, RI_LH, RI_RH;
extern RtToken RI_P, RI_PZ, RI_PW, RI_N, RI_NP,
RI_CS, RI_OS, RI_S, RI_T, RI_ST;
extern RtToken RI_BILINEAR, RI_BICUBIC;
extern RtToken RI_LINEAR, RI_CUBIC;
extern RtToken RI_PRIMITIVE, RI_INTERSECTION, RI_UNION, RI_DIFFERENCE;
extern RtToken RI_PERIODIC, RI_NONPERIODIC, RI_CLAMP, RI_BLACK;
extern RtToken RI_IGNORE, RI_PRINT, RI_ABORT, RI_HANDLER;
extern RtToken RI_COMMENT, RI_STRUCTURE, RI_VERBATIM;
extern RtToken RI_IDENTIFIER, RI_NAME, RI_SHADINGGROUP;
extern RtToken RI_WIDTH, RI_CONSTANTWIDTH;
extern RtBasis RiBezierBasis, RiBSplineBasis, RiCatmullRomBasis,
RiHermiteBasis, RiPowerBasis;
#define RI_BEZIERSTEP ((RtInt)3)
#define RI_BSPLINESTEP ((RtInt)1)
#define RI_CATMULLROMSTEP ((RtInt)1)
#define RI_HERMITESTEP ((RtInt)2)
#define RI_POWERSTEP ((RtInt)4)
extern RtInt RiLastError;
/* Declarations of All the RenderMan Interface Subroutines */
extern RtFloat RiGaussianFilter(RtFloat x, RtFloat y,
	RtFloat xwidth, RtFloat ywidth);
extern RtFloat RiBoxFilter(RtFloat x, RtFloat y,
	RtFloat xwidth, RtFloat ywidth);
extern RtFloat RiTriangleFilter(RtFloat x, RtFloat y,
	RtFloat xwidth, RtFloat ywidth);
extern RtFloat RiCatmullRomFilter(RtFloat x, RtFloat y,
	RtFloat xwidth, RtFloat ywidth);
extern RtFloat RiSincFilter(RtFloat x, RtFloat y,
	RtFloat xwidth, RtFloat ywidth);
extern RtVoid RiErrorIgnore(RtInt code, RtInt severity, char *msg);
extern RtVoid RiErrorPrint(RtInt code, RtInt severity, char *msg);
extern RtVoid RiErrorAbort(RtInt code, RtInt severity, char *msg);
extern RtVoid RiProcDelayedReadArchive(RtPointer data, RtFloat detail);
extern RtVoid RiProcRunProgram(RtPointer data, RtFloat detail);
extern RtVoid RiProcDynamicLoad(RtPointer data, RtFloat detail);
extern RtContextHandle RiGetContext(void);
extern RtVoid RiContext(RtContextHandle);
extern RtToken
RiDeclare(char *name, char *declaration);
extern RtVoid
RiBegin(RtToken name),
RiEnd(void),
RiFrameBegin(RtInt frame),
RiFrameEnd(void),
RiWorldBegin(void),
RiWorldEnd(void);
extern RtVoid
RiFormat(RtInt xres, RtInt yres, RtFloat aspect),
RiFrameAspectRatio(RtFloat aspect),
RiScreenWindow(RtFloat left, RtFloat right, RtFloat bot, RtFloat top),
RiCropWindow(RtFloat xmin, RtFloat xmax, RtFloat ymin, RtFloat ymax),
RiProjection(RtToken name), //MODIFIED
RiProjection(RtToken name, char* arg, float* fov, ...), //ADDED
RiProjectionV(RtToken name, RtInt n, RtToken tokens[], RtPointer parms[]),
RiClipping(RtFloat hither, RtFloat yon),
RiClippingPlane(RtFloat x, RtFloat y, RtFloat z,
	RtFloat nx, RtFloat ny, RtFloat nz),
	RiShutter(RtFloat min, RtFloat max);
extern RtVoid
RiPixelVariance(RtFloat variation),
RiPixelSamples(RtFloat xsamples, RtFloat ysamples),
RiPixelFilter(RtFilterFunc filterfunc, RtFloat xwidth, RtFloat ywidth),
RiExposure(RtFloat gain, RtFloat gamma),
RiImager(RtToken name, ...),
RiImagerV(RtToken name, RtInt n, RtToken tokens[], RtPointer parms[]),
RiQuantize(RtToken type, RtInt one, RtInt min, RtInt max, RtFloat ampl),
RiDisplay(char *name, RtToken type, RtToken mode, ...),
RiDisplayV(char *name, RtToken type, RtToken mode,
	RtInt n, RtToken tokens[], RtPointer parms[]);
extern RtVoid
RiHider(RtToken type, ...),
RiHiderV(RtToken type, RtInt n, RtToken tokens[], RtPointer parms[]),
RiColorSamples(RtInt n, RtFloat nRGB[], RtFloat RGBn[]),
RiRelativeDetail(RtFloat relativedetail),
RiOption(RtToken name, ...),
RiOptionV(RtToken name, RtInt n, RtToken tokens[], RtPointer parms[]);
extern RtVoid
RiAttributeBegin(void),
RiAttributeEnd(void),
RiColor(RtColor color),
RiOpacity(RtColor color),
RiTextureCoordinates(RtFloat s1, RtFloat t1, RtFloat s2, RtFloat t2,
	RtFloat s3, RtFloat t3, RtFloat s4, RtFloat t4);
extern RtLightHandle
RiLightSource(RtToken name, ...),
RiLightSourceV(RtToken name, RtInt n, RtToken tokens[], RtPointer parms[]),
RiAreaLightSource(RtToken name, ...),
RiAreaLightSourceV(RtToken name,
	RtInt n, RtToken tokens[], RtPointer parms[]);
extern RtVoid
RiIlluminate(RtLightHandle light, RtBoolean onoff),
//RiSurface(RtToken name, ...),
RiBackgroundColor(RtColor color), //ADDED
RiSurface(void (*shaderFunc)(void)), //MODIFIED
RiSurfaceV(RtToken name, RtInt n, RtToken tokens[], RtPointer parms[]),
RiAtmosphere(RtToken name, ...),
RiAtmosphereV(RtToken name, RtInt n, RtToken tokens[], RtPointer parms[]),
RiInterior(RtToken name, ...),
RiInteriorV(RtToken name, RtInt n, RtToken tokens[], RtPointer parms[]),
RiExterior(RtToken name, ...),
RiExteriorV(RtToken name, RtInt n, RtToken tokens[], RtPointer parms[]),
RiShadingRate(RtFloat size),
RiShadingInterpolation(RtToken type),
RiMatte(RtBoolean onoff);
extern RtVoid
RiBound(RtBound bound),
RiDetail(RtBound bound),
RiDetailRange(RtFloat minvis, RtFloat lowtran, RtFloat uptran, RtFloat maxvis),
RiGeometricApproximation(RtToken type, RtFloat value),
RiOrientation(RtToken orientation),
RiReverseOrientation(void),
RiSides(RtInt sides);
extern RtVoid
RiIdentity(void),
RiTransform(RtMatrix transform),
RiConcatTransform(RtMatrix transform),
RiPerspective(RtFloat fov),
RiTranslate(RtFloat dx, RtFloat dy, RtFloat dz),
RiRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz),
RiScale(RtFloat sx, RtFloat sy, RtFloat sz),
RiSkew(RtFloat angle, RtFloat dx1, RtFloat dy1, RtFloat dz1, RtFloat dx2, RtFloat dy2, RtFloat dz2),
RiDeformation(RtToken name, ...),
RiDeformationV(RtToken name, RtInt n, RtToken tokens[], RtPointer parms[]),
//RiDisplacement(RtToken name, ...),
RiDisplacement(void(*shaderFunc)(void)), //MODIFIED
RiDisplacementV(RtToken name, RtInt n, RtToken tokens[], RtPointer	parms[]),
RiCoordinateSystem(RtToken space),
RiCoordSysTransform(RtToken space);
extern RtPoint *
RiTransformPoints(RtToken fromspace, RtToken tospace, RtInt n, RtPoint points[]);
extern RtVoid
RiTransformBegin(void),
RiTransformEnd(void);
extern RtVoid
RiAttribute(RtToken name, ...),
RiAttributeV(RtToken name, RtInt n, RtToken tokens[], RtPointer parms[]);
extern RtVoid
RiPolygon(RtInt nverts, ...),
RiPolygonV(RtInt nverts, RtInt n, RtToken tokens[], RtPointer parms[]),
RiGeneralPolygon(RtInt nloops, RtInt nverts[], ...),
RiGeneralPolygonV(RtInt nloops, RtInt nverts[],
	RtInt n, RtToken tokens[], RtPointer parms[]),
	RiPointsPolygons(RtInt npolys, RtInt nverts[], RtInt verts[], ...),
	RiPointsPolygonsV(RtInt npolys, RtInt nverts[], RtInt verts[],
		RtInt n, RtToken tokens[], RtPointer parms[]),
	RiPointsGeneralPolygons(RtInt npolys, RtInt nloops[], RtInt nverts[],
		RtInt verts[], ...),
	RiPointsGeneralPolygonsV(RtInt npolys, RtInt nloops[], RtInt nverts[],
		RtInt verts[], RtInt n, RtToken tokens[], RtPointer parms[]),
	RiBasis(RtBasis ubasis, RtInt ustep, RtBasis vbasis, RtInt vstep),
	RiPatch(RtToken type, ...),
	RiPatchV(RtToken type, RtInt n, RtToken tokens[], RtPointer parms[]),
	RiPatchMesh(RtToken type, RtInt nu, RtToken uwrap,
		RtInt nv, RtToken vwrap, ...),
	RiPatchMeshV(RtToken type, RtInt nu, RtToken uwrap,
		RtInt nv, RtToken vwrap, RtInt n, RtToken tokens[], RtPointer parms[]),
	RiNuPatch(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin,
		RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[],
		RtFloat vmin, RtFloat vmax, ...),
	RiNuPatchV(RtInt nu, RtInt uorder, RtFloat uknot[], RtFloat umin,
		RtFloat umax, RtInt nv, RtInt vorder, RtFloat vknot[],
		RtFloat vmin, RtFloat vmax, RtInt n, RtToken tokens[], RtPointer parms[]),
	RiTrimCurve(RtInt nloops, RtInt ncurves[], RtInt order[],
		RtFloat knot[], RtFloat min[], RtFloat max[], RtInt n[],
		RtFloat u[], RtFloat v[], RtFloat w[]);
extern RtVoid
RiSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat tmax, ...),
RiSphereV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat tmax,
	RtInt n, RtToken tokens[], RtPointer parms[]),
	RiCone(RtFloat height, RtFloat radius, RtFloat tmax, ...),
	RiConeV(RtFloat height, RtFloat radius, RtFloat tmax,
		RtInt n, RtToken tokens[], RtPointer parms[]),
	RiCylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat tmax, ...),
	RiCylinderV(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat tmax,
		RtInt n, RtToken tokens[], RtPointer parms[]),
	RiHyperboloid(RtPoint point1, RtPoint point2, RtFloat tmax, ...),
	RiHyperboloidV(RtPoint point1, RtPoint point2, RtFloat tmax,
		RtInt n, RtToken tokens[], RtPointer parms[]),
	RiParaboloid(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat tmax, ...),
	RiParaboloidV(RtFloat rmax, RtFloat zmin, RtFloat zmax, RtFloat tmax,
		RtInt n, RtToken tokens[], RtPointer parms[]),
	RiDisk(RtFloat height, RtFloat radius, RtFloat tmax, ...),
	RiDiskV(RtFloat height, RtFloat radius, RtFloat tmax,
		RtInt n, RtToken tokens[], RtPointer parms[]),
	RiTorus(RtFloat majrad, RtFloat minrad, RtFloat phimin,
		RtFloat phimax, RtFloat tmax, ...),
	RiTorusV(RtFloat majrad, RtFloat minrad,
		RtFloat phimin, RtFloat phimax, RtFloat tmax,
		RtInt n, RtToken tokens[], RtPointer parms[]);
extern RtVoid RiBlobby(RtInt nleaf, RtInt ncode, RtInt code[],
	RtInt nflt, RtFloat flt[],
	RtInt nstr, RtToken str[], ...);
extern RtVoid RiBlobbyV(RtInt nleaf, RtInt ncode, RtInt code[],
	RtInt nflt, RtFloat flt[],
	RtInt nstr, RtToken str[],
	RtInt n, RtToken tokens[], RtPointer parms[]);
extern RtVoid
RiCurves(RtToken type, RtInt ncurves,
	RtInt nvertices[], RtToken wrap, ...),
	RiCurvesV(RtToken type, RtInt ncurves, RtInt nvertices[], RtToken wrap,
		RtInt n, RtToken tokens[], RtPointer parms[]),
	RiPoints(RtInt nverts, ...),
	RiPointsV(RtInt nverts, RtInt n, RtToken tokens[], RtPointer parms[]),
	RiSubdivisionMesh(RtToken mask, RtInt nf, RtInt nverts[],
		RtInt verts[],
		RtInt ntags, RtToken tags[], RtInt numargs[],
		RtInt intargs[], RtFloat floatargs[], ...),
	RiSubdivisionMeshV(RtToken mask, RtInt nf, RtInt nverts[],
		RtInt verts[], RtInt ntags, RtToken tags[],
		RtInt nargs[], RtInt intargs[],
		RtFloat floatargs[], RtInt n,
		RtToken tokens[], RtPointer *parms);
extern RtVoid
RiProcedural(RtPointer data, RtBound bound,
	RtVoid(*subdivfunc)(RtPointer, RtFloat),
	RtVoid(*freefunc)(RtPointer)),
	RiGeometry(RtToken type, ...),
	RiGeometryV(RtToken type, RtInt n, RtToken tokens[], RtPointer parms[]);
extern RtVoid
RiSolidBegin(RtToken operation),
RiSolidEnd(void);
extern RtObjectHandle
RiObjectBegin(void);
extern RtVoid
RiObjectEnd(void),
RiObjectInstance(RtObjectHandle handle),
RiMotionBegin(RtInt n, ...),
RiMotionBeginV(RtInt n, RtFloat times[]),
RiMotionEnd(void);
RtVoid RiMotionBlur(float dx, float dy, float dz); // ADDED
extern RtVoid
RiMakeTexture(const char* file, int unit), //ADDED
//RiMakeTexture(char *pic, char *tex, RtToken swrap, RtToken twrap, //MODIFIED
	//RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...),
	/*RiMakeTextureV(char *pic, char *tex, RtToken swrap, RtToken twrap, //MODIFIED
		RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer parms[]),*/
	RiMakeBump(char *pic, char *tex, RtToken swrap, RtToken twrap,
		RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth, ...),
	RiMakeBumpV(char *pic, char *tex, RtToken swrap, RtToken twrap,
		RtFilterFunc filterfunc, RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer parms[]),
	RiMakeLatLongEnvironment(char *pic, char *tex,
		RtFilterFunc filterfunc,
		RtFloat swidth, RtFloat twidth, ...),
	RiMakeLatLongEnvironmentV(char *pic, char *tex,
		RtFilterFunc filterfunc,
		RtFloat swidth, RtFloat twidth,
		RtInt n, RtToken tokens[], RtPointer parms[]),
	RiMakeCubeFaceEnvironment(char *px, char *nx, char *py, char *ny,
		char *pz, char *nz, char *tex, RtFloat fov,
		RtFilterFunc filterfunc, RtFloat swidth, RtFloat ywidth, ...),
	RiMakeCubeFaceEnvironmentV(char *px, char *nx, char *py, char *ny,
		char *pz, char *nz, char *tex, RtFloat fov,
		RtFilterFunc filterfunc, RtFloat swidth, RtFloat ywidth,
		RtInt n, RtToken tokens[], RtPointer parms[]),
	RiMakeShadow(char *pic, char *tex, ...),
	RiMakeShadowV(char *pic, char *tex,
		RtInt n, RtToken tokens[], RtPointer parms[]);
extern RtVoid
RiArchiveRecord(RtToken type, char *format, ...),
RiReadArchive(RtToken name, RtArchiveCallback callback, ...),
RiReadArchiveV(RtToken name, RtArchiveCallback callback,
	RtInt n, RtToken tokens[], RtPointer parms[]);
extern RtVoid
RiErrorHandler(RtErrorHandler handler);
/*
Error Codes
1 - 10 System and File Errors
11 - 20 Program Limitations
21 - 40 State Errors
41 - 60 Parameter and Protocol Errors
61 - 80 Execution Errors
*/
#define RIE_NOERROR ((RtInt)0)
#define RIE_NOMEM ((RtInt)1) /* Out of memory */
#define RIE_SYSTEM ((RtInt)2) /* Miscellaneous system error */
#define RIE_NOFILE ((RtInt)3) /* File nonexistent */
#define RIE_BADFILE ((RtInt)4) /* Bad file format */
#define RIE_VERSION ((RtInt)5) /* File version mismatch */
#define RIE_DISKFULL ((RtInt)6) /* Target disk is full */
#define RIE_INCAPABLE ((RtInt)11) /* Optional RI feature */
#define RIE_UNIMPLEMENT ((RtInt)12) /* Unimplemented feature */
#define RIE_LIMIT ((RtInt)13) /* Arbitrary program limit */
#define RIE_BUG ((RtInt)14) /* Probably a bug in renderer */
#define RIE_NOTSTARTED ((RtInt)23) /* RiBegin not called */
#define RIE_NESTING ((RtInt)24) /* Bad begin-end nesting */
#define RIE_NOTOPTIONS ((RtInt)25) /* Invalid state for options */
#define RIE_NOTATTRIBS ((RtInt)26) /* Invalid state for attribs */
#define RIE_NOTPRIMS ((RtInt)27) /* Invalid state for primitives */
#define RIE_ILLSTATE ((RtInt)28) /* Other invalid state */
#define RIE_BADMOTION ((RtInt)29) /* Badly formed motion block */
#define RIE_BADSOLID ((RtInt)30) /* Badly formed solid block */
#define RIE_BADTOKEN ((RtInt)41) /* Invalid token for request */
#define RIE_RANGE ((RtInt)42) /* Parameter out of range */
#define RIE_CONSISTENCY ((RtInt)43) /* Parameters inconsistent */
#define RIE_BADHANDLE ((RtInt)44) /* Bad object/light handle */
#define RIE_NOSHADER ((RtInt)45) /* Can't load requested shader */
#define RIE_MISSINGDATA ((RtInt)46) /* Required parameters not provided */
#define RIE_SYNTAX ((RtInt)47) /* Declare type syntax error */
#define RIE_MATH ((RtInt)61) /* Zerodivide, noninvert matrix, etc. */
/* Error severity levels */
#define RIE_INFO ((RtInt)0) /* Rendering stats and other info */
#define RIE_WARNING ((RtInt)1) /* Something seems wrong, maybe okay */
#define RIE_ERROR ((RtInt)2) /* Problem. Results may be wrong */
#define RIE_SEVERE ((RtInt)3) /* So bad you should probably abort */
#endif