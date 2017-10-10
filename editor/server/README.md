###### How to implement

The file "RuntimeShaderEditor/RuntimeShaderEditor.cpp" must be compiled as part of
your project.

You need to invoke: ``RuntimeShaderEditor::GetInstance()->Init();
``
from your program initialization code, and invoke: `` RuntimmeShaderEditor::GetInstance()->Update();
`` from your main loop.
