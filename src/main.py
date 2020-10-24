import simulator

v = simulator.Vec2f([1.0, 2.0])
p = simulator.Particle2f(v)
p.step(0.001)
