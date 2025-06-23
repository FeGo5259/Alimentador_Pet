from rest_framework import serializers
from .models import Configuracao, StatusESP32

class ConfiguracaoSerializer(serializers.ModelSerializer):
    class Meta:
        model = Configuracao
        fields = '__all__'

class StatusSerializer(serializers.ModelSerializer):
    class Meta:
        model = StatusESP32
        fields = '__all__'