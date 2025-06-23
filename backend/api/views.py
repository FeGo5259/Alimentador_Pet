from rest_framework.views import APIView
from rest_framework.response import Response
from .models import Configuracao, StatusESP32
from .serializers import ConfiguracaoSerializer, StatusSerializer
from django.shortcuts import get_object_or_404

class ComandoAPIView(APIView):
    def post(self, request):
        comando = request.data.get('comando')
        valor = request.data.get('valor')
        print(f'Comando: {comando} - Valor: {valor}')
        return Response({'mensagem': 'Comando enviado com sucesso'})

class StatusAPIView(APIView):
    def get(self, request):
        status_obj = StatusESP32.objects.last()
        serializer = StatusSerializer(status_obj)
        return Response(serializer.data)

class ConfiguracaoAPIView(APIView):
    def get(self, request):
        configs = Configuracao.objects.all()
        serializer = ConfiguracaoSerializer(configs, many=True)
        return Response(serializer.data)

    def post(self, request):
        serializer = ConfiguracaoSerializer(data=request.data)
        if serializer.is_valid():
            serializer.save()
            return Response(serializer.data, status=201)
        return Response(serializer.errors, status=400)

    def delete(self, request):
        id_horario = request.data.get('id')
        if id_horario:
            configuracao = get_object_or_404(Configuracao, id=id_horario)
            configuracao.delete()
            return Response({'mensagem': 'Horário deletado com sucesso.'})
        else:
            return Response({'erro': 'ID não informado.'}, status=400)