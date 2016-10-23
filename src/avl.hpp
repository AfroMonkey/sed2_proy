#ifndef AVL_HPP
#define AVL_HPP

template <typename T>
class AvlNode
{
public:
    T data;
    AvlNode* parent;
    AvlNode* left;
    AvlNode* right;

    AvlNode();
    AvlNode(T data);
    bool is_leaf();
};

template <typename T>
AvlNode<T>::AvlNode()
{
    parent = nullptr;
    left = nullptr;
    right = nullptr;
}

template <typename T>
AvlNode<T>::AvlNode(T data)
{
    parent = nullptr;
    left = nullptr;
    right = nullptr;
    this->data = data;
}

template <typename T>
bool AvlNode<T>::is_leaf()
{
    return left == nullptr && right == nullptr;
}

/* -------------------------------------------------------------------------- */

template <typename T, int(*compare)(T&, T&)>
class Avl
{
private:
    AvlNode<T>* _root;
    int _nodes;

    AvlNode<T>* _get(AvlNode<T>* subtree, T &data);
    void _insert(AvlNode<T>* subtree, AvlNode<T>* node);
    void _balance(AvlNode<T>* subtree);
    int _balance_number(AvlNode<T>* node);
    AvlNode<T>* _min(AvlNode<T>* node);
    AvlNode<T>* _max(AvlNode<T>* node);
    void _trim(AvlNode<T>* subtree);
    int _childs(AvlNode<T>* subtree);
    void _slr(AvlNode<T>* subtree);
    void _dlr(AvlNode<T>* subtree);
    void _srr(AvlNode<T>* subtree);
    void _drr(AvlNode<T>* subtree);
public:
    Avl();
    ~Avl();
    int nodes();
    AvlNode<T>* get(T &data);
    void insert(T& data);
    void remove(AvlNode<T>* node);
    void trim(AvlNode<T>* subtree);
    bool empty() {return _root == nullptr;}
    AvlNode<T>* root() {return _root;}
    template <typename F>
    void preorder(AvlNode<T>* subtree, F function);
    template <typename F>
    void postorder(AvlNode<T>* subtree, F function);
    template <typename F>
    void inorder(AvlNode<T>* subtree, F function);
};

template <typename T, int(*compare)(T&, T&)>
Avl<T, compare>::Avl()
{
    _root = nullptr;
    _nodes = 0;
}

template <typename T, int(*compare)(T&, T&)>
Avl<T, compare>::~Avl()
{
    trim(_root);
}

template <typename T, int(*compare)(T&, T&)>
AvlNode<T>* Avl<T, compare>:: _get(AvlNode<T>* subtree, T &data)
{
    if (!subtree) return nullptr;
    if (compare(data, subtree->data) < 0) return _get(subtree->left, data);
    if (compare(data, subtree->data) == 0) return subtree;
    return _get(subtree->right, data);
}

template <typename T, int(*compare)(T&, T&)>
void Avl<T, compare>::_insert(AvlNode<T>* subtree, AvlNode<T>* node)
{
    if (!subtree) return;
    if (compare(node->data, subtree->data) < 0)
    {
        if (!subtree->left)
        {
            node->parent = subtree;
            subtree->left = node;
            for (AvlNode<T>* aux = node->parent; aux; aux = aux->parent)
            {
                _balance(aux);
            }
        }
        else
        {
            _insert(subtree->left, node);
        }
    }
    else
    {
        if (!subtree->right)
        {
            node->parent = subtree;
            subtree->right = node;
            for (AvlNode<T>* aux = node->parent; aux; aux = aux->parent)
            {
                _balance(aux);
            }
        }
        else
        {
            _insert(subtree->right, node);
        }
    }
}

template <typename T, int(*compare)(T&, T&)>
void Avl<T, compare>::_balance(AvlNode<T>* subtree)
{
    if (!subtree) return;
    int bnr = _balance_number(subtree->right);
    if (subtree->right) ++bnr;
    int bnl = _balance_number(subtree->left);
    if (subtree->left) ++bnl;
    int bn = bnr - bnl;
    switch (bn)
    {
        case 2:
            if (bnr < 0)
            {
                _dlr(subtree);
            }
            else
            {
                _slr(subtree);
            }
            break;
        case -2:
            if (bnl > 0)
            {
                _drr(subtree);
            }
            else
            {
                _srr(subtree);
            }
            break;
    }
}

template <typename T, int(*compare)(T&, T&)>
int Avl<T, compare>::_balance_number(AvlNode<T>* node)
{
    if (!node) return 0;
    return _childs(node->right) - _childs(node->left);
}


template <typename T, int(*compare)(T&, T&)>
AvlNode<T>* Avl<T, compare>::_min(AvlNode<T>* node)
{
    if (!node) return nullptr;
    if (!node->left) return node;
    return _min(node->left);
}

template <typename T, int(*compare)(T&, T&)>
AvlNode<T>* Avl<T, compare>::_max(AvlNode<T>* node)
{
    if (!node) return nullptr;
    if (!node->right) return node;
    return _min(node->right);
}

template <typename T, int(*compare)(T&, T&)>
void Avl<T, compare>::_trim(AvlNode<T>* subtree)
{
    if(subtree)
    {
        _trim(subtree->left);
        _trim(subtree->right);
        delete subtree;
    }
}

template <typename T, int(*compare)(T&, T&)>
int Avl<T, compare>::_childs(AvlNode<T>* subtree)
{
    if (!subtree) return 0;
    return _childs(subtree->left) + _childs(subtree->right);
}

template <typename T, int(*compare)(T&, T&)>
void Avl<T, compare>::_slr(AvlNode<T>* subtree)
{
	AvlNode<T>* aux1 = subtree->right;
	AvlNode<T>* aux2 = aux1->left;

	subtree->right = aux2;
	aux1->left = subtree;

	AvlNode<T>* parent = subtree->parent;
	if(parent)
	{
		if(parent->left == subtree)
		{
			parent->left = aux1;
		}
		else
		{
			parent->right = aux1;
		}
	}
	else
	{
		_root = aux1;
	}

	aux1->parent = subtree->parent;
	subtree->parent = aux1;
	if(aux2)
	{
		aux2->parent = subtree;
	}
}

template <typename T, int(*compare)(T&, T&)>
void Avl<T, compare>::_dlr(AvlNode<T>* subtree)
{
	_srr(subtree->right);
	_slr(subtree);
}

template <typename T, int(*compare)(T&, T&)>
void Avl<T, compare>::_srr(AvlNode<T>* subtree)
{
	AvlNode<T>* aux1 = subtree->left;
	AvlNode<T>* aux2 = aux1->right;

	subtree->left = aux2;
	aux1->right = subtree;

	AvlNode<T>* parent = subtree->parent;
	if(parent)
	{
		if(parent->left == subtree)
		{
			parent->left = aux1;
		}
		else
		{
			parent->right = aux1;
		}
	}
	else
	{
		_root = aux1;
	}

	aux1->parent = subtree->parent;
	subtree->parent = aux1;
	if(aux2)
	{
		aux2->parent = subtree;
	}
}

template <typename T, int(*compare)(T&, T&)>
void Avl<T, compare>::_drr(AvlNode<T>* subtree)
{
	_slr(subtree->left);
	_srr(subtree);
}


template <typename T, int(*compare)(T&, T&)>
int Avl<T, compare>::nodes()
{
    return _nodes;
}

template <typename T, int(*compare)(T&, T&)>
AvlNode<T>* Avl<T, compare>::get(T &data)
{
    return _get(_root, data);
}

template <typename T, int(*compare)(T&, T&)>
void Avl<T, compare>::insert(T& data)
{
    if (!_root)
    {
        _root = new AvlNode<T>(data);
    }
    else
    {
        _insert(_root, new AvlNode<T>(data));
    }
}

template <typename T, int(*compare)(T&, T&)>
void Avl<T, compare>::remove(AvlNode<T>* node)
{
    AvlNode<T>* aux;
    AvlNode<T>* parent = node->parent;

    if (aux->left)
    {
        aux = _max(node);
    }
    else if (aux->right)
    {
        aux = _min(node);
    }
    else
    {
        aux = nullptr;
    }

    if (node->parent->left == node)
    {
        node->parent->left = aux;
    }
    else
    {
        node->parent->right = aux;
    }

    if (aux) aux->parent = node->parent;

    if (node == _root) _root = aux;


    delete node;

    if (parent)
    {
        _balance(parent);
    }
    else
    {
        _balance(_root);
    }
}

template <typename T, int(*compare)(T&, T&)>
void Avl<T, compare>::trim(AvlNode<T>* subtree)
{
    if (!subtree) return;

    if (subtree->parent)
    {
        if (subtree->parent->left == subtree)
        {
            subtree->parent->left = nullptr;
        }
        else
        {
            subtree->parent->right = nullptr;
        }
        _trim(subtree);
    }
    else
    {
        _trim(subtree);
        _root = nullptr;
    }
}

template <typename T, int(*compare)(T&, T&)>
template <typename F>
void Avl<T, compare>::preorder(AvlNode<T>* subtree, F function)
{
    if(subtree)
    {
        function(subtree->data);
        preorder(subtree->left, function);
        preorder(subtree->right, function);
    }
}

template <typename T, int(*compare)(T&, T&)>
template <typename F>
void Avl<T, compare>::postorder(AvlNode<T>* subtree, F function)
{
    if(subtree)
    {
        postorder(subtree->left, function);
        postorder(subtree->right, function);
        function(subtree->data);
    }
}

template <typename T, int(*compare)(T&, T&)>
template <typename F>
void Avl<T, compare>::inorder(AvlNode<T>* subtree, F function)
{
    if(subtree)
    {
        inorder(subtree->left, function);
        function(subtree->data);
        inorder(subtree->right, function);
    }
}


#endif
