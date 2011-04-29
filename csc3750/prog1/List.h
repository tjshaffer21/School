#if !defined (LIST_H)
#define LIST_H

#include "ListIterator.h"

template < class ListType >
class List
{
   private:
      int maxList;
      ListType** items;  // an array of list items
      int sze;  // number of items in list
      void arrayResize(int newSize);

   public:
      List();
      ~List();
      bool isEmpty();
      int size();
      void removeAll(); 
      ListType* get(int index);
      void add(int index, ListType* item);
      void add(ListType* item);
      void remove(int index);
      void set(int index, ListType* item);
      ListIterator<ListType>* iterator();
};

template < class ListType >
List<ListType>::List()
{
   maxList = 3;
   items = new ListType*[maxList];
   sze = 0;
}

template < class ListType >
List<ListType>::~List()
{
   delete[] items;
}

template < class ListType >
bool List<ListType>::isEmpty()
{
   return (sze == 0);
}

template < class ListType >
int List<ListType>::size()
{
   return sze;
}

template < class ListType >
void List<ListType>::removeAll()
{
    delete[] items;
    maxList = 3;
    items = new ListType*[maxList];
    sze = 0;
}

template < class ListType >  //1-based
ListType* List<ListType>::get(int index)
{
   if (index >= 1 && index <= sze) 
   {  
      return items[index - 1];
   }

   return items[0];
}

template < class ListType >
void List<ListType>::add(ListType* item)
{
   add(sze + 1, item);
}

template < class ListType >
void List<ListType>::add(int index, ListType* item)
{
    if (sze == maxList) 
    {
      arrayResize(maxList*2);
    }  

    if (index >= 1 && index <= sze+1) 
    {
       // make room for new element by shifting all items at 
       // positions >= index toward the end of the 
       // list (no shift if index == size+1)
      
       int pos=sze;

       while(pos >= index)
       {
          items[pos] = items[pos - 1];
          pos--;
       }

       // insert new item
       items[index - 1] = item;
       sze++;
    } 
}

template < class ListType >
void List<ListType>::remove(int index)
{
    if (index >= 1 && index <= sze) 
    {
      // delete item by shifting all items at 
      // positions > index toward the beginning of the list
      // (no shift if index == size)

      int pos = index;
      while(pos < sze)
      {
         items[pos - 1] = items[pos];
         pos++;
      }
      sze--;
    }

    if (size < maxList/2 - 1) //halve the size of the array, smallest size of maxList should be 2
    {
      arrayResize(maxList/2);
    }  
}

template < class ListType >
void List<ListType>::set(int index, ListType* item)
{
    //could use other methods already written, but this is more efficient
    if (index >= 1 && index <= sze) 
    {
       items[index - 1] = item;  //overwrite contents at that location
    } 
}

template < class ListType >
ListIterator<ListType>* List<ListType>::iterator()
{
   ListIterator<ListType>* iter = new ListIterator<ListType>(items, sze);
   return iter;
}

template < class ListType >
void List<ListType>::arrayResize(int newSize)
{
   maxList = newSize;
   ListType** temp = new ListType*[maxList];

   for (int i = 0; i < sze; i++)
   {
      temp[i] = items[i];
   }

   delete[] items;
   items = temp;
}

#endif

