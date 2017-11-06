#pragma once





class Actor : public sf::Transformable, public sf::Drawable
{
public:

	
	Actor();
	~Actor();

	virtual void update(const float& deltaTime);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


	uint64_t getIdAsInt() const;



protected:


private:
};

