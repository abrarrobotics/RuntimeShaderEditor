

#pragma once
#ifndef _MODEL_LOADER_H_
#define _MODEL_LOADER_H_

struct Model {
  // Vertex data
  float *data;
  short vertex_count;
  short vertex_stride;
  short vertex_offset;
  short color_offset;
  short normal_offset;
  short uv_offset;
  // indices data
  int indices_count;
  int *indices;
};

class ModelLoader {
  public:
    static bool loadModel(Model * model, const char* filename);
    static void freeModel(Model *model);
};

#endif // _MODEL_LOADER_H_
