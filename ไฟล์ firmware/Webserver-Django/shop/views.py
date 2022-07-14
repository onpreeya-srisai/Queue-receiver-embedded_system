from django.http.response import HttpResponseRedirect
from django.shortcuts import render
from django.http import HttpResponse, request

from .serializers import TaskSerializer
from rest_framework import generics ,permissions
from rest_framework.renderers import JSONRenderer
from django.views.generic import View
from django.contrib import messages

from .models import *
# Create your views here.

class TaskList(generics.ListAPIView):
    queryset = Memu.objects.all()
    serializer_class  = TaskSerializer
        
class TaskDetail(generics.RetrieveUpdateDestroyAPIView):
    queryset = Memu.objects.all()
    serializer_class = TaskSerializer


class Create_oder(generics.ListAPIView):
    
    def put(self, request, **kwargs):
        serializer = Create_oder_Serializer(data=request.data)
        serializer.is_valid(raise_exception=True)
        obj, created = Cart.objects.update_or_create(
        id=kwargs['id'],
        defaults=serializer.validated_data)
        return Response()


def index(request):

    pass
    return render(request, 'ShoppingCart_1.html')

def getrequest(request,order_id):
    obj = Q.objects.filter(id=1)
    obj = obj[0]
    obj.order_id = order_id
    obj.save()
    return HttpResponseRedirect(request.META.get('HTTP_REFERER'))


def client(request,order_id,client_id):
    obj2 = Q.objects.filter(id=1)
    obj2 = obj2[0]
    obj = Client.objects.filter(id=client_id)
    obj = obj[0]
    
    obj3 = Client.objects.all()
    temp = []
    for i in obj3:
        temp.append(i.order_id)
        if(i.order_id in temp and i.order_id != 0):
            print(temp)
            print("เข้า")
            messages.success(request,"เครื่องมีออเดอร์อยู่แล้ว")
            return HttpResponseRedirect(request.META.get('HTTP_REFERER'))
            
            
    if(obj2.order_id == 0 and obj.order_id == 0):   
        obj.order_id = order_id
        obj.save()
        messages.success(request,"ตั้งค่าคิวเรียบร้อย")
        return HttpResponseRedirect(request.META.get('HTTP_REFERER'))

    else:
         messages.success(request,"เครื่องมีออเดอร์อยู่แล้ว")
         return HttpResponseRedirect(request.META.get('HTTP_REFERER'))
    

def deq(request,order_id):
    obj2 = Client.objects.filter(order_id=order_id)
    obj2 =obj2[0]
    obj2.order_id = 0
    obj2.save()
    Cart.objects.filter(id=order_id).delete()
    obj = Q.objects.filter(id=1)
    obj = obj[0]
    obj.order_id = 0
    obj.save()
    return HttpResponseRedirect(request.META.get('HTTP_REFERER'))

def staff(request):
    lst_cart = []
    lst_cortall =[]
    lst_order_id =[]

    Cart_obj = Cart.objects.all()
    Menu_obj = Memu.objects.all()
    Client_obj = Client.objects.all()

    
    value_id = []
    for i in Cart_obj:
        order_number = i.order_food_items.split("+")
        order_number = list(map(int,order_number))
        print(order_number)
        order_temp_number = []
        list_string_menu = []
        key_obj = []
        
        for j in range(0,10):

            if(order_number[j] != 0): 
                obj = Memu.objects.get(id=j+1)
                list_string_menu.append(obj.menu_name)
                order_temp_number.append(order_number[j])
                lst_cart.append({"id_order":i.id,"name":obj.menu_name,"ordernum":order_number[j],"cost":int(obj.menu_cost)*int(order_number[j])})
              
        
        value_id.append({"id_order":i.id,"cart_priceall":i.order_cost})
 
   # context ={"cart":obj_all, "menu":Menu_obj,"cost":lst_cortall}
    q = Q.objects.get(id=1)
    q_obj = q.order_id
    return render(request, 'ShoppingBasket_1.html',{'lst_cart':lst_cart,"value_id":value_id,"q":q_obj,"client":Client_obj})    