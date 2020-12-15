#pragma once


#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include "TriangleMesh.h"

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Geometry : public TriangleMesh
{
public:

	Geometry();
	~Geometry();

	void GenerateQuad();

	void GenerateCube();

	void GenerateSphere(float a_radius, unsigned int a_nSlices, unsigned int a_nStacks);

	void GenerateTeaPot(int a_grid, const glm::mat4& a_lidTransform);

	void GenerateTorus(GLfloat a_outerRadius, GLfloat a_innerRadius, GLuint a_nsides, GLuint a_nRings);

protected:

private:

	void GeneratePatches(std::vector<GLfloat>& a_p, std::vector<GLfloat>& a_n,
						 std::vector<GLfloat>& a_tc, std::vector<GLuint>& a_el,
						 int a_grid);

	void BuildPatchReflect(int a_patchNum, std::vector<GLfloat>& a_b,
						   std::vector<GLfloat>& a_db, std::vector<GLfloat>& a_v,
						   std::vector<GLfloat>& a_n, std::vector<GLfloat>& a_tc,
		                   std::vector<GLuint>& a_el, int& a_index, int& a_elIndex,
					       int& a_tcIndex, int a_grid, bool a_reflectX, bool a_reflectY);

	void BuildPatch(glm::vec3 patch[][4],
		std::vector<GLfloat>& a_B, std::vector<GLfloat>& a_dB,
		std::vector<GLfloat>& a_v, std::vector<GLfloat>& a_n,
		std::vector<GLfloat>& a_tc, std::vector<GLuint>& a_el,
		int& a_index, int& a_elIndex, int& a_tcIndex, int a_grid, glm::mat3 a_reflect,
		bool a_invertNormal);

	void GetPatch(int a_patchNum, glm::vec3 a_patch[][4], bool a_reverseV);

	void ComputerBasesFunctions(std::vector<GLfloat>& a_b, std::vector<GLfloat>&a_db,
								int a_grid);

	glm::vec3 Evaluate(int a_gridU, int a_gridV, std::vector<GLfloat>& a_b,
					   glm::vec3 a_patch[][4]);

	glm::vec3 EvaluateNormal(int a_gridU, int a_gridV, std::vector<GLfloat>& a_b,
					         std::vector<GLfloat>& a_db, glm::vec3 a_patch[][4]);

	void MoveLid(int a_grid, std::vector<GLfloat>& a_p, const glm::mat4& a_lidTransform);

};