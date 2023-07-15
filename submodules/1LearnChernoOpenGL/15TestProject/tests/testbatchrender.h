#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <memory>

namespace test
{
	//批量渲染（一次渲染两个正方形）
	class TestBatchRender : public Test
	{
	private:
		std::unique_ptr<VertexArray>	m_VAO;
		std::unique_ptr<IndexBuffer>	m_IndexBuffer;
		std::unique_ptr<VertexBuffer>	m_VertexBuffer;
		std::unique_ptr<Shader>			m_Shader;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_Translation;

	public:
		TestBatchRender();
		~TestBatchRender();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}