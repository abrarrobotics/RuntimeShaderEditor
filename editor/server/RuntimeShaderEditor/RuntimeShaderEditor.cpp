

#include <RuntimeShaderEditor\RuntimeShaderEditor.hpp>
#if defined (WIN32)
# include <stdio.h>
# define RSE_LOG(...) printf(__VA_ARGS__); printf("\r\n")
#elif defined(OS_ANDROID)
# include <android/log.h>
# define RSE_LOG(...) __android_log_print(ANDROID_LOG_INFO, "RuntimeShaderEditor", __VA_ARGS__)
#endif
#include <stdio.h>


RuntimeShaderEditor* RuntimeShaderEditor::s_instance = nullptr;
#if defined (WIN32)
bool RuntimeShaderEditor::s_initialized = false;
WSAData RuntimeShaderEditor::s_wsaData;
#endif

#define SHADER_BUFFER_SIZE 20000

RuntimeShaderEditor::RuntimeShaderEditor()
  : m_port(12346)
  , m_socket(0)
  , m_client(0)
{
	m_message.clear();
}

RuntimeShaderEditor::~RuntimeShaderEditor()
{

}

RuntimeShaderEditor *RuntimeShaderEditor::GetInstance()
{
    if(s_instance == nullptr) 
    {
        s_instance = new RuntimeShaderEditor();
    }
    return s_instance;
}

void RuntimeShaderEditor::Init()
{
#if defined (WIN32)
    if(!s_initialized) {
        if (WSAStartup(MAKEWORD (2,2),&s_wsaData) != 0)
        {
            RSE_LOG("WSAStartup Error");
        }
        s_initialized = true;
    }
#endif
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_socket < 1)
    {
        RSE_LOG("Error Creating socket");
        return;
    }
    sockaddr_in sockaddr1;
    sockaddr1.sin_family = AF_INET;
    sockaddr1.sin_addr.s_addr = INADDR_ANY;
    sockaddr1.sin_port = htons(m_port);
    if(bind(m_socket, (sockaddr*)&sockaddr1, sizeof sockaddr1) == -1)
    {
        RSE_LOG("Error binding socket");
        return;
    }
    if(listen(m_socket,SOMAXCONN) == -1)
    {
        RSE_LOG("Error listening on port %i", m_port);
        return;
    }
}

void RuntimeShaderEditor::PatchShader(GLint program, const char*source, GLenum type)
{
    RSE_LOG("Patching program: %i, type: %s, with source: %s", program, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT", source);
    GLint shadersCount = 0;
    GLuint shaderNames[3];
    GLuint toBeReplaced = 0;
    glGetAttachedShaders(program, 3, &shadersCount, shaderNames);

    for(int i = 0; i < shadersCount; i++)
    {
        GLint typeAux = 0;
        glGetShaderiv(shaderNames[i], GL_SHADER_TYPE, &typeAux);
        if(typeAux == type)
        {
            toBeReplaced = shaderNames[i];
        }
    }
    GLint newShader = glCreateShader(type);
    glShaderSource(newShader, 1, (const GLchar **)&source, 0);
    glCompileShader(newShader);
    glDetachShader(program, toBeReplaced);
    glAttachShader(program, newShader);
    glLinkProgram(program);
}

void RuntimeShaderEditor::getShaderSource(GLint program, char* buffer, GLint size)
{
    GLint shadersCount = 0;
    GLuint shaderNames[3];
    GLuint toBeReplaced = 0;
    char vs[SHADER_BUFFER_SIZE];
    char fs[SHADER_BUFFER_SIZE];
	vs[0] = 0x00;
	fs[0] = 0x00;
    glGetAttachedShaders(program, 3, &shadersCount, shaderNames);
    for(int i = 0; i < shadersCount; i++)
    {
        int len = 0;
        GLint typeAux = 0;
        glGetShaderiv(shaderNames[i], GL_SHADER_TYPE, &typeAux);
        glGetShaderSource(shaderNames[i], SHADER_BUFFER_SIZE, &len, typeAux == GL_VERTEX_SHADER ? vs : fs);
    }
    const char* format = "%s[SEPARATOR]%s";
    sprintf(buffer, format, vs, fs);
}

void RuntimeShaderEditor::Update()
{
    timeval tval = {0, 100};
    fd_set readset;
    FD_ZERO(&readset);
    FD_SET(m_socket, &readset);
    if(m_client)
    {
        FD_SET(m_client, &readset);
    }

    int no_events = select(m_socket > m_client ? (m_socket + 1) : m_client + 1, &readset, NULL, NULL, &tval);

    if(no_events == 0)
    {
        return;
    }

    // check for incomming connections
    if(FD_ISSET(m_socket, &readset))
    {
        m_client = accept(m_socket, NULL, NULL);
		char rawMessage[20];
		m_message.prepareMessage("RSE-ACK\r\n", rawMessage);
        send(m_client, rawMessage,strlen(rawMessage), 0);
        RSE_LOG("New connection received");
    }

    if(FD_ISSET(m_client, &readset))
    {
        char buffer[SHADER_BUFFER_SIZE];
        int len = recv(m_client, buffer, SHADER_BUFFER_SIZE-1, 0);
        if(len < 1)
        {
            RSE_LOG("Connection lost!");
            m_client = 0;
        }
        else
        {
            buffer[len] = 0x00;
			m_message.addData(buffer);
			if (m_message.isComplete())
			{
				const char *msg = m_message.getMessage();
				RSE_LOG("New command received: %s, len:%i", msg, len);
				if (strstr(msg, "getShaderSource"))
				{
					int program = 0;
					sscanf(msg, "getShaderSource(%i)\r\n", &program);
					RSE_LOG("Se ha solicitado el codigo del shader: %i", program);
					getShaderSource(program, buffer, SHADER_BUFFER_SIZE);
					char rawMessage[SHADER_BUFFER_SIZE * 2];
					m_message.prepareMessage(buffer, rawMessage);
					send(m_client, rawMessage, strlen(rawMessage), 0);
				}
				else if (strstr(msg, "patchShader"))
				{
					int program = 0;
					int shaderType = 0;
					sscanf(msg, "patchShader(%i,%i)", &program, &shaderType);
					const char* aux = strstr(msg, ")");
					if (shaderType == 0)
					{
						shaderType = GL_VERTEX_SHADER;
					}
					else
					{
						shaderType = GL_FRAGMENT_SHADER;
					}
					PatchShader(program, &aux[1], shaderType);
					char rawMessage[10];
					m_message.prepareMessage("done\r\n", rawMessage);
					send(m_client, rawMessage, strlen(rawMessage), 0);
				}
				m_message.clear();
			}
        }
    }
}