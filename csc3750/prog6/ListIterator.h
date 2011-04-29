#if !defined (LISTITERATOR_H)
#define LISTITERATOR_H

template < class ListType >
class ListIterator
{

   private:
      int index;
      int size;
      ListType** items;

   public:
      ListIterator(ListType** item, int sz);
      ~ListIterator();
      bool hasNext();
      ListType* next();

};

template < class ListType >
ListIterator<ListType>::ListIterator(ListType** item, int sz)
{
   items = new ListType*[sz];
   for (int x = 0; x < sz; x++)
   {
      items[x] = item[x];
   }

   index = 1;
   size = sz;
}

template < class ListType >
ListIterator<ListType>::~ListIterator()
{
   delete[] items;
}

template < class ListType >
bool ListIterator<ListType>::hasNext()
{
   return (index <= size);
}

template < class ListType >
ListType* ListIterator<ListType>::next()
{

   ListType* temp = items[index-1];
   index++;
   return temp; 

}

#endif
