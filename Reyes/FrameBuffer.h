#pragma once
#include "Mesh.h"
#include "Triangle.h"
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
using std::max;
using std::min;

#define NEAR_PLANE 0.01f
#define FAR_PLANE 100.0f

// Furthest distance
#define MAX_DEPTH 1.0f

// All alphas below this value will be considered fully transparent and discarded
#define ALPHA_TRANSPARENT_THRESHOLD 0.04f
// All alphas above this value will be considered fully opaque and will block
#define ALPHA_OPAQUE_THRESHOLD 0.96f

// Motion blur reference
// http://graphics.stanford.edu/papers/mprast/rast_hpg09.pdf

extern RtColor Ri_backgroundColor;

// The values of a sampled point as a linked-list node
struct SampleNode {
	//data
	float color[3];
	float opacity[3];
	float depth;

	//next
	SampleNode* next;

	SampleNode() {
		next = NULL;
	}
};

// Holds state of current frame
class FrameBuffer {
private:
	// Dimensions
	int width, height;
	int mSamples, nSamples;
	int numIntervals;

	float timeStart, timeStop;

	// Sample kernel (x, y, t)
	// x and y are in pixel space
	// t is from timeStart to timeStop
	float* sampleKernel;

	// Z-buffer and color buffer
	SampleNode** zbuffer;

	// Linked-list manipulation
	// Insert a sample
	void insertSample(int x, int y, int m, int n, SampleNode* node);
	// Collapse list of samples and apply opacity
	void collapseSamples(SampleNode* head, float* color);
	// Deletes list from node until the end
	void deleteList(SampleNode* node);

public:
	FrameBuffer() {
		width = height = mSamples = nSamples = 0;
		zbuffer = NULL;
		sampleKernel = NULL;
	}

	// Initalize and allocate a framebuffer with the given dimensions
	FrameBuffer(int width, int height, int mSamples, int nSamples, float timeStart, float timeStop);

	~FrameBuffer() {
		delete[] sampleKernel;
		Clear();
		delete[] zbuffer;
	}

	// Apply a mesh to depth/color buffer
	void DrawMesh(Mesh* mesh, Vector4& movementVector, Matrix4& projection);

	// Filter and output to bitmap file
	void FilterAndSaveToFile(std::string path);

	// Completely clear buffer
	void Clear() {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				for (int m = 0; m < mSamples; m++) {
					for (int n = 0; n < nSamples; n++) {
						SampleNode** head = getHeadPtr(x, y, m, n);
						deleteList(*head);
						*head = NULL;
					}
				}
			}
		}
	}

	// Get the head of the linkedlist at the given pixel and sample point
	SampleNode** getHeadPtr(int x, int y, int m, int n) {
		return (zbuffer + (y * mSamples * nSamples * width + x * mSamples * nSamples + n * mSamples + m));
	}

	// Get the sample position as an array of 3 floats
	float* getKernelPtr(int x, int y, int m, int n) {
		return (sampleKernel + (y * mSamples * nSamples * width + x * mSamples * nSamples + n * mSamples + m) * 3);
	}

	int GetWidth() {
		return width;
	}

	int GetHeight() {
		return height;
	}

	int GetSampleWidth() {
		return mSamples;
	}

	int GetSampleHeight() {
		return nSamples;
	}
};