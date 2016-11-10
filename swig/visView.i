%module visView
%{
#define SWIG_FILE_WITH_INIT
#include "visView.h"

%}

%include "numpy.i"

%init %{
import_array();
%}

%inline %{
    int _getVisViewValue(int *value, visView *in, int x, int y){
      if(x > in->width){ return -1;};
      if(y > in->height){ return -1;};

      *value = in->data[x + in->width * y];
      return 0;
    }
%}

%apply int *OUTPUT { int *output};
%extend visView{
  visView(int width, int height){
    visView *new_visView = CreateVisView(width, height);
    return new_visView;
  }

  ~visView(){
    DestroyVisView(&$self);
  }

    /*char *__str__() {
      static char tmp[1024];
      snprintf(tmp, 1024, "the value is %d", 2323);
      return tmp;
    }*/

    int __len__(){
        return $self->width * $self->height;
    }

    int _value(int *output, int x, int y){
        int value;
        if(_getVisViewValue(&value, $self, x, y) == 0){
            *output = value;
            return 0;
        }
        return -1;
    }

}




/*%apply(int DIM1, int DIM2, PixelValue* IN_ARRAY1) {(int height, int width, PixelValue *data)}*/
// %apply (int * INPLACE_ARRAY int DIM1, int DIM2) {(int height, int width, PixelValue *data)}
%include "visView.h"

%pythoncode %{
class VisView(visView):
    def value(self, x, y):
        res, answer = self._value(x,y)
        if res == 0:
            return answer
        else:
            raise IndexError
%}
