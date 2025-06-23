from django.urls import path
from .views import ComandoAPIView, StatusAPIView, ConfiguracaoAPIView

urlpatterns = [
    path('comando/', ComandoAPIView.as_view()),
    path('status/', StatusAPIView.as_view()),
    path('configuracoes/', ConfiguracaoAPIView.as_view()),
]