#include "Test.h"

#include "imgui/imgui.h"

namespace test
{
	TestMenu::TestMenu(Test*& currentTestPtr)
		:m_CurrentTest(currentTestPtr)
	{
	}

	void TestMenu::OnImGuiRender()
	{
		//将每个测试，绘制成一个Button
		for (auto& test : m_Tests) 
		{
			if (ImGui::Button(test.first.c_str())) 
			{
				m_CurrentTest = test.second();
			}
		}
	}
}
