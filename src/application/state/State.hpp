#pragma once

#include <memory>

#include <SFML/Graphics.hpp>


enum class StateType
{
	Menu,
	Game,
};

enum class Signal
{
	Switch,
	Exit,
};

class State : public sf::Drawable
{
public:
	State();
	~State();

	virtual void enter(sf::RenderWindow& window);
	virtual void update(const float& deltaTime, sf::RenderWindow& window);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void exit();

	StateType getStateType() const;
	std::vector<Signal> getSignals() const;
	void clearSignals();

	virtual sf::Vector2f getView() const;

protected:
	StateType m_stateType;
	bool m_escape, m_options;

	std::vector<Signal> m_signalStack;
};

