from web_app.urls import path

from . import views

urlpatterns = [
    path('', views.start, name = 'start'),
    path('sim/', views.sim, name = 'sim'),
    path('sim/get_thruster', views.get_thruster, name = 'get_thruster'),
    path('about/', views.about, name = 'about')
]
