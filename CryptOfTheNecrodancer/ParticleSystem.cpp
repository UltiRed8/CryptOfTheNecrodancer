#include "ParticleSystem.h"
#include "ParticleManager.h"

ParticleSystem::ParticleSystem(const string& _name, const int _amount, const Vector2f& _position, const Time& _time, const Color& _color) : IManagable(_name)
{
	particles = vector<Particle>(_amount);
	vertices = VertexArray(PrimitiveType::Points, _amount);
	lifetime = _time;
	emitter = _position;
    color = _color;
    InitParticles();
    Register();
}

void ParticleSystem::draw(RenderTarget& _target, RenderStates _states) const
{
    _states.transform *= getTransform();

    _states.texture = nullptr;

    _target.draw(vertices, _states);
}

void ParticleSystem::InitParticles()
{
    const int _size = static_cast<const int>(particles.size());
    for (int _index = 0; _index < _size; _index++)
    {
        float angle = (rand() % 360) * 3.14f / 180.f;
        float speed = (rand() % 50) + 50.f;
        particles[_index].velocity = Vector2f(cos(angle) * speed, sin(angle) * speed);
        particles[_index].lifetime = static_cast<const float>(lifetime.asMilliseconds() - milliseconds(rand() % 1000).asMilliseconds());

        vertices[_index].position = emitter;
    }
}

void ParticleSystem::Update(const float _deltaTime)
{
	const int _size = static_cast<const int>(particles.size());
    for (int _index = 0; _index < _size; _index++)
    {
        Particle& _particle = particles[_index];
        _particle.lifetime -= _deltaTime;

        if (_particle.lifetime <= 0.0f)
        {
            vertices[_index].color.a = 0;
            ComputeDestroy();
            continue;
        }
        vertices[_index].position += _particle.velocity * (_deltaTime / 1000.0f);

        const float _ratio = (_particle.lifetime / 1000.0f) / lifetime.asSeconds();
        vertices[_index].color.a = static_cast<Uint8>(_ratio * 255);

        vertices.resize(particles.size() * 4);
        vertices[_index * 4].color = color;
        vertices[_index * 4 + 1].color = color;
        vertices[_index * 4 + 2].color = color;
        vertices[_index * 4 + 3].color = color;
    }
}

void ParticleSystem::Register()
{
	ParticleManager::GetInstance()->Add(id, this);
}

void ParticleSystem::ComputeDestroy()
{
    const int _size = static_cast<const int>(particles.size());
    for (int _index = 0; _index < _size; _index++)
    {
        if (vertices[_index].color.a >= 1) return;
    }
    Destroy();
}