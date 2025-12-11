package lr3

import (
	"bufio"
	"encoding/binary"
	"errors"
	"os"
)

type dNode struct {
	data string
	prev *dNode
	next *dNode
}

type DoublyList struct {
	head *dNode
	tail *dNode
	size int
}

func NewDoublyList() *DoublyList {
	return &DoublyList{}
}

func (l *DoublyList) Size() int {
	return l.size
}

func (l *DoublyList) IsEmpty() bool {
	return l.size == 0
}

func (l *DoublyList) Clear() {
	cur := l.head
	for cur != nil {
		nxt := cur.next
		cur.prev = nil
		cur.next = nil
		cur = nxt
	}
	l.head = nil
	l.tail = nil
	l.size = 0
}

func (l *DoublyList) PushFront(v string) {
	n := &dNode{data: v}
	if l.head == nil {
		l.head = n
		l.tail = n
	} else {
		n.next = l.head
		l.head.prev = n
		l.head = n
	}
	l.size++
}

func (l *DoublyList) PushBack(v string) {
	n := &dNode{data: v}
	if l.tail == nil {
		l.head = n
		l.tail = n
	} else {
		n.prev = l.tail
		l.tail.next = n
		l.tail = n
	}
	l.size++
}

func (l *DoublyList) PopFront() (string, error) {
	if l.head == nil {
		return "", errors.New("empty list")
	}
	v := l.head.data
	l.head = l.head.next
	if l.head != nil {
		l.head.prev = nil
	} else {
		l.tail = nil
	}
	l.size--
	return v, nil
}

func (l *DoublyList) PopBack() (string, error) {
	if l.tail == nil {
		return "", errors.New("empty list")
	}
	v := l.tail.data
	l.tail = l.tail.prev
	if l.tail != nil {
		l.tail.next = nil
	} else {
		l.head = nil
	}
	l.size--
	return v, nil
}

func (l *DoublyList) InsertAfter(target, v string) bool {
	cur := l.head
	for cur != nil {
		if cur.data == target {
			n := &dNode{data: v}
			n.prev = cur
			n.next = cur.next
			if cur.next != nil {
				cur.next.prev = n
			} else {
				l.tail = n
			}
			cur.next = n
			l.size++
			return true
		}
		cur = cur.next
	}
	return false
}

func (l *DoublyList) InsertBefore(target, v string) bool {
	cur := l.head
	for cur != nil {
		if cur.data == target {
			n := &dNode{data: v}
			n.next = cur
			n.prev = cur.prev
			if cur.prev != nil {
				cur.prev.next = n
			} else {
				l.head = n
			}
			cur.prev = n
			l.size++
			return true
		}
		cur = cur.next
	}
	return false
}

func (l *DoublyList) RemoveByValue(v string) bool {
	cur := l.head
	for cur != nil {
		if cur.data == v {
			if cur.prev != nil {
				cur.prev.next = cur.next
			} else {
				l.head = cur.next
			}
			if cur.next != nil {
				cur.next.prev = cur.prev
			} else {
				l.tail = cur.prev
			}
			l.size--
			return true
		}
		cur = cur.next
	}
	return false
}

func (l *DoublyList) Search(v string) bool {
	cur := l.head
	for cur != nil {
		if cur.data == v {
			return true
		}
		cur = cur.next
	}
	return false
}

// ---------- текстовая сериализация ----------

func (l *DoublyList) SaveText(path string) error {
	f, err := os.Create(path)
	if err != nil {
		return err
	}
	defer f.Close()

	w := bufio.NewWriter(f)
	cur := l.head
	for cur != nil {
		if _, err := w.WriteString(cur.data + "\n"); err != nil {
			return err
		}
		cur = cur.next
	}
	return w.Flush()
}

func (l *DoublyList) LoadText(path string) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()

	l.Clear()
	sc := bufio.NewScanner(f)
	for sc.Scan() {
		l.PushBack(sc.Text())
	}
	return sc.Err()
}

// ---------- бинарная сериализация ----------

func (l *DoublyList) SaveBinary(path string) error {
	f, err := os.Create(path)
	if err != nil {
		return err
	}
	defer f.Close()

	if err := binary.Write(f, binary.LittleEndian, int32(l.size)); err != nil {
		return err
	}
	cur := l.head
	for cur != nil {
		b := []byte(cur.data)
		if err := binary.Write(f, binary.LittleEndian, int32(len(b))); err != nil {
			return err
		}
		if _, err := f.Write(b); err != nil {
			return err
		}
		cur = cur.next
	}
	return nil
}

func (l *DoublyList) LoadBinary(path string) error {
	f, err := os.Open(path)
	if err != nil {
		return err
	}
	defer f.Close()

	l.Clear()

	var n int32
	if err := binary.Read(f, binary.LittleEndian, &n); err != nil {
		return err
	}
	for i := int32(0); i < n; i++ {
		var ln int32
		if err := binary.Read(f, binary.LittleEndian, &ln); err != nil {
			return err
		}
		buf := make([]byte, ln)
		if _, err := f.Read(buf); err != nil {
			return err
		}
		l.PushBack(string(buf))
	}
	return nil
}
