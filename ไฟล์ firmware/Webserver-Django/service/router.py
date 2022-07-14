from shop.viewsets import Cart_view, Client_view,Q_view
from rest_framework import routers

router = routers.DefaultRouter()
router.register('cart',Cart_view)
router.register('Q',Q_view)
router.register('Client',Client_view)