from rest_framework import viewsets
from . import models
from . import serializers

class Cart_view (viewsets.ModelViewSet):
    queryset = models.Cart.objects.all()
    serializer_class = serializers.Create_oder_Serializer

class Q_view(viewsets.ModelViewSet):
    queryset = models.Q.objects.all()
    serializer_class = serializers.Q_Serializer

class Client_view(viewsets.ModelViewSet):
    queryset = models.Client.objects.all()
    serializer_class = serializers.Client_Serializer
    
    
    