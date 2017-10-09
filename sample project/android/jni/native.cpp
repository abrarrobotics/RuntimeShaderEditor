#include "native.hpp"
#include <Scene.hpp>
#include <GLES3/gl3.h>
#include <GLES/glext.h>


JNIEXPORT 
void JNICALL Java_com_example_glbasicframework_Native_init(JNIEnv *env, jclass jclass1)
{
    Scene::init();
}


JNIEXPORT 
void JNICALL Java_com_example_glbasicframework_Native_resize(JNIEnv *env, jclass class1, jint width, jint height)
{
    glViewport(0, 0, width, height);
}

JNIEXPORT 
void JNICALL Java_com_example_glbasicframework_Native_update(JNIEnv *env, jclass jclass1)
{
    Scene::update(0.05);
    Scene::render();
}