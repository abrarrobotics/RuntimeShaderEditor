

#include <RuntimeShaderEditor\RunetimeShaderEditor.hpp>
#if defined (WIN32)
# include <stdio.h>
# define RSE_LOG(...) printf(__VA_ARGS__); printf("\r\n")
#elif defined(OS_ANDROID)
# include <android/log.h>
# define RSE_LOG(...) __android_log_print(ANDROID_LOG_INFO, "RuntimeShaderEditor", __VA_ARGS__)
#endif


RuntimeShaderEditor* RuntimeShaderEditor::s_instance = nullptr;
bool RuntimeShaderEditor::s_initialized = false;
WSAData RuntimeShaderEditor::s_wsaData;

RuntimeShaderEditor::RuntimeShaderEditor()
  : m_port(1111)
  , m_socket(0)
  , m_client(0)
{

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
    if(bind(m_socket, (sockaddr*)&sockaddr1, sizeof sockaddr1) == SOCKET_ERROR)
    {
        RSE_LOG("Error binding socket");
        return;
    }
    if(listen(m_socket,SOMAXCONN) == SOCKET_ERROR)
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
    char vs[1024];
    char fs[1024];
    glGetAttachedShaders(program, 3, &shadersCount, shaderNames);
    for(int i = 0; i < shadersCount; i++)
    {
        int len = 0;
        GLint typeAux = 0;
        glGetShaderiv(shaderNames[i], GL_SHADER_TYPE, &typeAux);
        glGetShaderSource(shaderNames[i], 1024, &len, typeAux == GL_VERTEX_SHADER ? vs : fs);
    }
    const char* format = "VERTEX:\r\n%sFRAGMENT:%s\r\n";
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

    int no_events = select(m_socket + 1, &readset, NULL, NULL, &tval);

    if(no_events == 0)
    {
        return;
    }

    // check for incomming connections
    if(FD_ISSET(m_socket, &readset))
    {
        m_client = accept(m_socket, NULL, NULL);
        RSE_LOG("New connection received");
    }

    if(FD_ISSET(m_client, &readset))
    {
        char buffer[2048];
        int len = recv(m_client, buffer, 2048-1, 0);
        if(len == -1)
        {
            RSE_LOG("Connection lost!");
            m_client = 0;
        }
        else
        {
            buffer[len] = 0x00;
            //RSE_LOG("New command received: %s", buffer);
            if(strstr(buffer, "getShaderSource"))
            {
                int program = 0;
                sscanf(buffer, "getShaderSource(%i)\r\n", &program);
                RSE_LOG("Se ha solicitado el codigo del shader: %i", program);
                getShaderSource(program, buffer, 2048);
                send(m_client, buffer, strlen(buffer), 0);
            }
            else if(strstr(buffer, "patchShader"))
            {
                int program = 0;
                int shaderType = 0;
                sscanf(buffer, "patchShader(%i,%i)", &program, &shaderType);
                char* aux = strstr(buffer, ")");
                if(shaderType == 0)
                {
                    shaderType = GL_VERTEX_SHADER;
                }
                else
                {
                    shaderType = GL_FRAGMENT_SHADER;
                }
                PatchShader(program, &aux[1], shaderType);
                send(m_client, "done\r\n", 6, 0);
            }
        }
    }
}