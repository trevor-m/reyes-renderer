#include "FrameBuffer.h"
//#include "CImg.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

FrameBuffer::FrameBuffer(int width, int height, int mSamples, int nSamples, float timeStart, float timeStop) {
	this->width = width;
	this->height = height;
	this->mSamples = mSamples;
	this->nSamples = nSamples;
	//motion blur
	this->numIntervals = mSamples * nSamples;
	this->timeStart = timeStart;
	this->timeStop = timeStop;

	//allocate zbuffer
	zbuffer = new SampleNode*[width * mSamples * height * nSamples];

	//create sample kernel
	sampleKernel = new float[width * mSamples * height * nSamples * 3];
	//m x n jittering
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			//which time interval to place sample in (there are mxn total intervals)
			std::vector<int> intervals;
			for (int i = 0; i < numIntervals; i++)
				intervals.push_back(i);
			std::random_shuffle(intervals.begin(), intervals.end());
			int index = 0;
			for (int m = 0; m < mSamples; m++) {
				for (int n = 0; n < nSamples; n++) {
					float mMin = (float)m / mSamples;
					float nMin = (float)n / nSamples;
					float mMax = (float)(m + 1) / mSamples;
					float nMax = (float)(n + 1) / nSamples;

					//pick random position values between min and max
					getKernelPtr(x, y, m, n)[0] = x + ((float)rand() / RAND_MAX) * (mMax - mMin) + mMin;
					getKernelPtr(x, y, m, n)[1] = y + ((float)rand() / RAND_MAX) * (nMax - nMin) + nMin;

					//pick time sample within this interval
					float tMin = ((float)intervals[index] / numIntervals) * (timeStop - timeStart) + timeStart;
					float tMax = ((float)(intervals[index] + 1) / numIntervals) * (timeStop - timeStart) + timeStart;
					getKernelPtr(x, y, m, n)[2] = ((float)rand() / RAND_MAX) * (tMax - tMin) + tMin;
					index++;

					//also initalize zbuffer to NULL at every pixel and sample point
					*getHeadPtr(x, y, m, n) = NULL;
				}
			}
		}
	}
}

void FrameBuffer::FilterAndSaveToFile(std::string path) {
	//cimg_library::cimg::imagemagick_path("ImageMagick-7.0.5-Q16/convert.exe");
	//cimg_library::CImg<unsigned char> outputBuffer(width, height, 1, 3);
	//filter
	unsigned char* outputBuffer = new unsigned char[width * height * 3];
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			//average all samples in this pixel (box filter)
			float pixelColor[3] = { 0 };
			for (int m = 0; m < mSamples; m++) {
				for (int n = 0; n < nSamples; n++) {
					//collapse all opacities at each sample point
					float sampleColor[3] = { 0 };
					collapseSamples(*getHeadPtr(x, y, m, n), sampleColor);

					pixelColor[0] += sampleColor[0];
					pixelColor[1] += sampleColor[1];
					pixelColor[2] += sampleColor[2];
				}
			}
			//average
			pixelColor[0] /= (mSamples * nSamples);
			pixelColor[1] /= (mSamples * nSamples);
			pixelColor[2] /= (mSamples * nSamples);
			//clamp and quantize
			outputBuffer[3 * (y * width + x) + 0] = max(0, min(255, (int)(pixelColor[0] * 255.0f)));
			outputBuffer[3 * (y * width + x) + 1] = max(0, min(255, (int)(pixelColor[1] * 255.0f)));
			outputBuffer[3 * (y * width + x) + 2] = max(0, min(255, (int)(pixelColor[2] * 255.0f)));
			//outputBuffer(x, y, 0, 0) = max(0, min(255, (int)(pixelColor[0] * 255.0f)));
			//outputBuffer(x, y, 0, 1) = max(0, min(255, (int)(pixelColor[1] * 255.0f)));
			//outputBuffer(x, y, 0, 2) = max(0, min(255, (int)(pixelColor[2] * 255.0f)));
		}
	}

	//save to file
	if (path.substr(path.find_last_of(".") + 1) == "png")
		stbi_write_png(path.c_str(), width, height, 3, outputBuffer, width*3);
	else if (path.substr(path.find_last_of(".") + 1) == "tga")
		stbi_write_tga(path.c_str(), width, height, 3, outputBuffer);
	else
		stbi_write_bmp(path.c_str(), width, height, 3, outputBuffer);

	delete[] outputBuffer;
	//outputBuffer.save(path.c_str());
}

void FrameBuffer::DrawMesh(Mesh* mesh, Vector4& movementVector, Matrix4& projection) {
	//perform perspective divide and transform to pixel space
	//mesh->Project(width, height);

	//break into micropolygons
	for (int u = 0; u < mesh->width - 1; u++) {
		for (int v = 0; v < mesh->height - 1; v++) {
			//get points of micropolygon
			MeshPoint* points[4];
			points[0] = mesh->getGridPtr(u, v);
			points[1] = mesh->getGridPtr(u, v + 1);
			points[2] = mesh->getGridPtr(u + 1, v + 1);
			points[3] = mesh->getGridPtr(u + 1, v);

			//if any points have been invalidated, skip
			//if (!points[0]->valid || !points[1]->valid || !points[2]->valid || !points[3]->valid)
				//continue;

			//get bounding box (over time)
			Vector4 startPoints[4];
			Vector4 endPoints[4];
			int min_x = width;
			int max_x = 0;
			int min_y = height;
			int max_y = 0;
			bool valid = true;
			for (int j = 0; j < 4; j++) {
				startPoints[j] = points[j]->position + movementVector * timeStart;
				endPoints[j] = points[j]->position + movementVector * timeStop;
				
				//check against near and far planes
				if (startPoints[j].z < 0 || endPoints[j].z < 0)
					valid = false;
				//project
				startPoints[j] = projection * startPoints[j];
				endPoints[j] = projection * endPoints[j];
				//perspective divide
				startPoints[j] = startPoints[j].homogenize();
				endPoints[j] = endPoints[j].homogenize();

				//convert to pixel space
				startPoints[j].x = (startPoints[j].x + 1.0f) * 0.5f * width;
				startPoints[j].y = (-startPoints[j].y + 1.0f) * 0.5f * height; //y axis is inverted in pixel space
				endPoints[j].x = (endPoints[j].x + 1.0f) * 0.5f * width;
				endPoints[j].y = (-endPoints[j].y + 1.0f) * 0.5f * height;

				min_x = min(min_x, (int)floor(startPoints[j].x));
				min_x = min(min_x, (int)floor(endPoints[j].x));
				min_y = min(min_y, (int)floor(startPoints[j].y));
				min_y = min(min_y, (int)floor(endPoints[j].y));
				max_x = max(max_x, (int)ceil(startPoints[j].x));
				max_x = max(max_x, (int)ceil(endPoints[j].x));
				max_y = max(max_y, (int)ceil(startPoints[j].y));
				max_y = max(max_y, (int)ceil(endPoints[j].y));
			}
			if (!valid)
				continue;
			/*int min_x = floor(min(min(triangles[i].v[0].x, triangles[i].v[1].x), triangles[i].v[2].x));
			int max_x = ceil(max(max(triangles[i].v[0].x, triangles[i].v[1].x), triangles[i].v[2].x));
			int min_y = floor(min(min(triangles[i].v[0].y, triangles[i].v[1].y), triangles[i].v[2].y));
			int max_y = ceil(max(max(triangles[i].v[0].y, triangles[i].v[1].y), triangles[i].v[2].y));*/

			//clamp bounds to window
			min_x = max(0, min_x);
			max_x = min(width, max_x);
			min_y = max(0, min_y);
			max_y = min(height, max_y);

			//sample all points within bounding box
			for (int x = min_x; x < max_x; x++) {
				for (int y = min_y; y < max_y; y++) {
					for (int m = 0; m < mSamples; m++) {
						for (int n = 0; n < nSamples; n++) {
							//get sample pos
							float sampleX = getKernelPtr(x, y, m, n)[0];
							float sampleY = getKernelPtr(x, y, m, n)[1];
							float sampleTime = getKernelPtr(x, y, m, n)[2];

							//sample each triangle
							Triangle triangles[2];
							triangles[0] = Triangle(points[0]->position, points[1]->position, points[2]->position);
							triangles[1] = Triangle(points[0]->position, points[2]->position, points[3]->position);

							for (int i = 0; i < 2; i++) {
								//calc position based on time
								for (int j = 0; j < 3; j++) {
									triangles[i].v[j] = triangles[i].v[j] + movementVector * sampleTime;
									//project
									triangles[i].v[j] = projection * triangles[i].v[j];
									//perspective divide
									triangles[i].v[j] = triangles[i].v[j].homogenize();

									//convert to pixel space
									triangles[i].v[j].x = (triangles[i].v[j].x + 1.0f) * 0.5f * width;
									triangles[i].v[j].y = (-triangles[i].v[j].y + 1.0f) * 0.5f * height; //y axis is inverted in pixel space
								}

								//set up interpolation
								float area = abs(triangles[i].normal().length()) / 2;
								Vector4 edge0 = triangles[i].v[2] - triangles[i].v[1];
								Vector4 edge1 = triangles[i].v[0] - triangles[i].v[2];

								//if sample point is inside triangle
								if (triangles[i].insideTriangle(sampleX, sampleY)) {
									//create node
									SampleNode* node = new SampleNode();

									//interpolate attributes
									Vector4 p = Vector4(sampleX, sampleY, 1);
									Vector4 vp1 = p - triangles[i].v[1];
									Vector4 vp2 = p - triangles[i].v[2];
									float b0 = (edge0.cross(vp1).length() / 2) / area;
									float b1 = (edge1.cross(vp2).length() / 2) / area;
									//interpolate depth
									node->depth = b0 * triangles[i].v[0].z + b1 * triangles[i].v[1].z + (1 - b0 - b1) * triangles[i].v[2].z;
									//interpolate color and opacity
									for (int c = 0; c < 3; c++) {
										if (i == 0) {
											//triangle 0 has points 0,1,2
											node->color[c] = b0 * points[0]->color[c] + b1 * points[1]->color[c] + (1 - b0 - b1) * points[2]->color[c];
											node->opacity[c] = b0 * points[0]->opacity[c] + b1 * points[1]->opacity[c] + (1 - b0 - b1) * points[2]->opacity[c];
										}
										else {
											//triangle 1 has points 0,2,3
											node->color[c] = b0 * points[0]->color[c] + b1 * points[2]->color[c] + (1 - b0 - b1) * points[3]->color[c];
											node->opacity[c] = b0 * points[0]->opacity[c] + b1 * points[2]->opacity[c] + (1 - b0 - b1) * points[3]->opacity[c];
										}

										//clamp color and opacity between 0 and 1
										node->color[c] = min(max(0.0f, node->color[c]), 1.0f);
										node->opacity[c] = min(max(0.0f, node->opacity[c]), 1.0f);
									}

									//insert into buffer (sorted by depth - this will effectively perform depth test)
									insertSample(x, y, m, n, node);
								}
							}
						}
					}
				}
			}
			//}
		}
	}
}

void FrameBuffer::insertSample(int x, int y, int m, int n, SampleNode* node) {
	//discard samples below minimum threshold
	if (node->opacity[0] < ALPHA_TRANSPARENT_THRESHOLD && node->opacity[1] < ALPHA_TRANSPARENT_THRESHOLD && node->opacity[2] < ALPHA_TRANSPARENT_THRESHOLD)
		return;

	SampleNode** headPtr = getHeadPtr(x, y, m, n);

	//empty or first item
	if (*headPtr == NULL ||  node->depth < (*headPtr)->depth) {
		node->next = *headPtr;
		*headPtr = node;
	}
	else {
		SampleNode* curr = *headPtr;
		while (curr->next != NULL && curr->next->depth < node->depth) {
			curr = curr->next;
		}
		node->next = curr->next;
		curr->next = node;
		//if newly inserted item is fully opaque, delete everything after it
		if (node->opacity[0] > ALPHA_OPAQUE_THRESHOLD && node->opacity[1] > ALPHA_OPAQUE_THRESHOLD && node->opacity[2] > ALPHA_OPAQUE_THRESHOLD) {
			deleteList(node->next);
			node->next = NULL;
		}
	}
}

void FrameBuffer::collapseSamples(SampleNode* node, float* color) {
	//no entries or end of list - color is background
	if (node == NULL) {
		color[0] = Ri_backgroundColor[0];
		color[1] = Ri_backgroundColor[1];
		color[2] = Ri_backgroundColor[2];
		return;
	}
	//look at next sample
	collapseSamples(node->next, color);

	//apply this nodes color
	color[0] = node->color[0] * node->opacity[0] + color[0] * (1 - node->opacity[0]);
	color[1] = node->color[1] * node->opacity[1] + color[1] * (1 - node->opacity[1]);
	color[2] = node->color[2] * node->opacity[2] + color[2] * (1 - node->opacity[2]);
}

void FrameBuffer::deleteList(SampleNode* node) {
	while (node != NULL) {
		SampleNode* temp = node;
		node = node->next;
		delete temp;
	}
}