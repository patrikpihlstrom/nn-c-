#pragma once

#include <algorithm>
#include <memory>

#include "nn/NeuralNet.hpp"
#include "ActorId.hpp"
#include "../application/Math.hpp"

#include <SFML/Graphics.hpp>


class NNActor : public sf::Drawable, public sf::Transformable {
public:
    const float MAX_SPEED = 200;
    const float MAX_ACC = 0.05f;
    const float MAX_ROTATION_RATE = 0.05f;
    const float MAX_ROTATION_ACC = 0.05f;
    const float SENSOR_DISTANCE = 100.f;
    const float SIGHT_DISTANCE = 1000.f;
    const unsigned short SENSOR_COUNT = 9;
    const unsigned short EYE_COUNT = 12;

public:
    NNActor();

    NNActor(const std::vector<double> dna);

    ~NNActor();

    ActorId getId() const;

    void assign(const ActorId &id);

    void update(const float &deltaTime);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    void drawNeuralNet(sf::RenderTarget &target, sf::RenderStates states) const;

    sf::Rect<int> getBounds() const;

    sf::Rect<float> getPhysicalBounds() const;

    bool isDead() const;

    void setDead(const bool dead);

    std::vector<double> getDna() const;

    void printDna() const;

    double getInput(const int index) const;

    void setInput(const double value, const int index);

    std::vector<sf::Vector2f> getSensors() const;

    sf::Vector2f getSensor(const int index);

    void setSensor(const int index, const sf::Vector2f sensor);

    int getHealth() const;

    void setHealth(const uint8_t health);

    void addHealth(const int health);

    void move(const float x, const float y);

    bool operator<(const NNActor &actor) const;

    float getSize() const;

    void setSize(const float size);

    float getAge() const;

private:
    ActorId m_id;
    float m_desiredSpeed,
            m_desiredRotationRate,
            m_rotationRate,
            m_angle,
            m_theta,
            m_size,
            m_distance,
            m_age;

    bool m_dead;
    int m_health;

    sf::Vector2f m_velocity;

    std::vector<sf::Vector2f> m_sensors;
    std::vector<double> m_inputs;
    std::vector<double> m_dna;

    NeuralNet m_neuralNet;

private:
    void control();

};

