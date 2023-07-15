#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iostream>

namespace test
{
	//测试的抽象类
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		//更新
		virtual void OnUpdate(float deltaTime) {}
		//渲染
		virtual void OnRender() {}
		//ImGUI渲染
		virtual void OnImGuiRender() {}
	};

	//测试菜单
	class TestMenu : public Test
	{
	private:
		Test*& m_CurrentTest;													//当前的测试
		/*所有的测试
		 * key:		测试名
		 * value:	激活此测试时的回调函数
		 */
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;	//所有的测试
	public:
		TestMenu(Test*& currentTestPtr);

		void OnImGuiRender() override;

		//注册一个测试
		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Register test: " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, 
				[]() //回调函数需要返回此测试的一个实例 
				{ 
					return new T(); 
				}
			));
		}
	};
}