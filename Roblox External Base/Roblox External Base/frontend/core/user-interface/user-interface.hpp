#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

class UI
{
public:
	bool Setup();
	bool Render();
	void Styles();
	void Menu();
}; static UI* ui = new UI;

#endif // USER_INTERFACE_HPP