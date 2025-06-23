from django.db import models

class Configuracao(models.Model):
    quantidade = models.IntegerField(default=50)  # gramas
    horario = models.TimeField()

    def __str__(self):
        return f'{self.horario} - {self.quantidade}g'

class StatusESP32(models.Model):
    temperatura = models.FloatField()
    container_status = models.CharField(max_length=50)
    atualizado_em = models.DateTimeField(auto_now=True)

    def __str__(self):
        return f'{self.container_status} - {self.temperatura}°C'