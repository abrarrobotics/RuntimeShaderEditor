

#include <Drawables/ModelLoader.hpp>
#include <Drawables/PLYFormatHelper.hpp>


bool ModelLoader::loadModel(Model* model, const char *filename)
{
    // declarations
    PLYFormatHelper formatHelper;
    int vertex_count = 0;
    int face_count = 0;
    FILE *file = NULL;
    char line[200];

    // open the file
    file = fopen(filename, "r");
    if(!file)
      return false;

    //it reads the magic number with a BUFFER OVERFLOW VULNERABILITY
    fscanf(file, "%s\r", line);
    if(strcmp(line, "ply") != 0) return false;
    // ir reads the format (ascci/binary)
    fscanf(file, "format ascii %s\r", line);
    if(strcmp(line, "1.0") != 0) return false;

    printf("PLY ascii 1.0\r\n");
    while(!strstr(line, "end_header"))
    {
      fgets(line, 200, file);
      if(strstr(line, "element"))
      {
        //char elem[100];
        char name[100];
        int count;
        sscanf(line, "element %s %i\r", name, &count);
        if(strstr(name, "vertex")) vertex_count = count;
        else if(strstr(name, "face")) face_count = count;
      }
      else if(strstr(line, "property"))
      {
        //char prop[100];
        char type[100];
        char name[10];
        sscanf(line, "property %s %s\r", type, name);

        int type_param = -1;
        int name_param = -1;

        if(strstr(type,"float")) type_param = PLYFormatHelper::FLOAT;
        else if(strstr(type,"int")) type_param = PLYFormatHelper::INT;

        if(strstr("x",name)) { name_param = PLYFormatHelper::X; }
        else if(strstr("y",name)) { name_param = PLYFormatHelper::Y; }
        else if(strstr("z",name)) { name_param = PLYFormatHelper::Z; }
        else if(strstr("nx",name)) { name_param = PLYFormatHelper::NX; }
        else if(strstr("ny",name)) { name_param = PLYFormatHelper::NY; }
        else if(strstr("nz",name)) { name_param = PLYFormatHelper::NZ; }
        else if(strstr("red",name)) { name_param = PLYFormatHelper::R; }
        else if(strstr("green",name)) { name_param = PLYFormatHelper::G; }
        else if(strstr("blue",name)) { name_param = PLYFormatHelper::B; }
        else if(strstr("u",name)) { name_param = PLYFormatHelper::U; }
        else if(strstr("v",name)) { name_param = PLYFormatHelper::V; }

        if(type_param >= 0 && name_param >= 0)
            formatHelper.addProperty(type_param, name_param);
      }
    }

    model->vertex_count = vertex_count;
    model->indices_count = face_count * 3;

    unsigned char floats_per_vertex = 0;
    model->vertex_stride = 0;
    if(formatHelper.status.position == ENABLED_ATTRIBUTE)
    {
        model->vertex_offset = model->vertex_stride;
        model->vertex_stride += 3 * sizeof(float);
        floats_per_vertex += 3;
    }
    if(formatHelper.status.colors == ENABLED_ATTRIBUTE)
    {
      model->color_offset = model->vertex_stride;
      model->vertex_stride += 3 * sizeof(float);
      floats_per_vertex += 3;
    }
    if(formatHelper.status.normals == ENABLED_ATTRIBUTE)
    {
        model->normal_offset = model->vertex_stride;
        model->vertex_stride += 3 * sizeof(float);
        floats_per_vertex += 3;
    }
    if(formatHelper.status.uvs == ENABLED_UV)
    {
      model->uv_offset = model->vertex_stride;
      model->vertex_stride += 2 * sizeof(float);
      floats_per_vertex += 2;
    }

    int inserted_floats = 0;
    model->data = new float[ model->vertex_count * floats_per_vertex];
    for(int i = 0; i < vertex_count; i++)
    {
      fscanf(file, formatHelper.format,
                   formatHelper.addresses[0],
                   formatHelper.addresses[1],
                   formatHelper.addresses[2],
                   formatHelper.addresses[3],
                   formatHelper.addresses[4],
                   formatHelper.addresses[5],
                   formatHelper.addresses[6],
                   formatHelper.addresses[7],
                   formatHelper.addresses[8],
                   formatHelper.addresses[9],
                   formatHelper.addresses[10]);

          if(formatHelper.status.position ==  ENABLED_ATTRIBUTE){
            model->data[inserted_floats++] = formatHelper.vertex.x;
            model->data[inserted_floats++] = formatHelper.vertex.y;
            model->data[inserted_floats++] = formatHelper.vertex.z;
          }
          if(formatHelper.status.colors ==  ENABLED_ATTRIBUTE) {
            model->data[inserted_floats++] = formatHelper.vertex.r / 250.0f;
            model->data[inserted_floats++] = formatHelper.vertex.g / 250.0f;
            model->data[inserted_floats++] = formatHelper.vertex.b / 250.0f;
          }
          if(formatHelper.status.normals ==  ENABLED_ATTRIBUTE) {
            model->data[inserted_floats++] = formatHelper.vertex.nx;
            model->data[inserted_floats++] = formatHelper.vertex.ny;
            model->data[inserted_floats++] = formatHelper.vertex.nz;
          }
          if(formatHelper.status.uvs ==  ENABLED_UV) {
            model->data[inserted_floats++] = formatHelper.vertex.u;
            model->data[inserted_floats++] = formatHelper.vertex.v;
          }

    }


    model->indices = new int[ model->indices_count];
    int vars[3];
    for(int i = 0; i < face_count; i++)
    {
      fscanf(file, "3 %i %i %i\n",&vars[0], &vars[1], &vars[2]);
      memcpy(&model->indices[i * 3], vars, 3 * 4);
    }
    return true;
}

void ModelLoader::freeModel(Model* model)
{
  if(model->data)
    delete[] model->data;
  if(model->indices)
    delete[] model->indices;
}
