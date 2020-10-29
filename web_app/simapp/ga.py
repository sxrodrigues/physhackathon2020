from PIL import Image, ImageDraw
from io import BytesIO
import base64
import datetime
import random
import statistics
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from simulator import Vec, Particle, Wall, Simulator

class GA:
    def __init__(self, pop_size, num_verts=8):
        self.pop_size = pop_size
        self.pop = []
        for _ in range(self.pop_size):
            verts = [(random.uniform(-1.0, 1.0), random.uniform(-1.0, 1.0)) for __ in range(num_verts)]

            # pop: List[[Wall, fitness]]
            self.pop.append([Wall([Vec(v[0], v[1]) for v in verts]), float('-inf')])
        self.test_fitness()

    def test_fitness(self):
        best_fitness = float('-inf')
        best_wall = self.pop[0][0]
        best_stdev_x = 0
        best_stdev_y = 0

        for elem in self.pop:
            wall = elem[0]

            # Grab a simulator and simulate 1 second
            sim = Simulator([wall])
            sim.many_step(1000, 0.001) # 1 second

            # 1e-5 to make the force vector more manageable
            lift = sim.get_walls()[0].force
            lift = Vec(lift.x * 1e-5, lift.y * 1e-5)

            # Assign a new fitness
            stdev_x = statistics.stdev([v.x for v in elem[0].get_vertices()])
            stdev_y = statistics.stdev([v.y for v in elem[0].get_vertices()])
            fitness = lift.y - abs(lift.x)
            fitness -= (10*(stdev_x + stdev_y))**5
            elem[1] = fitness

            if elem[1] > best_fitness:
                best_fitness = elem[1]
                best_wall = wall
                best_stdev_x = stdev_x
                best_stdev_y = stdev_y
        # print(f'stddev_x: {best_stdev_x}, stdev_y: {best_stdev_y}')
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
            baby[0].mutate()
            new_pop.append(baby)
        self.pop = new_pop

        return self.test_fitness()

#  ga = GA(30)
ga = GA(3)

def get_image():
    wall, fitness = ga.epoch()
    verts = wall.get_vertices()
    # print(f'{fitness:e}')

    img = Image.new('RGB', (600, 600), (255, 255, 255))
    draw = ImageDraw.Draw(img)
    draw.line([((v.x + 1) * 150, (v.y + 1) * 300) for v in verts], fill=0, width=8, joint="curve")

    buffered = BytesIO()
    img.save(buffered, format="JPEG")
    return base64.b64encode(buffered.getvalue())
