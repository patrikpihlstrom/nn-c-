#pragma once

#include <stdio.h>
#include <memory>

#include "application/state/State.hpp"
#include "application/ui/Button.hpp"
#include "application/TextureHolder.hpp"


class MenuState : public State
{
public:
	MenuState();
	~MenuState();

	void enter(sf::RenderWindow& window);
	void update(const float& deltaTime, const sf::RenderWindow& window);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void exit();

private:
	void sendSwitchSignal();
	void sendExitSignal();

	std::unique_ptr<TextureHolder> m_textureHolder;

	std::unique_ptr<Button> m_resume;
	std::unique_ptr<Button> m_exit;

	sf::View m_view;

	sf::Sprite m_gameStateSprite;
	sf::Texture m_gameStateTexture;
	sf::Shader m_blurShader;
};

