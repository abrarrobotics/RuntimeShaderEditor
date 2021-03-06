#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/select.h>
#endif
# ifdef OS_ANDROID
#  include <GLES3/gl3.h>
#  include <GLES/glext.h>
# else
#  include <stdlib.h>
#  include <GL/glew.h>
#  include <GL/gl.h>
# endif
#include <string>
#include <vector>

class RSEMessage
{
private:
	std::string buffer;
public:
	void addData(char*data);
	bool isComplete();
	void clear();
	void prepareMessage(const char*input, char*output);
	const char* getMessage();
};

class RuntimeShaderEditor
{
    public:
        static RuntimeShaderEditor *GetInstance();
        void Init();
        void Update();
		void AddProgram(const char* desc, int program);
    private:
		std::string m_existingPrograms;
        static GLuint PatchShader(GLint program, const char*source, GLenum type);
		static int getCompilationError(char* buffer, int size, GLuint shaderHandler);
        static void getShaderSource(GLint program, char* buffer, GLint size);
        static RuntimeShaderEditor *s_instance;
        int m_port;
        int m_socket;
        int m_client;
		RSEMessage m_message;
#if defined (WIN32)
        static bool s_initialized;
        static WSAData s_wsaData;
#endif
        RuntimeShaderEditor();
        ~RuntimeShaderEditor();
};
