from simulator import Vec2f, Particle2f, Wall, Simulator
import random
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

sim = Simulator(30)

fig, ax = plt.subplots()
scatter, = plt.plot([], [], 'o')

def anim_init():
    ax.set_xlim(-2, 2)
    ax.set_ylim(-2, 2)
    return scatter,

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
    return scatter,

anim = FuncAnimation(fig, anim_update, init_func=anim_init, blit=True, interval=10, frames=400)
anim.save('/Users/pvirally/Desktop/gif.gif', writer='imagemagick', fps=60)
#  plt.show()
