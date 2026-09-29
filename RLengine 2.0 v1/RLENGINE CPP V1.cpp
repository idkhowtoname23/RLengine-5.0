#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <format>
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

void GLAPIENTRY glDebugOutput(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei,
    const char* message,
    const void*) {
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::string_view srcStr = "Other";
    switch (source) {
    case GL_DEBUG_SOURCE_API:             srcStr = "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   srcStr = "Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: srcStr = "Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     srcStr = "Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     srcStr = "Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           srcStr = "Other"; break;
    }

    std::string_view typeStr = "Other";
    switch (type) {
    case GL_DEBUG_TYPE_ERROR:               typeStr = "Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeStr = "Deprecated"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeStr = "Undefined Behavior"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         typeStr = "Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         typeStr = "Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              typeStr = "Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          typeStr = "Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           typeStr = "Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               typeStr = "Other"; break;
    }

    std::string_view sevStr = "Notification";
    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:         sevStr = "HIGH"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       sevStr = "MEDIUM"; break;
    case GL_DEBUG_SEVERITY_LOW:          sevStr = "LOW"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: sevStr = "NOTIFICATION"; break;
    }

    std::cerr << std::format("[GL Debug] [{}] [{}] [{}] (ID: {}): {}\n",
        sevStr, srcStr, typeStr, id, message);
}

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

[[nodiscard]] std::pair<std::vector<Vertex>, std::vector<uint32_t>> generateCubeData() {
    std::vector<Vertex> vertices = {
        { .position = { -0.5f, -0.5f,  0.5f }, .normal = { 0.0f,  0.0f,  1.0f }, .uv = { 0.0f, 0.0f } },
        { .position = { 0.5f, -0.5f,  0.5f }, .normal = { 0.0f,  0.0f,  1.0f }, .uv = { 1.0f, 0.0f } },
        { .position = { 0.5f,  0.5f,  0.5f }, .normal = { 0.0f,  0.0f,  1.0f }, .uv = { 1.0f, 1.0f } },
        { .position = { -0.5f,  0.5f,  0.5f }, .normal = { 0.0f,  0.0f,  1.0f }, .uv = { 0.0f, 1.0f } },

        { .position = { 0.5f, -0.5f, -0.5f }, .normal = { 0.0f,  0.0f, -1.0f }, .uv = { 0.0f, 0.0f } },
        { .position = { -0.5f, -0.5f, -0.5f }, .normal = { 0.0f,  0.0f, -1.0f }, .uv = { 1.0f, 0.0f } },
        { .position = { -0.5f,  0.5f, -0.5f }, .normal = { 0.0f,  0.0f, -1.0f }, .uv = { 1.0f, 1.0f } },
        { .position = { 0.5f,  0.5f, -0.5f }, .normal = { 0.0f,  0.0f, -1.0f }, .uv = { 0.0f, 1.0f } },

        { .position = { -0.5f, -0.5f, -0.5f }, .normal = { -1.0f,  0.0f,  0.0f }, .uv = { 0.0f, 0.0f } },
        { .position = { -0.5f, -0.5f,  0.5f }, .normal = { -1.0f,  0.0f,  0.0f }, .uv = { 1.0f, 0.0f } },
        { .position = { -0.5f,  0.5f,  0.5f }, .normal = { -1.0f,  0.0f,  0.0f }, .uv = { 1.0f, 1.0f } },
        { .position = { -0.5f,  0.5f, -0.5f }, .normal = { -1.0f,  0.0f,  0.0f }, .uv = { 0.0f, 1.0f } },

        { .position = { 0.5f, -0.5f,  0.5f }, .normal = { 1.0f,  0.0f,  0.0f }, .uv = { 0.0f, 0.0f } },
        { .position = { 0.5f, -0.5f, -0.5f }, .normal = { 1.0f,  0.0f,  0.0f }, .uv = { 1.0f, 0.0f } },
        { .position = { 0.5f,  0.5f, -0.5f }, .normal = { 1.0f,  0.0f,  0.0f }, .uv = { 1.0f, 1.0f } },
        { .position = { 0.5f,  0.5f,  0.5f }, .normal = { 1.0f,  0.0f,  0.0f }, .uv = { 0.0f, 1.0f } },

        { .position = { -0.5f,  0.5f,  0.5f }, .normal = { 0.0f,  1.0f,  0.0f }, .uv = { 0.0f, 0.0f } },
        { .position = { 0.5f,  0.5f,  0.5f }, .normal = { 0.0f,  1.0f,  0.0f }, .uv = { 1.0f, 0.0f } },
        { .position = { 0.5f,  0.5f, -0.5f }, .normal = { 0.0f,  1.0f,  0.0f }, .uv = { 1.0f, 1.0f } },
        { .position = { -0.5f,  0.5f, -0.5f }, .normal = { 0.0f,  1.0f,  0.0f }, .uv = { 0.0f, 1.0f } },

        { .position = { -0.5f, -0.5f, -0.5f }, .normal = { 0.0f, -1.0f,  0.0f }, .uv = { 0.0f, 0.0f } },
        { .position = { 0.5f, -0.5f, -0.5f }, .normal = { 0.0f, -1.0f,  0.0f }, .uv = { 1.0f, 0.0f } },
        { .position = { 0.5f, -0.5f,  0.5f }, .normal = { 0.0f, -1.0f,  0.0f }, .uv = { 1.0f, 1.0f } },
        { .position = { -0.5f, -0.5f,  0.5f }, .normal = { 0.0f, -1.0f,  0.0f }, .uv = { 0.0f, 1.0f } }
    };

    std::vector<uint32_t> indices;
    indices.reserve(36);
    for (uint32_t face = 0; face < 6; ++face) {
        uint32_t base = face * 4;
        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        indices.push_back(base + 2);
        indices.push_back(base + 3);
        indices.push_back(base + 0);
    }

    return { std::move(vertices), std::move(indices) };
}

class Shader {
public:
    Shader(std::string_view vertexSrc, std::string_view fragmentSrc) {
        GLuint vertShader = compileStage(GL_VERTEX_SHADER, vertexSrc);
        GLuint fragShader = compileStage(GL_FRAGMENT_SHADER, fragmentSrc);

        m_program = glCreateProgram();
        glAttachShader(m_program, vertShader);
        glAttachShader(m_program, fragShader);
        glLinkProgram(m_program);

        GLint success = 0;
        glGetProgramiv(m_program, GL_LINK_STATUS, &success);
        if (!success) {
            GLint length = 0;
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);
            std::string log(length, '\0');
            glGetProgramInfoLog(m_program, length, nullptr, log.data());
            std::cerr << std::format("[Shader Program Link Error]:\n{}\n", log);
        }

        glDeleteShader(vertShader);
        glDeleteShader(fragShader);
    }

    ~Shader() {
        if (m_program) {
            glDeleteProgram(m_program);
        }
    }

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept
        : m_program(std::exchange(other.m_program, 0)),
        m_uniformCache(std::move(other.m_uniformCache)) {
    }

    Shader& operator=(Shader&& other) noexcept {
        if (this != &other) {
            if (m_program) glDeleteProgram(m_program);
            m_program = std::exchange(other.m_program, 0);
            m_uniformCache = std::move(other.m_uniformCache);
        }
        return *this;
    }

    void bind() const noexcept {
        glUseProgram(m_program);
    }

    void unbind() const noexcept {
        glUseProgram(0);
    }

    void setMat4(std::string_view name, const glm::mat4& mat) const {
        glProgramUniformMatrix4fv(m_program, getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void setMat3(std::string_view name, const glm::mat3& mat) const {
        glProgramUniformMatrix3fv(m_program, getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void setVec3(std::string_view name, const glm::vec3& vec) const {
        glProgramUniform3fv(m_program, getUniformLocation(name), 1, glm::value_ptr(vec));
    }

    void setFloat(std::string_view name, float val) const {
        glProgramUniform1f(m_program, getUniformLocation(name), val);
    }

private:
    [[nodiscard]] static GLuint compileStage(GLenum stageType, std::string_view source) {
        GLuint shader = glCreateShader(stageType);
        const char* srcPtr = source.data();
        GLint length = static_cast<GLint>(source.size());
        glShaderSource(shader, 1, &srcPtr, &length);
        glCompileShader(shader);

        GLint success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLint logLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
            std::string log(logLength, '\0');
            glGetShaderInfoLog(shader, logLength, nullptr, log.data());
            std::cerr << std::format("[Shader Stage Compile Error] ({}):\n{}\n",
                (stageType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT"), log);
        }
        return shader;
    }

    [[nodiscard]] GLint getUniformLocation(std::string_view name) const {
        std::string nameStr(name);
        if (auto it = m_uniformCache.find(nameStr); it != m_uniformCache.end()) {
            return it->second;
        }
        GLint location = glGetUniformLocation(m_program, nameStr.c_str());
        m_uniformCache[std::move(nameStr)] = location;
        return location;
    }

    GLuint m_program = 0;
    mutable std::unordered_map<std::string, GLint> m_uniformCache;
};

enum class CameraDirection : uint8_t {
    Forward,
    Backward,
    Left,
    Right,
    Up,
    Down
};

struct CameraConfig {
    glm::vec3 position{ 0.0f, 1.5f, 4.0f };
    glm::vec3 worldUp{ 0.0f, 1.0f, 0.0f };
    float yaw = -90.0f;
    float pitch = -15.0f;
    float speed = 3.5f;
    float sensitivity = 0.1f;
    float fov = 45.0f;
};

class Camera {
public:
    explicit Camera(const CameraConfig& config = {})
        : m_position(config.position),
        m_worldUp(config.worldUp),
        m_yaw(config.yaw),
        m_pitch(config.pitch),
        m_speed(config.speed),
        m_sensitivity(config.sensitivity),
        m_fov(config.fov) {
        updateVectors();
    }

    [[nodiscard]] glm::mat4 getViewMatrix() const noexcept {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }

    [[nodiscard]] glm::mat4 getProjectionMatrix(float aspect, float nearClip = 0.1f, float farClip = 100.0f) const noexcept {
        return glm::perspective(glm::radians(m_fov), aspect, nearClip, farClip);
    }

    [[nodiscard]] glm::vec3 getPosition() const noexcept { return m_position; }
    [[nodiscard]] float getFov() const noexcept { return m_fov; }

    void processKeyboard(CameraDirection direction, float dt) noexcept {
        const float velocity = m_speed * dt;
        switch (direction) {
        case CameraDirection::Forward:  m_position += m_front * velocity; break;
        case CameraDirection::Backward: m_position -= m_front * velocity; break;
        case CameraDirection::Left:     m_position -= m_right * velocity; break;
        case CameraDirection::Right:    m_position += m_right * velocity; break;
        case CameraDirection::Up:       m_position += m_worldUp * velocity; break;
        case CameraDirection::Down:     m_position -= m_worldUp * velocity; break;
        }
    }

    void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true) noexcept {
        xOffset *= m_sensitivity;
        yOffset *= m_sensitivity;

        m_yaw += xOffset;
        m_pitch += yOffset;

        if (constrainPitch) {
            m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
        }

        updateVectors();
    }

    void processScroll(float yOffset) noexcept {
        m_fov = std::clamp(m_fov - yOffset, 1.0f, 90.0f);
    }

private:
    void updateVectors() noexcept {
        glm::vec3 frontVec;
        frontVec.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
        frontVec.y = std::sin(glm::radians(m_pitch));
        frontVec.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));

        m_front = glm::normalize(frontVec);
        m_right = glm::normalize(glm::cross(m_front, m_worldUp));
        m_up = glm::normalize(glm::cross(m_right, m_front));
    }

    glm::vec3 m_position;
    glm::vec3 m_front{ 0.0f, 0.0f, -1.0f };
    glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };
    glm::vec3 m_right{ 1.0f, 0.0f, 0.0f };
    glm::vec3 m_worldUp{ 0.0f, 1.0f, 0.0f };

    float m_yaw;
    float m_pitch;
    float m_speed;
    float m_sensitivity;
    float m_fov;
};

class Mesh {
public:
    Mesh(std::span<const Vertex> vertices, std::span<const uint32_t> indices)
        : m_indexCount(static_cast<GLsizei>(indices.size())) {

        glCreateVertexArrays(1, &m_vao);
        glCreateBuffers(1, &m_vbo);
        glCreateBuffers(1, &m_ebo);

        glNamedBufferStorage(m_vbo, static_cast<GLsizeiptr>(vertices.size_bytes()), vertices.data(), 0);
        glNamedBufferStorage(m_ebo, static_cast<GLsizeiptr>(indices.size_bytes()), indices.data(), 0);

        constexpr GLuint bindingIndex = 0;
        glVertexArrayVertexBuffer(m_vao, bindingIndex, m_vbo, 0, sizeof(Vertex));
        glVertexArrayElementBuffer(m_vao, m_ebo);

        glEnableVertexArrayAttrib(m_vao, 0);
        glVertexArrayAttribFormat(m_vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
        glVertexArrayAttribBinding(m_vao, 0, bindingIndex);

        glEnableVertexArrayAttrib(m_vao, 1);
        glVertexArrayAttribFormat(m_vao, 1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
        glVertexArrayAttribBinding(m_vao, 1, bindingIndex);

        glEnableVertexArrayAttrib(m_vao, 2);
        glVertexArrayAttribFormat(m_vao, 2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv));
        glVertexArrayAttribBinding(m_vao, 2, bindingIndex);
    }

    ~Mesh() {
        cleanup();
    }

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept
        : m_vao(std::exchange(other.m_vao, 0)),
        m_vbo(std::exchange(other.m_vbo, 0)),
        m_ebo(std::exchange(other.m_ebo, 0)),
        m_indexCount(std::exchange(other.m_indexCount, 0)) {
    }

    Mesh& operator=(Mesh&& other) noexcept {
        if (this != &other) {
            cleanup();
            m_vao = std::exchange(other.m_vao, 0);
            m_vbo = std::exchange(other.m_vbo, 0);
            m_ebo = std::exchange(other.m_ebo, 0);
            m_indexCount = std::exchange(other.m_indexCount, 0);
        }
        return *this;
    }

    void draw() const noexcept {
        glBindVertexArray(m_vao);
        glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

private:
    void cleanup() noexcept {
        if (m_ebo) { glDeleteBuffers(1, &m_ebo); m_ebo = 0; }
        if (m_vbo) { glDeleteBuffers(1, &m_vbo); m_vbo = 0; }
        if (m_vao) { glDeleteVertexArrays(1, &m_vao); m_vao = 0; }
    }

    GLuint m_vao = 0;
    GLuint m_vbo = 0;
    GLuint m_ebo = 0;
    GLsizei m_indexCount = 0;
};

class Renderer {
public:
    Renderer() {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        glEnable(GL_MULTISAMPLE);
    }

    void setViewport(int width, int height) const noexcept {
        glViewport(0, 0, width, height);
    }

    void clear(const glm::vec4& color = { 0.05f, 0.06f, 0.08f, 1.0f }) const noexcept {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void render(const Mesh& mesh, const Shader& shader) const noexcept {
        shader.bind();
        mesh.draw();
        shader.unbind();
    }
};

constexpr std::string_view lightingVertexShaderSource = R"glsl(
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;

layout(location = 0) out vec3 v_FragPos;
layout(location = 1) out vec3 v_Normal;
layout(location = 2) out vec2 v_UV;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat3 u_NormalMatrix;

void main() {
    vec4 worldPos = u_Model * vec4(a_Position, 1.0);
    v_FragPos = worldPos.xyz;
    v_Normal = normalize(u_NormalMatrix * a_Normal);
    v_UV = a_UV;

    gl_Position = u_Projection * u_View * worldPos;
}
)glsl";

constexpr std::string_view lightingFragmentShaderSource = R"glsl(
#version 450 core

layout(location = 0) in vec3 v_FragPos;
layout(location = 1) in vec3 v_Normal;
layout(location = 2) in vec2 v_UV;

layout(location = 0) out vec4 FragColor;

uniform vec3 u_LightPos;
uniform vec3 u_LightColor;
uniform vec3 u_ViewPos;
uniform vec3 u_ObjectColor;

void main() {
    float ambientStrength = 0.12;
    vec3 ambient = ambientStrength * u_LightColor;

    vec3 norm = normalize(v_Normal);
    vec3 lightDir = normalize(u_LightPos - v_FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;

    vec3 viewDir = normalize(u_ViewPos - v_FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    vec3 specular = 0.6 * spec * u_LightColor;

    float distance = length(u_LightPos - v_FragPos);
    float attenuation = 1.0 / (1.0 + 0.14 * distance + 0.07 * (distance * distance));

    vec3 result = (ambient + (diffuse + specular) * attenuation) * u_ObjectColor;
    FragColor = vec4(result, 1.0);
}
)glsl";

constexpr std::string_view unlitVertexShaderSource = R"glsl(
#version 450 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {
    gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}
)glsl";

constexpr std::string_view unlitFragmentShaderSource = R"glsl(
#version 450 core

layout(location = 0) out vec4 FragColor;
uniform vec3 u_Color;

void main() {
    FragColor = vec4(u_Color, 1.0);
}
)glsl";

class Engine {
public:
    Engine(int width, int height, std::string_view title)
        : m_width(width), m_height(height) {
        initWindow(width, height, title);
        initOpenGL();
    }

    ~Engine() {
        if (m_window) {
            glfwDestroyWindow(m_window);
        }
        glfwTerminate();
    }

    void run() {
        auto [vertices, indices] = generateCubeData();
        Mesh cubeMesh(vertices, indices);

        Shader lightingShader(lightingVertexShaderSource, lightingFragmentShaderSource);
        Shader unlitShader(unlitVertexShaderSource, unlitFragmentShaderSource);

        Renderer renderer;

        float lastFrameTime = static_cast<float>(glfwGetTime());

        while (!glfwWindowShouldClose(m_window)) {
            const float currentFrameTime = static_cast<float>(glfwGetTime());
            const float deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;

            processInput(deltaTime);

            if (m_width == 0 || m_height == 0) {
                glfwPollEvents();
                continue;
            }

            renderer.clear({ 0.08f, 0.09f, 0.12f, 1.0f });

            const float aspect = static_cast<float>(m_width) / static_cast<float>(m_height);
            const glm::mat4 view = m_camera.getViewMatrix();
            const glm::mat4 projection = m_camera.getProjectionMatrix(aspect);

            const float time = currentFrameTime * 1.2f;
            const glm::vec3 lightPos{ std::sin(time) * 2.5f, 1.0f + std::sin(time * 0.5f) * 0.5f, std::cos(time) * 2.5f };
            const glm::vec3 lightColor{ 1.0f, 0.95f, 0.85f };

            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::rotate(model, currentFrameTime * 0.7f, glm::vec3(0.5f, 1.0f, 0.0f));
                const glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));

                lightingShader.setMat4("u_Model", model);
                lightingShader.setMat4("u_View", view);
                lightingShader.setMat4("u_Projection", projection);
                lightingShader.setMat3("u_NormalMatrix", normalMatrix);

                lightingShader.setVec3("u_LightPos", lightPos);
                lightingShader.setVec3("u_LightColor", lightColor);
                lightingShader.setVec3("u_ViewPos", m_camera.getPosition());
                lightingShader.setVec3("u_ObjectColor", glm::vec3(0.2f, 0.7f, 0.95f));

                renderer.render(cubeMesh, lightingShader);
            }

            {
                glm::mat4 floorModel = glm::mat4(1.0f);
                floorModel = glm::translate(floorModel, glm::vec3(0.0f, -1.2f, 0.0f));
                floorModel = glm::scale(floorModel, glm::vec3(8.0f, 0.2f, 8.0f));
                const glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(floorModel)));

                lightingShader.setMat4("u_Model", floorModel);
                lightingShader.setMat3("u_NormalMatrix", normalMatrix);
                lightingShader.setVec3("u_ObjectColor", glm::vec3(0.35f, 0.38f, 0.42f));

                renderer.render(cubeMesh, lightingShader);
            }

            {
                glm::mat4 lightModel = glm::mat4(1.0f);
                lightModel = glm::translate(lightModel, lightPos);
                lightModel = glm::scale(lightModel, glm::vec3(0.18f));

                unlitShader.setMat4("u_Model", lightModel);
                unlitShader.setMat4("u_View", view);
                unlitShader.setMat4("u_Projection", projection);
                unlitShader.setVec3("u_Color", lightColor);

                renderer.render(cubeMesh, unlitShader);
            }

            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
    }

private:
    void initWindow(int width, int height, std::string_view title) {
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        m_window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);
        if (!m_window) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(m_window);
        glfwSwapInterval(1);

        glfwSetWindowUserPointer(m_window, this);
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* w, int wWidth, int wHeight) {
            auto* engine = static_cast<Engine*>(glfwGetWindowUserPointer(w));
            engine->m_width = wWidth;
            engine->m_height = wHeight;
            glViewport(0, 0, wWidth, wHeight);
            });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* w, double xpos, double ypos) {
            auto* engine = static_cast<Engine*>(glfwGetWindowUserPointer(w));
            if (engine->m_firstMouse) {
                engine->m_lastX = static_cast<float>(xpos);
                engine->m_lastY = static_cast<float>(ypos);
                engine->m_firstMouse = false;
            }

            const float xOffset = static_cast<float>(xpos) - engine->m_lastX;
            const float yOffset = engine->m_lastY - static_cast<float>(ypos);

            engine->m_lastX = static_cast<float>(xpos);
            engine->m_lastY = static_cast<float>(ypos);

            engine->m_camera.processMouseMovement(xOffset, yOffset);
            });

        glfwSetScrollCallback(m_window, [](GLFWwindow* w, double, double yoffset) {
            auto* engine = static_cast<Engine*>(glfwGetWindowUserPointer(w));
            engine->m_camera.processScroll(static_cast<float>(yoffset));
            });
    }

    void initOpenGL() {
        if (!gladLoadGL(glfwGetProcAddress)) {
            throw std::runtime_error("Failed to initialize GLAD");
        }

        std::cout << std::format("=== Modern OpenGL Engine Initialized ===\n"
            "Vendor:   {}\n"
            "Renderer: {}\n"
            "Version:  {}\n"
            "GLSL:     {}\n"
            "========================================\n",
            reinterpret_cast<const char*>(glGetString(GL_VENDOR)),
            reinterpret_cast<const char*>(glGetString(GL_RENDERER)),
            reinterpret_cast<const char*>(glGetString(GL_VERSION)),
            reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION)));

        GLint flags = 0;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(glDebugOutput, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }
    }

    void processInput(float dt) {
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(m_window, true);
        }

        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
            m_camera.processKeyboard(CameraDirection::Forward, dt);
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
            m_camera.processKeyboard(CameraDirection::Backward, dt);
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
            m_camera.processKeyboard(CameraDirection::Left, dt);
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
            m_camera.processKeyboard(CameraDirection::Right, dt);
        if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
            m_camera.processKeyboard(CameraDirection::Up, dt);
        if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            m_camera.processKeyboard(CameraDirection::Down, dt);
    }

    GLFWwindow* m_window = nullptr;
    int m_width;
    int m_height;

    Camera m_camera{ CameraConfig{
        .position = { 0.0f, 1.5f, 4.5f },
        .yaw = -90.0f,
        .pitch = -10.0f,
        .speed = 3.5f,
        .sensitivity = 0.08f,
        .fov = 45.0f
    } };

    float m_lastX = 640.0f;
    float m_lastY = 360.0f;
    bool m_firstMouse = true;
};

int main() {
    try {
        Engine engine(1280, 720, "C++23 Modern OpenGL 4.5 DSA Engine");
        engine.run();
    }
    catch (const std::exception& e) {
        std::cerr << std::format("[FATAL ERROR]: {}\n", e.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}