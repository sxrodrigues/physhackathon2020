from django.shortcuts import render
import plotly.graph_objects as go
import plotly.express as px
import plotly.offline as off


def start(request):
    return render(request, 'simapp/start.html', {'title': 'Home'})


def sim(request):

    sc = px.scatter(x=[0, 1, 2, 3, 4], y=[0, 1, 4, 9, 16], title="Hey, Im an example plot!")

    plot = off.plot(sc,output_type='div', include_plotlyjs=True)

    context = {'plot' : plot}

    return render(request, 'simapp/sim.html', context)

def about(request):

    

    return render(request, 'simapp/about.html')