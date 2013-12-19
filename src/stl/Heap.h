/******************************************************************************************[Heap.h]
Copyright (c) 2003-2006, Niklas Een, Niklas Sorensson
Copyright (c) 2007-2010, Niklas Sorensson
Copyright (c) 2013 Mario Alviano, Carmine Dodaro, and Francesco Ricca.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**************************************************************************************************/

/**
 * DISCLAIMER: This code is part of the software MiniSAT2. It has been adapted in order to work
 * with WASP data structures.
 * 
 */

#ifndef HEAP_H
#define HEAP_H

//=================================================================================================
// A heap implementation with support for decrease/increase key.
template< class K >
class Heap {
    vector< K* > heap;     // Heap of Keys
    Comparator lt;       // The heap is a minimum-heap with respect to this comparator

    // Index "traversal" functions
    static inline unsigned int left  (unsigned int i) { return i*2+1; }
    static inline unsigned int right (unsigned int i) { return (i+1)*2; }
    static inline unsigned int parent(unsigned int i) { return (i-1) >> 1; }

    void percolateUp(unsigned int i)
    {
        K*   x  = heap[i];
        int p  = parent(i);
        
        while (i != 0 && lt(x, heap[p])){
            heap[i]          = heap[p];
            heap[p]->setHandle(i);
            i                = p;
            p                = parent(p);
        }
        heap   [i] = x;
        x->setHandle(i);
    }


    void percolateDown(unsigned int i)
    {
        K* x = heap[i];
        while (left(i) < heap.size()){
            int child = right(i) < heap.size() && lt(heap[right(i)], heap[left(i)]) ? right(i) : left(i);
            if (!lt(heap[child], x)) break;
            heap[i]          = heap[child];
            heap[i]->setHandle(i);
            i                = child;
        }
        heap   [i] = x;
        x->setHandle(i);
    }


  public:
    Heap(){}

    int  size      ()          const { return heap.size(); }
    bool empty     ()          const { return heap.size() == 0; }
    bool inHeap    (K* k)       const { return k->isInHeap(); }
    int  operator[](int index) const { assert(index < heap.size()); return heap[index]; }

    void decrease  (K* k) { assert(inHeap(k)); percolateUp  (k->getHandle()); }
    void increase  (K* k) { assert(inHeap(k)); percolateDown(k->getHandle()); }


    // Safe variant of insert/decrease/increase:
    void update(K* k)
    {
        if (!inHeap(k))
            insert(k);
        else {
            percolateUp(k->getHandle());
            percolateDown(k->getHandle()); }
    }


    void push(K* k)
    {
        if( k->isInHeap() )
            return;
        pushNoCheck( k );
    }

    void pushNoCheck(K* k)
    {
        assert( !k->isInHeap() );
		k->setInHeap(true);
		k->setHandle(heap.size());
        heap.push_back(k);
        percolateUp(k->getHandle());
    }

    void remove(K* k)
    {
        assert(inHeap(k));

        int k_pos  = k->getHandle();
        k->setInHeap(false);

        if (k_pos < heap.size()-1){
            heap[k_pos]          = heap.back();
			heap[k_pos]->setHandle(k_pos);
            heap.pop_back();
            percolateDown(k_pos);
        }else
            heap.pop();
    }

    K* top()
    {
        return heap[ 0 ];        
    }
    
    void pop()
    {
        heap[0]->setInHeap(false);
        heap[0] = heap.back();
        heap[0]->setHandle(0);
        heap.pop_back();
        if (heap.size() > 1) percolateDown(0);
    }

    K* removeMin()
    {
        K x              = heap[0];
        heap[0]          = heap.back();
        heap[0]->setHandle(0);
        x->setInHeap(false);
        heap.pop_back();
        if (heap.size() > 1) percolateDown(0);
        return x; 
    }


/*    // Rebuild the heap from scratch, using the elements in 'ns':
    void build(const vec<K>& ns) {
        for (int i = 0; i < heap.size(); i++)
            indices[heap[i]] = -1;
        heap.clear();

        for (int i = 0; i < ns.size(); i++){
            // TODO: this should probably call reserve instead of relying on it being reserved already.
            assert(indices.has(ns[i]));
            indices[ns[i]] = i;
            heap.push(ns[i]); }

        for (int i = heap.size() / 2 - 1; i >= 0; i--)
            percolateDown(i);
    }

    void clear(bool dispose = false) 
    { 
        // TODO: shouldn't the 'indices' map also be dispose-cleared?
        for (int i = 0; i < heap.size(); i++)
            indices[heap[i]] = -1;
        heap.clear(dispose); 
    }
*/
};

#endif