#include <Drawables/PLYFormatHelper.hpp>


PLYFormatHelper::PLYFormatHelper() : format_len(0), addr_len(0) {
  memset(format, 0, sizeof format);
  memset(addresses, 0, sizeof addresses);
  memset(&vertex, 0, sizeof vertex);
  memset(&status, 0, sizeof status);
}

void PLYFormatHelper::addProperty(int type, int name) {
  if(format_len + 3 >= 500) return;
  format[format_len++] = '%';
  format[format_len++] = type == FLOAT ? 'f' : 'i';
  format[format_len++] = ' ';

  if(addr_len == 11) return;

       if(name == X) { addresses[addr_len++] = &vertex.x; status.position = status.position | X_COMPONENT; }
  else if(name == Y) { addresses[addr_len++] = &vertex.y; status.position = status.position | Y_COMPONENT; }
  else if(name == Z) { addresses[addr_len++] = &vertex.z; status.position = status.position | Z_COMPONENT; }
  else if(name == NX){ addresses[addr_len++] = &vertex.nx; status.normals = status.normals | X_COMPONENT; }
  else if(name == NY){ addresses[addr_len++] = &vertex.ny; status.normals = status.normals | Y_COMPONENT; }
  else if(name == NZ){ addresses[addr_len++] = &vertex.nz; status.normals = status.normals | Z_COMPONENT; }
  else if(name == R) { addresses[addr_len++] = &vertex.r; status.colors = status.colors | X_COMPONENT; }
  else if(name == G) { addresses[addr_len++] = &vertex.g; status.colors = status.colors | Y_COMPONENT; }
  else if(name == B) { addresses[addr_len++] = &vertex.b; status.colors = status.colors | Z_COMPONENT; }
  else if(name == U) { addresses[addr_len++] = &vertex.u; status.uvs = status.uvs | X_COMPONENT; }
  else if(name == V) { addresses[addr_len++] = &vertex.v; status.uvs = status.uvs | Y_COMPONENT; }
}
