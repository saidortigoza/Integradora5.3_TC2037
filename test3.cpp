#include <stdio.h>

int main() {
  double x,y;
  
  printf ("Escribe el primer número");
  scanf ("%lf", &x);
  
  printf ("Escribe el segundo número");
  scanf ("%lf", &y);
  
  if (x>y) {
    printf("El mayor es %f", x);
  }
  
  else if(y>x) {
    printf("El mayor es %f", y);
  }
  
  else {
    printf("Son iguales");
  }
}
