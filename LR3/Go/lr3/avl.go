package lr3

// avlNode — узел АВЛ‑дерева.
type avlNode struct {
	key    int
	value  string
	height int
	left   *avlNode
	right  *avlNode
}

// AVLTree — самобалансирующееся АВЛ‑дерево.
type AVLTree struct {
	root *avlNode
	size int
}

// NewAVL создаёт пустое АВЛ‑дерево.
func NewAVL() *AVLTree {
	return &AVLTree{}
}

// Size возвращает количество элементов.
func (t *AVLTree) Size() int {
	return t.size
}

// IsEmpty сообщает, пусто ли дерево.
func (t *AVLTree) IsEmpty() bool {
	return t.size == 0
}

// публичный Insert.
func (t *AVLTree) Insert(key int, value string) {
	var inserted bool
	t.root, inserted = t.insertRec(t.root, key, value)
	if inserted {
		t.size++
	}
}

func height(n *avlNode) int {
	if n == nil {
		return 0
	}
	return n.height
}

func updateHeight(n *avlNode) {
	hl := height(n.left)
	hr := height(n.right)
	if hl > hr {
		n.height = hl + 1
	} else {
		n.height = hr + 1
	}
}

func balanceFactor(n *avlNode) int {
	return height(n.left) - height(n.right)
}

// правый поворот.
func rotateRight(y *avlNode) *avlNode {
	x := y.left
	T2 := x.right

	x.right = y
	y.left = T2

	updateHeight(y)
	updateHeight(x)
	return x
}

// левый поворот.
func rotateLeft(x *avlNode) *avlNode {
	y := x.right
	T2 := y.left

	y.left = x
	x.right = T2

	updateHeight(x)
	updateHeight(y)
	return y
}

func rebalance(node *avlNode) *avlNode {
	updateHeight(node)
	bf := balanceFactor(node)

	// Left heavy
	if bf > 1 {
		// Left-Right
		if balanceFactor(node.left) < 0 {
			node.left = rotateLeft(node.left)
		}
		// Left-Left
		return rotateRight(node)
	}

	// Right heavy
	if bf < -1 {
		// Right-Left
		if balanceFactor(node.right) > 0 {
			node.right = rotateRight(node.right)
		}
		// Right-Right
		return rotateLeft(node)
	}

	return node
}

func (t *AVLTree) insertRec(node *avlNode, key int, value string) (*avlNode, bool) {
	if node == nil {
		return &avlNode{key: key, value: value, height: 1}, true
	}
	if key < node.key {
		var inserted bool
		node.left, inserted = t.insertRec(node.left, key, value)
		if inserted {
			node = rebalance(node)
		}
		return node, inserted
	}
	if key > node.key {
		var inserted bool
		node.right, inserted = t.insertRec(node.right, key, value)
		if inserted {
			node = rebalance(node)
		}
		return node, inserted
	}

	// одинаковый ключ — просто обновляем значение, size не меняем.
	node.value = value
	return node, false
}

// Find ищет ключ и возвращает значение и флаг найден/нет.
func (t *AVLTree) Find(key int) (string, bool) {
	n := t.root
	for n != nil {
		if key < n.key {
			n = n.left
		} else if key > n.key {
			n = n.right
		} else {
			return n.value, true
		}
	}
	return "", false
}

// Contains проверяет наличие ключа.
func (t *AVLTree) Contains(key int) bool {
	_, ok := t.Find(key)
	return ok
}

// Remove удаляет ключ, возвращает true, если узел удалён.
func (t *AVLTree) Remove(key int) bool {
	var removed bool
	t.root, removed = t.removeRec(t.root, key)
	if removed {
		t.size--
	}
	return removed
}

func (t *AVLTree) removeRec(node *avlNode, key int) (*avlNode, bool) {
	if node == nil {
		return nil, false
	}

	var removed bool

	if key < node.key {
		node.left, removed = t.removeRec(node.left, key)
	} else if key > node.key {
		node.right, removed = t.removeRec(node.right, key)
	} else {
		// нашли узел
		removed = true
		// 1) 0 или 1 ребёнок
		if node.left == nil {
			return node.right, true
		}
		if node.right == nil {
			return node.left, true
		}
		// 2) два ребёнка: берём мин справа
		minRight := node.right
		for minRight.left != nil {
			minRight = minRight.left
		}
		node.key = minRight.key
		node.value = minRight.value
		node.right, _ = t.removeRec(node.right, minRight.key)
	}

	if node == nil {
		return nil, removed
	}
	node = rebalance(node)
	return node, removed
}

// Min возвращает минимальный ключ и значение.
func (t *AVLTree) Min() (int, string, bool) {
	if t.root == nil {
		return 0, "", false
	}
	n := t.root
	for n.left != nil {
		n = n.left
	}
	return n.key, n.value, true
}

// Max возвращает максимальный ключ и значение.
func (t *AVLTree) Max() (int, string, bool) {
	if t.root == nil {
		return 0, "", false
	}
	n := t.root
	for n.right != nil {
		n = n.right
	}
	return n.key, n.value, true
}

// InOrder возвращает пары (key, value) в отсортированном порядке.
func (t *AVLTree) InOrder() [][2]interface{} {
	res := make([][2]interface{}, 0, t.size)
	var dfs func(*avlNode)
	dfs = func(n *avlNode) {
		if n == nil {
			return
		}
		dfs(n.left)
		res = append(res, [2]interface{}{n.key, n.value})
		dfs(n.right)
	}
	dfs(t.root)
	return res
}
