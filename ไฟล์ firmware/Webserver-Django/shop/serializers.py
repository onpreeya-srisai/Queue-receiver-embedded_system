from rest_framework import serializers
from .models import *


class TaskSerializer(serializers.HyperlinkedModelSerializer):

    class Meta:
        model = Memu
        fields = ('id', 'menu_name', 'menu_cost','isvalid')

class Create_oder_Serializer(serializers.ModelSerializer):
    class Meta:
        model = Cart
        fields = '__all__'

class Q_Serializer(serializers.ModelSerializer):
    class Meta:
        model = Q
        fields = '__all__'

class Client_Serializer(serializers.ModelSerializer):
    class Meta:
        model = Client
        fields = '__all__'
        
        