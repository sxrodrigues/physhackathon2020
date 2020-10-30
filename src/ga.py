from simulator import Vec, Particle, Wall, Simulator
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import random
import math
import datetime
import statistics

def genThermalIsotropic(temp,density,x, y, n_col, N = -1):
    partcles = []
    for i in range(N):
        E = -temp*math.log( 1-random.random() )
        theta = random.random()*2*math.pi

        v = [ math.cos(theta) * math.sqrt(E*2), math.sin(theta) * math.sqrt(E*2) ] 

        px = i%n_col
        py = math.floor(i/n_col)

        partcles.append( Particle( Vec(px*density + x, py*density + y) , Vec( v[0] , v[1] ) ) )
    return partcles

class GA:
    def __init__(self, pop_size, num_verts=8):
        
        throat = 0.3
        self.n_partilces = 10000
        self.dt = 0.001
        starting_angle = math.pi/4
        leghth = 1.5

        self.pop_size = pop_size
        self.pop = []
        for _ in range(self.pop_size):
            #verts = [(random.uniform(-1.0, 1.0), random.uniform(-1.0, 1.0)) for __ in range(num_verts)]
            x_pos = np.linspace(1.05 , 1.05-leghth*math.cos(starting_angle) , num_verts)
            y_pos = np.linspace(throat/2 , leghth*math.sin(starting_angle) , num_verts)
            verts = [ (x_pos[i],y_pos[i]) for i in range(num_verts) ]

            # pop: List[[Wall, fitness]]
            self.pop.append([Wall([Vec(v[0], v[1]) for v in verts]), float('-inf')])
        
        
        self.const_geom = [[(1.0, -throat/2),(1.5, -throat/2),(2.0, -(throat/2 + 0.4) ),(2.0, -2.0), (4.0, -2.0), (4.0, 2.0), (2.0, 2.0),(2.0, (throat/2 + 0.4) ), (1.5, throat/2), (1.0, throat/2)],
                        ]#[(1.05, throat/2 + 0.1),(0.0, 2.0)],
                        #[(1.05, -throat/2 - 0.1),(0.0, -2.0)]]
        self.const_walls = [Wall([Vec(v[0], v[1]) for v in w]) for w in self.const_geom]
        self.bottom_wall = Wall([ Vec(verts[i][0],-verts[i][1]) for i in range(len(verts)) ])

        #walls = [Wall([Vec(v[0], v[1]) for v in w]) for w in wall_vertices]
        
        self.test_fitness()

    def objective(self,wall):
        pop_bottom_wall = Wall([ Vec(vert.x,-vert.y) for vert in wall.get_vertices() ])
        sim = Simulator(self.const_walls+[wall,pop_bottom_wall],genThermalIsotropic( 100.0 , 0.02 , 2.0 + 0.1 , -2.0 + 0.1 , 55 , self.n_partilces ) )
        
        sim.many_step(100,self.dt) # magic number that works well-ish 1000

        fx = 0
        fy = 0

        for wall in sim.get_walls():
            fx += wall.force.x / self.n_partilces / self.dt / 2000
            #fy += wall.force.y / self.n_partilces / self.dt / 2000
        
        return -fx

    def test_fitness(self):
        best_fitness = float('-inf')
        best_wall = self.pop[0][0]
        #best_stdev_x = 0
        #best_stdev_y = 0

        for elem in self.pop:
            wall = elem[0]

            fitness = self.objective(wall)

            if fitness > best_fitness:
                best_fitness = fitness
                best_wall = wall
                #best_stdev_x = stdev_x
                #best_stdev_y = stdev_y
        #print(f'stddev_x: {best_stdev_x}, stdev_y: {best_stdev_y}')
        #just for drawing
        self.bottom_wall = Wall([ Vec(vert.x,-vert.y) for vert in best_wall.get_vertices() ])
        return (best_wall, best_fitness)

    def select(self, num):
        fitnesses = [elem[1] for elem in self.pop]
        return random.choices(self.pop, weights=fitnesses, k=num)

    def crossover(self, parents):
        parents.sort(key=lambda elem: len(elem[0].get_vertices()))

        crossover_point = random.randint(0, len(parents[0][0].get_vertices())-1)
        new_verts = parents[0][0].get_vertices()[:crossover_point] + parents[1][0].get_vertices()[crossover_point:]
        return [Wall(new_verts), float('-inf')]

    def epoch(self):
        new_pop = []
        for _ in range(self.pop_size):
            parents = self.select(2)
            baby = self.crossover(parents)
            baby[0].mutate(0.01)
            new_pop.append(baby)
        self.pop = new_pop

        return self.test_fitness()

ga = GA(30)

fig, ax = plt.subplots()
wall_plot, = ax.plot([], [], '-g')
const_wall_plots = [ax.plot([], [], '-k')[0] for _ in range(len(ga.const_walls))]
wall_bottom, = ax.plot([], [], '-g')

for plot, w in zip(const_wall_plots, ga.const_geom):
    xs = [v[0] for v in w]
    ys = [v[1] for v in w]
    plot.set_data(xs, ys)

def anim_init():
    ax.set_xlim(-3.0, 3.0)
    ax.set_ylim(-3.0, 3.0)
    return wall_plot,wall_bottom,

def anim_update(_frame):
    wall, fitness = ga.epoch()

    print(f'{fitness:e}')

    wall_verts = wall.get_vertices()
    wall_plot.set_data([v.x for v in wall_verts], [v.y for v in wall_verts])

    wall_verts_bottom = ga.bottom_wall.get_vertices()
    wall_bottom.set_data([v.x for v in wall_verts_bottom], [v.y for v in wall_verts_bottom])

    return wall_plot,wall_bottom,

anim = FuncAnimation(fig, anim_update, init_func=anim_init, blit=True, interval=100)
plt.show()
#  anim = FuncAnimation(fig, anim_update, init_func=anim_init, blit=True, interval=200, frames=300)
#  anim.save('/Users/pvirally/Desktop/gif.gif', writer='imagemagick', fps=15)
