#include <TRV2.h>
#include <cstdio>

#include <TRGame.h>
//static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
//    glViewport(0, 0, width, height);
//}    glfwInit();
//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
//
//auto _window = glfwCreateWindow(800, 600, "Crystal", nullptr, nullptr);
//if (!_window) {
//    fprintf(stderr, "Failed to create window\n");
//    glfwTerminate();
//    return 0;
//}
//
//glfwMakeContextCurrent(_window);
//if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
//    fprintf(stderr, "Failed to load glad!\n");
//    glfwTerminate();
//    return 0;
//}
//glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
//glEnable(GL_LINE_SMOOTH);
//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
//
//while (!glfwWindowShouldClose(_window)) {
//    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    //ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//    glfwSwapBuffers(_window);
//    glfwPollEvents();
//}



int main(int argc, char** argv)
{
    try {
        auto& game = TRGame::GetInstance();
        game.Initialize(argc, argv);
        game.Run();
    }
    catch (std::exception ex) {
        fprintf(stderr, "ERROR: %s", ex.what());
    }
    return 0;
}
