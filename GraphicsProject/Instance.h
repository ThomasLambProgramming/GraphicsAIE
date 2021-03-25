#pragma once
#include <glm/glm.hpp>
#include <string>
class Camera;
class Scene;

namespace aie
{
	class OBJMesh;
	class ShaderProgram;
}
class Instance
{
public:
	Instance(glm::mat4 a_transform, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader);
	
	Instance(glm::vec3 a_position, glm::vec3 a_eulerAngles,
		glm::vec3 a_scale, aie::OBJMesh* a_mesh, aie::ShaderProgram* a_shader,
		std::string objName);

	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();

	glm::mat4& GetTransform() { return m_transform; }

	void SetTransform(glm::mat4 a_transform) { m_transform = a_transform; }
	void Draw(Scene* a_scene);

	static glm::mat4 MakeTransform(glm::vec3 a_position, glm::vec3 a_eulerAngles, glm::vec3 a_scale);

	std::string m_objectName;
	glm::vec3 m_rotation;
protected:
	glm::mat4 m_transform;
	aie::OBJMesh* m_mesh;
	aie::ShaderProgram* m_shader;
};

