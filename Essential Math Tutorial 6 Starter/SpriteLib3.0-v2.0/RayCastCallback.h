//Hi, to be transparent, I learned how to write this callback class from this website - Shesh Gaur
//https://www.raywenderlich.com/3036-intermediate-box2d-physics-forces-ray-casts-and-sensors

#include <Box2D/Box2D.h>
//Hey there
class RayCastCallback : public b2RayCastCallback
{
public:
    RayCastCallback() : m_fixture(NULL) {
    }

    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
        m_fixture = fixture;
        m_point = point;
        m_normal = normal;
        m_fraction = fraction;
        return fraction;
    }

    b2Fixture* m_fixture;
    b2Vec2 m_point;
    b2Vec2 m_normal;
    float32 m_fraction;

};