from django.shortcuts import render
import plotly.express as px
import plotly.graph_objects as go
import plotly.offline as off

from .ga import get_image

def start(request):
    return render(request, 'simapp/start.html', {'title': 'Home'})

def sim(request):
    return render(request, 'simapp/sim.html')

def get_thruster(request):
    context = {'img' : get_image()}
    return render(request, 'simapp/get_thruster.html', context)

def about(request):
    return render(request, 'simapp/about.html')
