from django.contrib import admin
from .models import Configuracao, StatusESP32

@admin.register(Configuracao)
class ConfiguracaoAdmin(admin.ModelAdmin):
    list_display = ['horario', 'quantidade']

@admin.register(StatusESP32)
class StatusAdmin(admin.ModelAdmin):
    list_display = ['temperatura', 'container_status', 'atualizado_em']