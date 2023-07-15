#pragma once
#include <string>
#include <vector>
#include <functional>
#include <iostream>

namespace test
{
	//���Եĳ�����
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		//����
		virtual void OnUpdate(float deltaTime) {}
		//��Ⱦ
		virtual void OnRender() {}
		//ImGUI��Ⱦ
		virtual void OnImGuiRender() {}
	};

	//���Բ˵�
	class TestMenu : public Test
	{
	private:
		Test*& m_CurrentTest;													//��ǰ�Ĳ���
		/*���еĲ���
		 * key:		������
		 * value:	����˲���ʱ�Ļص�����
		 */
		std::vector<std::pair<std::string, std::function<Test* ()>>> m_Tests;	//���еĲ���
	public:
		TestMenu(Test*& currentTestPtr);

		void OnImGuiRender() override;

		//ע��һ������
		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Register test: " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, 
				[]() //�ص�������Ҫ���ش˲��Ե�һ��ʵ�� 
				{ 
					return new T(); 
				}
			));
		}
	};
}