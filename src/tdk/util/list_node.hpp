#ifndef __list_node_h__
#define __list_node_h__

namespace tdk {

template < typename type >
class slist_node {
public:
    slist_node( void ) : _next( nullptr ) {

    }

    type* next( void ) {
        return _next;
    }

    type* next( type* n ) {
        type* p = _next;
		_next = n;
		return p;
    }
protected:
    ~slist_node( void ) {
    }
private:
    type* _next;
};

template < typename type >
class slist_queue {
public:
    explicit slist_queue( type* head = nullptr ) : _head( head ) {
        type* it = head;
        while ( it && ( it->next() != nullptr ) ) {
            it = it->next();
        }
        _tail = it;
    }

    ~slist_queue( void ) {

	}

	void add_front( type* node ) {
		if ( _head == nullptr ) {
			_head = _tail = node;
		} else {
			type* head = _head;
			_head = node;
			_head->next( head );
		}		
	}

    void add_tail( type* node ) {
		if ( _head == nullptr ) {
			_head = _tail = node;
		} else {
			_tail->next( node );
			_tail = node;
		}
	}

    type*  front( void ) {
		return _head;
	}

    void pop_front( void ) {
		if ( _head ) {
			if ( _head == _tail ) {
				_head = _tail = nullptr;
			} else {
				type* node = _head->next(nullptr);
				_head = node;
			}
		} 
	}

    void add_tail( slist_queue<type>& list ) {
		if ( !list.is_empty() ) {
			this->add_tail( list.front() );
			this->_tail = list._tail;
			list._head = list._tail = nullptr;
		}
	}

	bool is_empty( void ) const {
		return _head == nullptr;
	}

    int count( void ) const {
        int cnt = 0;
        type* p = _head;
        while( p ) {
            ++cnt;
            p = p->next();
        }
        return cnt;
        //return slist_op< type >::count( _head );
    }
public:
    class iterator {
    public:
        iterator( type* node ) : _node( node ) {
        }
        ~iterator( void ) {
        }

        iterator operator++( int ) {
            iterator it( _node );
            if ( _node != nullptr ) 
                _node = _node->next();
            return it;
        }

        iterator& operator++( void ) {
            if ( _node != nullptr ) 
                _node = _node->next();
            return *this;
        }

        bool operator==( const iterator& rhs ) {
            return _node == rhs._node;
        }

        bool operator!=( const iterator& rhs ) {
            return _node != rhs._node;
        }

        bool operator==( const type* node ) {
            return _node == node;
        }

        bool operator!=( const type* node ) {
            return _node != node;
        }

        type* operator->(void)const{
            return _node;
        }

        type& operator*(void) const{
            return *_node;
        }
    private:
        type* _node;
    };

    iterator begin()  {
        return iterator( _head );
    }

    iterator end() {
        return iterator( _tail->next() );
    }
    
private:
    type* _head;
    type* _tail;
};

}

#endif
