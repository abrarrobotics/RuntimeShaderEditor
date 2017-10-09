
#include <WindowManager/WindowManager.hpp>
#include <RuntimeShaderEditor\RunetimeShaderEditor.hpp>
#include <Scene.hpp>

int main(int argc, char*argv[])
{
    RuntimeShaderEditor::GetInstance()->Init();
    WindowManager::createWindow("3DBasicFramework", WindowManager::SMALL);
    glewInit();
    Scene::init();
    while(!WindowManager::isClosed)
    {
        RuntimeShaderEditor::GetInstance()->Update();
        WindowManager::processEvents();
        Scene::update(WindowManager::getElapsedTime());
        Scene::render();
        WindowManager::swapBuffers();
        SDL_Delay(5);
    }
    Scene::cleanUp();
    return 0;
}
