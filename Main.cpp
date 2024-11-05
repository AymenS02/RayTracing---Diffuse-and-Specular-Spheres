
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_m.h"
#define TINYOBJLOADER_IMPLEMENTATION  
#include "tiny_obj_loader.h"
#include <string>
#include <fstream>

#include <iostream>

static unsigned int ss_id = 0;
void dump_framebuffer_to_ppm(std::string prefix, unsigned int width, unsigned int height) {
    int pixelChannel = 3;
    int totalPixelSize = pixelChannel * width * height * sizeof(GLubyte);
    GLubyte* pixels = new GLubyte[totalPixelSize];
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    std::string file_name = prefix + std::to_string(ss_id) + ".ppm";
    std::ofstream fout(file_name);
    fout << "P3\n" << width << " " << height << "\n" << 255 << std::endl;
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            size_t cur = pixelChannel * ((height - i - 1) * width + j);
            fout << (int)pixels[cur] << " " << (int)pixels[cur + 1] << " " << (int)pixels[cur + 2] << " ";
        }
        fout << std::endl;
    }
    ss_id++;
    delete[] pixels;
    fout.flush();
    fout.close();
}

//key board control
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window)
{
    //press escape to exit
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //press p to capture screen
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        std::cout << "Capture Window " << ss_id << std::endl;
        int buffer_width, buffer_height;
        glfwGetFramebufferSize(window, &buffer_width, &buffer_height);
        dump_framebuffer_to_ppm("Assignment0-ss", buffer_width, buffer_height);
    }
}

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "A2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    //wireframe mode
    // -----------------------------
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 


    // build and compile our shader zprogram
    // ------------------------------------

    Shader ourShader("default.vert", "default.frag");

    std::string obj_path = "data/faces/";

    std::string line_of_weights;

    std::string weight_path = "data/weights/11.weights";

    std::fstream weight_file(weight_path, std::ios::in);

    std::string base_loader = "data/faces/base.obj";
    std::vector<glm::vec3> shape_base;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    // tinyobj load obj
    std::string warn, err;
    bool bTriangulate = true;
    bool bSuc = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, base_loader.c_str(), nullptr, bTriangulate);
    if (!bSuc)
    {
        std::cout << "tinyobj error: " << err.c_str() << std::endl;
        return -1;
    }

    std::vector<tinyobj::real_t> base_v = attrib.vertices;

    std::vector<tinyobj::real_t> final_vertices = base_v;
    

    if (weight_file.is_open()) {

        int j = 0;
        while (std::getline(weight_file, line_of_weights)) {
            std::string file_name = "data/faces/" + std::to_string(j++) + ".obj";

            tinyobj::attrib_t attrib_n;
            std::vector<tinyobj::shape_t> shapes_n;
            std::vector<tinyobj::material_t> materials_n;

            // tinyobj load obj
            std::string warn, err;
            bool bTriangulate = true;
            bool bSuc = tinyobj::LoadObj(&attrib_n, &shapes_n, &materials_n, &warn, &err, file_name.c_str(), nullptr, bTriangulate);
            if (!bSuc)
            {
                std::cout << "tinyobj error: " << err.c_str() << std::endl;
                return -1;
            }
            
            std::vector<tinyobj::real_t> result_vertices = attrib_n.vertices;


            //assert(result_vertices.attrib.vertices.size() == result_vertices.attrib.vertices.size());
            for (unsigned i = 0; i < result_vertices.size(); ++i)
            {
                final_vertices[i] += std::stof(line_of_weights) * (result_vertices[i] - base_v[i]);
            }
            
      
        }

    }
  


 


    std::vector<float> vbuffer;
    std::vector<float> nbuffer;
    for (const auto& shape : shapes) {
        for (auto face : shape.mesh.indices)
        {
            int vid = face.vertex_index;
            int nid = face.normal_index;
            //fill in vertex buffer with vertex positions
            vbuffer.push_back(final_vertices[vid * 3]);//vertex vid’s x
            vbuffer.push_back(final_vertices[vid * 3 + 1]);//vertex vid’s y
            vbuffer.push_back(final_vertices[vid * 3 + 2]);//vertex vid’s z
            //fill in normal buffer with normal directions
            nbuffer.push_back(attrib.normals[nid * 3]);
            nbuffer.push_back(attrib.normals[nid * 3 + 1]);
            nbuffer.push_back(attrib.normals[nid * 3 + 2]);
        }
    }

    unsigned int vertexVBO, normalVBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vertexVBO);
    glGenBuffers(1, &normalVBO);


    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, vbuffer.size() * sizeof(float), &vbuffer[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, nbuffer.size() * sizeof(float), &nbuffer[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);



    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.3f, 0.4f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        glEnable(GL_CULL_FACE);  // Enable back-face culling
        glCullFace(GL_BACK);     // Specify which face to cull

        // activate shader
        ourShader.use();

        // create transformations
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        ourShader.setMat4("model", model);

        view = glm::lookAt(glm::vec3(20, 50, 200), glm::vec3(0, 90, 0), glm::vec3(0, 1, 0)); // Camera is at (0,0,10), in World Space
        ourShader.setMat4("view", view);

        projection = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 1000.0f); // Camera is at (0,0,10), in World Space
        ourShader.setMat4("projection", projection);

        // render boxes
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vbuffer.size() / 3);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &vertexVBO);
    glDeleteBuffers(1, &normalVBO);


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}