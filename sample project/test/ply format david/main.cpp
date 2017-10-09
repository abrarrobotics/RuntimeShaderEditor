#include <iostream>
#include <cstdio>
#include <Drawables/ModelLoader.hpp>

using namespace std;

int main(int argc, char*argv[])
{
  Model m;
  if(!ModelLoader::loadModel(&m, "./monkey.ply"))
  {
    cout << "No fue posible cargar el modelo" << endl;
    return 1;
  }


  printf("El modelo es PLY ascii v1.0\r\nTiene %i vertices y %i caras\r\n",
     m.vertex_count, m.indices_count);

  printf("El stride es %i\r\n", m.vertex_stride);

  printf("vertex_offset: %i\r\nnormal_offset: %i\r\n", m.vertex_offset, m.normal_offset);

  for (int i = 0; i < 10; i++) {
      int index = i * 6;
      printf("x: %f, y: %f, z: %f\r\n", m.data[index + 0], m.data[index + 1], m.data[index + 2]);
  }
  for (int i = 610; i < 620; i++) {
      printf("indice %i : %i\r\n", i,  m.indices[i]);
  }
  return 0;
}
