#if !defined (DELETEOBJECT_H)
#define DELETEOBJECT_H

class DeleteObject
{
   private:
      int ref;

   public:
      DeleteObject();
      virtual ~DeleteObject();
      void addRef();
      void removeRef();
};

#endif
