#include <SpheresEngine/RenderEngine/RenderBackendSDLTesting.h>
#include <SpheresEngine/RenderEngine/CommonOpenGL/ShaderBackend.h>
#include <SpheresEngine/ResourceEngine/ResourceEngineTesting.h>

#include <gtest/gtest.h>

#include <iostream>

TEST(ShaderBackendTest, glmMatrixStructure) {
	glm::mat4 m1;

	for (size_t i = 0; i < 4; i++)
		for (size_t k = 0; k < 4; k ++)
			m1[i][k] = 0.0f;

	glm::vec4 v1( 1.0, 0.0, 0.0, 0.0);

	m1 = glm::mat4 ( 0.0, 0.0, 0.0, 1.0, /* this 1.0 is in colum 0 & row 3 !! */
			 0.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 0.0, 0.0 );

	// this is [column] [row] addressed (aka x/y), in constrast to regular matrix addressing
	EXPECT_EQ(1.0f, m1[0][3]);

	// is it possible to write back ?
	m1[0][3] = 2.0f;
	EXPECT_EQ(2.0f, m1[0][3]);

	// if the above assignment to m1 is [row][column] (classical matrix notation), we expect
	// res_vec to be zero
	auto res_vec = m1 * v1;
	EXPECT_EQ(2.0f, res_vec[3]);
}

TEST(ShaderBackendTest, loadCompileShader) {

	RenderBackendSDLTesting testingBackend;

	testingBackend.openDisplay();
	testingBackend.initRenderer();

	if (!testingBackend.isValid()) {
		std::cerr
				<< "Skipping OpenGL test because required features not supported by driver"
				<< std::endl;
		return;
	}

	ResourceEngineTesting reTest;

	auto fsSource =
			"uniform sampler2D tex;\
			in vec2 fragTexCoord;\
			out vec4 finalColor;\
			void main() { finalColor = texture(tex, fragTexCoord); }";

	reTest.addTestShader("fs", fsSource);

	auto vtxSource =
			"uniform mat4 projection;\
			uniform mat4 camera;\
			uniform mat4 model;\
			in vec3 vert;\
			in vec2 vertTexCoord;\
			out vec2 fragTexCoord;\
			void main(void) {\
			fragTexCoord = vertTexCoord;\
			gl_Position = projection * camera * model * vec4(vert, 1);}";

	reTest.addTestShader("vtx", vtxSource);

	ShaderBackend shBackend(reTest, "#version 130");

	shBackend.clearProgramDefinition();
	shBackend.addProgramDefinition("default",
			{ { { "vtx", Shader::Type::Vertex },
					{ "fs", Shader::Type::Fragment } } });

	shBackend.loadProgram("default");

	testingBackend.closeRenderer();
	testingBackend.closeDisplay();
}

