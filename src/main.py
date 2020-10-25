import simulator

v1 = simulator.Vec2f([1.0, 2.0])
v2 = simulator.Vec2f([2.0, 3.0])
v3 = simulator.Vec2f([3.0, 2.0])

p = simulator.Particle2f(v1)
p.step(0.001)

w = simulator.Wall([v1, v2, v3])

print(v1, v2, v3)
print(p)
print(w)
