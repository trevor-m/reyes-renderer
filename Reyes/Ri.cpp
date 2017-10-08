#include "Ri.h"
#include "Primitive.h"
#include "FrameBuffer.h"
#include <stdarg.h>
#include <string>
#include <stack>
#include <unordered_map>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

RtToken RI_PERSPECTIVE = "perspective";
RtToken RI_ORTHOGRAPHIC = "orthgraphic";

// Global state
//Frame
int frameWidth = 0;
int frameHeight = 0;
int samplesWidth = 0;
int samplesHeight = 0;
float aspectRatio = 0;
std::string outputFilename;
FrameBuffer* frameBuffer = NULL;

//Transformations
Matrix4 modelView;
Matrix4 projection;
std::stack<Matrix4> transformationStack;

//Motion blur
float Ri_timeStart = 0.0f;
float Ri_timeStop = 1.0f;
Vector4 movementVector;

//Primitive creation
RtColor Ri_drawColor;
RtColor Ri_drawOpacity;
//Background
RtColor Ri_backgroundColor;

//Shaders
void(*Ri_shaderSurfaceFunction)();
void(*Ri_shaderDisplacementFunction)();

//Shader "arguments"
RtPoint __my_shader_P;
RtPoint __my_shader_N;
RtFloat __my_shader_u;
RtFloat __my_shader_v;
RtFloat __my_shader_du;
RtFloat __my_shader_dv;
RtColor __my_shader_Ci;
RtColor __my_shader_Oi;
RtVector __my_shader_dPdu;
RtVector __my_shader_dPdv;

//Textures
struct RtTexture {
	int width, height;
	unsigned char* data;
};
//hashmap of textures
std::unordered_map<int, RtTexture> textures;

// Functions to set up graphics state
RtVoid RiBegin(RtToken name) {
	//set everything to default values
	frameWidth = 640;
	frameHeight = 480;
	samplesWidth = 2;
	samplesHeight = 2;
	aspectRatio = 1.0f;
	outputFilename = "output.bmp";
	frameBuffer = NULL;

	modelView = Matrix4::Identity();
	projection = Matrix4::Identity();
	while (!transformationStack.empty())
		transformationStack.pop();

	Ri_timeStart = 0.0f;
	Ri_timeStop = 1.0f;
	movementVector = Vector4(0, 0, 0, 0);

	Ri_drawColor[0] = 1.0f;
	Ri_drawColor[1] = 1.0f;
	Ri_drawColor[2] = 1.0f;
	Ri_drawOpacity[0] = 1.0f;
	Ri_drawOpacity[1] = 1.0f;
	Ri_drawOpacity[2] = 1.0f;
	Ri_backgroundColor[0] = 0.0f;
	Ri_backgroundColor[1] = 0.0f;
	Ri_backgroundColor[2] = 0.0f;

	Ri_shaderSurfaceFunction = NULL;
	Ri_shaderDisplacementFunction = NULL;

	textures.clear();
}

RtVoid RiEnd() {
	//delete framebuffer
	if (frameBuffer != NULL) {
		delete frameBuffer;
		frameBuffer = NULL;
	}
	//free textures
	for (auto it = textures.begin(); it != textures.end(); ++it) {
		free((*it).second.data);
	}
}

RtVoid RiFormat(RtInt xres, RtInt yres, RtFloat aspect) {
	frameWidth = xres;
	frameHeight = yres;
	aspectRatio = aspect;
}

RtVoid RiProjection(RtToken name) {
	if (name == RI_PERSPECTIVE) {
		float aspect = aspectRatio * (float)frameWidth / frameHeight;
		projection = Matrix4::Perspective(45.0f * M_PI / 180.0f, aspect, NEAR_PLANE, FAR_PLANE);
	}
	else if (name == RI_ORTHOGRAPHIC) {
		projection = Matrix4::Orthographic(-5, 5, 5, -5, NEAR_PLANE, FAR_PLANE);
	}
}

RtVoid RiProjection(RtToken name, char* arg, float* fov, ...) {
	if (name == RI_PERSPECTIVE) {
		float aspect = aspectRatio * (float)frameWidth / frameHeight;
		projection = Matrix4::Perspective(*fov * M_PI / 180.0f, aspect, NEAR_PLANE, FAR_PLANE);
	}
}


RtVoid RiFrameAspectRatio(RtFloat aspect) {
	aspectRatio = aspect;
}

RtVoid RiPixelSamples(RtFloat xsamples, RtFloat ysamples) {
	samplesWidth = xsamples;
	samplesHeight = ysamples;
}

RtVoid RiDisplay(char * name, RtToken type, RtToken mode, ...) {
	outputFilename = std::string(name);
}

RtVoid RiFrameBegin(RtInt frame) {
	//if no framebuffer exists
	if (frameBuffer == NULL)
		frameBuffer = new FrameBuffer(frameWidth, frameHeight, samplesWidth, samplesHeight, Ri_timeStart, Ri_timeStop);
	//if a dimension has changed, remake
	else if (frameBuffer->GetWidth() != frameWidth || frameBuffer->GetHeight() != frameHeight || frameBuffer->GetSampleWidth() != samplesWidth || frameBuffer->GetSampleHeight() != samplesHeight) {
		delete frameBuffer;
		frameBuffer = new FrameBuffer(frameWidth, frameHeight, samplesWidth, samplesHeight, Ri_timeStart, Ri_timeStop);
	}
	//no change, just need to clear
	else
		frameBuffer->Clear();

	//reset transformation stack
	modelView = Matrix4::Identity();
	while (!transformationStack.empty())
		transformationStack.pop();

	movementVector = Vector4(0, 0, 0, 0);
}

RtVoid RiFrameEnd(void) {
	//save current frame
	printf("filtering\n");
	frameBuffer->FilterAndSaveToFile(outputFilename);
	printf("created %s\n\n", outputFilename.c_str());
}

RtVoid RiWorldBegin() {
	
}

RtVoid RiWorldEnd() {

}

// Transformations
RtVoid RiTransformBegin() {
	//save current transformation on stack
	transformationStack.push(modelView);
}

RtVoid RiTransformEnd(void) {
	//get transformation from top of stack and overwrite current
	modelView = transformationStack.top();
	transformationStack.pop();
}

RtVoid RiIdentity() {
	modelView = Matrix4::Identity();
}

RtVoid RiTransform(RtMatrix transform) {
	modelView = modelView * Matrix4(transform);
}

RtVoid RiPerspective(RtFloat fov) {
	modelView = modelView * Matrix4::Perspective(fov * M_PI / 180.0f, aspectRatio, NEAR_PLANE, FAR_PLANE);
}

RtVoid RiTranslate(RtFloat dx, RtFloat dy, RtFloat dz) {
	modelView =  modelView * Matrix4::Translate(dx, dy, dz);
}

RtVoid RiRotate(RtFloat angle, RtFloat dx, RtFloat dy, RtFloat dz) {
	//which axis?
	/*if (dx >= 0.99f)
		modelView = modelView * Matrix4::RotateX(angle * M_PI / 180.0f);
	if (dy >= 0.99f)
		modelView = modelView * Matrix4::RotateY(angle * M_PI / 180.0f);
	if (dz >= 0.99f)
		modelView = modelView * Matrix4::RotateZ(angle * M_PI / 180.0f);*/
	modelView = modelView * Matrix4::Rotate(angle * M_PI / 180.0f, dx, dy, dz);
}

RtVoid RiScale(RtFloat sx, RtFloat sy, RtFloat sz) {
	modelView = modelView * Matrix4::Scale(sx, sy, sz);
}

RtVoid RiConcatTransform(RtMatrix transform) {
	modelView = Matrix4(transform) * modelView;
}

void drawPrimitive(Primitive* primitive) {
	//check if in bounds and get dicing factors
	int diceFactor;
	Matrix4 mvp = projection * modelView;
	if (primitive->Bound(mvp, frameWidth, frameHeight, diceFactor)) {
		printf("mesh %d x %d\n", diceFactor, diceFactor);
		//dice to create mesh
		Mesh* mesh = primitive->Dice(diceFactor, diceFactor);
		//perform displacement in model space
		mesh->ShadeDisplacement();
		//transform points/normals to eye space
		mesh->ApplyMatrix(modelView);
		//shade surfaces in eye space
		mesh->ShadeSurface();
		//transform to "clip space"
		//mesh->ApplyMatrix(projection);
		//project and sample
		frameBuffer->DrawMesh(mesh, movementVector, projection);
		delete mesh;
	}
}

// Primitives
RtVoid RiSphere(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat tmax, ...) {
	Sphere primitive(radius, zmin, zmax, tmax);
	printf("sphere: ");
	drawPrimitive(&primitive);
}

RtVoid RiCone(RtFloat height, RtFloat radius, RtFloat tmax, ...) {
	Cone primitive(height, radius, tmax);
	printf("cone: ");
	drawPrimitive(&primitive);
}

RtVoid RiCylinder(RtFloat radius, RtFloat zmin, RtFloat zmax, RtFloat tmax, ...) {
	Cylinder primitive(radius, zmin, zmax, tmax);
	printf("cylinder: ");
	drawPrimitive(&primitive);
}

RtVoid RiTorus(RtFloat majrad, RtFloat minrad, RtFloat phimin, RtFloat phimax, RtFloat tmax, ...) {
	Torus primitive(majrad, minrad, phimin, phimax, tmax);
	printf("torus: ");
	drawPrimitive(&primitive);
}

// Shading
RtVoid RiColor(RtColor color) {
	Ri_drawColor[0] = color[0];
	Ri_drawColor[1] = color[1];
	Ri_drawColor[2] = color[2];
}

RtVoid RiOpacity(RtColor color) {
	Ri_drawOpacity[0] = color[0];
	Ri_drawOpacity[1] = color[1];
	Ri_drawOpacity[2] = color[2];
}

RtVoid RiBackgroundColor(RtColor color) {
	Ri_backgroundColor[0] = color[0];
	Ri_backgroundColor[1] = color[1];
	Ri_backgroundColor[2] = color[2];
}

RtVoid RiMakeTexture(const char* file, int unit) {
	RtTexture tex;
	int n;
	tex.data = stbi_load(file, &tex.width, &tex.height, &n, 3);
	if (tex.data == NULL) {
		printf("error loading \"%s\"\n", file);
		return;
	}

	//insert into hashmap
	textures[unit] = tex;
}

RtVoid RiSurface(void(*shaderFunc)(void)) {
	Ri_shaderSurfaceFunction = shaderFunc;
}

RtVoid RiDisplacement(void(*shaderFunc)(void)) {
	Ri_shaderDisplacementFunction = shaderFunc;
}

RtVoid RiShutter(RtFloat min, RtFloat max) {
	Ri_timeStart = min;
	Ri_timeStop = max;
}

RtVoid RiMotionBlur(float dx, float dy, float dz) {
	movementVector.x = dx;
	movementVector.y = dy;
	movementVector.z = dz;
	movementVector.w = 0;
}

float TEXTURE_TILE_X = 1.0f;
float TEXTURE_TILE_Y = 1.0f;
void texture(int unit) {
	//texture doesn't exist?
	if (textures.find(unit) == textures.end())
		return;
	RtTexture tex = textures[unit];
	
	//look up pixel
	int pixelX = (int)(__my_shader_u * TEXTURE_TILE_X * (tex.width - 1));
	int pixelY = (int)(__my_shader_v * TEXTURE_TILE_Y * (tex.height - 1));

	//tile
	pixelX = pixelX % tex.width;
	pixelY = pixelY % tex.height;
	
	__my_shader_Ci[0] = tex.data[(pixelY * tex.width + pixelX) * 3 + 0] / 255.0f;
	__my_shader_Ci[1] = tex.data[(pixelY * tex.width + pixelX) * 3 + 1] / 255.0f;
	__my_shader_Ci[2] = tex.data[(pixelY * tex.width + pixelX) * 3 + 2] / 255.0f;
}