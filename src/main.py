from simulator import Vec, Particle, Wall, Simulator
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

#  wall_vertices = [[(-2.0, -2.0), (2.0, -2.0), (2.0, 2.0), (-2.0, 2.0), (-2.0, -2.0)],
                 #  [(1.0, 1.8), (1.8, 0.0), (1.0, -1.8)]]
wall_vertices = [[(1.0, 1.8), (1.8, 0.0), (1.0, -1.8)]]
#  wall_vertices = []

walls = [Wall([Vec(v[0], v[1]) for v in w]) for w in wall_vertices]

fig, ax = plt.subplots()
scatter, = plt.plot([], [], 'o')
wall_plots = [plt.plot([], [], '-k')[0] for _ in range(len(walls))]

def anim_init():
    ax.set_xlim(-2.2, 2.2)
    ax.set_ylim(-2.2, 2.2)

    return [scatter, *wall_plots]

sim = Simulator(walls)

def anim_update(_frame):
    for plot, w in zip(wall_plots, wall_vertices):
        xs = [v[0] for v in w]
        ys = [v[1] for v in w]
        plot.set_data(xs, ys)

    sim.many_step(10000, 0.001)
    print(f"{sim.get_walls()[0].force.x}, {sim.get_walls()[0].force.y}")

    # positions is a list like so: [[x0, y0], [x1, y1], ...]
    positions = sim.get_particle_positions().tolist()
    xs = []
    ys = []
    for pos in positions:
        xs.append(pos[0])
        ys.append(pos[1])
    sim.free_allocation()

    scatter.set_data(xs, ys)
    return [scatter, *wall_plots]

#  anim = FuncAnimation(fig, anim_update, init_func=anim_init, blit=True, interval=1000)
#  plt.show()
anim = FuncAnimation(fig, anim_update, init_func=anim_init, blit=True, interval=10, frames=400)
anim.save('/Users/pvirally/Desktop/gif.gif', writer='imagemagick', fps=60)
