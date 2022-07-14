from django.db import models

class Cart (models.Model):
    order_food_items = models.CharField(max_length=200)
    order_cost = models.IntegerField()
    created_at = models.DateTimeField(auto_now_add=True)
class Memu (models.Model):
    menu_name = models.CharField(max_length=200)
    menu_cost = models.IntegerField()
    isvalid = models.BooleanField(default=True)
    created_at = models.DateTimeField(auto_now_add=True)
class Q (models.Model):
    order_id = models.IntegerField()
    created_at = models.DateTimeField(auto_now_add=True)

class Client (models.Model):
    order_id = models.IntegerField()
    created_at = models.DateTimeField(auto_now_add=True)

# Create your models here.
