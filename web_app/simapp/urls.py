from web_app.urls import path

from . import views

urlpatterns = [
    path('', views.start, name = 'start'),
    path('sim/', views.sim, name = 'sim'),
    path('about/', views.about, name = 'about')
]