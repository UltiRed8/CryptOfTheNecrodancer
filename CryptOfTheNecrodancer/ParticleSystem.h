#include <SFML/Graphics.hpp>

#include "IManagable.h"

using namespace std;
using namespace sf;

struct Particle
{
    Vector2f velocity;
    float lifetime;
};

class ParticleSystem : public Drawable, public Transformable, public IManagable<string>
{
    vector<Particle> particles;
    VertexArray vertices;
    Time lifetime;
    Vector2f emitter;
    Color color;

public:
    void SetPosition(const Vector2f& _position)
    {
        emitter = _position;
    }

public:
    ParticleSystem(const string& _name, const int _amount, const Vector2f& _position, const Time& _time = seconds(1.0f), const Color& _color = Color::White);

private:
    virtual void draw(RenderTarget& _target, RenderStates _states) const;
    void InitParticles();
    virtual void Register() override;
    void ComputeDestroy();

public:
    void Update(const float _deltaTime);
};