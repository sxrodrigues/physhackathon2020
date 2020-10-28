from simulator import Vec, Particle, Wall, Simulator
import random
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

wall_vertices = [[(-2.0, -2.0), (2.0, -2.0), (2.0, 2.0), (-2.0, 2.0), (-2.0, -2.0)], 
                 [(1.0, 1.8), (1.8, 0.0), (1.0, -1.8)]]

walls = [Wall([Vec(v[0], v[1]) for v in w]) for w in wall_vertices]

sim = Simulator(30, walls)

fig, ax = plt.subplots()
scatter, = plt.plot([], [], 'o')

for w in wall_vertices:
    xs = [v[0] for v in w]
    ys = [v[1] for v in w]
    plt.plot(xs, ys, '-k')

for w in wall_vertices:
    xs = [v[0] for v in w]
    ys = [v[1] for v in w]
    plt.plot(xs, ys, '-k')

def anim_init():
    ax.set_xlim(-2.2, 2.2)
    ax.set_ylim(-2.2, 2.2)

    return [scatter]

def anim_update(_frame):
    sim.step(0.001)

    # positions is a list like so: [[x0, y0], [x1, y1], ...]
    positions = sim.get_particle_positions().tolist()
    xs = []
    ys = []
    for pos in positions:
        xs.append(pos[0])
        ys.append(pos[1])
    sim.free_allocation()

    scatter.set_data(xs, ys)
    return [scatter]

anim = FuncAnimation(fig, anim_update, init_func=anim_init, blit=True, interval=10)
plt.show()
#  anim = FuncAnimation(fig, anim_update, init_func=anim_init, blit=True, interval=10, frames=400)
#  anim.save('/Users/pvirally/Desktop/gif.gif', writer='imagemagick', fps=60)
