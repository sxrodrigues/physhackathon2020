from simulator import Vec, Particle, Wall, Simulator
import matplotlib.pyplot as plt
import math
import random as rand
import matplotlib.animation as animation

def genThermalIsotropic(temp,density,x, y, n_col, N = -1):
    partcles = []
    for i in range(N):
        E = -temp*math.log( 1-rand.random() )
        theta = rand.random()*2*math.pi

        v = [ math.cos(theta) * math.sqrt(E*2), math.sin(theta) * math.sqrt(E*2) ] 

        px = i%n_col
        py = math.floor(i/n_col)

        partcles.append( Particle( Vec(px*density + x, py*density + y) , Vec( v[0] , v[1] ) ) )
    return partcles

throat = 0.3
n_partilces = 2000
dt = 0.001
wall_vertices = [[(1.0, -throat/2),(1.5, -throat/2),(2.0, -(throat/2 + 0.4) ),(2.0, -2.0), (4.0, -2.0), (4.0, 2.0), (2.0, 2.0),(2.0, (throat/2 + 0.4) ), (1.5, throat/2), (1.0, throat/2)],
                 [(1.05, throat/2 + 0.1),(0.0, 2.0)],
                 [(1.05, -throat/2 - 0.1),(0.0, -2.0)]]
#  wall_vertices = [[(1.0, 1.8), (1.8, 0.0), (1.0, -1.8)]]
#  wall_vertices = []

walls = [Wall([Vec(v[0], v[1]) for v in w]) for w in wall_vertices]
thrust_value = []
thrust_frame = []

fig, ax = plt.subplots(1)
axs = [ax]
scatter, = axs[0].plot([], [], 'o' ,markersize=1)
wall_plots = [axs[0].plot([], [], '-k')[0] for _ in range(len(walls))]
#thrust, = axs[1].plot([],[],"-r")


def anim_init():
    axs[0].set_xlim(-4.0,4.0)
    axs[0].set_ylim(-4.0,4.0)

    #axs[1].set_ylim(-0.1,4.0)
    #axs[1].set_xlim(0.0,30.0)
    
    #axs[0].set_aspect('equal')

    return [scatter, *wall_plots]
sim = Simulator(walls,genThermalIsotropic( 100.0 , 0.02 , 2.0 + 0.1 , -2.0 + 0.1 , 55 , n_partilces ) )
sim.many_step(100,dt)

def update(i):
    #  sim.many_step(10000, 0.001)
    sim.many_step(1000,dt)
    fx = 0
    fy = 0

    for wall in sim.get_walls():
        fx += wall.force.x / n_partilces / dt / 200
        fy += wall.force.y / n_partilces / dt / 200
    #sim.zero_walls()
    thrust_frame.append(i)
    thrust_value.append(math.sqrt( fx*fx + fy*fy ))

def anim_update(_frame):
    for plot, w in zip(wall_plots, wall_vertices):
        xs = [v[0] for v in w]
        ys = [v[1] for v in w]
        plot.set_data(xs, ys)
    
    update(_frame)
    
    #axs[1].set_xlim(_frame-100,_frame+20)
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

#writer = animation.FFMpegWriter(fps=30)
#anim = animation.FuncAnimation(fig, anim_update, init_func=anim_init, blit=True, interval=10, frames=1000)
#anim.save('truster.gif', writer='pillow')#writer=writer)

n_frames = 100
for i in range(n_frames):
    if i%(n_frames // 10) == 0:
        print(100*i/n_frames,'%')
    update(i)

plt.plot(thrust_frame, thrust_value,label="2.0")

#seocnd run
wall_vertices = [[(1.0, -throat/2),(1.5, -throat/2),(2.0, -(throat/2 + 0.4) ),(2.0, -2.0), (4.0, -2.0), (4.0, 2.0), (2.0, 2.0),(2.0, (throat/2 + 0.4) ), (1.5, throat/2), (1.0, throat/2)],
                 [(1.05, throat/2 + 0.1),(0.0, 4.0)],
                 [(1.05, -throat/2 - 0.1),(0.0, -4.0)]]

sim = Simulator(walls,genThermalIsotropic( 100.0 , 0.02 , 2.0 + 0.1 , -2.0 + 0.1 , 55 , n_partilces ) )
sim.many_step(100,dt)

thrust_value = []
thrust_frame = []

n_frames = 100
for i in range(n_frames):
    if i%(n_frames // 10) == 0:
        print(100*i/n_frames,'%')
    update(i)

plt.plot(thrust_frame, thrust_value,label="4.0")
plt.legend()

plt.show()
#  anim = FuncAnimation(fig, anim_update, init_func=anim_init, blit=True, interval=10, frames=400)
#  anim.save('/Users/pvirally/Desktop/gif.gif', writer='imagemagick', fps=60)
