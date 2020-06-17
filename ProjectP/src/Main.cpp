#include <iostream>

#include "Application.h"

int main()
{
	/* Public Variable */
	Application* m_application = new Application();

	if (m_application->StartUp())
	{
		m_application->Update();
	}
	else
	{
		m_application->Terminate();
	}

	return 0;
}